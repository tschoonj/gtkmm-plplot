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

void Gtk::PLplot::change_plstream_color(plstream *pls, Gdk::RGBA color) {
  pls->scol0a(5, color.get_red_u()/256, color.get_green_u()/256, color.get_blue_u()/256, color.get_alpha());
  pls->col0(5);
}

void Gtk::PLplot::change_plstream_colormap(plstream *pls, Gtk::PLplot::ColorMapPalette colormap) {
  pls->spal1(_colormaps[colormap].c_str(), true);
}
