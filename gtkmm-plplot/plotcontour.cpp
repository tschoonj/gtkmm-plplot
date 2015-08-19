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
#include <plConfig.h>

#ifdef GTKMM_PLPLOT_PLPLOT_5_11_0
	#define PLCALLBACK plcallback
#else
	#define PLCALLBACK plstream
#endif

using namespace Gtk::PLplot;

PlotContour::PlotContour(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  double _edge_width,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  Plot(_axis_title_x, _axis_title_y, _plot_title,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm),
  nlevels(_nlevels),
  edge_color(_edge_color),
  edge_width(_edge_width),
	showing_labels(true),
  clevels(_nlevels) {
  //ensure edge_width is strictly positive
  if (edge_width <= 0.0) {
    throw Exception("Gtk::PLplot::PlotContour::PlotContour-> edge width must be strictly positive");
  }

  //ensure nlevels is at least 3
  if (nlevels < 3) {
    throw Exception("Gtk::PLplot::PlotContour::PlotContour-> nlevels must be greater than or equal to 3");
  }
}

PlotContour::PlotContour(
  const PlotDataSurface &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  double _edge_width,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotContour(_axis_title_x, _axis_title_y, _plot_title,
  _nlevels, _edge_color, _edge_width,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {

  add_data(_data);
}

PlotContour::PlotContour(const PlotContour &_source) :
  PlotContour(_source.axis_title_x, _source.axis_title_y,
  _source.plot_title, _source.nlevels,
  _source.edge_color, _source.edge_width,
  _source.plot_width_norm,
  _source.plot_height_norm,
  _source.plot_offset_horizontal_norm,
  _source.plot_offset_vertical_norm) {

	showing_labels = _source.showing_labels;

  for (auto &iter : _source.plot_data) {
    add_data(*iter);
  }
}

PlotContour::~PlotContour() {}

void PlotContour::plot_data_modified() {
  auto data = dynamic_cast<PlotDataSurface*>(plot_data[0]);
  std::vector<PLFLT> x = data->get_vector_x();
  std::vector<PLFLT> y = data->get_vector_y();

  plot_data_range_x[0] = x.front();
  plot_data_range_x[1] = x.back();
  plot_data_range_y[0] = y.front();
  plot_data_range_y[1] = y.back();

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

  //get maximum of z
  PLFLT **z = data->get_array2d_z();
  plMinMax2dGrid(z, x.size(), y.size(), &zmax, &zmin);
  free_array2d((void **) z, x.size());

  //fill up level
  for (unsigned int i = 0 ; i < nlevels ; i++) {
    clevels[i] = zmin + (zmax - zmin) * i / (PLFLT) (nlevels - 1);
  }

  _signal_changed.emit();
}

PlotDataSurface *PlotContour::add_data(const PlotData &data) {
  //ensure plot_data is empty
  if (!plot_data.empty())
    throw Exception("Gtk::PLplot::PlotContour::add_data -> cannot add data when plot_data is not empty!");

  PlotDataSurface *data_copy = nullptr;
  try {
    //ensure our data is PlotDataSurface
    data_copy = new PlotDataSurface(dynamic_cast<const PlotDataSurface &>(data));
  }
  catch (std::bad_cast &e) {
    throw Exception("Gtk::PLplot::PlotContour::add_data -> data must be of PlotDataSurface type!");
  }
  plot_data.push_back(data_copy);
  data_copy->signal_changed().connect([this](){_signal_changed.emit();});
  data_copy->signal_data_modified().connect([this](){plot_data_modified();});

  _signal_data_added.emit(data_copy);
  return data_copy;
}

void PlotContour::set_edge_color(Gdk::RGBA _edge_color) {
  edge_color = _edge_color;
  _signal_changed.emit();
}

Gdk::RGBA PlotContour::get_edge_color() {
  return edge_color;
}

void PlotContour::set_edge_width(double _edge_width) {
  //ensure edge_width is strictly positive
  if (_edge_width <= 0.0) {
    throw Exception("Gtk::PLplot::PlotContour::PlotContour -> edge width must be strictly positive");
  }

  edge_width = _edge_width;
  _signal_changed.emit();
}

double PlotContour::get_edge_width() {
  return edge_width;
}

void PlotContour::set_nlevels(unsigned int _nlevels) {
  //ensure nlevels is at least 3
  if (_nlevels < 3) {
    throw Exception("Gtk::PLplot::PlotContour::PlotContour -> nlevels must be greater than or equal to 3");
  }

  if (_nlevels == nlevels)
    return;

  clevels.clear();
  clevels.resize(_nlevels);
  nlevels = _nlevels;

  //fill up level
  for (unsigned int i = 0 ; i < nlevels ; i++) {
    clevels[i] = zmin + (zmax - zmin) * i / (PLFLT) (nlevels - 1);
  }
  _signal_changed.emit();
}

unsigned int PlotContour::get_nlevels() {
  return nlevels;
}

void PlotContour::show_labels() {
	if (showing_labels)
		return;
	showing_labels = true;
	_signal_changed.emit();
}

void PlotContour::hide_labels() {
	if (!showing_labels)
		return;
	showing_labels = false;
	_signal_changed.emit();
}

bool PlotContour::is_showing_labels() const {
	return showing_labels;
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

  //thinks to set before drawing:
  // 1) edge_color
  // 2) edge_width
  change_plstream_color(pls, edge_color);

  pls->width(edge_width);

  auto data = dynamic_cast<PlotDataSurface*>(plot_data[0]);
  std::vector<PLFLT> x = data->get_vector_x();
  std::vector<PLFLT> y = data->get_vector_y();
  PLcGrid cgrid;
  cgrid.xg = &x[0];
  cgrid.yg = &y[0];
  cgrid.nx = x.size();
  cgrid.ny = y.size();
  PLFLT **z = data->get_array2d_z();

	pls->setcontlabelparam(0.01, 0.4, 0.1, is_showing_labels());

  pls->cont(z, x.size(), y.size(), 1, x.size(), 1, y.size(), &clevels[0], nlevels, PLCALLBACK::tr1, (void *) &cgrid);

  free_array2d((void **) z, x.size());

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
