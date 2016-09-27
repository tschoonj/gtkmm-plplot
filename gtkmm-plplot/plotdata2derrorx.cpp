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

#include <gtkmm-plplot/plotdata2derrorx.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>

using namespace Gtk::PLplot;

PlotData2DErrorX::PlotData2DErrorX(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  const std::vector<double> &_errorx_low,
  const std::vector<double> &_errorx_high,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPLplotPlotData2DErrorX"),
  PlotData2D(_x, _y, _color, _line_style, _line_width),
  errorx_low(_errorx_low),
  errorx_high(_errorx_high),
  errorx_color(_color) {

  //ensure all arrays have the same size
  if (y.size() != _errorx_low.size() || y.size() != _errorx_high.size()) {
    throw Exception("Gtk::PLplot::PlotData2DErrorX::PlotData2DErrorX -> data arrays x, y, errorx_low and errorx_high must have the same size!");
  }

  //ensure that the errorx_low values are greater than x
  for (int i = 0 ; i < x.size() ; i++) {
    if (errorx_low[i] > x[i])
      throw Exception("Gtk::PLplot::PlotData2DErrorX::PlotData2DErrorX -> errorx_low elements must be less than the corresponding elements in x");
    else if (errorx_high[i] < x[i])
      throw Exception("Gtk::PLplot::PlotData2DErrorX::PlotData2DErrorX -> errorx_high elements must be greater than the corresponding elements in x");
  }
}

PlotData2DErrorX::PlotData2DErrorX(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  const std::valarray<double> &_errorx_low,
  const std::valarray<double> &_errorx_high,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotData2DErrorX(
    std::vector<double>(std::begin(_x), std::end(_x)),
    std::vector<double>(std::begin(_y), std::end(_y)),
    std::vector<double>(std::begin(_errorx_low), std::end(_errorx_low)),
    std::vector<double>(std::begin(_errorx_high), std::end(_errorx_high)),
    _color, _line_style, _line_width) {}

PlotData2DErrorX::~PlotData2DErrorX() {}

void PlotData2DErrorX::add_datapoint(double _x, double _y) {
  throw Exception("Gtk::PLplot::PlotData2DErrorX::add_datapoint -> This method is not supported for PlotData2DErrorX");
}

void PlotData2DErrorX::add_datapoint(std::pair<double, double> _xy_pair) {
  throw Exception("Gtk::PLplot::PlotData2DErrorX::add_datapoint -> This method is not supported for PlotData2DErrorX");
}

std::vector<double> PlotData2DErrorX::get_vector_error_x_low() {
  return errorx_low;
}

std::vector<double> PlotData2DErrorX::get_vector_error_x_high() {
  return errorx_high;
}

void PlotData2DErrorX::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  xmin = *std::min_element(errorx_low.begin(), errorx_low.end());
  xmax = *std::max_element(errorx_high.begin(), errorx_high.end());
  ymin = *std::min_element(y.begin(), y.end());
  ymax = *std::max_element(y.begin(), y.end());
}

void PlotData2DErrorX::set_error_x_color(Gdk::RGBA _errorx_color) {
  if (_errorx_color == errorx_color)
    return;
  errorx_color = _errorx_color;
  _signal_changed.emit();
}

Gdk::RGBA PlotData2DErrorX::get_error_x_color() {
  return errorx_color;
}

void PlotData2DErrorX::add_datapoint(double xval, double yval, double xval_error_low, double xval_error_high) {
  // sanity check
  if (xval_error_low > xval)
    throw Exception("Gtk::PLplot::PlotData2DErrorX::add_datapoint -> xval_error_low elements must be less than xval");
  else if (xval_error_high < xval)
    throw Exception("Gtk::PLplot::PlotData2DErrorX::add_datapoint -> xval_error_high elements must be greater than xval");

  x.push_back(xval);
  y.push_back(yval);
  errorx_low.push_back(xval_error_low);
  errorx_high.push_back(xval_error_high);
  _signal_data_modified.emit();
}

void PlotData2DErrorX::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  double *x_pl = &x[0], *y_pl = &y[0];
  double *errorx_low_pl = &errorx_low[0];
  double *errorx_high_pl = &errorx_high[0];

  // plot the line if requested
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->line(x.size(), x_pl, y_pl);
  }

  // plot the x errorbars
  change_plstream_color(pls, errorx_color);
  // need to see what happens with the errorbars when changing the line_width and line_style
  pls->errx(x.size(), errorx_low_pl, errorx_high_pl, y_pl);

  // plot the symbols if requested
  if (!symbol.empty()) {
    change_plstream_color(pls, symbol_color);
    pls->schr(0, symbol_scale_factor);
    pls->string(x.size(), x_pl, y_pl, symbol.c_str());
  }
}
