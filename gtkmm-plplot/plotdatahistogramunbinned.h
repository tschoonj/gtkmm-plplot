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
      double *x; ///< the X-values of the bins
      double *y; ///< the Y-values (heights) of the bins
    protected:
      std::vector<double> data; ///< unbinned data
      bool ignore_outliers; ///< flag that will determine what happens to outliers, i.e. data outside of datmin and datmax
      void rebin(); ///< rebin the data
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset for a PlotHistogram from unbinned data.
       * \param data the unbinned data, as std::vector
       * \param datmin left-hand edge of the lowest-valued bin
       * \param datmax right-hand edge of the highest-valued bin
       * \param nbins number of bins into which to divide the data (minimum = 3)
       * \exception Gtk::PLplot::Exception
       */
      PlotDataHistogramUnbinned(const std::vector<double> &data,
                                double datmin,
                                double datmax,
                                int nbins);

      /** Constructor
       *
       * This constructor initializes a new dataset for a PlotHistogram from unbinned data.
       * \param data the unbinned data, as std::valarray
       * \param datmin left-hand edge of the lowest-valued bin
       * \param datmax right-hand edge of the highest-valued bin
       * \param nbins number of bins into which to divide the data (minimum = 3)
       * \exception Gtk::PLplot::Exception
       */
      PlotDataHistogramUnbinned(const std::valarray<double> &data,
                                double datmin,
                                double datmax,
                                int nbins);

      /** Destructor
       *
       */
      virtual ~PlotDataHistogramUnbinned();

      /** Add a single datapoint to the unbinned data
       *
       * This will lead to the bins being recalculated, which will be reflected
       * in the plot being updated.
       * \param value a new value to add to the unbinned data
       */
      virtual void add_datapoint(double value);

      /** Sets the left-hand edge of the lowest-valued bin
       *
       * This value must be less than the right-hand edge of the highest-valued bin (obtained using get_data_maximum())
       * \param datmin the new value
       * \exception Gtk::PLplot::Exception
       */
      void set_data_minimum(double datmin);

      /** Sets the right-hand edge of the highest-valued bin
       *
       * This value must be more than the left-hand edge of the lowest-valued bin (obtained using get_data_minimum())
       * \param datmax the new value
       * \exception Gtk::PLplot::Exception
       */
      void set_data_maximum(double datmax);

      /** Sets the number of histogram bins
       *
       * This value must be greater than or equal to 3
       * \param nbins the new value
       * \exception Gtk::PLplot::Exception
       */
      void set_nbins(int nbins);

      /** Get whether outliers should be ignored or added to the lowest or highest-valued bins.
       *
       * \returns the requested value
       */
      bool get_ignore_outliers();

      /** Sets whether outliers should be ignored or added to the lowest or highest-valued bins.
       *
       * \param ignore_outliers the new value
       */
      void set_ignore_outliers(bool ignore_outliers);

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
