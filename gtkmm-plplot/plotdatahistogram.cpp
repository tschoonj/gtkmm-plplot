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

using namespace Gtk::PLplot;

PlotDataHistogram::PlotDataHistogram(
  bool _expand_bins,
  bool _empty_bins) :
  Glib::ObjectBase("GtkmmPlplotPlotDataHistogram"),
  PlotDataLine(Gdk::RGBA("red"), CONTINUOUS, PLOTDATA_DEFAULT_LINE_WIDTH),
  expand_bins(_expand_bins),
  empty_bins(_empty_bins) {}

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
