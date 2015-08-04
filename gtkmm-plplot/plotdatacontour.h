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

#ifndef GTKMMPLPLOT_PLOT_DATA_CONTOUR_H
#define GTKMMPLPLOT_PLOT_DATA_CONTOUR_H

#include <gtkmm-plplot/gtkmm-plplotconfig.h>
#ifdef GTKMM_PLPLOT_BOOST_ENABLED
  #include <boost/multi_array.hpp>
#endif
#include <gtkmm-plplot/plotdataabstract.h>
#include <gtkmm-plplot/enums.h>
#include <vector>
#include <valarray>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataContour plotdatacontour.h <gtkmm-plplot/plotdatacontour.h>
     *  \brief a class for PlotContour datasets
     *
     *  Instances of this class will hold a single (and the only possible) dataset for
     *  a PlotContour plot. Apart from the data necessary to generate a contour plot, it contains
     *  several properties that will determine the appearance of the plot. The input data can be provided
     *  as std::vector (X and Y), std::valarray (X and Y), PLFLT** (Z, array of arrays) and boost:multi_array (Z).
     *  This datatype is demonstrated in \ref example7.
     */
    class PlotDataContour : public PlotDataAbstract {
    private:
      std::vector<PLFLT> x; ///< The X-grid coordinates of the dataset, which are assumed to correspond to the first dimension of Z
      std::vector<PLFLT> y; ///< The Y-grid coordinates of the dataset, which are assumed to correspond to the second dimension of Z
      PLFLT **z; ///< The Z-values of the dataset. This is an array of arrays whose first dimension must correspond to the length of x, while the second must correspond to the length of y.
      ColorMapPalette color_map_palette; ///< The color map that will be used to render the contourplot shades.


    };
  }
}


#endif
