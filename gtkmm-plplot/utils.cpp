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

#include <gtkmm-plplot/utils.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <plstream.h>

static std::vector<Glib::ustring> _colormaps = {
  "cmap1_default.pal",
  "cmap1_blue_red.pal",
  "cmap1_blue_yellow.pal",
  "cmap1_gray.pal",
  "cmap1_highfreq.pal",
  "cmap1_lowfreq.pal",
  "cmap1_radar.pal"
};

std::vector<double> Gtk::PLplot::indgen(unsigned int n) {
  std::vector<double> rv(n);
  for (unsigned int i = 0 ; i < n ; i++)
    rv[i] = (double) i;
  return rv;
}

std::valarray<double> Gtk::PLplot::indgen_va(unsigned int n) {
  std::valarray<double> rv(n);
  for (unsigned int i = 0 ; i < n ; i++)
    rv[i] = (double) i;
  return rv;
}

void Gtk::PLplot::change_plstream_color(plstream *pls, Gdk::RGBA color, bool activate, int index) {
  pls->scol0a(index, color.get_red_u()/256, color.get_green_u()/256, color.get_blue_u()/256, color.get_alpha());
  if (activate)
    pls->col0(index);
}

void Gtk::PLplot::change_plstream_colormap(plstream *pls, Gtk::PLplot::ColormapPalette colormap) {
  pls->spal1(_colormaps[colormap].c_str(), true);
}

double **Gtk::PLplot::deep_copy_array2d(double **input, int nx, int ny) {
  double **copy = (double **) malloc(sizeof(double *) * nx);
  for (int i = 0 ; i < nx ; i++) {
    copy[i] = (double *) malloc(sizeof(double) * ny);
    memcpy(copy[i], input[i], sizeof(double) * ny);
  }
  return copy;
}

void Gtk::PLplot::free_array2d(void **input, int nx) {
  for (int i = 0 ; i < nx ; i++) {
    free(input[i]);
  }
  free(input);
}

double **Gtk::PLplot::calloc_array2d(int nx, int ny) {
  double **rv= (double **) malloc(sizeof(double *) * nx);
  for (int i = 0 ; i < nx ; i++) {
    rv[i] = (double *) calloc(ny, sizeof(double));
  }
  return rv;
}

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
double **Gtk::PLplot::boost_multi_array_to_array2d(const boost::multi_array<double, 2> &array) {
  //get data pointer from boost array
  const double *data = array.data();
  //get dimensions
  size_t nx = array.shape()[0];
  size_t ny = array.shape()[1];


  double **copy = (double **) malloc(sizeof(double *) * nx);
  for (int i = 0 ; i < nx ; i++) {
    copy[i] = (double *) malloc(sizeof(double) * ny);
    memcpy(copy[i], data + i * ny, sizeof(double) * ny);
  }
  return copy;
}
#endif
