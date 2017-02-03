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

#include <gtkmm-plplot/plot2d.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <iostream>
#include <gdkmm/rgba.h>
#include <plstream.h>

#define PLPLOT_LIN_LIN 0
#define PLPLOT_LIN_LOG 20
#define PLPLOT_LOG_LIN 10
#define PLPLOT_LOG_LOG 30


using namespace Gtk::PLplot;

Plot2D::Plot2D(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Glib::ObjectBase("GtkmmPLplotPlot2D"),
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm),
  Legend(),
  RegionSelection(),
  log10_x(false),
  log10_y(false),
  box_style(BOX_TICKS_TICK_LABELS) {

}

Plot2D::Plot2D(
  PlotData2D &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Plot2D(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {

  add_data(_data);
}

Plot2D::~Plot2D() {}

void Plot2D::plot_data_modified() {
  //update ranges
  std::vector<double> min_x, min_y;
  std::vector<double> max_x, max_y;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotData2D*>(iter);
    double xmin, xmax, ymin, ymax;
    iter2->get_extremes(xmin, xmax, ymin, ymax);
    min_x.push_back(xmin);
    max_x.push_back(xmax);
    min_y.push_back(ymin);
    max_y.push_back(ymax);
  }

  if (min_x.empty()) {
    min_x.push_back(0.0);
    max_x.push_back(1.0);
    min_y.push_back(0.0);
    max_y.push_back(1.0);
  }

  plot_data_range_x[0] = *std::min_element(min_x.begin(), min_x.end());
  plot_data_range_x[1] = *std::max_element(max_x.begin(), max_x.end());
  plot_data_range_y[0] = *std::min_element(min_y.begin(), min_y.end());
  plot_data_range_y[1] = *std::max_element(max_y.begin(), max_y.end());

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

void Plot2D::add_data(PlotData2D &data) {
  // ensure data is not already present in plot_data
  auto iter = std::find(plot_data.begin(), plot_data.end(), &data);
  if (iter != plot_data.end())
    throw Exception("Gtk::PLplot::Plot2D::add_data -> Data has been added before to this plot");

  plot_data.push_back(&data);
  data.signal_changed().connect([this](){_signal_changed.emit();});
  data.signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(&data);
}

void Plot2D::remove_data(unsigned int index) {
  if (plot_data.size() == 0)
    throw Exception("Gtk::PLplot::Plot2D::remove_data -> No more datasets left to remove");

  if (index >= plot_data.size())
    throw Exception("Gtk::PLplot::Plot2D::remove_data -> Invalid index");

  PlotData *removed_plot = plot_data[index];

  plot_data.erase(plot_data.begin() + index);
  _signal_data_removed.emit(removed_plot);
}

void Plot2D::remove_data(PlotData &plot_data_member) {
  if (plot_data.size() == 0)
    throw Exception("Gtk::PLplot::Plot2D::remove_data -> No more datasets left to remove");

  auto iter = std::find(plot_data.begin(), plot_data.end(), &plot_data_member);
  if (iter == plot_data.end())
    throw Exception("Gtk::PLplot::Plot::remove_data -> No match for argument");

  PlotData *removed_plot = *iter;

  plot_data.erase(iter);
  _signal_data_removed.emit(removed_plot);
}

void Plot2D::set_axis_logarithmic_x(bool _log10) {
  //need to check that all values are positive!
  if (_log10) {
    for (auto &iter : plot_data) {
      auto iter2 = dynamic_cast<PlotData2D*>(iter);
      std::vector<double> x = iter2->get_vector_x();
      if (std::count_if(x.begin(), x.end(), std::bind2nd(std::less_equal<double>(), double(0.0))) > 0) {
        throw Exception("Gtkmm::Plplot::Plot2D::set_axis_logarithmic_x -> plot X-values must be strictly positive");
      }
    }
  }
  log10_x = _log10;
  plot_data_modified();
}

void Plot2D::set_axis_logarithmic_y(bool _log10) {
  //need to check that all values are positive!
  if (_log10) {
    for (auto &iter : plot_data) {
      auto iter2 = dynamic_cast<PlotData2D*>(iter);
      std::vector<double> y = iter2->get_vector_y();
      if (std::count_if(y.begin(), y.end(), std::bind2nd(std::less_equal<double>(), double(0.0))) > 0) {
        throw Exception("Gtkmm::Plplot::Plot2D::set_axis_logarithmic_y -> plot Y-values must be strictly positive");
      }
    }
  }
  log10_y = _log10;
  plot_data_modified();
}

bool Plot2D::get_axis_logarithmic_x() {
  return log10_x;
}

bool Plot2D::get_axis_logarithmic_y() {
  return log10_y;
}

void Plot2D::set_box_style(BoxStyle _box_style) {
  box_style = _box_style;
  _signal_changed.emit();
}

BoxStyle Plot2D::get_box_style() {
  return box_style;
}

void Plot2D::coordinate_transform_world_to_plplot(double x_old, double y_old, double *x_new, double *y_new, PLPointer object) {
  Plot2D *plot2d = static_cast<Plot2D*>(object);

  //let's start with giving the new ones the old values, in case no transformation is necessary
  *x_new = x_old;
  *y_new = y_old;

  if (plot2d->log10_x)
    *x_new = log10(x_old);

  if (plot2d->log10_y)
    *y_new = log10(y_old);
}

void Plot2D::coordinate_transform_world_to_plplot(double x_old, double y_old, double &x_new, double &y_new) {
    coordinate_transform_world_to_plplot(x_old, y_old, &x_new, &y_new, this);
}

void Plot2D::coordinate_transform_plplot_to_world(double x_old, double y_old, double *x_new, double *y_new, PLPointer object) {
  Plot2D *plot2d = static_cast<Plot2D*>(object);

  //let's start with giving the new ones the old values, in case no transformation is necessary
  *x_new = x_old;
  *y_new = y_old;

  if (plot2d->log10_x)
    *x_new = pow(10.0, x_old);

  if (plot2d->log10_y)
    *y_new = pow(10.0, y_old);
}

void Plot2D::coordinate_transform_plplot_to_world(double x_old, double y_old, double &x_new, double &y_new) {
    coordinate_transform_plplot_to_world(x_old, y_old, &x_new, &y_new, this);
}


//it's possible I can get the width and height also from the Cairo context through its surface
//problem is that this only works for image surfaces, not for PDF etc :-(
void Plot2D::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

  draw_plot_init(cr, width, height);

  int plplot_axis_style = box_style;

  if (box_style >= 0 ) {
    if (log10_x && log10_y)
      plplot_axis_style += PLPLOT_LOG_LOG;
    else if (log10_x)
      plplot_axis_style += PLPLOT_LOG_LIN;
    else if (log10_y)
      plplot_axis_style += PLPLOT_LIN_LOG;
    else
      plplot_axis_style += PLPLOT_LIN_LIN;
  }

  change_plstream_color(pls, axes_color);

  //plot the box with its axes
  pls->env(plotted_range_x[0], plotted_range_x[1],
           plotted_range_y[0], plotted_range_y[1],
           0, plplot_axis_style);

  //set the label color
  change_plstream_color(pls, titles_color);

  pls->lab(axis_title_x.c_str(), axis_title_y.c_str(), plot_title.c_str());

  //hook up the coordinate transform
  pls->stransform(&Plot2D::coordinate_transform_world_to_plplot, this);

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
