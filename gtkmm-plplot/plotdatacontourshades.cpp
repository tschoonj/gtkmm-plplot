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
#include <gtkmm-plplot/plotdatacontourshades.h>
#include <gtkmm-plplot/utils.h>
#include <gtkmm-plplot/exception.h>

#ifdef GTKMM_PLPLOT_PLPLOT_5_11_0
	#define PLCALLBACK plcallback
#else
	#define PLCALLBACK plstream
#endif

using namespace Gtk::PLplot;

PlotDataContourShades::PlotDataContourShades(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  PLFLT **_z,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(_x, _y, _z, _nlevels, _edge_color, _edge_style, _edge_width),
  colormap_palette(_colormap_palette),
  area_fill_pattern(SOLID),
  fill_width(PLOTDATA_DEFAULT_LINE_WIDTH),
  showing_edges(true) {}

PlotDataContourShades::PlotDataContourShades(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  PLFLT **_z,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContourShades(
    std::vector<PLFLT>(std::begin(_x), std::end(_x)),
    std::vector<PLFLT>(std::begin(_y), std::end(_y)),
    _z,
    _nlevels,
    _colormap_palette,
    _edge_color,
    _edge_style,
    _edge_width) {}

PlotDataContourShades::PlotDataContourShades(
  unsigned int _nx,
  unsigned int _ny,
  PLFLT **_z,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContourShades(
    std::vector<PLFLT>(indgen(_nx)),
    std::vector<PLFLT>(indgen(_ny)),
    _z,
    _nlevels,
    _colormap_palette,
    _edge_color,
    _edge_style,
    _edge_width) {}

#ifdef GTKMM_PLPLOT_BOOST_ENABLED

PlotDataContourShades::PlotDataContourShades(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  const boost::multi_array<PLFLT, 2> &_z,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(_x, _y, _z, _nlevels, _edge_color, _edge_style, _edge_width),
  colormap_palette(_colormap_palette),
  area_fill_pattern(SOLID),
  fill_width(PLOTDATA_DEFAULT_LINE_WIDTH),
  showing_edges(true) {}

PlotDataContourShades::PlotDataContourShades(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  const boost::multi_array<PLFLT, 2> &_z,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContourShades(
    std::vector<PLFLT>(std::begin(_x), std::end(_x)),
    std::vector<PLFLT>(std::begin(_y), std::end(_y)),
    _z,
    _nlevels,
    _colormap_palette,
    _edge_color,
    _edge_style,
    _edge_width) {}

PlotDataContourShades::PlotDataContourShades(
  const boost::multi_array<PLFLT, 2> &_z,
  unsigned int _nlevels,
  ColormapPalette _colormap_palette,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContourShades(
    std::vector<PLFLT>(indgen(_z.shape()[0])),
    std::vector<PLFLT>(indgen(_z.shape()[1])),
    _z,
    _nlevels,
    _colormap_palette,
    _edge_color,
    _edge_style,
    _edge_width) {}

#endif

PlotDataContourShades::PlotDataContourShades(const PlotDataContourShades &_data) :
  PlotDataContourShades(_data.x, _data.y, _data.z, _data.nlevels,
  _data.colormap_palette, _data.edge_color, _data.edge_style,
  _data.edge_width) {

  area_fill_pattern = _data.area_fill_pattern;
  fill_width = _data.fill_width;
  showing_edges = _data.showing_edges;
}

PlotDataContourShades::~PlotDataContourShades() {}

void PlotDataContourShades::set_colormap_palette(ColormapPalette _colormap_palette) {
  if (_colormap_palette == colormap_palette)
    return;
  colormap_palette = _colormap_palette;
  _signal_changed.emit();
}

ColormapPalette PlotDataContourShades::get_colormap_palette() {
  return colormap_palette;
}

void PlotDataContourShades::set_area_fill_pattern(AreaFillPattern _area_fill_pattern) {
  if (_area_fill_pattern == area_fill_pattern)
    return;
  area_fill_pattern = _area_fill_pattern;
  _signal_changed.emit();
}

AreaFillPattern PlotDataContourShades::get_area_fill_pattern() {
  return area_fill_pattern;
}

void PlotDataContourShades::set_area_lines_width(double _width) {
  if (fill_width == _width)
    return;
  if (_width <= 0.0)
    throw Exception("Gtk::PLplot::PlotDataContourShades::set_area_lines_width -> width must be greater than zero");
  fill_width = _width;
  _signal_changed.emit();
}

double PlotDataContourShades::get_area_lines_width() {
  return fill_width;
}

void PlotDataContourShades::show_edges() {
  if (showing_edges)
    return;
  showing_edges = true;
  _signal_changed.emit();
}

void PlotDataContourShades::hide_edges() {
  if (!showing_edges)
    return;
  showing_edges = false;
  _signal_changed.emit();
}

bool PlotDataContourShades::is_showing_edges() {
  return showing_edges;
}

void PlotDataContourShades::draw_colorbar(
	const Cairo::RefPtr<Cairo::Context> &cr,
	plstream *pls,
	Glib::ustring colorbar_title,
	Gdk::RGBA background_color,
	Gdk::RGBA bounding_box_color) {
  pls->lsty(edge_style);

	change_plstream_colormap(pls, colormap_palette);

	change_plstream_color(pls, edge_color, false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX);
	change_plstream_color(pls, background_color, false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX+1);
	change_plstream_color(pls, bounding_box_color, false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX+2);

	pls->psty(area_fill_pattern);

	int cont_color = GTKMM_PLPLOT_DEFAULT_COLOR_INDEX;
	PLFLT cont_width = edge_width;

	if (!showing_edges) {
		cont_color = 0;
		cont_width = 0.0;
	}

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
	PLFLT *values[NUM_AXES];
	PLFLT axis_ticks[NUM_AXES] = {
		0.0,
	};
	PLINT axis_subticks[NUM_AXES] = {
		0,
	};
	num_values[0] = nlevels;
	values[0] = &clevels[0];
	PLFLT colorbar_width, colorbar_height;
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
		GTKMM_PLPLOT_DEFAULT_COLOR_INDEX+1, //background color
		GTKMM_PLPLOT_DEFAULT_COLOR_INDEX+2, //bounding box color
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
		(const PLFLT * const *) values //contour levels
	);

	// Reset text and tick sizes
	pls->schr( 0.0, 1.0 );
	pls->smaj( 0.0, 1.0 );
	pls->smin( 0.0, 1.0 );
}

void PlotDataContourShades::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  pls->lsty(edge_style);

	change_plstream_colormap(pls, colormap_palette);

	change_plstream_color(pls, edge_color, false);

	pls->psty(area_fill_pattern);

  PLcGrid cgrid;
  cgrid.xg = &x[0];
  cgrid.yg = &y[0];
  cgrid.nx = x.size();
  cgrid.ny = y.size();

	int cont_color = GTKMM_PLPLOT_DEFAULT_COLOR_INDEX;
	PLFLT cont_width = edge_width;

	if (!showing_edges) {
		cont_color = 0;
		cont_width = 0.0;
	}

  //pls->cont(z, x.size(), y.size(), 1, x.size(), 1, y.size(), &clevels[0], nlevels, PLCALLBACK::tr1, (void *) &cgrid);
	pls->shades(z, x.size(), y.size(), NULL, x.front(), x.back(), y.front(), y.back(),
              &clevels[0], nlevels, fill_width,
              cont_color, cont_width,
              PLCALLBACK::fill, true, PLCALLBACK::tr1, (void *) &cgrid);
}
