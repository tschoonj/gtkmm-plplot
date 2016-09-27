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

#include <gtkmm-plplot/gtkmm-plplotconfig.h>
#include <gtkmm-plplot/plotcontourshades.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <iostream>
#include <plConfig.h>
#include <plstream.h>

#ifdef GTKMM_PLPLOT_PLPLOT_5_11_0
	#define PLCALLBACK plcallback
#else
	#define PLCALLBACK plstream
#endif

using namespace Gtk::PLplot;


PlotContourShades::PlotContourShades(
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  double _edge_width,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
	Glib::ObjectBase("GtkmmPLplotPlotContourShades"),
  PlotContour(_axis_title_x, _axis_title_y,
  _plot_title, _nlevels,
  _edge_color, _edge_width,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm),
  showing_colorbar(true),
  showing_edges(true),
  colorbar_title("Magnitude"),
  colormap_palette(_colormap_palette),
  area_fill_pattern(SOLID),
  fill_width(PLOTDATA_DEFAULT_LINE_WIDTH) {}

PlotContourShades::PlotContourShades(
  PlotDataSurface &_data,
  const Glib::ustring &_axis_title_x,
  const Glib::ustring &_axis_title_y,
  const Glib::ustring &_plot_title,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  double _edge_width,
  const double _plot_width_norm,
  const double _plot_height_norm,
  const double _plot_offset_horizontal_norm,
  const double _plot_offset_vertical_norm) :
  PlotContourShades(_axis_title_x, _axis_title_y,
  _plot_title, _nlevels,
  _colormap_palette,
  _edge_color, _edge_width,
  _plot_width_norm, _plot_height_norm,
  _plot_offset_horizontal_norm,
  _plot_offset_vertical_norm) {

  add_data(_data);
}

PlotContourShades::~PlotContourShades() {}

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

void PlotContourShades::set_colormap_palette(ColormapPalette _colormap_palette) {
  if (_colormap_palette == colormap_palette)
    return;
  colormap_palette = _colormap_palette;
  _signal_changed.emit();
}

ColormapPalette PlotContourShades::get_colormap_palette() {
  return colormap_palette;
}

void PlotContourShades::set_area_fill_pattern(AreaFillPattern _area_fill_pattern) {
  if (_area_fill_pattern == area_fill_pattern)
    return;
  area_fill_pattern = _area_fill_pattern;
  _signal_changed.emit();
}

AreaFillPattern PlotContourShades::get_area_fill_pattern() {
  return area_fill_pattern;
}

void PlotContourShades::set_area_lines_width(double _width) {
  if (fill_width == _width)
    return;
  if (_width <= 0.0)
    throw Exception("Gtk::PLplot::PlotContourShades::set_area_lines_width -> width must be greater than zero");
  fill_width = _width;
  _signal_changed.emit();
}

double PlotContourShades::get_area_lines_width() {
  return fill_width;
}

void PlotContourShades::show_edges() {
  if (showing_edges)
    return;
  showing_edges = true;
  _signal_changed.emit();
}

void PlotContourShades::hide_edges() {
  if (!showing_edges)
    return;
  showing_edges = false;
  _signal_changed.emit();
}

bool PlotContourShades::is_showing_edges() {
  return showing_edges;
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
  change_plstream_colormap(pls, colormap_palette);

	change_plstream_color(pls, edge_color, false);

	pls->psty(area_fill_pattern);

  auto data = dynamic_cast<PlotDataSurface*>(plot_data[0]);
  std::vector<double> x = data->get_vector_x();
  std::vector<double> y = data->get_vector_y();
  PLcGrid cgrid;
  cgrid.xg = &x[0];
  cgrid.yg = &y[0];
  cgrid.nx = x.size();
  cgrid.ny = y.size();
  double **z = data->get_array2d_z();

	int cont_color = GTKMM_PLPLOT_DEFAULT_COLOR_INDEX;
	double cont_width = edge_width;

	if (!showing_edges) {
		cont_color = 0;
		cont_width = 0.0;
	}

	//this will only work if showing_edges is true!
	pls->setcontlabelparam(0.01, 0.6, 0.1, is_showing_labels());

	pls->shades(z, x.size(), y.size(), NULL, x.front(), x.back(), y.front(), y.back(),
              &clevels[0], nlevels, fill_width,
              cont_color, cont_width,
              PLCALLBACK::fill, true, PLCALLBACK::tr1, (void *) &cgrid);


  //draw colorbar if requested
  if (showing_colorbar)
    draw_colorbar(cr, pls);

  free_array2d((void **) z, x.size());


  //plot the box with its axes
  change_plstream_color(pls, axes_color);
  pls->box("bcnst", 0.0, 0, "bcnstv", 0.0, 0);

  cr->restore();

  coordinate_transform_plplot_to_cairo(plotted_range_x[0], plotted_range_y[0],
                                      cairo_range_x[0], cairo_range_y[0]);
  coordinate_transform_plplot_to_cairo(plotted_range_x[1], plotted_range_y[1],
                                      cairo_range_x[1], cairo_range_y[1]);
}

void PlotContourShades::draw_colorbar(
	const Cairo::RefPtr<Cairo::Context> &cr,
	plstream *pls) {

	change_plstream_colormap(pls, colormap_palette);

	change_plstream_color(pls, axes_color, false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX);

	pls->psty(area_fill_pattern);
	//cannot pass the fill_width to the colorbar :-(
	//pls->width(fill_width);

	int cont_color = GTKMM_PLPLOT_DEFAULT_COLOR_INDEX;
	double cont_width = 1.0;

	/*if (!showing_edges) {
		cont_color = 0;
		cont_width = 0.0;
	}*/

	// Smaller text
	pls->schr(0.0, 0.75);
	// Small ticks on the vertical axis
	pls->smaj(0.0, 0.5);
	pls->smin(0.0, 0.5);

#define NUM_AXES 1
	PLINT n_axis_opts = NUM_AXES;
	const char *axis_opts[] = {
		"bcvtm",
	};
	PLINT num_values[NUM_AXES];
	double *values[NUM_AXES];
	double axis_ticks[NUM_AXES] = {
		0.0,
	};
	PLINT axis_subticks[NUM_AXES] = {
		0,
	};
	num_values[0] = nlevels;
	values[0] = &clevels[0];
	double colorbar_width, colorbar_height;
#define NUM_LABELS 1
	PLINT n_labels = NUM_LABELS;
	PLINT label_opts[] = {
		PL_COLORBAR_LABEL_BOTTOM,
	};
	const char *labels[1] = {
					colorbar_title.c_str(),
	};
	pls->colorbar(
		&colorbar_width,
		&colorbar_height,
		PL_COLORBAR_SHADE | PL_COLORBAR_SHADE_LABEL, //options
		0, //position
		0.005, //X offset
		0.0, //Y offset
		0.0375, //X length
		0.875, //Y length
		0, //background color
		0, //bounding box color
		1, //bounding box style
		0.0, //low end color bar cap
		0.0, //high end color bar cap
		cont_color, //contour color
		cont_width, //contour width
		n_labels, //number of labels
		label_opts,//label options
		labels,//label text
		n_axis_opts,//number of axes
		axis_opts,//axes options
		axis_ticks,//major tick mark spacings
		axis_subticks,//number of subticks
		num_values,//number of contour levels
		(const double * const *) values //contour levels
	);

	// Reset text and tick sizes
	pls->schr( 0.0, 1.0 );
	pls->smaj( 0.0, 1.0 );
	pls->smin( 0.0, 1.0 );
}
