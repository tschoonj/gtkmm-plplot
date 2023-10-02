/*
Copyright (C) 2015 Tom Schoonjans

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm-plplot/canvas.h>
#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/legend.h>
#include <gtkmm-plplot/regionselection.h>
#include <valarray>
#include <cmath>
#include <gdkmm/general.h>
#include <iostream>
#include <typeinfo>

using namespace Gtk::PLplot;

Canvas::Canvas(Plot &plot, Gdk::RGBA _background_color) :
  Canvas(_background_color) {

    add_plot(plot);
}

Canvas::Canvas(Gdk::RGBA _background_color) :
  Glib::ObjectBase("GtkmmPLplotCanvas"),
  start_event{-1.0, -1.0},
  start_cairo{-1.0, -1.0},
  end_event{-1.0, -1.0},
  end_cairo{-1.0, -1.0},
  selecting(false),
  left_mouse_button_clicked(false),
  shift_pressed(false),
  selected_plot(nullptr),
  inside_plot(nullptr),
  inside_plot_current_coords{0.0, 0.0},
  mouse_current_coords{0.0, 0.0},
  background_color(_background_color) {
  set_can_focus(true);

  signal_changed().connect(sigc::mem_fun(*this, &Canvas::on_changed));

  set_draw_func(sigc::mem_fun(*this, &Canvas::on_draw));

  auto scroll_controller = Gtk::EventControllerScroll::create();
  scroll_controller->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
  scroll_controller->signal_scroll().connect(sigc::mem_fun(*this, &Canvas::on_scroll_event), true);
  add_controller(scroll_controller);

  auto motion_controller = Gtk::EventControllerMotion::create();
  motion_controller->signal_enter().connect(sigc::mem_fun(*this, &Canvas::on_motion_notify_event), false);
  motion_controller->signal_motion().connect(sigc::mem_fun(*this, &Canvas::on_motion_notify_event), false);
  add_controller(motion_controller);

  auto key_controller = Gtk::EventControllerKey::create();
  key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &Canvas::on_key_press_event), false);
  key_controller->signal_key_released().connect(sigc::mem_fun(*this, &Canvas::on_key_release_event), false);
  add_controller(key_controller);

  auto gesture_click = Gtk::GestureClick::create();
  gesture_click->signal_pressed().connect(sigc::mem_fun(*this, &Canvas::on_button_press_event), false);
  gesture_click->signal_released().connect(sigc::mem_fun(*this, &Canvas::on_button_release_event), false);
  add_controller(gesture_click);
}

bool Canvas::on_scroll_event(double dx, double dy) {
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  double start_cairo[2];

  start_cairo[0] = mouse_current_coords[0] + dx;
  start_cairo[1] = height - (mouse_current_coords[1] + dy);

  if (inside_plot != nullptr && !selecting && !left_mouse_button_clicked && !shift_pressed) {
    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);
    if (region_selection != nullptr && region_selection->get_region_zoomable()) {
      double cursor_x, cursor_y;

      region_selection->coordinate_transform_cairo_to_world(
        start_cairo[0],
        start_cairo[1],
        cursor_x,
        cursor_y
      );
      region_selection->signal_zoom_region().emit(cursor_x, cursor_y, dy > 0 ? GdkScrollDirection::GDK_SCROLL_DOWN : GdkScrollDirection::GDK_SCROLL_UP);
      return true;
    }
  }

  return false;
}

void Canvas::add_plot(Plot &plot) {
  // ensure plot is not already present in plot
  auto iter = std::find(plots.begin(), plots.end(), &plot);
  if (iter != plots.end())
    throw Exception("Gtk::PLplot::Canvas::add_plot -> Plot has been added before to this canvas");

  plots.push_back(&plot);

  // ensure plot signal_changed gets re-emitted by the canvas
  plots.back()->signal_changed().connect([this](){_signal_changed.emit();});

  _signal_changed.emit();
}

Canvas::~Canvas() {
  for (auto &iter : plots) {
    if (iter->is_managed_()) {
      delete iter;
    }
  }
}

void Canvas::on_changed() {
  //this catches all signal_changed emissions recursively from the Plot and PlotData classes
  //so this is the method that ensures things get redrawn when one of the parameters is changed.
  this->queue_draw();
}

void Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
  draw_plot(cr, width, height);

  if (selecting &&
    start_cairo[0] >= 0.0 &&
    start_cairo[1] >= 0.0 &&
    end_cairo[0] >= 0.0 &&
    end_cairo[1] >= 0.0) {

    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(selected_plot);

    cr->set_line_width(region_selection->get_region_selection_width());
    Gdk::Cairo::set_source_rgba(cr, region_selection->get_region_selection_color());
    cr->rectangle(MIN(start_cairo[0], end_cairo[0]) ,
                  MIN(height - start_cairo[1], height - end_cairo[1]) ,
                  fabs(end_cairo[0] - start_cairo[0]),
                  fabs(end_cairo[1] - start_cairo[1]));
    cr->stroke();
  }

  if (has_focus()) {
    cr->save();

    // draw focus box
    const Glib::RefPtr<Gtk::StyleContext> style_context = get_style_context();
    style_context->render_focus(cr, 0.0, 0.0, width, height);

    cr->restore();
  }
}

void Canvas::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
  //start by drawing the background...
  cr->rectangle(0.0, 0.0, width, height);
  Gdk::Cairo::set_source_rgba(cr, background_color);
  cr->fill();

  //...followed by the plots, according to their order in the plots vector
  for (auto &iter : plots) {
      iter->draw_plot(cr, width, height);
  }
}

void Canvas::on_button_press_event(int n_press, double x, double y) {
  grab_focus();
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  start_event[0] = x;
  start_event[1] = y;
  start_cairo[0] = x;
  start_cairo[1] = height - y;
  end_event[0] = -1.0;
  end_event[1] = -1.0;
  end_cairo[0] = -1.0;
  end_cairo[1] = -1.0;


  if (inside_plot != nullptr) {
    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);
    if (region_selection != nullptr &&
      region_selection->get_region_selectable() &&
      !shift_pressed) {
      // double-click -> zoom out!
      if (n_press == 2) {
        //convert event coordinates to plot coordinates
        double cursor_x, cursor_y;

        region_selection->coordinate_transform_cairo_to_world(
          start_cairo[0],
          start_cairo[1],
          cursor_x,
          cursor_y
        );

        region_selection->signal_double_press().emit(cursor_x, cursor_y);

        return;
      }
      selecting = true;
      selected_plot = inside_plot;

      return;
    }
    else if (region_selection != nullptr &&
      region_selection->get_region_pannable() &&
      shift_pressed == true) {
      left_mouse_button_clicked = true;

      //change cursor
      Glib::RefPtr<Gdk::Cursor> grabbing = Gdk::Cursor::create("grabbing");
      set_cursor(grabbing);

      return;
    }
  }

  selecting = false;
  left_mouse_button_clicked = false;
}

void Canvas::on_button_release_event(int n_press, double x, double y) {
  left_mouse_button_clicked = false;
  if (shift_pressed) {
    Glib::RefPtr<Gdk::Cursor> grab = Gdk::Cursor::create("grab");
    set_cursor(grab);

    return;
  }
  else if (!selecting)
    return;

  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  end_event[0] = x;
  end_event[1] = y;
  end_cairo[0] = x;
  end_cairo[1] = height - y;

  if (start_cairo[0] == end_cairo[0] && start_cairo[1] ==  end_cairo[1]) {
    selecting = false;
    return;
  }

  RegionSelection *region_selection = dynamic_cast<RegionSelection *>(selected_plot);

  //this should never happen
  if (region_selection == nullptr)
    throw Exception("Gtk::PLplot::Canvas::on_button_release_event -> selecting a non-RegionSelection object");

  //make sure we stay within the plot while selecting
  if (end_cairo[0] > start_cairo[0]) {
    //this 1E-10 subtraction is necessary to ensure calc_world works properly
    //when dragging a box that touches the right axis.
    end_cairo[0] = MIN(end_cairo[0], region_selection->cairo_range_x[1] - 1E-10);
  }
  else if (end_cairo[0] < start_cairo[0]) {
    end_cairo[0] = MAX(end_cairo[0], region_selection->cairo_range_x[0]);
  }

  if (end_cairo[1] > start_cairo[1]) {
    end_cairo[1] = MIN(end_cairo[1], region_selection->cairo_range_y[1] - 1E-10);
  }
  else if (end_cairo[1] < start_cairo[1]) {
    end_cairo[1] = MAX(end_cairo[1], region_selection->cairo_range_y[0]);
  }

  selecting = false;

  //emit signal!
  //prepare plplot coordinates
  //inspired by https://www.mail-archive.com/plplot-devel@lists.sourceforge.net/msg03079.html

  double start_plplot[2];
  double end_plplot[2];

  //get the plot coordinates corresponding to the cairo coordinates
  region_selection->coordinate_transform_cairo_to_plplot(start_cairo[0], start_cairo[1],
                  start_plplot[0], start_plplot[1]);
  region_selection->coordinate_transform_cairo_to_plplot(end_cairo[0], end_cairo[1],
                  end_plplot[0], end_plplot[1]);
  double start_plplot_def[2];
  double end_plplot_def[2];

  //ensure that the coordinates are within the extremes based on the x and y vectors
  //in case of the full view, due to precision errors, the extremes calculated based on calc_world
  //are actually slightly outside of these data extremes, meaning that it's not possible to drag the selection
  //along the plot grid
  start_plplot_def[0] = MIN(start_plplot[0], end_plplot[0]);
  start_plplot_def[1] = MIN(start_plplot[1], end_plplot[1]);
  end_plplot_def[0] = MAX(start_plplot[0], end_plplot[0]);
  end_plplot_def[1] = MAX(start_plplot[1], end_plplot[1]);

  //this is necessary to get rid of the box on the plot, even if the signal_select_region is not caught by the plot
  _signal_changed.emit();

  double start_world[2];
  double end_world[2];

  region_selection->coordinate_transform_plplot_to_world(
    start_plplot_def[0],
    start_plplot_def[1],
    start_world[0],
    start_world[1]
  );

  region_selection->coordinate_transform_plplot_to_world(
    end_plplot_def[0],
    end_plplot_def[1],
    end_world[0],
    end_world[1]
  );

  try {
    region_selection->signal_select_region().emit(start_world[0], end_world[0], start_world[1], end_world[1]);
  }
  catch (Exception &e) {
    g_warning("Exception caught when emitting signal_select_region()!");
  }
}

void Canvas::on_motion_notify_event (double x, double y) {
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  end_event[0] = x;
  end_event[1] = y;
  end_cairo[0] = x;
  end_cairo[1] = height - y;

  //inside_plot = nullptr;

  mouse_current_coords[0] = x;
  mouse_current_coords[1] = y;

  //emit signal for new cursor coordinates
  bool cursor_checked = false;
  bool is_enter = false;
  for (auto plot = plots.rbegin() ; plot != plots.rend() ; ++plot) {
    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(*plot);

    if (region_selection != nullptr &&
        (*plot)->is_showing() &&
        end_cairo[0] >= region_selection->cairo_range_x[0] &&
        end_cairo[0] <= region_selection->cairo_range_x[1] &&
        end_cairo[1] >= region_selection->cairo_range_y[0] &&
        end_cairo[1] <= region_selection->cairo_range_y[1]) {
      //std::cout << "on_motion_notify_event plot " << plot << std::endl;
      double cursor_x, cursor_y;
      region_selection->coordinate_transform_cairo_to_world(
        end_cairo[0],
        end_cairo[1],
        cursor_x,
        cursor_y
      );

      if (inside_plot != *plot) {
        // if there is a change of inside_plot, we cannot rely on inside_plot_current_coords!
        inside_plot_current_coords[0] = cursor_x;
        inside_plot_current_coords[1] = cursor_y;
        is_enter = true;
      }

      inside_plot = *plot;

      // change cursor to crosshair or system default if necessary
      if (region_selection->get_region_pannable() && left_mouse_button_clicked && shift_pressed) {
        Glib::RefPtr<Gdk::Cursor> grabbing = Gdk::Cursor::create("grabbing");
        set_cursor(grabbing);
        std::vector<double> rv = region_selection->signal_pan().emit(inside_plot_current_coords[0], inside_plot_current_coords[1], cursor_x, cursor_y);
        cursor_x = rv[2];
        cursor_y = rv[3];
      }
      else if (region_selection->get_region_pannable() && shift_pressed) {
        Glib::RefPtr<Gdk::Cursor> grab = Gdk::Cursor::create("grab");
        set_cursor(grab);
      }
      else {
        Glib::RefPtr<Gdk::Cursor> crosshair = Gdk::Cursor::create("crosshair");
        set_cursor(crosshair);
      }

      // this should be emitted after signal_pan(), as cursor_x and cursor_y may have changed
      if (is_enter)
        region_selection->signal_cursor_enter().emit(cursor_x, cursor_y);
      else
        region_selection->signal_cursor_motion().emit(cursor_x, cursor_y);

      cursor_checked = true;

      inside_plot_current_coords[0] = cursor_x;
      inside_plot_current_coords[1] = cursor_y;

      break;
    }
  }

  if (!cursor_checked) {
    if (inside_plot) {
      RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);
      region_selection->signal_cursor_leave().emit();
    }
    inside_plot = nullptr;
    set_cursor();
  }

  // if not dragging a selection box, stop here
  if (!selecting)
    return;

  RegionSelection *region_selection = dynamic_cast<RegionSelection *>(selected_plot);

  //this should never happen
  if (region_selection == nullptr)
    throw Exception("Gtk::PLplot::Canvas::on_motion_notify_event -> selecting a non-RegionSelection object");

  //make sure we stay within the plot while selecting
  if (end_cairo[0] > start_cairo[0]) {
    end_cairo[0] = MIN(end_cairo[0], region_selection->cairo_range_x[1]);
  }
  else if (end_cairo[0] < start_cairo[0]) {
    end_cairo[0] = MAX(end_cairo[0], region_selection->cairo_range_x[0]);
  }

  if (end_cairo[1] > start_cairo[1]) {
    end_cairo[1] = MIN(end_cairo[1], region_selection->cairo_range_y[1]);
  }
  else if (end_cairo[1] < start_cairo[1]) {
    end_cairo[1] = MAX(end_cairo[1], region_selection->cairo_range_y[0]);
  }

  _signal_changed.emit();
}

bool Canvas::on_key_press_event(guint keyval, guint keycode, Gdk::ModifierType state) {

  if (inside_plot == nullptr || selecting)
    return false;

  RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);

  if (region_selection != nullptr &&
    region_selection->get_region_pannable() &&
    (keyval == GDK_KEY_Shift_L || keyval == GDK_KEY_Shift_R)
    ) {
    shift_pressed = true;

    if (left_mouse_button_clicked) {
        Glib::RefPtr<Gdk::Cursor> grabbing = Gdk::Cursor::create("grabbing");
        set_cursor(grabbing);
    }
    else {
        Glib::RefPtr<Gdk::Cursor> grab = Gdk::Cursor::create("grab");
        set_cursor(grab);
    }

    return true;
  }

  return false;
}

void Canvas::on_key_release_event(guint keyval, guint keycode, Gdk::ModifierType state) {
  if (keyval == GDK_KEY_Shift_L || keyval == GDK_KEY_Shift_R) {
    shift_pressed = false;
    if (inside_plot != nullptr) {
      RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);
      if (region_selection != nullptr) {
        Glib::RefPtr<Gdk::Cursor> crosshair = Gdk::Cursor::create("crosshair");
        set_cursor(crosshair);
        return;
      }
    }
    set_cursor();
  }
}

Plot* Canvas::get_plot(unsigned int index) {
  if (index < plots.size()) {
    return plots[index];
  }
  throw Exception("Gtk::PLplot::Canvas::get_plot -> Invalid index");
}

Gdk::RGBA Canvas::get_background_color() {
  return background_color;
}

void Canvas::set_background_color(Gdk::RGBA _background_color) {
  background_color = _background_color;
  _signal_changed.emit();
}

void Canvas::remove_plot(unsigned int index) {
  if (plots.empty())
    throw Exception("Gtk::PLplot::Canvas::remove_plot -> No plots on canvas");

  if (index >= plots.size())
    throw Exception("Gtk::PLplot::Canvas::remove_plot -> Invalid index");

  delete plots[index];

  plots.erase(plots.begin() + index);
  _signal_changed.emit();
}

void Canvas::remove_plot(Plot &plot) {
  if (plots.empty())
    throw Exception("Gtk::PLplot::Canvas::remove_plot -> No plots on canvas");

  auto iter = std::find(plots.begin(), plots.end(), &plot);
  if (iter == plots.end())
    throw Exception("Gtk::PLplot::Canvas::remove_plot -> No match for input");

  if ((*iter)->is_managed_())
    delete *iter;

  plots.erase(iter);
  _signal_changed.emit();
}
