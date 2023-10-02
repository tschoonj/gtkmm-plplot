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

#include <gtkmm-plplot/plotobject2dtext.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>
#include <cmath>

using namespace Gtk::PLplot;

PlotObject2DText::PlotObject2DText(
  Glib::ustring _text,
  double _coord_x,
  double _coord_y,
  Gdk::RGBA _color) :
  Glib::ObjectBase("GtkmmPlplotPlotObject2DText"),
  Gtk::Orientable(),
  PlotObject2D(_color),
  justification(0.0),
  text(_text),
  coords{_coord_x, _coord_y},
  scale_factor(1.0)
  {
  property_orientation().signal_changed().connect([this](){_signal_changed.emit();});
}

PlotObject2DText::~PlotObject2DText() {}

double PlotObject2DText::get_justification() {
  return justification;
}

void PlotObject2DText::set_justification(double _justification) {
  if (_justification < 0.0 || _justification > 1.0) {
    throw Exception("Gtk::PLplot::PlotObject2DText::set_justification -> justification must be within [0.0, 1.0]");
  }

  if (_justification == justification)
    return;

  justification = _justification;

  _signal_changed.emit();
}

Glib::ustring PlotObject2DText::get_text() {
  return text;
}

void PlotObject2DText::set_text(Glib::ustring _text) {
  if (_text == text)
    return;

  text = _text;

  _signal_changed.emit();
}

void PlotObject2DText::get_coordinates(double &coord_x, double &coord_y) {
  coord_x = coords[0]; 
  coord_y = coords[1]; 
}

void PlotObject2DText::set_coordinates(double coord_x, double coord_y) {
  if (coord_x == coords[0] && coord_y == coords[1])
    return;

  coords[0] = coord_x;
  coords[1] = coord_y;

  _signal_changed.emit();
}

double PlotObject2DText::get_scale_factor() {
  return scale_factor;
}

void PlotObject2DText::set_scale_factor(double _scale_factor) {
  if (_scale_factor <= 0.0) {
    throw Exception("Gtk::PLplot::PlotObject2DText::set_scale_factor -> scale factor must be strictly positive");
  }
  scale_factor = _scale_factor;
  _signal_changed.emit();
}

void PlotObject2DText::draw_plot_object(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, PlotObjectAuxData &data) {
  if (!is_showing())
    return;

  change_plstream_color(pls, color);
  pls->schr(0, scale_factor);
  double dx, dy;
  if (get_orientation() == Gtk::Orientation::HORIZONTAL) {
    dx = 1.0;
    dy = 0.0;
  }
  else {
    dx = 0.0;
    dy = 1.0;
  }
  pls->ptex(coords[0], coords[1], dx, dy, justification, text.c_str());
  pls->schr(0, 1.0);
}

bool PlotObject2DText::is_logarithmic_x_compatible() {
  return coords[0] > 0.0; // perhaps better to use something like 1E-25 or so??
}

bool PlotObject2DText::is_logarithmic_y_compatible() {
  return coords[1] > 0.0; // perhaps better to use something like 1E-25 or so??
}
