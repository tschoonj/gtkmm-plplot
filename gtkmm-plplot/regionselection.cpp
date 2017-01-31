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

#include <gtkmm-plplot/regionselection.h>
#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/exception.h>
#include <plstream.h>

using namespace Gtk::PLplot;

RegionSelection::RegionSelection() :
  region_selectable(true),
  region_zoomable(true),
  region_pannable(true),
  region_zoom_scale_factor(2.0),
  region_selection_color(Gdk::RGBA("Black")),
  region_selection_width(2.0),
  plotted_range_x{0.1, 1.0},
  plotted_range_y{0.1, 1.0},
  plot_data_range_x{0.1, 1.0},
  plot_data_range_y{0.1, 1.0} {
  //connect our default signal handlers
  this->signal_select_region().connect(sigc::mem_fun(*this, &RegionSelection::on_select_region));
  this->signal_cursor_motion().connect(sigc::mem_fun(*this, &RegionSelection::on_cursor_motion));
  this->signal_double_press().connect(sigc::mem_fun(*this, &RegionSelection::on_double_press));
  this->signal_zoom_region().connect(sigc::mem_fun(*this, &RegionSelection::on_zoom_region));
  this->signal_pan().connect(sigc::mem_fun(*this, &RegionSelection::on_pan));
}

RegionSelection::~RegionSelection() {}

void RegionSelection::on_select_region(double xmin, double xmax, double ymin, double ymax) {
  //default implementation -> override in a derived class to get a different behavior
  set_region(xmin, xmax, ymin, ymax);
}

std::vector<double> RegionSelection::on_pan(double old_x, double old_y, double new_x, double new_y) {
  double delta_x = new_x - old_x;
  double delta_y = new_y - old_y;

  if (delta_x == 0.0 && delta_y == 0.0)
    return std::vector<double>{old_x, old_y, new_x, new_y};

  // transform old and new coords to cairo
  double cairo_old_x;
  double cairo_old_y;
  double cairo_new_x;
  double cairo_new_y;

  coordinate_transform_world_to_cairo(old_x, old_y, cairo_old_x, cairo_old_y);
  coordinate_transform_world_to_cairo(new_x, new_y, cairo_new_x, cairo_new_y);

  double cairo_delta_x = cairo_new_x - cairo_old_x;
  double cairo_delta_y = cairo_new_y - cairo_old_y;

  double new_world_plotted_range_x[2];
  double new_world_plotted_range_y[2];

  coordinate_transform_cairo_to_world(
    cairo_range_x[0] - cairo_delta_x,
    cairo_range_y[0] - cairo_delta_y,
    new_world_plotted_range_x[0],
    new_world_plotted_range_y[0]
  );

  coordinate_transform_cairo_to_world(
    cairo_range_x[1] - cairo_delta_x,
    cairo_range_y[1] - cairo_delta_y,
    new_world_plotted_range_x[1],
    new_world_plotted_range_y[1]
  );

  set_region(
    new_world_plotted_range_x[0], new_world_plotted_range_x[1],
    new_world_plotted_range_y[0], new_world_plotted_range_y[1]
  );

  return std::vector<double>{old_x - delta_x, old_y - delta_y, new_x - delta_x, new_y - delta_y};
}

void RegionSelection::on_cursor_motion(double x, double y) {
  //does nothing: write your own handler or override the method
  //std::cout << "x: " << x << " y: " << y << std::endl;
}

void RegionSelection::on_double_press(double x, double y) {
  //default implementation -> override in a derived class to get a different behavior
  double temp_x[2];
  double temp_y[2];

  coordinate_transform_plplot_to_world(
    plot_data_range_x[0],
    plot_data_range_y[0],
    temp_x[0],
    temp_y[0]
  );
  coordinate_transform_plplot_to_world(
    plot_data_range_x[1],
    plot_data_range_y[1],
    temp_x[1],
    temp_y[1]
  );
  set_region(
    temp_x[0],
    temp_x[1],
    temp_y[0],
    temp_y[1]
  );
}

