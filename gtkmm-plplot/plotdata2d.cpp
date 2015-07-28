/*
Copyright (C) 2015 Tom Schoonjans

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gtkmm-plplot/plot2ddata.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>

using namespace Gtk::PLplot;

Plot2DData::Plot2DData(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  x(_x), y(_y), color(_color), line_style(_line_style),
  line_width(_line_width), shown(true),
  symbol(""), symbol_color(Gdk::RGBA("Red")),
  symbol_scale_factor(1.0) {

   //ensure both arrays have the same size
  if (x.size() != y.size()) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data arrays x and y must have the same size!");
  }
  //ensure there are at least two elements in the arrays
  if (x.size() < 2) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data arrays x and y must have at least two elements");
  }
  //ensure the X-values are not all the same
  if (std::count(x.begin(), x.end(), x[0]) == x.size()) {
    throw Exception("Gtk::PLplot::Plot2DData::Plot2DData -> data array x must have at least two different elements");
  }
  this->signal_changed().connect(sigc::mem_fun(*this,
             &Plot2DData::on_changed));
  this->signal_data_modified().connect(sigc::mem_fun(*this,
             &Plot2DData::on_data_modified));
}

Plot2DData::Plot2DData(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Plot2DData(std::vector<PLFLT>(std::begin(_x), std::end(_x)),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

Plot2DData::Plot2DData(
  const std::vector<PLFLT> &_y,
  Gdk::RGBA _color,
  LineStyle _line_style,
  double _line_width) :
  Plot2DData(indgen(_y.size()), _y, _color, _line_style, _line_width) {}

Plot2DData::Plot2DData(
  const std::valarray<PLFLT> &_y,
  Gdk::RGBA _color, LineStyle _line_style,
  double _line_width) :
  Plot2DData(std::vector<PLFLT>(indgen(_y.size())),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _color, _line_style, _line_width) {}

Plot2DData::Plot2DData(const Plot2DData &_data) :
  Plot2DData(_data.x, _data.y, _data.color, _data.line_style, _data.line_width) {
    shown = _data.shown;
    symbol = _data.symbol;
    symbol_color = _data.symbol_color;
    symbol_scale_factor = _data.symbol_scale_factor;
}

void Plot2DData::set_color(Gdk::RGBA _color) {
  color = _color;
  _signal_changed.emit();
}

Gdk::RGBA Plot2DData::get_color() {
  return color;
}

void Plot2DData::set_line_style(LineStyle _line_style) {
  line_style = _line_style;
  _signal_changed.emit();
}

LineStyle Plot2DData::get_line_style() {
  return line_style;
}

void Plot2DData::set_line_width(double _line_width) {
  line_width = _line_width;
  _signal_changed.emit();
}

double Plot2DData::get_line_width() {
  return line_width;
}

void Plot2DData::set_symbol(Glib::ustring _symbol) {
  symbol = _symbol;
  _signal_changed.emit();
}

Glib::ustring Plot2DData::get_symbol() {
  return symbol;
}

void Plot2DData::set_symbol_color(Gdk::RGBA _color) {
  symbol_color = _color;
  _signal_changed.emit();
}

Gdk::RGBA Plot2DData::get_symbol_color() {
  return symbol_color;
}

void Plot2DData::set_symbol_height_scale_factor(double _scale_factor) {
  if (_scale_factor <= 0.0) {
    throw Exception("Plot2DData::set_symbol_height_scale_factor -> scale factor must be strictly positive");
  }
  symbol_scale_factor = _scale_factor;
  _signal_changed.emit();
}

double Plot2DData::get_symbol_height_scale_factor() {
  return symbol_scale_factor;
}

void Plot2DData::add_datapoint(PLFLT _x, PLFLT _y) {
  x.push_back(_x);
  y.push_back(_y);
  _signal_data_modified.emit();
}

void Plot2DData::add_datapoint(std::pair<PLFLT, PLFLT> _xy_pair) {
  add_datapoint(_xy_pair.first, _xy_pair.second);
}

void Plot2DData::on_changed() {
  //do nothing
}

void Plot2DData::on_data_modified() {
  //do nothing
}

void Plot2DData::show() {
  shown = true;
  _signal_changed.emit();
}

void Plot2DData::hide() {
  shown = false;
  _signal_changed.emit();
}

bool Plot2DData::is_showing() const {
  return shown;
}

void Plot2DData::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  if (!shown)
    return;

  PLFLT *x_pl = &x[0], *y_pl = &y[0];
  PLINT red_u_old, green_u_old, blue_u_old;
  PLFLT alpha_old;

  // plot the line if requested
  if (line_style != LineStyle::NONE) {
    change_plstream_color(pls, color);
    pls->lsty(line_style);
    pls->width(line_width);
    pls->line(x.size(), x_pl, y_pl);
  }

  // plot the symbols if requested
  if (!symbol.empty()) {
    change_plstream_color(pls, symbol_color);
    pls->schr(0, symbol_scale_factor);
    pls->string(x.size(), x_pl, y_pl, symbol.c_str());
  }

}
