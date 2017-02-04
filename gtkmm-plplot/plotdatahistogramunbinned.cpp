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
#include <gtkmm-plplot/utils.h>
#include <algorithm>
#include <plstream.h>

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
  if (x)
    delete[] x;
  x = new double[nbins];

  if (y)
    delete[] y;
  y = new double[nbins];

  /* shamelessly stolen from plhist.c */
  double dx = ( datmax - datmin ) / nbins;

  for (int i = 0; i < nbins; i++ ) {
    x[i] = datmin + i * dx;
    y[i] = 0.0;
  }

  for (int i = 0; i < data.size(); i++ ) {
    int bin = (int) ( ( data[i] - datmin ) / dx );
    if (ignore_outliers == false) {
      bin = bin > 0 ? bin : 0;
      bin = bin < nbins ? bin : nbins - 1;
      y[bin]++;
    }
    else if (bin >= 0 && bin < nbins ) {
      y[bin]++;
    }
  }
}

void PlotDataHistogramUnbinned::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  int flags = 0;
  if (expand_bins == false)
    flags |= PL_BIN_NOEXPAND;
  if (empty_bins == false)
    flags |= PL_BIN_NOEMPTY;

  // plot the histogram if line_style allows for it...
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->bin(nbins, x, y, flags);
  }
}

void PlotDataHistogramUnbinned::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  xmin = datmin;
  xmax = datmax;
  ymin = 0.0;
  ymax = 1.1 * *std::max_element(y, y + nbins);
}
