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

#include <gtkmm-plplot/plotobject2dline.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>
#include <cmath>

using namespace Gtk::PLplot;


PlotObject2DLine::PlotObject2DLine(
  double _coord_x_begin,
  double _coord_y_begin,
  double _coord_x_end,
  double _coord_y_end,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Glib::ObjectBase("GtkmmPlplotPlotObject2DLine"),
  PlotObject2D(_color),
  line_style(_line_style),
  line_width(_line_width),
  coords_begin{_coord_x_begin, _coord_y_begin},
  coords_end{_coord_x_end, _coord_y_end} {
  
  if (_line_width <= 0.0)
    throw Exception("Gtk::PLplot::PlotObject2DLine::PlotObject2DLine -> line width must be strictly positive");
}

PlotObject2DLine::PlotObject2DLine(
  Gtk::Orientation _orientation,
  double _offset,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  PlotObject2DLine(
    _orientation == Gtk::Orientation::HORIZONTAL ? -HUGE_VAL : _offset,
    _orientation == Gtk::Orientation::VERTICAL ? -HUGE_VAL : _offset,
    _orientation == Gtk::Orientation::HORIZONTAL ? HUGE_VAL : _offset,
    _orientation == Gtk::Orientation::VERTICAL ? HUGE_VAL : _offset,
    _color,
    _line_style,
    _line_width
  ) {}

PlotObject2DLine::~PlotObject2DLine() {}

void PlotObject2DLine::set_line_style(LineStyle _line_style) {
  if (line_style == _line_style)
    return;
  line_style = _line_style;
  _signal_changed.emit();
}

LineStyle PlotObject2DLine::get_line_style() {
  return line_style;
}

void PlotObject2DLine::set_line_width(double _line_width) {
  if (line_width == _line_width)
    return;
  if (_line_width <= 0.0)
    throw Exception("Gtk::PLplot::PlotObject2DLine::set_line_width -> line_width must be strictly positive");
  line_width = _line_width;
  _signal_changed.emit();
}

double PlotObject2DLine::get_line_width() {
  return line_width;
}

void PlotObject2DLine::draw_plot_object(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, PlotObjectAuxData &data) {
  if (!is_showing())
    return;

  PlotObject2DAuxData &aux_data = static_cast<PlotObject2DAuxData &>(data);

  change_plstream_color(pls, color);

  // plot the line if requested
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    //std::cout << "coords_begin[0]" << coords_begin[0] << std::endl;
    //std::cout << "coords_begin[1]" << coords_begin[1] << std::endl;
    //std::cout << "coords_end[0]" << coords_end[0] << std::endl;
    //std::cout << "coords_end[1]" << coords_end[1] << std::endl;
    pls->join(MAX(coords_begin[0], aux_data.world_range_x[0]),
              MAX(coords_begin[1], aux_data.world_range_y[0]),
              MIN(coords_end[0], aux_data.world_range_x[1]),
	      MIN(coords_end[1], aux_data.world_range_y[1]));
  }
}

bool PlotObject2DLine::is_logarithmic_x_compatible() {
  return MAX(coords_begin[0], coords_end[0]) > 0.0; 
}

bool PlotObject2DLine::is_logarithmic_y_compatible() {
  return MAX(coords_begin[1], coords_end[1]) > 0.0; 
}

