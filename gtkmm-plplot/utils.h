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

#ifndef GTKMMPLPLOTUTILS_H
#define GTKMMPLPLOTUTILS_H

#include <gtkmm-plplotconfig.h>
#ifdef GTKMM_PLPLOT_BOOST_ENABLED
  #include <boost/multi_array.hpp>
#endif
#include <vector>
#include <valarray>
#include <gdkmm/rgba.h>
#include <gtkmm-plplot/enums.h>

#define GTKMM_PLPLOT_DEFAULT_COLOR_INDEX 2

class plstream;

namespace Gtk {
  namespace PLplot {
    /** Returns a vector of doubles with each element set to the value of its subscript.
     *
     * \param n the length of the vector
     */
    std::vector<double> indgen(unsigned int n);

    /** Returns a valarray of doubles with each element set to the value of its subscript.
     *
     * \param n the length of the valarray
     */
    std::valarray<double> indgen_va(unsigned int n);

    /** Changes the current color of the plstream
     *
     * Convenience function that allows me to change PLplot's colormap
     * \param pls a pointer to a plstream object
     * \param color the new color
     * \param activate make this the currently active color for subsequent drawing commands
     * \param index the index that should be used for this color in PLplot's colormap
     */
    void change_plstream_color(plstream *pls, Gdk::RGBA color, bool activate = true, int index = GTKMM_PLPLOT_DEFAULT_COLOR_INDEX);

    /** Changes the current colormap palette of the plstream
     *
     * Convenience function that allows me to change PLplot's colormap palette for PlotDataContour
     * \param pls a pointer to a plstream object
     * \param colormap the new colormap
     */
    void change_plstream_colormap(plstream *pls, ColormapPalette colormap);

    /** Allocates a an array of arrays
     *
     * All elements will be initialized to zero.
     * The return value should be freed with free_array2d();
     * \param nx the extent along the first coordinate of the array of arrays
     * \param ny the extent along the second coordinate of the array of arrays
     * \returns a freshly allocated array of arrays of double's
     */
    double **calloc_array2d(int nx, int ny);

    /** Creates a deep copy of a dynamically allocated array of dynamically allocated array of double's (double)
     *
     * The return value should be freed with free_array2d();
     * This function is likely to produce a segmentation fault or result in undefined behavior when the input arguments are incorrect or invalid.
     * \param input the array of arrays that will be copied
     * \param nx the extent along the first coordinate of input
     * \param ny the extent along the second coordinate of input
     * \returns a freshly allocated array of arrays of double's
     */
    double **deep_copy_array2d(double **input, int nx, int ny);

    /** Frees a dynamically allocated array of dynamically allocated arrays.
     *
     * Typically used to free memory allocated by deep_copy_array2d().
     * \param input the array of arrays that will be recursively freed.
     * \param nx the extent along the first coordinate of input
     */
    void free_array2d(void **input, int nx);

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
    /** Copy a Boost two dimensional multi array to dynamically allocated array of dynamically allocated arrays.
     *
     * The return value should be freed with free_array2d();
     * \param array the Boost multi_array instance
     */
    double **boost_multi_array_to_array2d(const boost::multi_array<double, 2> &array);
#endif
   }
}


#endif
