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

#include <gtkmm-plplot/utils.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

static std::vector<std::string> _colormaps = {
  "cmap1_default.pal",
  "cmap1_blue_red.pal",
  "cmap1_blue_yellow.pal",
  "cmap1_gray.pal",
  "cmap1_highfreq.pal",
  "cmap1_lowfreq.pal",
  "cmap1_radar.pal"
};

std::vector<PLFLT> Gtk::PLplot::indgen(unsigned int n) {
  std::vector<PLFLT> rv(n);
  for (unsigned int i = 0 ; i < n ; i++)
    rv[i] = (PLFLT) i;
  return rv;
}

std::valarray<PLFLT> Gtk::PLplot::indgen_va(unsigned int n) {
  std::valarray<PLFLT> rv(n);
  for (unsigned int i = 0 ; i < n ; i++)
    rv[i] = (PLFLT) i;
  return rv;
}

void Gtk::PLplot::change_plstream_color(plstream *pls, Gdk::RGBA color, bool activate, int index) {
  std::cout << "Gtk::PLplot::change_plstream_color: " << index << std::endl;
  pls->scol0a(index, color.get_red_u()/256, color.get_green_u()/256, color.get_blue_u()/256, color.get_alpha());
  if (activate)
    pls->col0(index);
}

void Gtk::PLplot::change_plstream_colormap(plstream *pls, Gtk::PLplot::ColormapPalette colormap) {
  pls->spal1(_colormaps[colormap].c_str(), true);
}

PLFLT **Gtk::PLplot::deep_copy_array2d(PLFLT **input, int nx, int ny) {
  PLFLT **copy = (PLFLT **) malloc(sizeof(PLFLT *) * nx);
  for (int i = 0 ; i < nx ; i++) {
    copy[i] = (PLFLT *) malloc(sizeof(PLFLT) * ny);
    memcpy(copy[i], input[i], sizeof(PLFLT) * ny);
  }
  return copy;
}

void Gtk::PLplot::free_array2d(void **input, int nx) {
  for (int i = 0 ; i < nx ; i++) {
    free(input[i]);
  }
  free(input);
}

PLFLT **Gtk::PLplot::calloc_array2d(int nx, int ny) {
  PLFLT **rv= (PLFLT **) malloc(sizeof(PLFLT *) * nx);
  for (int i = 0 ; i < nx ; i++) {
    rv[i] = (PLFLT *) calloc(ny, sizeof(PLFLT));
  }
  return rv;
}

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
PLFLT **Gtk::PLplot::boost_multi_array_to_array2d(const boost::multi_array<PLFLT, 2> &array) {
  //get data pointer from boost array
  const PLFLT *data = array.data();
  //get dimensions
  size_t nx = array.shape()[0];
  size_t ny = array.shape()[1];


  PLFLT **copy = (PLFLT **) malloc(sizeof(PLFLT *) * nx);
  for (int i = 0 ; i < nx ; i++) {
    copy[i] = (PLFLT *) malloc(sizeof(PLFLT) * ny);
    memcpy(copy[i], data + i * ny, sizeof(PLFLT) * ny);
  }
  return copy;
}
#endif
