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

#include <gtkmm-plplot/plotdata2d.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>

using namespace Gtk::PLplot;

PlotData2D::PlotData2D(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPLplotPlotData2D"),
  PlotDataLine(_color, _line_style, _line_width),
  x(_x), y(_y),
  symbol(""), symbol_color(Gdk::RGBA("Red")),
  symbol_scale_factor(1.0) {

   //ensure both arrays have the same size
  if (x.size() != y.size()) {
    throw Exception("Gtk::PLplot::PlotData2D::PlotData2D -> data arrays x and y must have the same size!");
  }
  //ensure there are at least two elements in the arrays
  if (x.size() < 2) {
    throw Exception("Gtk::PLplot::PlotData2D::PlotData2D -> data arrays x and y must have at least two elements");
  }
}

PlotData2D::PlotData2D(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotData2D(std::vector<double>(std::begin(_x), std::end(_x)),
  std::vector<double>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

PlotData2D::PlotData2D(
  const std::vector<double> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotData2D(indgen(_y.size()), _y, _color, _line_style, _line_width) {}

PlotData2D::PlotData2D(
  const std::valarray<double> &_y,
  Gdk::RGBA _color, LineStyle _line_style,
  double _line_width) :
  PlotData2D(std::vector<double>(indgen(_y.size())),
  std::vector<double>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

PlotData2D::~PlotData2D() {}

void PlotData2D::set_symbol(Glib::ustring _symbol) {
  symbol = _symbol;
  _signal_changed.emit();
}

Glib::ustring PlotData2D::get_symbol() {
  return symbol;
}

void PlotData2D::set_symbol_color(Gdk::RGBA _color) {
  symbol_color = _color;
  _signal_changed.emit();
}

Gdk::RGBA PlotData2D::get_symbol_color() {
  return symbol_color;
}

void PlotData2D::set_symbol_height_scale_factor(double _scale_factor) {
  if (_scale_factor <= 0.0) {
    throw Exception("Gtk::PLplot::PlotData2D::set_symbol_height_scale_factor -> scale factor must be strictly positive");
  }
  symbol_scale_factor = _scale_factor;
  _signal_changed.emit();
}

double PlotData2D::get_symbol_height_scale_factor() {
  return symbol_scale_factor;
}

void PlotData2D::add_datapoint(double _x, double _y) {
  x.push_back(_x);
  y.push_back(_y);
  _signal_data_modified.emit();
}

void PlotData2D::add_datapoint(std::pair<double, double> _xy_pair) {
  add_datapoint(_xy_pair.first, _xy_pair.second);
}

void PlotData2D::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  double *x_pl = &x[0], *y_pl = &y[0];

  // plot the line if requested
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->line(x.size(), x_pl, y_pl);
  }

  // plot the symbols if requested
  if (!symbol.empty()) {
    change_plstream_color(pls, symbol_color);
    pls->schr(0, symbol_scale_factor);
    pls->string(x.size(), x_pl, y_pl, symbol.c_str());
  }
}

std::vector<double> PlotData2D::get_vector_x() {
  return x;
}

std::vector<double> PlotData2D::get_vector_y() {
  return y;
}

void PlotData2D::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  xmin = *std::min_element(x.begin(), x.end());
  xmax = *std::max_element(x.begin(), x.end());
  ymin = *std::min_element(y.begin(), y.end());
  ymax = *std::max_element(y.begin(), y.end());
}
