/*
Copyright (C) 2016 Tom Schoonjans

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

#include <gtkmm-plplot/plotdatapolar.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>

using namespace Gtk::PLplot;

PlotDataPolar::PlotDataPolar(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPLplotPlotDataPolar"),
  PlotData2D(_x, _y, _color, _line_style, _line_width) {
}

PlotDataPolar::PlotDataPolar(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotDataPolar(std::vector<double>(std::begin(_x), std::end(_x)),
  std::vector<double>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

PlotDataPolar::~PlotDataPolar() {}

std::vector<double> PlotDataPolar::get_vector_r() {
  return x;
}

std::vector<double> PlotDataPolar::get_vector_theta() {
  return y;
}

void PlotDataPolar::get_max_r(double &rmax) {
  rmax = *std::max_element(x.begin(), x.end());
}
