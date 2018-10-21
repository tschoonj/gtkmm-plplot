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

#ifndef GTKMMPLPLOT_PLOT_DATA_SURFACE_H
#define GTKMMPLPLOT_PLOT_DATA_SURFACE_H

#include <gtkmm-plplotconfig.h>
#ifdef GTKMM_PLPLOT_BOOST_ENABLED
  #include <boost/multi_array.hpp>
#endif
#include <gtkmm-plplot/plotdata.h>
#include <gtkmm-plplot/enums.h>
#include <vector>
#include <valarray>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataSurface plotdatasurface.h <gtkmm-plplot/plotdatasurface.h>
     *  \brief a class for surface datasets
     *
     *  Instances of this class will hold a single (and the only possible) dataset for
     *  a surface plot types like PlotContour and PlotContourShades. The input data can be provided
     *  as std::vector (X and Y), std::valarray (X and Y), double** (Z, array of arrays) and boost:multi_array (Z).
     *  This datatype is demonstrated in \ref example7.
     */
    class PlotDataSurface : public PlotData {
    private:

      PlotDataSurface() = delete; ///< no default constructor
      PlotDataSurface(const PlotDataSurface &) = delete; ///< no copy constructor
      PlotDataSurface &operator=(const PlotDataSurface &) = delete; ///< no assignment operator
    protected:
      std::vector<double> x; ///< The X-grid coordinates of the dataset, which are assumed to correspond to the first dimension of Z
      std::vector<double> y; ///< The Y-grid coordinates of the dataset, which are assumed to correspond to the second dimension of Z
      double **z; ///< The Z-values of the dataset. This is an array of arrays whose first dimension must correspond to the length of x, while the second must correspond to the length of y.
    public:
      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::vector. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::vector. Have to match the second dimension of z
       * \param z the actual surface data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \exception Gtk::PLplot::Exception
       */
      PlotDataSurface(const std::vector<double> &x,
                      const std::vector<double> &y,
                      double **z);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::valarray. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::valarray. Have to match the second dimension of z
       * \param z the actual surface data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       */
      PlotDataSurface(const std::valarray<double> &x,
                      const std::valarray<double> &y,
                      double **z);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour. The X- and Y-values will be
       * set to a vector of the appropriate size with elements set to correspond to their array subscript.
       * \param nx the extent of z along the first dimension
       * \param ny the extent of z along the second dimension
       * \param z the actual surface data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       */
      PlotDataSurface(unsigned int nx,
                      unsigned int ny,
                      double **z);

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::vector. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::vector. Have to match the second dimension of z
       * \param z the actual surface data, provided as a Boost \c multi_array.
       */
      PlotDataSurface(const std::vector<double> &x,
                      const std::vector<double> &y,
                      const boost::multi_array<double, 2> &z);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::valarray. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::valarray. Have to match the second dimension of z
       * \param z the actual surface data, provided as a Boost \c multi_array.
       */
      PlotDataSurface(const std::valarray<double> &x,
                      const std::valarray<double> &y,
                      const boost::multi_array<double, 2> &z);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.The X- and Y-values will be
       * set to a vector of the appropriate size with elements set to correspond to their array subscript.
       * \param z the actual surface data, provided as a Boost \c multi_array.
       */
      PlotDataSurface(const boost::multi_array<double, 2> &z);
#endif

      /** Destructor
       *
       */
      virtual ~PlotDataSurface();

      /**
       *
       * \returns a copy of the dataset X-values
       */
      std::vector<double> get_vector_x();

      /**
       *
       * \returns a copy of the dataset Y-values
       */
      std::vector<double> get_vector_y();

      /**
       * Free the memory with free_array2d().
       * \returns a copy of the dataset Z-values
       */
      double **get_array2d_z();
    };
  }
}


#endif
