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

#include <gtkmm-plplot/plotpolar.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <gtkmm-plplot/enums.h>
#include <iostream>
#include <gdkmm/rgba.h>
#include <gdkmm/general.h>
#include <cmath>
#include <sstream>

using namespace Gtk::PLplot;

PlotPolar::PlotPolar(
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
  _plot_offset_vertical_norm) {}

PlotPolar::PlotPolar(
  const PlotData2D &_data,
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

PlotPolar::PlotPolar(const PlotPolar &_source) :
  PlotPolar(_source.axis_title_x, _source.axis_title_y,
  _source.plot_title, _source.plot_width_norm,
  _source.plot_height_norm, _source.plot_offset_horizontal_norm,
  _source.plot_offset_vertical_norm) {

  for (auto &iter : _source.plot_data) {
    add_data(*iter);
  }
}

PlotPolar::~PlotPolar() {

}

void PlotPolar::plot_data_modified() {
  //update ranges
  //since we are dealing with polar coordinates, we really need to look only
  //at the maximum x (r here)

  std::vector<PLFLT> max_x;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotData2D*>(iter);
    std::vector<PLFLT> x = iter2->get_vector_x();
    max_x.push_back(*std::max_element(x.begin(), x.end()));
  }

  max_r = *std::max_element(max_x.begin(), max_x.end());

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

PlotData2D *PlotPolar::add_data(const PlotData &data) {
  PlotData2D *data_copy = nullptr;
  try {
    //ensure our data is PlotData2D
    data_copy = new PlotData2D(dynamic_cast<const PlotData2D &>(data));
  }
  catch (std::bad_cast &e) {
    throw Exception("Gtk::PLplot::PlotPolar::add_data -> data must be of PlotData2D type!");
  }

  //throw error if any of the r values are negative
  std::vector<PLFLT> x = data_copy->get_vector_x();
  if (std::count_if(x.begin(), x.end(), std::bind2nd(std::less<PLFLT>(), double(0.0))) > 0) {
    throw Exception("Gtkmm::Plplot::PlotPolar::add_data -> plot R-values must be  positive");
  }

  plot_data.push_back(data_copy);
  data_copy->signal_changed().connect([this](){_signal_changed.emit();});
  data_copy->signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(data_copy);
  return data_copy;
}

void PlotPolar::coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT *x_new, PLFLT *y_new, PLPointer object) {
  *x_new = x_old * cos(y_old);
  *y_new = x_old * sin(y_old);
}

void PlotPolar::coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new) {
    coordinate_transform_world_to_plplot(x_old, y_old, &x_new, &y_new, nullptr);
}

void PlotPolar::coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT *x_new, PLFLT *y_new, PLPointer object) {
  *x_new = sqrt(x_old * x_old + y_old * y_old);
  *y_new = atan2(y_old, x_old);
}

void PlotPolar::coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new) {
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
  cr->restore();

  convert_plplot_to_cairo_coordinates(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  convert_plplot_to_cairo_coordinates(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}

PlotPolar *PlotPolar::clone() const {
  PlotPolar *my_clone = new PlotPolar(*this);
  if(typeid(*this) != typeid(*my_clone)) {
    throw Exception("Gtk::PLplot::PlotPolar::clone -> Classes that derive from Plot must implement clone!");
  }
  return my_clone;
}