void RegionSelection::on_zoom_region(double x, double y, GdkScrollDirection direction) {
  //default implementation -> override in a derived class to get a different behavior
  double effective_scale_factor;

  // calculate effective_scale_factor based on scroll direction
  if (direction == GDK_SCROLL_UP) {
    effective_scale_factor = 1.0 / region_zoom_scale_factor;
  }
  else if (direction == GDK_SCROLL_DOWN) {
    effective_scale_factor = region_zoom_scale_factor;
  }
  else {
    // ignore other directions for now
    return;
  }
  // get cairo coordinates of (x,y)
  double cairo_x, cairo_y;
  coordinate_transform_world_to_cairo(x, y, cairo_x, cairo_y);

  double x_left = cairo_x - cairo_range_x[0];
  double x_right = cairo_range_x[1] - cairo_x;
  double y_bottom = cairo_y - cairo_range_y[0];
  double y_top = cairo_range_y[1] - cairo_y;

  double cairo_range_new_x[2];
  double cairo_range_new_y[2];

  // calculate new cairo coordinates...
  cairo_range_new_x[0] = cairo_x - x_left * effective_scale_factor;
  cairo_range_new_x[1] = cairo_x + x_right * effective_scale_factor;
  cairo_range_new_y[0] = cairo_y - y_bottom * effective_scale_factor;
  cairo_range_new_y[1] = cairo_y + y_top * effective_scale_factor;

  double world_range_new_x[2];
  double world_range_new_y[2];

  // ...and transform into world coordinates
  coordinate_transform_cairo_to_world(cairo_range_new_x[0], cairo_range_new_y[0], world_range_new_x[0], world_range_new_y[0]);
  coordinate_transform_cairo_to_world(cairo_range_new_x[1], cairo_range_new_y[1], world_range_new_x[1], world_range_new_y[1]);

  // call set_region with the new world coordinates
  set_region(
    world_range_new_x[0],
    world_range_new_x[1],
    world_range_new_y[0],
    world_range_new_y[1]
  );
}

bool RegionSelection::get_region_selectable() {
  return region_selectable;
}

void RegionSelection::set_region_selectable(bool _region_selectable) {
  region_selectable = _region_selectable;
}

bool RegionSelection::get_region_zoomable() {
  return region_zoomable;
}

void RegionSelection::set_region_zoomable(bool _region_zoomable) {
  region_zoomable = _region_zoomable;
}

bool RegionSelection::get_region_pannable() {
  return region_pannable;
}

void RegionSelection::set_region_pannable(bool _region_pannable) {
  region_pannable = _region_pannable;
}

double RegionSelection::get_region_zoom_scale_factor() {
  return region_zoom_scale_factor;
}

void RegionSelection::set_region_zoom_scale_factor(double scale_factor) {
  if (scale_factor == 1.0 || scale_factor <= 0.0) {
    throw Exception("Gtk::PLplot::RegionSelection::set_region_zoom_scale_factor -> invalid scale_factor provided");
  }
  region_zoom_scale_factor = scale_factor;
}

Gdk::RGBA RegionSelection::get_region_selection_color() {
  return region_selection_color;
}

void RegionSelection::set_region_selection_color(Gdk::RGBA _region_selection_color) {
  region_selection_color = _region_selection_color;
}

double RegionSelection::get_region_selection_width() {
  return region_selection_width;
}

void RegionSelection::set_region_selection_width(double _region_selection_width) {
  if (_region_selection_width <= 0.0) {
    throw Exception("Gtk::PLplot::RegionSelection::set_region_selection_width -> invalid width provided");
  }
  region_selection_width = _region_selection_width;
}

void RegionSelection::coordinate_transform_plplot_to_cairo(
  double x_pl, double y_pl,
  double &x_cr, double &y_cr) {
    //inspired by http://www.mail-archive.com/plplot-devel@lists.sourceforge.net/msg02383.html
    //but the last equation was incorrect and is fixed here
    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    Plot* plot = dynamic_cast<Plot *>(this);

    if (plot == nullptr)
      throw Exception("Gtk::PLplot::RegionSelection::coordinate_transform_plplot_to_cairo -> could not dynamic_cast this to Plot*");

    plot->pls->gvpd(nxmin, nxmax, nymin, nymax);
    plot->pls->gvpw(wxmin, wxmax, wymin, wymax);

    double xmin = plot->plot_width * nxmin + plot->plot_offset_x;
    double xmax = plot->plot_width * nxmax + plot->plot_offset_x;
    double ymin = plot->plot_height * (nymin - 1.0) + plot->canvas_height - plot->plot_offset_y;
    double ymax = plot->plot_height * (nymax - 1.0) + plot->canvas_height - plot->plot_offset_y;

    x_cr = xmin + ((xmax - xmin) * ((x_pl - wxmin) / (wxmax - wxmin)));
    y_cr = ymin + ((ymax - ymin) * ((y_pl - wymin) / (wymax - wymin)));
}

