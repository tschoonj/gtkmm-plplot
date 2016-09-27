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

#include <gtkmm-plplot/plotpolar.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <gtkmm-plplot/enums.h>
#include <iostream>
#include <gdkmm/rgba.h>
#include <gdkmm/general.h>
#include <cmath>
#include <sstream>
#include <plstream.h>

#ifndef M_SQRT2
  #define M_SQRT2 (sqrt(2.0))
#endif

#ifndef M_PI
  #define M_PI (3.14159265358979323846)
#endif

#ifndef M_PI_4
  #define M_PI_4 (M_PI / 4.0)
#endif

#ifndef M_PI_2
  #define M_PI_2 (M_PI / 2.0)
#endif


using namespace Gtk::PLplot;

PlotPolar::PlotPolar(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Glib::ObjectBase("GtkmmPLplotPlotPolar"),
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {}

PlotPolar::PlotPolar(
  PlotDataPolar &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotPolar(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {

  add_data(_data);
}

PlotPolar::~PlotPolar() {

}

void PlotPolar::plot_data_modified() {
  //update ranges
  //since we are dealing with polar coordinates, we really need to look only
  //at the maximum x (r here)

  std::vector<double> vec_max_r;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotDataPolar*>(iter);
    double r;
    iter2->get_max_r(r);
    vec_max_r.push_back(r);
  }

  if (vec_max_r.empty()) {
    max_r = 1.0;
  }
  else {
    max_r = *std::max_element(vec_max_r.begin(), vec_max_r.end());
  }

  plot_data_range_x[0] =
  plot_data_range_x[1] = max_r * M_SQRT2 * 1.3;
  plot_data_range_y[0] = 5.0 * M_PI_4;
  plot_data_range_y[1] = M_PI_4;

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

void PlotPolar::add_data(PlotDataPolar &data) {
  // ensure data is not already present in plot_data
  auto iter = std::find(plot_data.begin(), plot_data.end(), &data);
  if (iter != plot_data.end())
    throw Exception("Gtk::PLplot::PlotPolar::add_data -> Data has been added before to this plot");

  //throw error if any of the r values are negative
  std::vector<double> x = data.get_vector_x();
  if (std::count_if(x.begin(), x.end(), std::bind2nd(std::less<double>(), double(0.0))) > 0) {
    throw Exception("Gtkmm::Plplot::PlotPolar::add_data -> plot R-values must be  positive");
  }

  plot_data.push_back(&data);
  data.signal_changed().connect([this](){_signal_changed.emit();});
  data.signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(&data);
}

void PlotPolar::remove_data(unsigned int index) {
  if (plot_data.size() == 0)
    throw Exception("Gtk::PLplot::PlotPolar::remove_data -> No more datasets left to remove");

  if (index >= plot_data.size())
    throw Exception("Gtk::PLplot::PlotPolar::remove_data -> Invalid index");

  PlotData *removed_plot = plot_data[index];

  plot_data.erase(plot_data.begin() + index);
  _signal_data_removed.emit(removed_plot);
}

void PlotPolar::remove_data(PlotData &plot_data_member) {
  if (plot_data.size() == 0)
    throw Exception("Gtk::PLplot::PlotPolar::remove_data -> No more datasets left to remove");

  auto iter = std::find(plot_data.begin(), plot_data.end(), &plot_data_member);
  if (iter == plot_data.end())
    throw Exception("Gtk::PLplot::PlotPolar::remove_data -> No match for argument");

  PlotData *removed_plot = *iter;

  plot_data.erase(iter);
  _signal_data_removed.emit(removed_plot);
}

void PlotPolar::coordinate_transform_world_to_plplot(double x_old, double y_old, double *x_new, double *y_new, PLPointer object) {
  *x_new = x_old * cos(y_old);
  *y_new = x_old * sin(y_old);
}

void PlotPolar::coordinate_transform_world_to_plplot(double x_old, double y_old, double &x_new, double &y_new) {
    coordinate_transform_world_to_plplot(x_old, y_old, &x_new, &y_new, nullptr);
}

void PlotPolar::coordinate_transform_plplot_to_world(double x_old, double y_old, double *x_new, double *y_new, PLPointer object) {
  *x_new = sqrt(x_old * x_old + y_old * y_old);
  *y_new = atan2(y_old, x_old);
}

void PlotPolar::coordinate_transform_plplot_to_world(double x_old, double y_old, double &x_new, double &y_new) {
    coordinate_transform_plplot_to_world(x_old, y_old, &x_new, &y_new, nullptr);
}

void PlotPolar::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

  draw_plot_init(cr, width, height);

  change_plstream_color(pls, axes_color);

  //plot the box with its axes
  pls->env(plotted_range_x[0], plotted_range_x[1],
           plotted_range_y[0], plotted_range_y[1],
           1, -2);

  //hook up the coordinate transform
  //this will affect all following PLplot commands!
  pls->stransform(&PlotPolar::coordinate_transform_world_to_plplot, nullptr);

  //additional things to be drawn: circles and lines
  //8 major and 16 minor lines
  //major lines have label with angle
  for (int i = 0; i < 24; i++) {
    if (i % 3 != 0)
      pls->lsty(LineStyle::SHORT_DASH_SHORT_GAP);
    else {
      pls->lsty(LineStyle::CONTINUOUS);
      std::ostringstream oss;
      oss << round(i * 2.0 * 180.0/24.0) << "°";
      pls->ptex(max_r * 1.2, i * 2.0 * M_PI/24.0, 0.0, 0.0, 0.5, oss.str().c_str());
    }
    pls->join(0.0, 0.0, max_r * 1.1, i * 2.0 * M_PI/24.0);
  }
  pls->lsty(LineStyle::CONTINUOUS);

  //get tickstep
  double tickstep = 1E-10;
  int nticks = (int) floor(max_r/tickstep);

  while (nticks < 1 || nticks >= 10) {
    tickstep *= 10.0;
    nticks = (int) floor(max_r/tickstep);
  }

  if (nticks == 1) {
    tickstep /= 5.0;
  }
  else if (nticks == 2) {
    tickstep /= 2.0;
  }

  double tickpos = tickstep;

  do {
    pls->arc(0.0, 0.0, tickpos, tickpos, 0.0, 360.0, 0.0, 0);
    std::ostringstream oss;
    oss << tickpos;

    pls->ptex(tickpos + tickstep * 0.2, M_PI_2, 0.0, 0.0, 1.1, oss.str().c_str());

    tickpos += tickstep;
  } while(tickpos <= max_r);

  //set the label color
  change_plstream_color(pls, titles_color);

  pls->lab(axis_title_x.c_str(), axis_title_y.c_str(), plot_title.c_str());

  for (auto &iter : plot_data) {
    iter->draw_plot_data(cr, pls);
  }

  //undo the coordinate transform
  pls->stransform(NULL, NULL);

  //legend
  if (is_showing_legend())
    draw_legend(cr, plot_data, pls);

  cr->restore();

  coordinate_transform_plplot_to_cairo(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  coordinate_transform_plplot_to_cairo(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}
