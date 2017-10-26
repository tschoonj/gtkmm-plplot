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

#include <gtkmm-plplot/plotobject2d.h>

using namespace Gtk::PLplot;

PlotObject2D::PlotObject2D(
  Gdk::RGBA _color) :
  Glib::ObjectBase("GtkmmPlplotPlotObject2D"),
  color(_color) {

}

void PlotObject2D::set_color(Gdk::RGBA _color) {
  if (color == _color)
    return;
  color = _color;
  _signal_changed.emit();
}

Gdk::RGBA PlotObject2D::get_color() {
  return color;
}

PlotObject2D::~PlotObject2D() {}
