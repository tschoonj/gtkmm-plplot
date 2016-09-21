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
  background_color(_background_color) {

  add_events(Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_RELEASE_MASK |
             Gdk::SCROLL_MASK |
             Gdk::KEY_PRESS_MASK |
             Gdk::KEY_RELEASE_MASK
           );

  set_can_focus(true);

  signal_changed().connect(sigc::mem_fun(*this, &Canvas::on_changed));
}

bool Canvas::on_scroll_event(GdkEventScroll *event) {
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  double start_event[2];
  double start_cairo[2];

  start_event[0] = event->x;
  start_event[1] = event->y;
  start_cairo[0] = event->x;
  start_cairo[1] = height - event->y;

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
      region_selection->signal_zoom_region().emit(cursor_x, cursor_y, event->direction);
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

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

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

  return true;
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


bool Canvas::on_button_press_event(GdkEventButton *event) {
  if (event->button != GDK_BUTTON_PRIMARY) {
    return false;
  }
  grab_focus();
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  start_event[0] = event->x;
  start_event[1] = event->y;
  start_cairo[0] = event->x;
  start_cairo[1] = height - event->y;
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
      if (event->type == GDK_2BUTTON_PRESS) {
        //convert event coordinates to plot coordinates
        double cursor_x, cursor_y;

        region_selection->coordinate_transform_cairo_to_world(
          start_cairo[0],
          start_cairo[1],
          cursor_x,
          cursor_y
        );

        region_selection->signal_double_press().emit(cursor_x, cursor_y);

        return true;
      }
      selecting = true;
      selected_plot = inside_plot;

      return true;
    }
    else if (region_selection != nullptr &&
      event->type == GDK_BUTTON_PRESS &&
      region_selection->get_region_pannable() &&
      shift_pressed == true) {
      left_mouse_button_clicked = true;

      //change cursor if appropriate
      if (get_window()) {
          Glib::RefPtr<Gdk::Cursor> grabbing = Gdk::Cursor::create(get_window()->get_display(), "grabbing");
          get_window()->set_cursor(grabbing);
      }
      return true;
    }
  }

  selecting = false;
  left_mouse_button_clicked = false;

  return false;
}

