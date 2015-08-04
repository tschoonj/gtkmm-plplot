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

#ifndef GTKMMPLPLOTUTILS_H
#define GTKMMPLPLOTUTILS_H

#include <vector>
#include <valarray>
#include <plstream.h>
#include <gdkmm/rgba.h>
#include <gtkmm-plplot/enums.h>

namespace Gtk {
  namespace PLplot {
    /** Returns a vector of doubles with each element set to the value of its subscript.
     *
     * \param n the length of the vector
     */
    std::vector<PLFLT> indgen(unsigned int n);

    /** Returns a valarray of doubles with each element set to the value of its subscript.
     *
     * \param n the length of the valarray
     */
    std::valarray<PLFLT> indgen_va(unsigned int n);

    /** Changes the current color of the plstream
     *
     * Convenience function that allows me to change PLplot's colormap
     * \param pls a pointer to a plstream object
     * \param color the new color
     */
     void change_plstream_color(plstream *pls, Gdk::RGBA color);

    /** Changes the current colormap palette of the plstream
     *
     * Convenience function that allows me to change PLplot's colormap palette for PlotDataContour
     * \param pls a pointer to a plstream object
     * \param colormap the new colormap
     */
     void change_plstream_colormap(plstream *pls, ColorMapPalette colormap);
   }
}


#endif
