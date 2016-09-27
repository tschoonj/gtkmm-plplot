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

#include <gtkmm-plplot/plot3d.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <iostream>
#include <gdkmm/rgba.h>
#include <plstream.h>

using namespace Gtk::PLplot;

Plot3D::Plot3D(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_axis_title_z,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Glib::ObjectBase("GtkmmPLplotPlot3D"),
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm),
  axis_title_z(_axis_title_z),
  altitude(45.0),
  azimuth(45.0) {}

Plot3D::Plot3D(
  PlotData3D &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_axis_title_z,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Plot3D(_axis_title_x, _axis_title_y, _axis_title_z, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {

  add_data(_data);
}

Plot3D::~Plot3D() {}

void Plot3D::set_altitude(double _altitude) {
  if (altitude == _altitude)
    return;
  if (_altitude < 0 || _altitude > 90.0)
    throw Exception("Gtk::PLplot::Plot3D::set_altitude -> altitude must be between 0 and 90");
  altitude = _altitude;
  _signal_changed.emit();
}

double Plot3D::get_altitude() {
  return altitude;
}

void Plot3D::set_azimuth(double _azimuth) {
  if (azimuth == _azimuth)
    return;
  azimuth = _azimuth;
  _signal_changed.emit();
}

double Plot3D::get_azimuth() {
  return azimuth;
}

void Plot3D::plot_data_modified() {
  //update ranges
  std::vector<double> min_x, min_y, min_z;
  std::vector<double> max_x, max_y, max_z;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotData3D*>(iter);
    std::vector<double> x = iter2->get_vector_x();
    std::vector<double> y = iter2->get_vector_y();
    std::vector<double> z = iter2->get_vector_z();
    min_x.push_back(*std::min_element(x.begin(), x.end()));
    max_x.push_back(*std::max_element(x.begin(), x.end()));
    min_y.push_back(*std::min_element(y.begin(), y.end()));
    max_y.push_back(*std::max_element(y.begin(), y.end()));
    min_z.push_back(*std::min_element(z.begin(), z.end()));
    max_z.push_back(*std::max_element(z.begin(), z.end()));
  }
  plot_data_range_x[0] = *std::min_element(min_x.begin(), min_x.end());
  plot_data_range_x[1] = *std::max_element(max_x.begin(), max_x.end());
  plot_data_range_y[0] = *std::min_element(min_y.begin(), min_y.end());
  plot_data_range_y[1] = *std::max_element(max_y.begin(), max_y.end());
  plot_data_range_z[0] = *std::min_element(min_z.begin(), min_z.end());
  plot_data_range_z[1] = *std::max_element(max_z.begin(), max_z.end());

  plotted_range_x[0] = plot_data_range_x[0];
  plotted_range_x[1] = plot_data_range_x[1];
  plotted_range_y[0] = plot_data_range_y[0];
  plotted_range_y[1] = plot_data_range_y[1];
  plotted_range_z[0] = plot_data_range_z[0];
  plotted_range_z[1] = plot_data_range_z[1];

  _signal_changed.emit();
}

void Plot3D::add_data(PlotData3D &data) {
  // ensure data is not already present in plot_data
  auto iter = std::find(plot_data.begin(), plot_data.end(), &data);
  if (iter != plot_data.end())
    throw Exception("Gtk::PLplot::Plot3D::add_data -> Data has been added before to this plot");

  plot_data.push_back(&data);
  data.signal_changed().connect([this](){_signal_changed.emit();});
  data.signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(&data);
}

void Plot3D::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

  draw_plot_init(cr, width, height);

  change_plstream_color(pls, axes_color);

  pls->adv(0);
  pls->vpor(0.025, 0.975, 0.025, 0.9);
  pls->wind(-1.0, 1.0, -0.9, 1.1); //not sure what the effect of this is
  pls->w3d(1.0, 1.0, 1.0,
    plotted_range_x[0], plotted_range_x[1],
    plotted_range_y[0], plotted_range_y[1],
    plotted_range_z[0], plotted_range_z[1],
    altitude, azimuth);

  change_plstream_color(pls, titles_color);

  pls->box3("bnstu", axis_title_x.c_str(), 0.0, 0,
    "bnstu", axis_title_y.c_str(), 0.0, 0,
    "bcdmnstuv", axis_title_z.c_str(), 0.0, 0);


  for (auto &iter : plot_data) {
    iter->draw_plot_data(cr, pls);
  }

  change_plstream_color(pls, titles_color);
  pls->mtex("t", 1.0, 0.5, 0.5, plot_title.c_str());

  //legend
  if (is_showing_legend())
    draw_legend(cr, plot_data, pls);

  cr->restore();
}
