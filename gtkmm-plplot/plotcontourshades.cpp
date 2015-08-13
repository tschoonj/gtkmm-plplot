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

#include <gtkmm-plplot/gtkmm-plplotconfig.h>
#include <gtkmm-plplot/plotcontourshades.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <iostream>

using namespace Gtk::PLplot;


PlotContourShades::PlotContourShades(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotContour(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {}

PlotContourShades::PlotContourShades(
  const PlotDataContourShades &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotContour(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm),
  showing_colorbar(true),
  colorbar_title("Magnitude") {

  add_data(_data);
}

PlotContourShades::PlotContourShades(const PlotContourShades &_source) :
  PlotContourShades(_source.axis_title_x, _source.axis_title_y,
  _source.plot_title, _source.plot_width_norm, _source.plot_height_norm,
  _source.plot_offset_horizontal_norm, _source.plot_offset_vertical_norm) {

  showing_colorbar = _source.showing_colorbar;
  colorbar_title = _source.colorbar_title;

  for (auto &iter : _source.plot_data) {
    add_data(*iter);
  }
}

PlotContourShades::~PlotContourShades() {}

PlotDataContourShades *PlotContourShades::add_data(const PlotData &data) {
  //ensure plot_data is empty
  if (!plot_data.empty())
    throw Exception("Gtk::PLplot::PlotContourShades::add_data -> cannot add data when plot_data is not empty!");

  PlotDataContourShades *data_copy = nullptr;
  try {
    //ensure our data is PlotDataContour
    data_copy = new PlotDataContourShades(dynamic_cast<const PlotDataContourShades &>(data));
  }
  catch (std::bad_cast &e) {
    throw Exception("Gtk::PLplot::PlotContourShades::add_data -> data must be of PlotDataContourShades type!");
  }
  plot_data.push_back(data_copy);
  data_copy->signal_changed().connect([this](){_signal_changed.emit();});
  data_copy->signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(data_copy);
  return data_copy;
}

void PlotContourShades::show_colorbar() {
  if (showing_colorbar)
    return;
  showing_colorbar = true;
  _signal_changed.emit();
}

void PlotContourShades::hide_colorbar() {
  if (!showing_colorbar)
    return;
  showing_colorbar = false;
  _signal_changed.emit();
}

bool PlotContourShades::is_showing_colorbar() {
  return showing_colorbar;
}

void PlotContourShades::set_colorbar_title(Glib::ustring _colorbar_title) {
  if (_colorbar_title == colorbar_title)
    return;
  colorbar_title = _colorbar_title;
  _signal_changed.emit();
}

void PlotContourShades::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

  draw_plot_init(cr, width, height);


  pls->adv(0);
  pls->vpor(0.1, 0.9, 0.1, 0.9);
  pls->wind(plotted_range_x[0], plotted_range_x[1],
            plotted_range_y[0], plotted_range_y[1]);

  //set the label color
  change_plstream_color(pls, titles_color);

  //draw the labels
  pls->lab(axis_title_x.c_str(), axis_title_y.c_str(), plot_title.c_str());

  //draw the actual plot
  plot_data[0]->draw_plot_data(cr, pls);

  //draw colorbar if requested
  if (showing_colorbar)
    dynamic_cast<PlotDataContourShades *>(plot_data[0])->draw_colorbar(cr, pls, colorbar_title, axes_color);

  //plot the box with its axes
  change_plstream_color(pls, axes_color);
  pls->box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);

  cr->restore();

  convert_plplot_to_cairo_coordinates(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  convert_plplot_to_cairo_coordinates(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}

PlotContourShades *PlotContourShades::clone() const {
  PlotContourShades *my_clone = new PlotContourShades(*this);
  if(typeid(*this) != typeid(*my_clone)) {
    throw Exception("Gtk::PLplot::PlotContourShades::clone -> Classes that derive from Plot must implement clone!");
  }
  return my_clone;
}
