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

#ifndef GTKMMPLPLOT_PLOT_DATA_HISTOGRAM_BINNED_H
#define GTKMMPLPLOT_PLOT_DATA_HISTOGRAM_BINNED_H

#include <gtkmm-plplot/plotdatahistogram.h>
#include <vector>
#include <valarray>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataHistogramBinned plotdatahistogrambinned.h <gtkmm-plplot/plotdatahistogrambinned.h>
     *  \brief a class that will hold a single binned histogram dataset and its properties for a PlotHistogram plot
     *
     *  Instances of this class contain a single dataset for a PlotHistogram plot using binned data,
     *  along with a number of properties that will determine the appearance of the histogram.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example12
     */
    class PlotDataHistogramBinned : public PlotDataHistogram {
    private:
      PlotDataHistogramBinned() = delete; ///< no default constructor
      PlotDataHistogramBinned &operator=(const PlotDataHistogramBinned &) = delete; ///< no assignment operator
      PlotDataHistogramBinned(const PlotDataHistogramBinned &source) = delete; ///< no default copy constructor
    protected:
      std::vector<double> data_x; ///< bin X-values, must be monotonicly increasing!
      std::vector<double> data_y; ///< bin Y-values (heights)
      bool centred; ///< if \c true, yhe bin boundaries are to be midway between the \c data_x values. If the values in \c data_x are equally spaced, the values are the center values of the bins.
      static double get_new_datmin(std::vector<double> data_x, bool centred); ///< static method to calculate the data minumum
      static double get_new_datmax(std::vector<double> data_x, bool centred); ///< static method to calculate the data maximum
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset for a PlotHistogram from binned data.
       * \param data_x the X-values of the bins, as std::vector
       * \param data_y the Y-values of the bins, as std::vector
       * \param centred if \c true, yhe bin boundaries are to be midway between the \c data_x values. If the values in \c data_x are equally spaced, the values are the center values of the bins.
       * \exception Gtk::PLplot::Exception
       */
      PlotDataHistogramBinned(const std::vector<double> &data_x,
                              const std::vector<double> &data_y,
                              bool centred = false);

      /** Constructor
       *
       * This constructor initializes a new dataset for a PlotHistogram from binned data.
       * \param data_x the X-values of the bins, as std::valarray
       * \param data_y the Y-values of the bins, as std::valarray
       * \param centred if \c true, yhe bin boundaries are to be midway between the \c data_x values. If the values in \c data_x are equally spaced, the values are the center values of the bins.
       * \exception Gtk::PLplot::Exception
       */
      PlotDataHistogramBinned(const std::valarray<double> &data_x,
                              const std::valarray<double> &data_y,
                              bool centred = false);

      /** Destructor
       *
       */
      virtual ~PlotDataHistogramBinned();

      /** Set the manner in which the X-values should be interpreted
       *
       * If \c true, yhe bin boundaries are to be midway between the \c data_x values. If the values in \c data_x are equally spaced, the values are the center values of the bins.
       * \param centred new setting for X-values
       */
      void set_centred(bool centred);

      /** Get the manner in which the X-values should be interpreted
       *
       * If \c true, yhe bin boundaries are to be midway between the \c data_x values. If the values in \c data_x are equally spaced, the values are the center values of the bins.
       * \returns the current setting for X-values
       */
      bool get_centred();

      /** Method to draw the dataset
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       */
      virtual void draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) override;

      /** Get dataset extremes
       *
       * Will be used in determining the box and its axes
       */
      virtual void get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) override;
    };
  }
}

#endif
