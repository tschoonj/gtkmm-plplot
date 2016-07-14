/*
Copyright (C) 2016 Tom Schoonjans

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

#ifndef GTKMMPLPLOT_PLOTDATA_POLAR_H
#define GTKMMPLPLOT_PLOTDATA_POLAR_H

#include <vector>
#include <valarray>
#include <glibmm/ustring.h>
#include <gtkmm-plplot/plotdata2d.h>
#include <gdkmm/rgba.h>


namespace Gtk {
  namespace PLplot {
    /** \class PlotDataPolar plotdatapolar.h <gtkmm-plplot/plotdatapolar.h>
     *  \brief a class that will hold a single dataset and its properties for a PlotPolar plot
     *
     *  Instances of this class contain a single dataset for a PlotPolar two-dimensional polar plot,
     *  consisting of the raw R- and θ-data, along with a number of properties that will determine
     *  the appearance of the plot. Data may be represented as a line, symbols or both.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example6
     */
    class PlotDataPolar : public PlotData2D {
    private:
      PlotDataPolar() = delete; ///< no default constructor
      PlotDataPolar &operator=(const PlotDataPolar &) = delete; ///< no assignment operator
      PlotDataPolar(const PlotDataPolar &source) = delete; ///< no default copy constructor;
    protected:
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset using two vectors, one for the R-values, and one for the θ-values.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param R the R-values, as std::vector
       * \param theta the θ-values, as std::vector
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       * \exception Gtk::PLplot::Exception
       */
      PlotDataPolar(const std::vector<double> &R,
                    const std::vector<double> &theta,
                    Gdk::RGBA color = Gdk::RGBA("red"),
                    LineStyle line_style = CONTINUOUS,
                    double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using two valarrays, one for the R-values, and one for the θ-values.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param R the R-values, as std::valarray
       * \param theta the θ-values, as std::valarray
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotDataPolar(const std::valarray<double> &R,
                    const std::valarray<double> &theta,
                    Gdk::RGBA color = Gdk::RGBA("red"),
                    LineStyle line_style = CONTINUOUS,
                    double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Destructor
       *
       */
      virtual ~PlotDataPolar();

      /**
       *
       * \returns a copy of the dataset R-values
       */
      std::vector<double> get_vector_r();

      /**
       *
       * \returns a copy of the dataset θ-values
       */
      std::vector<double> get_vector_theta();

      /** Get the R-data maximum
       *
       * \param rmax R maximum value
       */
      virtual void get_max_r(double &rmax);
    };
  }
}

#endif
