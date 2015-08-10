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

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/exception.h>
#include <gdkmm/general.h>


using namespace Gtk::PLplot;

Plot::Plot(
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
  this->signal_select_region().connect(sigc::mem_fun(*this, &Plot::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &Plot::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &Plot::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &Plot::on_data_removed));
}

Plot::Plot(const Plot &_source) :
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

  this->signal_select_region().connect(sigc::mem_fun(*this, &Plot::on_select_region));
  this->signal_changed().connect(sigc::mem_fun(*this, &Plot::on_changed));
  this->signal_data_added().connect(sigc::mem_fun(*this, &Plot::on_data_added));
  this->signal_data_removed().connect(sigc::mem_fun(*this, &Plot::on_data_removed));
}

Plot::~Plot() {
  for (auto &iter : plot_data) {
    delete iter;
  }
  if (pls)
    delete pls;
}

void Plot::on_select_region(double xmin, double xmax, double ymin, double ymax) {
  set_region(xmin, xmax, ymin, ymax);
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

void Plot::on_data_removed() {
  plot_data_modified();
}

void Plot::set_axis_title_x(Glib::ustring &title) {
  axis_title_x = title;
  _signal_changed.emit();
}

void Plot::set_axis_title_y(Glib::ustring &title) {
  axis_title_y = title;
  _signal_changed.emit();
}

Glib::ustring Plot::get_axis_title_x() {
  return axis_title_x;
}

Glib::ustring Plot::get_axis_title_y() {
  return axis_title_y;
}

void Plot::set_plot_title(Glib::ustring &title) {
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

bool Plot::get_region_selectable() {
  return region_selectable;
}

void Plot::set_region_selectable(bool _region_selectable) {
  region_selectable = _region_selectable;
}

void Plot::convert_plplot_to_cairo_coordinates(
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

void Plot::convert_cairo_to_plplot_coordinates(
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

void Plot::coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new) {
  x_new = x_old;
  y_new = y_old;
}

void Plot::coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new) {
  x_new = x_old;
  y_new = y_old;
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

void Plot::set_region(double xmin, double xmax, double ymin, double ymax) {
  if (xmin == xmax && ymin == ymax) {
    //due to signal propagation, this function will actually be called twice on a double-click event,
    //the second time after the plot has already been resized to its normal geometry
    //this condition avoids the warning message...
    return;
  }

  coordinate_transform_world_to_plplot(
    xmin, ymin,
    xmin, ymin
  );
  coordinate_transform_world_to_plplot(
    xmax, ymax,
    xmax, ymax
  );

  if (xmin >= xmax || ymin >= ymax /*||
      xmin < plot_data_range_x[0] ||
      xmax > plot_data_range_x[1] ||
      ymin < plot_data_range_y[0] ||
      ymax > plot_data_range_y[1]*/) {
    throw Exception("Gtk::PLplot::Plot::set_region -> Invalid arguments");
  }
  plotted_range_x[0] = xmin;
  plotted_range_x[1] = xmax;
  plotted_range_y[0] = ymin;
  plotted_range_y[1] = ymax;

  _signal_changed.emit();
}
