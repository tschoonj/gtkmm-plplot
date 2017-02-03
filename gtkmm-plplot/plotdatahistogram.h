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

#ifndef GTKMMPLPLOT_PLOT_DATA_HISTOGRAM_H
#define GTKMMPLPLOT_PLOT_DATA_HISTOGRAM_H

#include <gtkmm-plplot/plotdataline.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataHistogram plotdatahistogram.h <gtkmm-plplot/plotdatahistogram.h>
     *  \brief abstract base class that will hold a single dataset that can be rendered as a histogram
     *
     *  Base class for histogram data classes and contains common properties,
     *  accessible via getters and setters.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot
     */
    class PlotDataHistogram : public PlotDataLine {
    private:
      PlotDataHistogram &operator=(const PlotDataHistogram &) = delete; ///< no assignment operator
      PlotDataHistogram(const PlotDataHistogram &source) = delete; ///< no default copy constructor
    protected:
      double datmin; ///< Left-hand edge of the lowest-valued bin
      double datmax; ///< Right-hand edge of the highest-valued bin
      int nbins; ///< Number of bins into which to divide the data
      bool expand_bins; ///< When \c false, the outer bins are drawn with equal size as the ones inside.
      bool empty_bins; ///< When \c false, bins with zero height are not drawn (there is a gap for such bins).

      /** Constructor
       *
       * PlotDataHistogram constructor.
       *
       * \param datmin left-hand edge of the lowest-valued bin
       * \param datmax right-hand edge of the highest-valued bin
       * \param nbins number of bins into which to divide the data (minimum = 3)
       * \exception Gtk::PLplot::Exception
       * \param expand_bins determines if outer bins will be drawn with the same width as the inner ones
       * \param empty_bins determines whether empty bins should be drawn
       */
      PlotDataHistogram(double datmin,
                        double datmax,
                        int nbins,
                        bool expand_bins = true,
                        bool empty_bins = true);
    public:
      /** Destructor
       *
       */
      virtual ~PlotDataHistogram();

      /** Get the left-hand edge of the lowest-valued bin
       *
       * \returns the requested value
       */
      double get_data_minimum();

      /** Get the right-hand edge of the highest-valued bin
       *
       * \returns the requested value
       */
      double get_data_maximum();

      /** Get the number of bins
       *
       * \returns the requested value
       */
      int get_nbins();

      /** Set outer bins expansion behavior
       *
       * \param expand_bins the new expansion behavior
       */
      void set_expand_bins(bool expand_bins);

      /** Get current outer bins expansion behavior
       *
       * \return the current expansion behavior
       */
      bool get_expand_bins();

      /** Set whether empty bins should be drawn
       *
       * \param empty_bins new value
       */
      void set_empty_bins(bool empty_bins);

      /** Get current empty bins drawing behavior
       *
       * \return the current empty bins drawing behavior
       */
      bool get_empty_bins();

      /** Get dataset extremes
       *
       * Will be used in determining the box and its axes
       */
      virtual void get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) = 0;
    };
  }
}
#endif
