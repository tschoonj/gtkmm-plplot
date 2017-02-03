/*
Copyright (C) 2017 Tom Schoonjans

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

#include <gtkmm-plplot/plotdatahistogramunbinned.h>
#include <gtkmm-plplot/exception.h>

using namespace Gtk::PLplot;

PlotDataHistogramUnbinned::PlotDataHistogramUnbinned(
  const std::vector<double> &_data,
  double _datmin,
  double _datmax,
  int _nbins) :
  Glib::ObjectBase("GtkmmPlplotPlotDataHistogramUnbinned"),
  PlotDataHistogram(_datmin, _datmax, _nbins),
  x(nullptr),
  y(nullptr),
  data(_data),
  ignore_outliers(false) {

  rebin();
}

PlotDataHistogramUnbinned::PlotDataHistogramUnbinned(
  const std::valarray<double> &_data,
  double _datmin,
  double _datmax,
  int _nbins) :
  PlotDataHistogramUnbinned(
    std::vector<double>(std::begin(_data), std::end(_data)),
    _datmin,
    _datmax,
    _nbins) {}

PlotDataHistogramUnbinned::~PlotDataHistogramUnbinned() {
  if (x)
    delete[] x;
  if (y)
    delete[] y;
}

void PlotDataHistogramUnbinned::add_datapoint(double value) {
  data.push_back(value);
  rebin();
  _signal_data_modified.emit();
}

void PlotDataHistogramUnbinned::set_data_minimum(double _datmin) {
  if (datmin == _datmin)
    return;
  if (_datmin >= datmax)
    throw Exception("PlotDataHistogramUnbinned::set_data_minimum -> datmin must be less than datmax!");

  datmin = _datmin;
  rebin();
  _signal_data_modified.emit();
}

void PlotDataHistogramUnbinned::set_data_maximum(double _datmax) {
  if (datmax == _datmax)
    return;
  if (_datmax <= datmin)
    throw Exception("PlotDataHistogramUnbinned::set_data_maximum -> datmax must be greater than datmin!");

  datmax = _datmax;
  rebin();
  _signal_data_modified.emit();
}

void PlotDataHistogramUnbinned::set_nbins(int _nbins) {
  if (nbins == _nbins)
    return;
  if (_nbins < 3)
    throw Exception("PlotDataHistogramUnbinned::set_nbins -> nbins must be greater than or equal to 3");

  nbins = _nbins;
  rebin();
  _signal_data_modified.emit();
}

bool PlotDataHistogramUnbinned::get_ignore_outliers() {
  return ignore_outliers;
}

void PlotDataHistogramUnbinned::set_ignore_outliers(bool _ignore_outliers) {
  if (_ignore_outliers == ignore_outliers)
    return;
  ignore_outliers = _ignore_outliers;
  rebin();
  _signal_data_modified.emit();
}

void PlotDataHistogramUnbinned::rebin() {
  // TODO
}

void PlotDataHistogramUnbinned::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  // TODO
}

void PlotDataHistogramUnbinned::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  // TODO
}