void RegionSelection::coordinate_transform_cairo_to_plplot(
  double x_cr, double y_cr,
  double &x_pl, double &y_pl) {

    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    Plot* plot = dynamic_cast<Plot *>(this);

    if (plot == nullptr)
      throw Exception("Gtk::PLplot::RegionSelection::coordinate_transform_plplot_to_cairo -> could not dynamic_cast this to Plot*");

    plot->pls->gvpd(nxmin, nxmax, nymin, nymax);
    plot->pls->gvpw(wxmin, wxmax, wymin, wymax);

    double xmin = plot->plot_width * nxmin + plot->plot_offset_x;
    double xmax = plot->plot_width * nxmax + plot->plot_offset_x;
    double ymin = plot->plot_height * (nymin - 1.0) + plot->canvas_height - plot->plot_offset_y;
    double ymax = plot->plot_height * (nymax - 1.0) + plot->canvas_height - plot->plot_offset_y;

    x_pl = wxmin + ((x_cr - xmin) * (wxmax - wxmin) / (xmax - xmin));
    y_pl = wymin + ((y_cr - ymin) * (wymax - wymin) / (ymax - ymin));
}

void RegionSelection::coordinate_transform_world_to_plplot(double x_old, double y_old, double &x_new, double &y_new) {
  x_new = x_old;
  y_new = y_old;
}

void RegionSelection::coordinate_transform_plplot_to_world(double x_old, double y_old, double &x_new, double &y_new) {
  x_new = x_old;
  y_new = y_old;
}

void RegionSelection::coordinate_transform_world_to_cairo(double x_old, double y_old, double &x_new, double &y_new) {
  double x_temp, y_temp;

  coordinate_transform_world_to_plplot(x_old, y_old, x_temp, y_temp);
  coordinate_transform_plplot_to_cairo(x_temp, y_temp, x_new, y_new);
}

void RegionSelection::coordinate_transform_cairo_to_world(double x_old, double y_old, double &x_new, double &y_new) {
  double x_temp, y_temp;

  coordinate_transform_cairo_to_plplot(x_old, y_old, x_temp, y_temp);
  coordinate_transform_plplot_to_world(x_temp, y_temp, x_new, y_new);
}

void RegionSelection::get_region(double &xmin, double &xmax, double &ymin, double &ymax) {
  coordinate_transform_plplot_to_world(plotted_range_x[0], plotted_range_y[0], xmin, ymin);
  coordinate_transform_plplot_to_world(plotted_range_x[1], plotted_range_y[1], xmax, ymax);
}

void RegionSelection::set_region(double xmin, double xmax, double ymin, double ymax) {
  if (xmin == xmax || ymin == ymax) {
    // due to signal propagation, this function will actually be called twice on a double-click event,
    // the second time after the plot has already been resized to its normal geometry
    // this condition avoids the warning message...
    // it also blocks zero width or length selection boxes, which would otherwise lead to the exception being thrown
    return;
  }

  coordinate_transform_world_to_plplot(
    xmin, ymin,
    xmin, ymin
  );
  coordinate_transform_world_to_plplot(
    xmax, ymax,
    xmax, ymax
  );

  if (xmin >= xmax || ymin >= ymax /*||
      xmin < plot_data_range_x[0] ||
      xmax > plot_data_range_x[1] ||
      ymin < plot_data_range_y[0] ||
      ymax > plot_data_range_y[1]*/) {
    throw Exception("Gtk::PLplot::RegionSelection::set_region -> Invalid arguments");
  }
  plotted_range_x[0] = xmin;
  plotted_range_x[1] = xmax;
  plotted_range_y[0] = ymin;
  plotted_range_y[1] = ymax;

  dynamic_cast<Plot *>(this)->signal_changed().emit();
}
