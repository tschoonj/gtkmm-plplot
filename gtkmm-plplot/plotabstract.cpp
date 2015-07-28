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

#include <gtkmm-plplot/plotabstract.h>

using namespace Gtk::PLplot;

PlotAbstract::PlotAbstract(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  axis_title_x(_axis_title_x),
  axis_title_y(_axis_title_y),
  plot_title(_plot_title),
  pls(nullptr),
  shown(true),
  background_color("Black"),
  plot_width_norm(_plot_width_norm),
  plot_height_norm(_plot_height_norm),
  plot_offset_horizontal_norm(_plot_offset_horizontal_norm),
  plot_offset_vertical_norm(_plot_offset_vertical_norm),
  region_selectable(true),
  axes_color("Black"),
  titles_color("Black"),
  box_style(BOX_TICKS_TICK_LABELS) {

  background_color.set_alpha(0.0);

  //connect our default signal handlers
  this->signal_select_region().connect(sigc::mem_fun(*this, &PlotAbstract::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &PlotAbstract::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &PlotAbstract::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &PlotAbstract::on_data_removed));
}

PlotAbstract::PlotAbstract(const PlotAbstract &_source) :
  axis_title_x(_source.axis_title_x),
  axis_title_y(_source.axis_title_y),
  plot_title(_source.plot_title),
  pls(nullptr),
  shown(true),
  background_color(_source.background_color),
  plot_width_norm(_source.plot_width_norm),
  plot_height_norm(_source.plot_height_norm),
  plot_offset_horizontal_norm(_source.plot_offset_horizontal_norm),
  plot_offset_vertical_norm(_source.plot_offset_vertical_norm),
  region_selectable(_source.region_selectable),
  axes_color(_source.axes_color),
  titles_color(_source.titles_color),
  box_style(_source.box_style) {

  this->signal_select_region().connect(sigc::mem_fun(*this, &Plot2D::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &Plot2D::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &Plot2D::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &Plot2D::on_data_removed));
}
