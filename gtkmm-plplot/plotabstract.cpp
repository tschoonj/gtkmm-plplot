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
#include <gtkmm-plplot/exception.h>

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
  titles_color("Black") {

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
  titles_color(_source.titles_color) {

  this->signal_select_region().connect(sigc::mem_fun(*this, &PlotAbstract::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &PlotAbstract::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &PlotAbstract::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &PlotAbstract::on_data_removed));
}

PlotAbstract::~PlotAbstract() {
  for (auto &iter : plot_data) {
    delete iter;
  }
  if (pls)
    delete pls;
}

void PlotAbstract::on_select_region(double xmin, double xmax, double ymin, double ymax) {
  set_region(xmin, xmax, ymin, ymax);
}

void PlotAbstract::on_changed() {
  //this function does nothing
  //it is designed to be overridden by a derived class
}

void PlotAbstract::show() {
  shown = true;
  _signal_changed.emit();
}

void PlotAbstract::hide() {
  shown = false;
  _signal_changed.emit();
}

bool PlotAbstract::is_showing() const {
  return shown;
}

void PlotAbstract::on_data_added(PlotDataAbstract *added_data) {
  plot_data_modified();
}

void PlotAbstract::on_data_removed() {
  plot_data_modified();
}

void PlotAbstract::set_axis_title_x(Glib::ustring &title) {
  axis_title_x = title;
  _signal_changed.emit();
}

void PlotAbstract::set_axis_title_y(Glib::ustring &title) {
  axis_title_y = title;
  _signal_changed.emit();
}

Glib::ustring PlotAbstract::get_axis_title_x() {
  return axis_title_x;
}

Glib::ustring PlotAbstract::get_axis_title_y() {
  return axis_title_y;
}

void PlotAbstract::set_plot_title(Glib::ustring &title) {
  plot_title = title;
  _signal_changed.emit();
}

Glib::ustring PlotAbstract::get_plot_title() {
  return plot_title;
}

PlotDataAbstract *PlotAbstract::get_data(unsigned int index) {
  if (index < plot_data.size()) {
    return plot_data[index];
  }
  throw Exception("Gtk::PLplot::PlotAbstract::get_data -> Invalid index");
}

Gdk::RGBA PlotAbstract::get_background_color() {
  return background_color;
}

void PlotAbstract::set_background_color(Gdk::RGBA _background_color) {
  background_color = _background_color;
  _signal_changed.emit();
}

Gdk::RGBA PlotAbstract::get_axes_color() {
  return axes_color;
}

void PlotAbstract::set_axes_color(Gdk::RGBA _axes_color) {
  axes_color = _axes_color;
  _signal_changed.emit();
}

Gdk::RGBA PlotAbstract::get_titles_color() {
  return titles_color;
}

void PlotAbstract::set_titles_color(Gdk::RGBA _titles_color) {
  titles_color = _titles_color;
  _signal_changed.emit();
}

bool PlotAbstract::get_region_selectable() {
  return region_selectable;
}

void PlotAbstract::set_region_selectable(bool _region_selectable) {
  region_selectable = _region_selectable;
}

void PlotAbstract::convert_plplot_to_cairo_coordinates(
  double x_pl, double y_pl,
  double &x_cr, double &y_cr) {
    //inspired by http://www.mail-archive.com/plplot-devel@lists.sourceforge.net/msg02383.html
    //but the last equation was incorrect and is fixed here
    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    pls->gvpd(nxmin, nxmax, nymin, nymax);
    pls->gvpw(wxmin, wxmax, wymin, wymax);

    double xmin = plot_width * nxmin + plot_offset_x;
    double xmax = plot_width * nxmax + plot_offset_x;
    double ymin = plot_height * (nymin - 1.0) + canvas_height - plot_offset_y;
    double ymax = plot_height * (nymax - 1.0) + canvas_height - plot_offset_y;

    x_cr = xmin + ((xmax - xmin) * ((x_pl - wxmin) / (wxmax - wxmin)));
    y_cr = ymin + ((ymax - ymin) * ((y_pl - wymin) / (wymax - wymin)));
}

void PlotAbstract::convert_cairo_to_plplot_coordinates(
  double x_cr, double y_cr,
  double &x_pl, double &y_pl) {

    double nxmin, nxmax, nymin, nymax;
    double wxmin, wxmax, wymin, wymax;

    pls->gvpd(nxmin, nxmax, nymin, nymax);
    pls->gvpw(wxmin, wxmax, wymin, wymax);

    double xmin = plot_width * nxmin + plot_offset_x;
    double xmax = plot_width * nxmax + plot_offset_x;
    double ymin = plot_height * (nymin - 1.0) + canvas_height - plot_offset_y;
    double ymax = plot_height * (nymax - 1.0) + canvas_height - plot_offset_y;

    x_pl = wxmin + ((x_cr - xmin) * (wxmax - wxmin) / (xmax - xmin));
    y_pl = wymin + ((y_cr - ymin) * (wymax - wymin) / (ymax - ymin));
}
