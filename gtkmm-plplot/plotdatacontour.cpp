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

#include <gtkmm-plplot/plotdatacontour.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <algorithm>
#include <iostream>
#include <plConfig.h>

#ifdef GTKMM_PLPLOT_PLPLOT_5_11_0
	#define PLCALLBACK plcallback
#else
	#define PLCALLBACK plstream
#endif


using namespace Gtk::PLplot;

PlotDataContour::PlotDataContour(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  PLFLT **_z,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotData(),
  x(_x), y(_y),
  nlevels(_nlevels),
  edge_color(_edge_color),
  edge_style(_edge_style),
  edge_width(_edge_width),
  clevels(_nlevels)
  {
    //do some checks
    //ensure there are at least two elements in the arrays
    if (x.size() < 2 || y.size() < 2) {
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> data arrays x and y must have at least two elements");
    }

    //ensure x and y are sorted in ascending order
    if (!std::is_sorted(x.begin(), x.end(), std::less_equal<PLFLT>())) {
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> data arrays x must consist of unique elements and be sorted in ascending order");
    }

    if (!std::is_sorted(y.begin(), y.end(), std::less_equal<PLFLT>())) {
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> data arrays y must consist of unique elements and be sorted in ascending order");
    }

    //ensure edge_width is strictly positive
    if (edge_width <= 0.0) {
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> edge width must be strictly positive");
    }

    //ensure nlevels is at least 3
    if (nlevels < 3) {
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> nlevels must be greater than or equal to 3");
    }

    //allocate memory for z. This could easily segfault if bad input was provided
    z = deep_copy_array2d(_z, x.size(), y.size());

    //get maximum of z
    plMinMax2dGrid(z, x.size(), y.size(), &zmax, &zmin);

    //fill up level
    for (unsigned int i = 0 ; i < nlevels ; i++) {
      clevels[i] = zmin + (zmax - zmin) * i / (PLFLT) (nlevels - 1);
    }
}

PlotDataContour::PlotDataContour(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  PLFLT **_z,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(std::vector<PLFLT>(std::begin(_x), std::end(_x)),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  _z, _nlevels, _edge_color, _edge_style, _edge_width) {}

PlotDataContour::PlotDataContour(
  unsigned int nx,
  unsigned int ny,
  PLFLT **_z,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(std::vector<PLFLT>(indgen(nx)),
  std::vector<PLFLT>(indgen(ny)),
  _z, _nlevels, _edge_color, _edge_style, _edge_width) {}

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
//the Boost constructors

PlotDataContour::PlotDataContour(
  const std::vector<PLFLT> &_x,
  const std::vector<PLFLT> &_y,
  const boost::multi_array<PLFLT, 2> &_z,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(_x, _y,
  boost_multi_array_to_array2d(_z), _nlevels, _edge_color, _edge_style, _edge_width) {
    if (_z.shape()[0] != _x.size() || _z.shape()[1] != _y.size())
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> dimensions of x and/or y do not match those of z");
  }

PlotDataContour::PlotDataContour(
  const std::valarray<PLFLT> &_x,
  const std::valarray<PLFLT> &_y,
  const boost::multi_array<PLFLT, 2> &_z,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(std::vector<PLFLT>(std::begin(_x), std::end(_x)),
  std::vector<PLFLT>(std::begin(_y), std::end(_y)),
  boost_multi_array_to_array2d(_z), _nlevels, _edge_color, _edge_style, _edge_width) {
    if (_z.shape()[0] != _x.size() || _z.shape()[1] != _y.size())
      throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> dimensions of x and/or y do not match those of z");
  }

PlotDataContour::PlotDataContour(
  const boost::multi_array<PLFLT, 2> &_z,
  unsigned int _nlevels,
  Gdk::RGBA _edge_color,
  LineStyle _edge_style,
  double _edge_width) :
  PlotDataContour(std::vector<PLFLT>(indgen(_z.shape()[0])),
  std::vector<PLFLT>(indgen(_z.shape()[1])),
  boost_multi_array_to_array2d(_z), _nlevels, _edge_color, _edge_style, _edge_width) {}

#endif

//copy constructor
PlotDataContour::PlotDataContour(const PlotDataContour &_data) :
  PlotDataContour(_data.x, _data.y, _data.z, _data.nlevels, _data.edge_color,
  _data.edge_style, _data.edge_width) {}

PlotDataContour::~PlotDataContour() {
  free_array2d((void **) z, x.size());
}

std::vector<PLFLT> PlotDataContour::get_vector_x() {
  return x;
}

std::vector<PLFLT> PlotDataContour::get_vector_y() {
  return y;
}

void PlotDataContour::set_edge_color(Gdk::RGBA _edge_color) {
  edge_color = _edge_color;
  _signal_changed.emit();
}

Gdk::RGBA PlotDataContour::get_edge_color() {
  return edge_color;
}

void PlotDataContour::set_edge_style(LineStyle _edge_style) {
  edge_style = _edge_style;
  _signal_changed.emit();
}

LineStyle PlotDataContour::get_edge_style() {
  return edge_style;
}

void PlotDataContour::set_edge_width(double _edge_width) {
  //ensure edge_width is strictly positive
  if (_edge_width <= 0.0) {
    throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> edge width must be strictly positive");
  }

  edge_width = _edge_width;
  _signal_changed.emit();
}

double PlotDataContour::get_edge_width() {
  return edge_width;
}

void PlotDataContour::set_nlevels(unsigned int _nlevels) {
  //ensure nlevels is at least 3
  if (_nlevels < 3) {
    throw Exception("Gtk::PLplot::PlotDataContour::PlotDataContour -> nlevels must be greater than or equal to 3");
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

unsigned int PlotDataContour::get_nlevels() {
  return nlevels;
}

void PlotDataContour::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  //thinks to set before drawing:
  // 1) edge_color
  // 2) edge_style
  // 3) edge_width
  change_plstream_color(pls, edge_color);

  if (edge_style == LineStyle::NONE) {
    // we do not allow LineStyle::NONE here -> that would be ridiculous
    // but it makes sense for derived classes like PlotDataContourShades
    // so they will not have this conditional block
    pls->lsty(LineStyle::CONTINUOUS);
  }
  else
    pls->lsty(edge_style);

  pls->width(edge_width);

  PLcGrid cgrid;
  cgrid.xg = &x[0];
  cgrid.yg = &y[0];
  cgrid.nx = x.size();
  cgrid.ny = y.size();

  pls->cont(z, x.size(), y.size(), 1, x.size(), 1, y.size(), &clevels[0], nlevels, PLCALLBACK::tr1, (void *) &cgrid);
}