bool Canvas::on_button_release_event(GdkEventButton *event) {
  if (event->button != GDK_BUTTON_PRIMARY) {
    return false;
  }
  left_mouse_button_clicked = false;
  if (shift_pressed) {
    if (get_window()) {
      Glib::RefPtr<Gdk::Cursor> grab= Gdk::Cursor::create(get_window()->get_display(), "grab");
      get_window()->set_cursor(grab);
    }
    return false;
  }
  else if (!selecting)
    return false;

  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  end_event[0] = event->x;
  end_event[1] = event->y;
  end_cairo[0] = event->x;
  end_cairo[1] = height - event->y;

  if (start_cairo[0] == end_cairo[0] && start_cairo[1] ==  end_cairo[1]) {
    selecting = false;
    return false;
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
  start_plplot_def[0] = MAX(MIN(start_plplot[0], end_plplot[0]), region_selection->plot_data_range_x[0]);
  start_plplot_def[1] = MAX(MIN(start_plplot[1], end_plplot[1]), region_selection->plot_data_range_y[0]);
  end_plplot_def[0] = MIN(MAX(start_plplot[0], end_plplot[0]), region_selection->plot_data_range_x[1]);
  end_plplot_def[1] = MIN(MAX(start_plplot[1], end_plplot[1]), region_selection->plot_data_range_y[1]);

  //this is necessary to get rid of the box on the plot, even if the signal_select_region is not caught by the plot
  _signal_changed.emit();

  region_selection->coordinate_transform_plplot_to_world(
    start_plplot_def[0],
    start_plplot_def[1],
    start_plplot_def[0],
    start_plplot_def[1]
  );

  region_selection->coordinate_transform_plplot_to_world(
    end_plplot_def[0],
    end_plplot_def[1],
    end_plplot_def[0],
    end_plplot_def[1]
  );

  region_selection->signal_select_region().emit(start_plplot_def[0], end_plplot_def[0], start_plplot_def[1], end_plplot_def[1]);

  return true;
}

bool Canvas::on_motion_notify_event (GdkEventMotion *event) {
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  end_event[0] = event->x;
  end_event[1] = event->y;
  end_cairo[0] = event->x;
  end_cairo[1] = height - event->y;

  //inside_plot = nullptr;

  //emit signal for new cursor coordinates
  bool cursor_checked = false;
  for (auto plot = plots.rbegin() ; plot != plots.rend() ; ++plot) {
    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(*plot);

    if (region_selection != nullptr &&
        end_cairo[0] >= region_selection->cairo_range_x[0] &&
        end_cairo[0] <= region_selection->cairo_range_x[1] &&
        end_cairo[1] >= region_selection->cairo_range_y[0] &&
        end_cairo[1] <= region_selection->cairo_range_y[1] &&
        (*plot)->is_showing()) {
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
      }

      inside_plot = *plot;

      // change cursor to crosshair or system default if necessary
      const Glib::RefPtr<Gdk::Window> window = get_window();
      if (window) {
        if (region_selection->get_region_pannable() && left_mouse_button_clicked && shift_pressed) {
          Glib::RefPtr<Gdk::Cursor> grabbing = Gdk::Cursor::create(window->get_display(), "grabbing");
          window->set_cursor(grabbing);
          std::vector<double> rv = region_selection->signal_pan().emit(inside_plot_current_coords[0], inside_plot_current_coords[1], cursor_x, cursor_y);
          cursor_x = rv[2];
          cursor_y = rv[3];
        }
        else if (region_selection->get_region_pannable() && shift_pressed) {
          Glib::RefPtr<Gdk::Cursor> grab = Gdk::Cursor::create(window->get_display(), "grab");
          window->set_cursor(grab);
        }
        else {
          Glib::RefPtr<Gdk::Cursor> crosshair = Gdk::Cursor::create(window->get_display(), "crosshair");
          window->set_cursor(crosshair);
        }
      }

      // this should be emitted after signal_pan(), as cursor_x and cursor_y may have changed
      region_selection->signal_cursor_motion().emit(cursor_x, cursor_y);

      cursor_checked = true;

      inside_plot_current_coords[0] = cursor_x;
      inside_plot_current_coords[1] = cursor_y;

      break;
    }
  }

  if (!cursor_checked) {
    inside_plot = nullptr;
    if (get_window()) {
      get_window()->set_cursor(Glib::RefPtr<Gdk::Cursor>(0));
    }
  }

  // if not dragging a selection box, stop here
  if (!selecting)
    return false;

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

  return true;
}

bool Canvas::on_key_press_event(GdkEventKey *event) {

  if (inside_plot == nullptr || selecting)
    return false;

  RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);

  if (region_selection != nullptr &&
    region_selection->get_region_pannable() &&
    (event->keyval == GDK_KEY_Shift_L || event->keyval == GDK_KEY_Shift_R)
    ) {
    shift_pressed = true;

    if (get_window()) {
      if (left_mouse_button_clicked) {
        Glib::RefPtr<Gdk::Cursor> grabbing = Gdk::Cursor::create(get_window()->get_display(), "grabbing");
        get_window()->set_cursor(grabbing);
      }
      else {
        Glib::RefPtr<Gdk::Cursor> grab= Gdk::Cursor::create(get_window()->get_display(), "grab");
        get_window()->set_cursor(grab);
      }
    }

    return true;
  }

  return false;
}

bool Canvas::on_key_release_event(GdkEventKey *event) {
  if (event->keyval == GDK_KEY_Shift_L || event->keyval == GDK_KEY_Shift_R) {
    shift_pressed = false;
    if (inside_plot != nullptr) {
      RegionSelection *region_selection = dynamic_cast<RegionSelection *>(inside_plot);
      if (region_selection != nullptr) {
        if (get_window()) {
          Glib::RefPtr<Gdk::Cursor> crosshair = Gdk::Cursor::create(get_window()->get_display(), "crosshair");
          get_window()->set_cursor(crosshair);
        }
        return true;
      }
    }
    if (get_window()) {
      get_window()->set_cursor(Glib::RefPtr<Gdk::Cursor>(0));
    }

    return true;
  }


  return false;
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
