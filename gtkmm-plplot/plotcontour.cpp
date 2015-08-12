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

#include <gtkmm-plplot/plotcontour.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <iostream>

using namespace Gtk::PLplot;

PlotContour::PlotContour(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {
}

PlotContour::PlotContour(
  const PlotDataContour &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {

  add_data(_data);
}

PlotContour::PlotContour(const PlotContour &_source) :
  PlotContour(_source.axis_title_x, _source.axis_title_y,
  _source.plot_title, _source.plot_width_norm,
  _source.plot_height_norm, _source.plot_offset_horizontal_norm,
  _source.plot_offset_vertical_norm) {

  for (auto &iter : _source.plot_data) {
    add_data(*iter);
  }
}

PlotContour::~PlotContour() {}

void PlotContour::plot_data_modified() {
  //update ranges
  std::vector<PLFLT> min_x, min_y;
  std::vector<PLFLT> max_x, max_y;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotDataContour*>(iter);
    std::vector<PLFLT> x = iter2->get_vector_x();
    std::vector<PLFLT> y = iter2->get_vector_y();
    min_x.push_back(*std::min_element(x.begin(), x.end()));
    max_x.push_back(*std::max_element(x.begin(), x.end()));
    min_y.push_back(*std::min_element(y.begin(), y.end()));
    max_y.push_back(*std::max_element(y.begin(), y.end()));
  }
  plot_data_range_x[0] = *std::min_element(min_x.begin(), min_x.end());
  plot_data_range_x[1] = *std::max_element(max_x.begin(), max_x.end());
  plot_data_range_y[0] = *std::min_element(min_y.begin(), min_y.end());
  plot_data_range_y[1] = *std::max_element(max_y.begin(), max_y.end());

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

PlotDataContour *PlotContour::add_data(const PlotData &data) {
  //ensure plot_data is empty
  if (!plot_data.empty())
    throw Exception("Gtk::PLplot::PlotContour::add_data -> cannot add data when plot_data is not empty!");

  PlotDataContour *data_copy = nullptr;
  try {
    //ensure our data is PlotDataContour
    data_copy = new PlotDataContour(dynamic_cast<const PlotDataContour &>(data));
  }
  catch (std::bad_cast &e) {
    throw Exception("Gtk::PLplot::PlotContour::add_data -> data must be of PlotDataContour type!");
  }
  plot_data.push_back(data_copy);
  data_copy->signal_changed().connect([this](){_signal_changed.emit();});
  data_copy->signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(data_copy);
  return data_copy;
}

void PlotContour::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

  draw_plot_init(cr, width, height);

  change_plstream_color(pls, axes_color);

  //plot the box with its axes
  pls->adv(0);
  pls->vpor(0.1, 0.9, 0.1, 0.9);
  pls->wind(plotted_range_x[0], plotted_range_x[1],
            plotted_range_y[0], plotted_range_y[1]);
  pls->box( "bcnst", 0.0, 0, "bcnstv", 0.0, 0 );


  //set the label color
  change_plstream_color(pls, titles_color);

  pls->lab(axis_title_x.c_str(), axis_title_y.c_str(), plot_title.c_str());

  plot_data[0]->draw_plot_data(cr, pls);

  cr->restore();

  convert_plplot_to_cairo_coordinates(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  convert_plplot_to_cairo_coordinates(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}

PlotContour *PlotContour::clone() const {
  PlotContour *my_clone = new PlotContour(*this);
  if(typeid(*this) != typeid(*my_clone)) {
    throw Exception("Gtk::PLplot::PlotContour::clone -> Classes that derive from Plot must implement clone!");
  }
  return my_clone;
}
