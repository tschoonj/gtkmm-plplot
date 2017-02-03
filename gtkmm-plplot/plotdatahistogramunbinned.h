/*
Copyright (C) 2017 Tom Schoonjans

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

#ifndef GTKMMPLPLOT_PLOT_DATA_HISTOGRAM_UNBINNED_H
#define GTKMMPLPLOT_PLOT_DATA_HISTOGRAM_UNBINNED_H

#include <gtkmm-plplot/plotdatahistogram.h>
#include <vector>
#include <valarray>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataHistogramUnbinned plotdatahistogramunbinned.h <gtkmm-plplot/plotdatahistogramunbinned.h>
     *  \brief a class that will hold a single unbinned histogram dataset and its properties for a PlotHistogram plot
     *
     *  Instances of this class contain a single dataset for a PlotHistogram plot using unbinned data,
     *  along with a number of properties that will determine the appearance of the histogram.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example12
     */
    class PlotDataHistogramUnbinned : public PlotDataHistogram {
    private:
      PlotDataHistogramUnbinned() = delete; ///< no default constructor
      PlotDataHistogramUnbinned &operator=(const PlotDataHistogramUnbinned &) = delete; ///< no assignment operator
      PlotDataHistogramUnbinned(const PlotDataHistogramUnbinned &source) = delete; ///< no default copy constructor;
    protected:
      /* unbinned data variables */
      std::vector<double> data; ///< unbinned data
      double datmin; ///< Left-hand edge of the lowest-valued bin
      double datmax; ///< Right-hand edge of the highest-valued bin
      int nbin; ///< Number of bins into which to divide the data
    };
  }
}





#endif
