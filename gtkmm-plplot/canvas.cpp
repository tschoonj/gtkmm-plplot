/*
Copyright (C) 2015 Tom Schoonjans

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
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
  selected_plot(0),
  background_color(_background_color) {

  add_events(Gdk::POINTER_MOTION_MASK |
             Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  signal_changed().connect(sigc::mem_fun(*this, &Canvas::on_changed));
}

void Canvas::add_plot(Plot &plot) {
  plots.push_back(&plot);

  //ensure plot signal_changed gets re-emitted by the canvas
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

    cr->set_line_width(3);
    Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("Black"));
    cr->rectangle(MIN(start_cairo[0], end_cairo[0]) ,
                  MIN(height - start_cairo[1], height - end_cairo[1]) ,
                  fabs(end_cairo[0] - start_cairo[0]),
                  fabs(end_cairo[1] - start_cairo[1]));
    cr->stroke();
  }
  return true;
}

void Canvas::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height) {
  //start by drawing the background...
  cr->rectangle(0.0, 0.0, width, height);
  Gdk::Cairo::set_source_rgba(cr, background_color);
  cr->fill();

  //...followed by the plots
  for (auto &iter : plots) {
      iter->draw_plot(cr, width, height);
  }
}


bool Canvas::on_button_press_event(GdkEventButton *event) {
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

  //check if the starting coordinates are valid
  for (unsigned int plot = 0 ; plot < plots.size() ; plot++) {
    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(plots[plot]);
    if (region_selection != nullptr &&
        start_cairo[0] >= region_selection->cairo_range_x[0] &&
        start_cairo[0] <= region_selection->cairo_range_x[1] &&
        start_cairo[1] >= region_selection->cairo_range_y[0] &&
        start_cairo[1] <= region_selection->cairo_range_y[1] &&
        plots[plot]->is_showing() &&
        region_selection->get_region_selectable()) {

      // double-click -> zoom out!
      if (event->type == GDK_2BUTTON_PRESS) {
        double plot_data_range_x[2];
        double plot_data_range_y[2];
        region_selection->coordinate_transform_plplot_to_world(
          region_selection->plot_data_range_x[0],
          region_selection->plot_data_range_y[0],
          plot_data_range_x[0],
          plot_data_range_y[0]
        );
        region_selection->coordinate_transform_plplot_to_world(
          region_selection->plot_data_range_x[1],
          region_selection->plot_data_range_y[1],
          plot_data_range_x[1],
          plot_data_range_y[1]
        );
        region_selection->set_region(
          plot_data_range_x[0],
          plot_data_range_x[1],
          plot_data_range_y[0],
          plot_data_range_y[1]
        );

        return false;
      }
      selecting = true;
      selected_plot = plot;

      //_signal_changed.emit();
      return false;
    }
  }

  selecting = false;

  return true;
}

bool Canvas::on_button_release_event(GdkEventButton *event) {
  if (!selecting)
    return true;

  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  end_event[0] = event->x;
  end_event[1] = event->y;
  end_cairo[0] = event->x;
  end_cairo[1] = height - event->y;

  if (start_cairo[0] == end_cairo[0] && start_cairo[1] ==  end_cairo[1]) {
    selecting = false;
    return true;
  }

  RegionSelection *region_selection = dynamic_cast<RegionSelection *>(plots[selected_plot]);

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
  region_selection->convert_cairo_to_plplot_coordinates(start_cairo[0], start_cairo[1],
                  start_plplot[0], start_plplot[1]);
  region_selection->convert_cairo_to_plplot_coordinates(end_cairo[0], end_cairo[1],
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

  region_selection->_signal_select_region.emit(start_plplot_def[0], end_plplot_def[0], start_plplot_def[1], end_plplot_def[1]);

  return true;
}

bool Canvas::on_motion_notify_event (GdkEventMotion *event) {
  Gtk::Allocation allocation = get_allocation();
  const int height = allocation.get_height();

  end_event[0] = event->x;
  end_event[1] = event->y;
  end_cairo[0] = event->x;
  end_cairo[1] = height - event->y;

  //emit signal for new cursor coordinates
  bool cursor_checked = false;
  for (unsigned int plot = 0 ; plot < plots.size() ; plot++) {
    RegionSelection *region_selection = dynamic_cast<RegionSelection *>(plots[plot]);

    if (region_selection != nullptr &&
        end_cairo[0] >= region_selection->cairo_range_x[0] &&
        end_cairo[0] <= region_selection->cairo_range_x[1] &&
        end_cairo[1] >= region_selection->cairo_range_y[0] &&
        end_cairo[1] <= region_selection->cairo_range_y[1] &&
        plots[plot]->is_showing()) {
      //std::cout << "on_motion_notify_event plot " << plot << std::endl;
      double cursor_pl_x, cursor_pl_y;
      double cursor_x, cursor_y;
      region_selection->convert_cairo_to_plplot_coordinates(
        end_cairo[0],
        end_cairo[1],
        cursor_pl_x,
        cursor_pl_y
      );
      region_selection->coordinate_transform_plplot_to_world(
        cursor_pl_x,
        cursor_pl_y,
        cursor_x,
        cursor_y
      );
      region_selection->_signal_cursor_motion(cursor_x, cursor_y);

      // change cursor to crosshair or system default if necessary
      const Glib::RefPtr<Gdk::Window> window = get_window();
      if (window && !window->get_cursor()) {
        Glib::RefPtr<Gdk::Cursor> crosshair = Gdk::Cursor::create(window->get_display(), "crosshair");
        window->set_cursor(crosshair);
      }

      cursor_checked = true;

      break;
    }
  }

  if (!cursor_checked && get_window()) {
    get_window()->set_cursor(Glib::RefPtr<Gdk::Cursor>(0));
  }

  // if not dragging a selection box, stop here
  if (!selecting)
    return true;

  RegionSelection *region_selection = dynamic_cast<RegionSelection *>(plots[selected_plot]);

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

Plot *Canvas::get_plot(unsigned int index) {
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
