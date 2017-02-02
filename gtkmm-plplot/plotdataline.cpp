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

#include <gtkmm-plplot/plotdataline.h>
#include <gtkmm-plplot/exception.h>

using namespace Gtk::PLplot;

PlotDataLine::PlotDataLine(
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPlplotPlotDataLine"),
  color(_color),
  line_style(_line_style),
  line_width(_line_width) {

  if (_line_width <= 0.0)
    throw Exception("Gtk::PLplot::PlotDataLine::set_line_width -> line_width must be strictly positive");

  }

void PlotDataLine::set_color(Gdk::RGBA _color) {
  if (color == _color)
    return;
  color = _color;
  _signal_changed.emit();
}

Gdk::RGBA PlotDataLine::get_color() {
  return color;
}

void PlotDataLine::set_line_style(LineStyle _line_style) {
  if (line_style == _line_style)
    return;
  line_style = _line_style;
  _signal_changed.emit();
}

LineStyle PlotDataLine::get_line_style() {
  return line_style;
}

void PlotDataLine::set_line_width(double _line_width) {
  if (line_width == _line_width)
    return;
  if (_line_width <= 0.0)
    throw Exception("Gtk::PLplot::PlotDataLine::set_line_width -> line_width must be strictly positive");
  line_width = _line_width;
  _signal_changed.emit();
}

double PlotDataLine::get_line_width() {
  return line_width;
}

PlotDataLine::~PlotDataLine() {}
