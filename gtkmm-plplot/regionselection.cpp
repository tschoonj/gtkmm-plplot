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

#include <gtkmm-plplot/regionselection.h>
#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/exception.h>
#include <iostream>

using namespace Gtk::PLplot;

RegionSelection::RegionSelection() :
  region_selectable(true) {
  //connect our default signal handlers
  this->signal_select_region().connect(sigc::mem_fun(*this, &RegionSelection::on_select_region));
  this->signal_cursor_motion().connect(sigc::mem_fun(*this, &RegionSelection::on_cursor_motion));
  this->signal_double_press().connect(sigc::mem_fun(*this, &RegionSelection::on_double_press));
}

void RegionSelection::on_select_region(double xmin, double xmax, double ymin, double ymax) {
  set_region(xmin, xmax, ymin, ymax);
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

bool RegionSelection::get_region_selectable() {
  return region_selectable;
}

void RegionSelection::set_region_selectable(bool _region_selectable) {
  region_selectable = _region_selectable;
}

void RegionSelection::convert_plplot_to_cairo_coordinates(
  double x_pl, double y_pl,
  double &x_cr, double &y_cr) {
    //inspired by http://www.mail-archive.com/plplot-devel@lists.sourceforge.net/msg02383.html
    //but the last equation was incorrect and is fixed here
    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    Plot* plot = dynamic_cast<Plot *>(this);

    if (plot == nullptr)
      throw Exception("Gtk::PLplot::RegionSelection::convert_plplot_to_cairo_coordinates -> could not dynamic_cast this to Plot*");

    plot->pls->gvpd(nxmin, nxmax, nymin, nymax);
    plot->pls->gvpw(wxmin, wxmax, wymin, wymax);

    double xmin = plot->plot_width * nxmin + plot->plot_offset_x;
    double xmax = plot->plot_width * nxmax + plot->plot_offset_x;
    double ymin = plot->plot_height * (nymin - 1.0) + plot->canvas_height - plot->plot_offset_y;
    double ymax = plot->plot_height * (nymax - 1.0) + plot->canvas_height - plot->plot_offset_y;

    x_cr = xmin + ((xmax - xmin) * ((x_pl - wxmin) / (wxmax - wxmin)));
    y_cr = ymin + ((ymax - ymin) * ((y_pl - wymin) / (wymax - wymin)));
}

void RegionSelection::convert_cairo_to_plplot_coordinates(
  double x_cr, double y_cr,
  double &x_pl, double &y_pl) {

    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    Plot* plot = dynamic_cast<Plot *>(this);

    if (plot == nullptr)
      throw Exception("Gtk::PLplot::RegionSelection::convert_plplot_to_cairo_coordinates -> could not dynamic_cast this to Plot*");

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

void RegionSelection::set_region(double xmin, double xmax, double ymin, double ymax) {
  if (xmin == xmax && ymin == ymax) {
    //due to signal propagation, this function will actually be called twice on a double-click event,
    //the second time after the plot has already been resized to its normal geometry
    //this condition avoids the warning message...
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

  dynamic_cast<Plot *>(this)->_signal_changed.emit();
}
