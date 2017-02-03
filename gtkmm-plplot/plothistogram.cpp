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

#include <gtkmm-plplot/plothistogram.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <plstream.h>

using namespace Gtk::PLplot;

PlotHistogram::PlotHistogram(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Glib::ObjectBase("GtkmmPLplotPlotHistogram"),
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {}

PlotHistogram::PlotHistogram(
  PlotDataHistogram &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotHistogram(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm, _plot_offset_vertical_norm) {

  add_data(_data);
}

PlotHistogram::~PlotHistogram() {}

void PlotHistogram::plot_data_modified() {
  auto data = dynamic_cast<PlotDataHistogram*>(plot_data[0]);

  data->get_extremes(
    plot_data_range_x[0], plot_data_range_x[1],
    plot_data_range_y[0], plot_data_range_y[1]);

  //This function won't do anything as I am using the default implementation
  //provided by Plot
  coordinate_transform_world_to_plplot(
    plot_data_range_x[0], plot_data_range_y[0],
    plot_data_range_x[0], plot_data_range_y[0]
  );
  coordinate_transform_world_to_plplot(
    plot_data_range_x[1], plot_data_range_y[1],
    plot_data_range_x[1], plot_data_range_y[1]
  );

  plotted_range_x[0] = plot_data_range_x[0];
  plotted_range_x[1] = plot_data_range_x[1];
  plotted_range_y[0] = plot_data_range_y[0];
  plotted_range_y[1] = plot_data_range_y[1];

  _signal_changed.emit();
}

void PlotHistogram::add_data(PlotDataHistogram &data) {
  //ensure plot_data is empty
  if (!plot_data.empty())
    throw Exception("Gtk::PLplot::PlotHistogram::add_data -> cannot add data when plot_data is not empty!");

  plot_data.push_back(&data);
  data.signal_changed().connect([this](){_signal_changed.emit();});
  data.signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(&data);
}

void PlotHistogram::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

  draw_plot_init(cr, width, height);

  int plplot_axis_style = 0;

  change_plstream_color(pls, axes_color);

  //plot the box with its axes
  pls->env(plotted_range_x[0], plotted_range_x[1],
           plotted_range_y[0], plotted_range_y[1],
           0, plplot_axis_style);

  //set the label color
  change_plstream_color(pls, titles_color);

  pls->lab(axis_title_x.c_str(), axis_title_y.c_str(), plot_title.c_str());

  plot_data[0]->draw_plot_data(cr, pls);

  cr->restore();

  coordinate_transform_plplot_to_cairo(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  coordinate_transform_plplot_to_cairo(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}
