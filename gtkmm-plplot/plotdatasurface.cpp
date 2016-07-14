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

#include <gtkmm-plplot/plotdatasurface.h>
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

PlotDataSurface::PlotDataSurface(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  double **_z) :
	Glib::ObjectBase("GtkmmPLplotPlotDataSurface"),
  PlotData(),
  x(_x), y(_y)
  {
    //do some checks
    //ensure there are at least two elements in the arrays
    if (x.size() < 2 || y.size() < 2) {
      throw Exception("Gtk::PLplot::PlotDataSurface::PlotDataSurface -> data arrays x and y must have at least two elements");
    }

    //ensure x and y are sorted in ascending order
    if (!std::is_sorted(x.begin(), x.end(), std::less_equal<double>())) {
      throw Exception("Gtk::PLplot::PlotDataSurface::PlotDataSurface -> data arrays x must consist of unique elements and be sorted in ascending order");
    }

    if (!std::is_sorted(y.begin(), y.end(), std::less_equal<double>())) {
      throw Exception("Gtk::PLplot::PlotDataSurface::PlotDataSurface -> data arrays y must consist of unique elements and be sorted in ascending order");
    }

    //allocate memory for z. This could easily segfault if bad input was provided
		if (_z)
    	z = deep_copy_array2d(_z, x.size(), y.size());
}

PlotDataSurface::PlotDataSurface(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  double **_z) :
  PlotDataSurface(std::vector<double>(std::begin(_x), std::end(_x)),
  std::vector<double>(std::begin(_y), std::end(_y)),
  _z) {}

PlotDataSurface::PlotDataSurface(
  unsigned int nx,
  unsigned int ny,
  double **_z) :
  PlotDataSurface(std::vector<double>(indgen(nx)),
  std::vector<double>(indgen(ny)),
  _z) {}

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
//the Boost constructors

PlotDataSurface::PlotDataSurface(
  const std::vector<double> &_x,
  const std::vector<double> &_y,
  const boost::multi_array<double, 2> &_z) :
  PlotDataSurface(_x, _y,
  nullptr) {
    if (_z.shape()[0] != _x.size() || _z.shape()[1] != _y.size())
      throw Exception("Gtk::PLplot::PlotDataSurface::PlotDataSurface -> dimensions of x and/or y do not match those of z");
		z = boost_multi_array_to_array2d(_z);
  }

PlotDataSurface::PlotDataSurface(
  const std::valarray<double> &_x,
  const std::valarray<double> &_y,
  const boost::multi_array<double, 2> &_z) :
  PlotDataSurface(std::vector<double>(std::begin(_x), std::end(_x)),
  std::vector<double>(std::begin(_y), std::end(_y)),
  _z) {}

PlotDataSurface::PlotDataSurface(
  const boost::multi_array<double, 2> &_z) :
  PlotDataSurface(std::vector<double>(indgen(_z.shape()[0])),
  std::vector<double>(indgen(_z.shape()[1])),
  _z) {}

#endif

PlotDataSurface::~PlotDataSurface() {
  free_array2d((void **) z, x.size());
}

std::vector<double> PlotDataSurface::get_vector_x() {
  return x;
}

std::vector<double> PlotDataSurface::get_vector_y() {
  return y;
}

double **PlotDataSurface::get_array2d_z() {
	return deep_copy_array2d(z, x.size(), y.size());
}
