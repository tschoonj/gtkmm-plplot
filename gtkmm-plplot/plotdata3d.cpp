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

#include <gtkmm-plplot/plotdata3d.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>

using namespace Gtk::PLplot;

PlotData3D::PlotData3D(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  const std::vector<double> &_z,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPLplotPlotData3D"),
  PlotData2D(_x, _y, _color, _line_style, _line_width),
  z(_z) {

  //ensure all arrays have the same size
  if (y.size() != z.size()) {
    throw Exception("Gtk::PLplot::PlotData3D::PlotData3D -> data arrays x, y and z must have the same size!");
  }
}

PlotData3D::PlotData3D(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  const std::valarray<double> &_z,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotData3D(std::vector<double>(std::begin(_x), std::end(_x)),
  std::vector<double>(std::begin(_y), std::end(_y)),
  std::vector<double>(std::begin(_z), std::end(_z)),
  _color, _line_style, _line_width) {}

PlotData3D::~PlotData3D() {}

void PlotData3D::add_datapoint(double _x, double _y) {
  throw Exception("Gtk::PLplot::PlotData3D::add_datapoint -> This method is not supported for PlotData3D");
}

void PlotData3D::add_datapoint(std::pair<double, double> _xy_pair) {
  throw Exception("Gtk::PLplot::PlotData3D::add_datapoint -> This method is not supported for PlotData3D");
}

void PlotData3D::add_datapoint(double _x, double _y, double _z) {
  x.push_back(_x);
  y.push_back(_y);
  z.push_back(_z);
  _signal_data_modified.emit();
}

void PlotData3D::add_datapoint(std::tuple<double, double, double> _xyz) {
  add_datapoint(std::get<0>(_xyz), std::get<1>(_xyz), std::get<2>(_xyz));
}

std::vector<double> PlotData3D::get_vector_z() {
  return z;
}

void PlotData3D::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  double *x_pl = &x[0], *y_pl = &y[0], *z_pl = &z[0];

  // plot the line if requested
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->line3(x.size(), x_pl, y_pl, z_pl);
  }

  // plot the symbols if requested
  if (!symbol.empty()) {
    change_plstream_color(pls, symbol_color);
    pls->schr(0, symbol_scale_factor);
    pls->string3(x.size(), x_pl, y_pl, z_pl, symbol.c_str());
  }
}
