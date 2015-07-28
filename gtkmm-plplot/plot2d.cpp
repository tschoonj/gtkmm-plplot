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

#include <gtkmm-plplot/plot2d.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <iostream>
#include <gdkmm/rgba.h>
#include <gdkmm/general.h>

#define PLPLOT_LIN_LIN 0
#define PLPLOT_LIN_LOG 20
#define PLPLOT_LOG_LIN 10
#define PLPLOT_LOG_LOG 30


using namespace Gtk::PLplot;

Plot2D::Plot2D(
  const PlotData2D &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotAbstract(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm),
  log10_x(false),
  log10_y(false) {

  add_data(_data);
}

Plot2D::Plot2D(const Plot2D &_source) :
  PlotAbstract(_source),
  log10_x(_source.log10_x),
  log10_y(_source.log10_y) {

  for (auto &iter : _source.plot_data) {
    add_data(*iter);
  }
}

Plot2D::~Plot2D() {

}

void Plot2D::plot_data_modified() {
  //update ranges
  std::vector<PLFLT> min_x, min_y;
  std::vector<PLFLT> max_x, max_y;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotData2D*>(iter);
    min_x.push_back(*std::min_element(iter2->x.begin(), iter2->x.end()));
    max_x.push_back(*std::max_element(iter2->x.begin(), iter2->x.end()));
    min_y.push_back(*std::min_element(iter2->y.begin(), iter2->y.end()));
    max_y.push_back(*std::max_element(iter2->y.begin(), iter2->y.end()));
  }
  plot_data_range_x[0] = *std::min_element(min_x.begin(), min_x.end());
  plot_data_range_x[1] = *std::max_element(max_x.begin(), max_x.end());
  plot_data_range_y[0] = *std::min_element(min_y.begin(), min_y.end());
  plot_data_range_y[1] = *std::max_element(max_y.begin(), max_y.end());

  if (log10_x) {
    plot_data_range_x[0] = log10(plot_data_range_x[0]);
    plot_data_range_x[1] = log10(plot_data_range_x[1]);
  }
  if (log10_y) {
    plot_data_range_y[0] = log10(plot_data_range_y[0]);
    plot_data_range_y[1] = log10(plot_data_range_y[1]);
  }

  plotted_range_x[0] = plot_data_range_x[0];
  plotted_range_x[1] = plot_data_range_x[1];
  plotted_range_y[0] = plot_data_range_y[0];
  plotted_range_y[1] = plot_data_range_y[1];
  _signal_changed.emit();
}

PlotDataAbstract *Plot2D::add_data(const PlotDataAbstract &data) {
  try {
    //ensure our data is PlotData2D
    const PlotData2D &ref = dynamic_cast<const PlotData2D &>(data);
  }
  catch (std::bad_cast &e) {
    throw Exception("Gtk::PLplot::Plot2D::add_data -> data must be of PlotData2D type!");
  }
  PlotDataAbstract *data_copy = new PlotData2D(static_cast<const PlotData2D &>(data));
  plot_data.push_back(data_copy);
  data_copy->signal_changed().connect([this](){_signal_changed.emit();});
  data_copy->signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(data_copy);
  return data_copy;
}

void Plot2D::set_axis_logarithmic_x(bool _log10) {
  //need to check that all values are positive!
  if (_log10) {
    for (auto &iter : plot_data) {
      auto iter2 = dynamic_cast<PlotData2D*>(iter);
      if (std::count_if(iter2->x.begin(), iter2->x.end(), std::bind2nd(std::less_equal<PLFLT>(), double(0.0))) > 0) {
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
      if (std::count_if(iter2->y.begin(), iter2->y.end(), std::bind2nd(std::less_equal<PLFLT>(), double(0.0))) > 0) {
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

void Plot2D::coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT *x_new, PLFLT *y_new, PLPointer object) {
  Plot2D *plot2d = static_cast<Plot2D*>(object);

  //let's start with giving the new ones the old values, in case no transformation is necessary
  *x_new = x_old;
  *y_new = y_old;

  if (plot2d->log10_x)
    *x_new = log10(x_old);

  if (plot2d->log10_y)
    *y_new = log10(y_old);
}

void Plot2D::coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new) {
    coordinate_transform_world_to_plplot(x_old, y_old, &x_new, &y_new, this);
}

void Plot2D::coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT *x_new, PLFLT *y_new, PLPointer object) {
  Plot2D *plot2d = static_cast<Plot2D*>(object);

  //let's start with giving the new ones the old values, in case no transformation is necessary
  *x_new = x_old;
  *y_new = y_old;

  if (plot2d->log10_x)
    *x_new = pow(10.0, x_old);

  if (plot2d->log10_y)
    *y_new = pow(10.0, y_old);
}

void Plot2D::coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new) {
    coordinate_transform_plplot_to_world(x_old, y_old, &x_new, &y_new, this);
}


//it's possible I can get the width and height also from the Cairo context through its surface
//problem is that this only works for image surfaces, not for PDF etc :-(
void Plot2D::draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) {
  if (!shown)
    return;

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

  int plplot_axis_style;

  plplot_axis_style = box_style;

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
  cr->restore();

  convert_plplot_to_cairo_coordinates(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  convert_plplot_to_cairo_coordinates(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}


void Plot2D::convert_plplot_to_cairo_coordinates(
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

void Plot2D::convert_cairo_to_plplot_coordinates(
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

void Plot2D::set_region(double xmin, double xmax, double ymin, double ymax) {
  if (xmin == xmax && ymin == ymax) {
    //due to signal propagation, this function will actually be called twice on a double-click event,
    //the second time after the plot has already been resized to its normal geometry
    //this condition avoids the warning message...
    return;
  }

  if (log10_x) {
    xmin = log10(xmin);
    xmax = log10(xmax);
  }
  if (log10_y) {
    ymin = log10(ymin);
    ymax = log10(ymax);
  }

  if (xmin >= xmax || ymin >= ymax /*||
      xmin < plot_data_range_x[0] ||
      xmax > plot_data_range_x[1] ||
      ymin < plot_data_range_y[0] ||
      ymax > plot_data_range_y[1]*/) {
    throw Exception("Gtk::PLplot::Plot2D::set_region -> Invalid arguments");
  }
  plotted_range_x[0] = xmin;
  plotted_range_x[1] = xmax;
  plotted_range_y[0] = ymin;
  plotted_range_y[1] = ymax;

  _signal_changed.emit();
}

PlotAbstract *Plot2D::clone() const {
  PlotAbstract *my_clone = new Plot2D(*this);
  if( typeid(*this) != typeid(*my_clone) ) {
    throw Exception("Gtk::PLplot::Plot2D::clone -> Classes that derive from PlotAbstract must implement clone!");
  }
  return my_clone;
}
