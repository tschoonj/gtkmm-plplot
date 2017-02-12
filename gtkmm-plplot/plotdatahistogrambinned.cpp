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

#include <gtkmm-plplot/plotdatahistogrambinned.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <algorithm>
#include <plstream.h>

using namespace Gtk::PLplot;

double PlotDataHistogramBinned::get_new_datmin(
  std::vector<double> data_x,
  bool centred) {

  if (!centred) {
    return data_x.front();
  } else {
    return data_x[0] - 0.5 * (data_x[1] - data_x[0]);
  }
}

double PlotDataHistogramBinned::get_new_datmax(
  std::vector<double> data_x,
  bool centred) {

  if (!centred) {
    return data_x[data_x.size()-1] + (data_x[data_x.size()-1] - data_x[data_x.size()-2]);
  } else {
    return data_x[data_x.size()-1] + 0.5 * (data_x[data_x.size()-1] - data_x[data_x.size()-2]);
  }
}

PlotDataHistogramBinned::PlotDataHistogramBinned(
  const std::vector<double> &_data_x,
  const std::vector<double> &_data_y,
  bool _centred) :
  Glib::ObjectBase("GtkmmPlplotPlotDataHistogramBinned"),
  PlotDataHistogram(get_new_datmin(_data_x, _centred), get_new_datmax(_data_x, _centred), _data_x.size()),
  data_x(_data_x),
  data_y(_data_y),
  centred(_centred) {

  // arg sanity check
  if (_data_x.size() < 3)
    throw Exception("PlotDataHistogramBinned::PlotDataHistogramBinned -> data_x must contain at least three elements");

  if (_data_x.size() != _data_y.size())
    throw Exception("PlotDataHistogramBinned::PlotDataHistogramBinned -> data_x and data_y must have the same size");

  for (size_t i = 0 ; i < _data_x.size()-1 ; i++)
    if (_data_x[i] >= _data_x[i+1])
      throw Exception("PlotDataHistogramBinned::PlotDataHistogramBinned -> data_x must be monotonic increasing");

  if (std::count_if(_data_y.begin(), _data_y.end(), [](double y)->bool{return y < 0.0;}) > 0)
    throw Exception("PlotDataHistogramBinned::PlotDataHistogramBinned -> data_y values must be greater than or equal to zero");

}

PlotDataHistogramBinned::PlotDataHistogramBinned(
  const std::valarray<double> &_data_x,
  const std::valarray<double> &_data_y,
  bool _centred) :
  PlotDataHistogramBinned(
    std::vector<double>(std::begin(_data_x), std::end(_data_x)),
    std::vector<double>(std::begin(_data_y), std::end(_data_y)),
    _centred) {}

PlotDataHistogramBinned::~PlotDataHistogramBinned() {}

void PlotDataHistogramBinned::set_centred(bool _centred) {
  if (centred == _centred)
    return;
  centred = _centred;
  datmin = get_new_datmin(data_x, centred);
  datmax = get_new_datmax(data_x, centred);
  _signal_data_modified.emit();
}

bool PlotDataHistogramBinned::get_centred() {
  return centred;
}

void PlotDataHistogramBinned::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!is_showing())
    return;

  int flags = 0;
  if (expand_bins == false)
    flags |= PL_BIN_NOEXPAND;
  if (empty_bins == false)
    flags |= PL_BIN_NOEMPTY;
  if (centred == true)
    flags |= PL_BIN_CENTRED;

  // plot the histogram if line_style allows for it...
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->bin(nbins, &data_x[0], &data_y[0], flags);
  }
}

void PlotDataHistogramBinned::get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) {
  xmin = datmin;
  xmax = datmax;
  ymin = 0.0;
  ymax = 1.1 * *std::max_element(data_y.begin(), data_y.end());
}
