/*
Copyright (C) 2015 Tom Schoonjans

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

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/exception.h>
#include <gdkmm/general.h>
#include <plstream.h>

using namespace Gtk::PLplot;

Plot::Plot(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Glib::ObjectBase("GtkmmPLplotPlot"),
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
  axes_color("Black"),
  titles_color("Black") {

  background_color.set_alpha(0.0);

  //connect our default signal handlers
  this->signal_changed().connect(sigc::mem_fun(*this, &Plot::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &Plot::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &Plot::on_data_removed));
}

Plot::~Plot() {
  for (auto &iter : plot_data) {
    if (iter->is_managed_()) {
      delete iter;
    }
  }
  if (pls)
    delete pls;
}

void Plot::on_changed() {
  //this function does nothing
  //it is designed to be overridden by a derived class
}

void Plot::show() {
  shown = true;
  _signal_changed.emit();
}

void Plot::hide() {
  shown = false;
  _signal_changed.emit();
}

bool Plot::is_showing() const {
  return shown;
}

void Plot::on_data_added(PlotData *added_data) {
  plot_data_modified();
}

void Plot::on_data_removed(PlotData *removed_data) {
  if (removed_data->is_managed_()) {
    delete removed_data; //this is necessary to avoid a memory leak.
  }
  plot_data_modified();
}

void Plot::remove_data(unsigned int index) {
  if (plot_data.size() == 1)
    throw Exception("Gtk::PLplot::Plot::remove_data -> At least one dataset must be present");

  if (index >= plot_data.size())
    throw Exception("Gtk::PLplot::Plot::remove_data -> Invalid index");

  PlotData *removed_plot = plot_data[index];

  plot_data.erase(plot_data.begin() + index);
  _signal_data_removed.emit(removed_plot);
}

void Plot::remove_data(PlotData &plot_data_member) {
  if (plot_data.size() == 1)
    throw Exception("Gtk::PLplot::Plot::remove_data -> At least one dataset must be present");

  auto iter = std::find(plot_data.begin(), plot_data.end(), &plot_data_member);
  if (iter == plot_data.end())
    throw Exception("Gtk::PLplot::Plot::remove_data -> No match for argument");

  PlotData *removed_plot = *iter;

  plot_data.erase(iter);
  _signal_data_removed.emit(removed_plot);
}


void Plot::set_axis_title_x(Glib::ustring title) {
  axis_title_x = title;
  _signal_changed.emit();
}

void Plot::set_axis_title_y(Glib::ustring title) {
  axis_title_y = title;
  _signal_changed.emit();
}

Glib::ustring Plot::get_axis_title_x() {
  return axis_title_x;
}

Glib::ustring Plot::get_axis_title_y() {
  return axis_title_y;
}

void Plot::set_plot_title(Glib::ustring title) {
  plot_title = title;
  _signal_changed.emit();
}

Glib::ustring Plot::get_plot_title() {
  return plot_title;
}

PlotData *Plot::get_data(unsigned int index) {
  if (index < plot_data.size()) {
    return plot_data[index];
  }
  throw Exception("Gtk::PLplot::Plot::get_data -> Invalid index");
}

Gdk::RGBA Plot::get_background_color() {
  return background_color;
}

void Plot::set_background_color(Gdk::RGBA _background_color) {
  background_color = _background_color;
  _signal_changed.emit();
}

Gdk::RGBA Plot::get_axes_color() {
  return axes_color;
}

void Plot::set_axes_color(Gdk::RGBA _axes_color) {
  axes_color = _axes_color;
  _signal_changed.emit();
}

Gdk::RGBA Plot::get_titles_color() {
  return titles_color;
}

void Plot::set_titles_color(Gdk::RGBA _titles_color) {
  titles_color = _titles_color;
  _signal_changed.emit();
}


void Plot::draw_plot_init(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  canvas_width = width;
  canvas_height = height;
  plot_width = width * plot_width_norm;
  plot_height= height * plot_height_norm;
  plot_offset_x = width * plot_offset_horizontal_norm;
  plot_offset_y = height * plot_offset_vertical_norm;

  if (pls)
    delete pls;
  pls = new plstream;

  //add support for more colors
  pls->scmap0n(1024);

  pls->sdev("extcairo");
  pls->spage(0.0, 0.0, plot_width , plot_height, plot_offset_x, plot_offset_y);
  Gdk::Cairo::set_source_rgba(cr, background_color);
  cr->rectangle(plot_offset_x, plot_offset_y, plot_width, plot_height);
  cr->fill();
  cr->save();
  cr->translate(plot_offset_x, plot_offset_y);
  pls->init();

  pls->cmd(PLESC_DEVINIT, cr->cobj());
}
