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

#include <gtkmm-plplot/plotdata2derrorxy.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>

using namespace Gtk::PLplot;

PlotData2DErrorXY::PlotData2DErrorXY(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  const std::vector<double> &_errorx_low,
  const std::vector<double> &_errorx_high,
  const std::vector<double> &_errory_low,
  const std::vector<double> &_errory_high,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPLplotPlotData2DErrorXY"),
  PlotData2D(_x, _y, _color, _line_style, _line_width),
  PlotData2DErrorX(_x, _y, _errorx_low, _errorx_high, _color, _line_style, _line_width),
  PlotData2DErrorY(_x, _y, _errory_low, _errory_high, _color, _line_style, _line_width)
   {}

PlotData2DErrorXY::PlotData2DErrorXY(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  const std::valarray<double> &_errorx_low,
  const std::valarray<double> &_errorx_high,
  const std::valarray<double> &_errory_low,
  const std::valarray<double> &_errory_high,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotData2DErrorXY(
    std::vector<double>(std::begin(_x), std::end(_x)),
    std::vector<double>(std::begin(_y), std::end(_y)),
    std::vector<double>(std::begin(_errorx_low), std::end(_errorx_low)),
    std::vector<double>(std::begin(_errorx_high), std::end(_errorx_high)),
    std::vector<double>(std::begin(_errory_low), std::end(_errory_low)),
    std::vector<double>(std::begin(_errory_high), std::end(_errory_high)),
    _color, _line_style, _line_width) {}

PlotData2DErrorXY::~PlotData2DErrorXY() {}

void PlotData2DErrorXY::add_datapoint(double _x, double _y) {
  throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> This method is not supported for PlotData2DErrorXY");
}

void PlotData2DErrorXY::add_datapoint(std::pair<double, double> _xy_pair) {
  throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> This method is not supported for PlotData2DErrorXY");
}

void PlotData2DErrorXY::add_datapoint(double _x, double _y, double _v, double _w) {
  throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> This method is not supported for PlotData2DErrorXY");
}

void PlotData2DErrorXY::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  xmin = *std::min_element(errorx_low.begin(), errorx_low.end());
  xmax = *std::max_element(errorx_high.begin(), errorx_high.end());
  ymin = *std::min_element(errory_low.begin(), errory_low.end());
  ymax = *std::max_element(errory_high.begin(), errory_high.end());
}

void PlotData2DErrorXY::add_datapoint(double xval, double yval, double xval_error_low, double xval_error_high, double yval_error_low, double yval_error_high) {
  // sanity check
  if (xval_error_low > xval)
    throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> xval_error_low elements must be less than xval");
  else if (xval_error_high < xval)
    throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> xval_error_high elements must be greater than xval");
  if (yval_error_low > yval)
    throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> yval_error_low elements must be less than yval");
  else if (yval_error_high < yval)
    throw Exception("Gtk::PLplot::PlotData2DErrorXY::add_datapoint -> yval_error_high elements must be greater than yval");

  x.push_back(xval);
  y.push_back(yval);
  errorx_low.push_back(xval_error_low);
  errorx_high.push_back(xval_error_high);
  errory_low.push_back(yval_error_low);
  errory_high.push_back(yval_error_high);
  _signal_data_modified.emit();
}

void PlotData2DErrorXY::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  double *x_pl = &x[0], *y_pl = &y[0];
  double *errorx_low_pl = &errorx_low[0];
  double *errorx_high_pl = &errorx_high[0];
  double *errory_low_pl = &errory_low[0];
  double *errory_high_pl = &errory_high[0];

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
  // plot the x errorbars
  change_plstream_color(pls, errory_color);
  // need to see what happens with the errorbars when changing the line_width and line_style
  pls->erry(x.size(), x_pl, errory_low_pl, errory_high_pl);

  // plot the symbols if requested
  if (!symbol.empty()) {
    change_plstream_color(pls, symbol_color);
    pls->schr(0, symbol_scale_factor);
    pls->string(x.size(), x_pl, y_pl, symbol.c_str());
  }
}
