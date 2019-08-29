/*
Copyright (C) 2015-2019 Tom Schoonjans

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

  // ensure all arrays have the same size
  if (y.size() != z.size()) {
    throw Exception("Gtk::PLplot::PlotData3D::PlotData3D -> data arrays x, y and z must have the same size!");
  }

  // require at least 2 datapoints
  if (x.size() < 2) {
    throw Exception("Gtk::PLplot::PlotData3D::PlotData3D -> data arrays x, y and z must contain at least two datapoints");
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

void PlotData3D::replace_datapoints(const std::vector<double> &_x, const std::vector<double> &_y) {
  throw Exception("Gtk::PLplot::PlotData3D::replace_datapoints -> This method is not supported for PlotData3D");
}

void PlotData3D::replace_datapoints(const std::valarray<double> &_x, const std::valarray<double> &_y) {
  throw Exception("Gtk::PLplot::PlotData3D::replace_datapoints -> This method is not supported for PlotData3D");
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

void PlotData3D::remove_datapoint(size_t _index) {
  if (_index >= x.size()) {
    throw Exception("Gtk::PLplot::PlotData3D::remove_datapoint -> invalid index!");
  }
  x.erase(x.begin() + _index);
  y.erase(y.begin() + _index);
  z.erase(z.begin() + _index);
  _signal_data_modified.emit();
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

void PlotData3D::replace_datapoints(const std::vector<double> &_x, const std::vector<double> &_y, const std::vector<double> &_z) {
   //ensure both arrays have the same size
  if (_x.size() != _y.size() || _x.size() != _z.size()) {
    throw Exception("Gtk::PLplot::PlotData3D::replace_datapoints -> data arrays x, y and zmust have the same size!");
  }
  
  x.assign(_x.begin(), _x.end());
  y.assign(_y.begin(), _y.end());
  z.assign(_z.begin(), _z.end());

  _signal_data_modified.emit();
}

void PlotData3D::replace_datapoints(const std::valarray<double> &_x, const std::valarray<double> &_y, const std::valarray<double> &_z) {
  replace_datapoints(
    std::vector<double>(std::begin(_x), std::end(_x)),
    std::vector<double>(std::begin(_y), std::end(_y)),
    std::vector<double>(std::begin(_z), std::end(_z))
  );
}
