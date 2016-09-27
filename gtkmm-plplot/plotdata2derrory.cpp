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

#include <gtkmm-plplot/plotdata2derrory.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>

using namespace Gtk::PLplot;

PlotData2DErrorY::PlotData2DErrorY(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  const std::vector<double> &_errory_low,
  const std::vector<double> &_errory_high,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPLplotPlotData2DErrorY"),
  PlotData2D(_x, _y, _color, _line_style, _line_width),
  errory_low(_errory_low),
  errory_high(_errory_high),
  errory_color(_color) {

  //ensure all arrays have the same size
  if (y.size() != _errory_low.size() || y.size() != _errory_high.size()) {
    throw Exception("Gtk::PLplot::PlotData2DErrorY::PlotData2DErrorY -> data arrays x, y, errory_low and errory_high must have the same size!");
  }

  //ensure that the errory_low values are greater than y
  for (int i = 0 ; i < y.size() ; i++) {
    if (errory_low[i] > y[i])
      throw Exception("Gtk::PLplot::PlotData2DErrorY::PlotData2DErrorY -> errory_low elements must be less than the corresponding elements in y");
    else if (errory_high[i] < y[i])
      throw Exception("Gtk::PLplot::PlotData2DErrorY::PlotData2DErrorY -> errory_high elements must be greater than the corresponding elements in y");
  }
}

PlotData2DErrorY::PlotData2DErrorY(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  const std::valarray<double> &_errory_low,
  const std::valarray<double> &_errory_high,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotData2DErrorY(
    std::vector<double>(std::begin(_x), std::end(_x)),
    std::vector<double>(std::begin(_y), std::end(_y)),
    std::vector<double>(std::begin(_errory_low), std::end(_errory_low)),
    std::vector<double>(std::begin(_errory_high), std::end(_errory_high)),
    _color, _line_style, _line_width) {}

PlotData2DErrorY::~PlotData2DErrorY() {}

void PlotData2DErrorY::add_datapoint(double _x, double _y) {
  throw Exception("Gtk::PLplot::PlotData2DErrorY::add_datapoint -> This method is not supported for PlotData2DErrorY");
}

void PlotData2DErrorY::add_datapoint(std::pair<double, double> _xy_pair) {
  throw Exception("Gtk::PLplot::PlotData2DErrorY::add_datapoint -> This method is not supported for PlotData2DErrorY");
}

std::vector<double> PlotData2DErrorY::get_vector_error_y_low() {
  return errory_low;
}

std::vector<double> PlotData2DErrorY::get_vector_error_y_high() {
  return errory_high;
}

void PlotData2DErrorY::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  xmin = *std::min_element(x.begin(), x.end());
  xmax = *std::max_element(x.begin(), x.end());
  ymin = *std::min_element(errory_low.begin(), errory_low.end());
  ymax = *std::max_element(errory_high.begin(), errory_high.end());
}

void PlotData2DErrorY::set_error_y_color(Gdk::RGBA _errory_color) {
  if (_errory_color == errory_color)
    return;
  errory_color = _errory_color;
  _signal_changed.emit();
}

Gdk::RGBA PlotData2DErrorY::get_error_y_color() {
  return errory_color;
}

void PlotData2DErrorY::add_datapoint(double xval, double yval, double yval_error_low, double yval_error_high) {
  // sanity check
  if (yval_error_low > yval)
    throw Exception("Gtk::PLplot::PlotData2DErrorY::add_datapoint -> yval_error_low elements must be less than yval");
  else if (yval_error_high < yval)
    throw Exception("Gtk::PLplot::PlotData2DErrorY::add_datapoint -> yval_error_high elements must be greater than yval");

  x.push_back(xval);
  y.push_back(yval);
  errory_low.push_back(yval_error_low);
  errory_high.push_back(yval_error_high);
  _signal_data_modified.emit();
}

void PlotData2DErrorY::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  double *x_pl = &x[0], *y_pl = &y[0];
  double *errory_low_pl = &errory_low[0];
  double *errory_high_pl = &errory_high[0];

  // plot the line if requested
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->line(x.size(), x_pl, y_pl);
  }

  // plot the y errorbars
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
