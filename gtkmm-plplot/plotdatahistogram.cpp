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

#include <gtkmm-plplot/plotdatahistogram.h>
#include <gtkmm-plplot/exception.h>

using namespace Gtk::PLplot;

PlotDataHistogram::PlotDataHistogram(
  double _datmin,
  double _datmax,
  int _nbins,
  bool _expand_bins,
  bool _empty_bins) :
  Glib::ObjectBase("GtkmmPlplotPlotDataHistogram"),
  PlotDataLine(Gdk::RGBA("red"), CONTINUOUS, PLOTDATA_DEFAULT_LINE_WIDTH),
  datmin(_datmin),
  datmax(_datmax),
  nbins(_nbins),
  expand_bins(_expand_bins),
  empty_bins(_empty_bins) {

  if (datmin >= datmax)
    throw Exception("PlotDataHistogram::PlotDataHistogram -> datmax must be greater than datmin");

  if (nbins < 3)
    throw Exception("PlotDataHistogram::PlotDataHistogram -> nbins must greater than or equal to 3");
}

PlotDataHistogram::~PlotDataHistogram() {}

void PlotDataHistogram::set_expand_bins(bool _expand_bins) {
  if (_expand_bins == expand_bins)
    return;
  expand_bins = _expand_bins;
  _signal_changed.emit();
}

bool PlotDataHistogram::get_expand_bins() {
  return expand_bins;
}

void PlotDataHistogram::set_empty_bins(bool _empty_bins) {
  if (_empty_bins == empty_bins)
    return;
  empty_bins = _empty_bins;
  _signal_changed.emit();
}

bool PlotDataHistogram::get_empty_bins() {
  return empty_bins;
}

double PlotDataHistogram::get_data_minimum() {
  return datmin;
}

double PlotDataHistogram::get_data_maximum() {
  return datmax;
}

int PlotDataHistogram::get_nbins() {
  return nbins;
}
