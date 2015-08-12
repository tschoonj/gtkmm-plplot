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
#include <gtkmm-plplot/plotdata.h>
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
    class PlotDataContour : public PlotData {
    private:

      PlotDataContour() = delete; ///< no default constructor
      PlotDataContour &operator=(const PlotDataContour &) = delete; ///< no copy constructor
    protected:
      std::vector<PLFLT> x; ///< The X-grid coordinates of the dataset, which are assumed to correspond to the first dimension of Z
      std::vector<PLFLT> y; ///< The Y-grid coordinates of the dataset, which are assumed to correspond to the second dimension of Z
      PLFLT **z; ///< The Z-values of the dataset. This is an array of arrays whose first dimension must correspond to the length of x, while the second must correspond to the length of y.
      unsigned int nlevels; ///< Number of contour levels to draw
      Gdk::RGBA edge_color; ///< Defines pen color used for contours defining edges.
      PLFLT edge_width; ///< Defines line width used for contours defining edges.
      PLFLT zmin; ///< Minimum of \c z, used to determine the contour edges
      PLFLT zmax; ///< Maximum of \c z, used to determine the contour edges

      std::vector<PLFLT> clevels; ///< Vector containing the data levels corresponding to the edges of each region that will be plotted. To work properly the levels should be monotonic.
    public:
      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::vector. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::vector. Have to match the second dimension of z
       * \param z the actual contour data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       * \exception Gtk::PLplot::Exception
       */
      PlotDataContour(const std::vector<PLFLT> &x,
                      const std::vector<PLFLT> &y,
                      PLFLT **z,
                      unsigned int nlevels = 6,
                      Gdk::RGBA edge_color = Gdk::RGBA("red"),
                      double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::valarray. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::valarray. Have to match the second dimension of z
       * \param z the actual contour data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContour(const std::valarray<PLFLT> &x,
                      const std::valarray<PLFLT> &y,
                      PLFLT **z,
                      unsigned int nlevels = 6,
                      Gdk::RGBA edge_color = Gdk::RGBA("red"),
                      double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour. The X- and Y-values will be
       * set to a vector of the appropriate size with elements set to correspond to their array subscript.
       * \param nx the extent of z along the first dimension
       * \param ny the extent of z along the second dimension
       * \param z the actual contour data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContour(unsigned int nx,
                      unsigned int ny,
                      PLFLT **z,
                      unsigned int nlevels = 6,
                      Gdk::RGBA edge_color = Gdk::RGBA("red"),
                      double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::vector. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::vector. Have to match the second dimension of z
       * \param z the actual contour data, provided as a Boost \c multi_array.
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContour(const std::vector<PLFLT> &x,
                      const std::vector<PLFLT> &y,
                      const boost::multi_array<PLFLT, 2> &z,
                      unsigned int nlevels = 6,
                      Gdk::RGBA edge_color = Gdk::RGBA("red"),
                      double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.
       * \param x the X-grid coordinates of the dataset, provided as a std::valarray. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::valarray. Have to match the second dimension of z
       * \param z the actual contour data, provided as a Boost \c multi_array.
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContour(const std::valarray<PLFLT> &x,
                      const std::valarray<PLFLT> &y,
                      const boost::multi_array<PLFLT, 2> &z,
                      unsigned int nlevels = 6,
                      Gdk::RGBA edge_color = Gdk::RGBA("red"),
                      double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContour.The X- and Y-values will be
       * set to a vector of the appropriate size with elements set to correspond to their array subscript.
       * \param z the actual contour data, provided as a Boost \c multi_array.
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContour(const boost::multi_array<PLFLT, 2> &z,
                      unsigned int nlevels = 6,
                      Gdk::RGBA edge_color = Gdk::RGBA("red"),
                      double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);
#endif

      /** Copy constructor
       *
       * \param data dataset to be copied
       */
      PlotDataContour(const PlotDataContour &data);

      /** Destructor
       *
       */
      virtual ~PlotDataContour();

      /**
       *
       * \returns a copy of the dataset X-values
       */
      std::vector<PLFLT> get_vector_x();

      /**
       *
       * \returns a copy of the dataset Y-values
       */
      std::vector<PLFLT> get_vector_y();

      /** Changes the color used for the contour defining edges
       *
       * \param color The new color that will be used to draw the contour edges
       */
      void set_edge_color(Gdk::RGBA color);

      /** Get the current contour edge color
       *
       * \returns the currently used contour edge color
       */
      Gdk::RGBA get_edge_color();

      /** Sets the width of the contour edge lines
       *
       * \param width The new width to be used for drawing the contour edge lines
       */
      void set_edge_width(double width);

      /** Get the currently used contour edge width
       *
       * \returns the currently used width of the contour edge lines
       * \exception Gtk::PLplot::Exception
       */
      double get_edge_width();

      /** Sets the number of contour levels to draw
       *
       * Note that the actually shown number of edges is equal to \c nlevels -2!
       * This is because the maximum and minimum values in the \c z dataset have their own edge,
       * which is just a single dot and is not shown!
       * \param nlevels The new number of contour levels
       */
      void set_nlevels(unsigned int nlevels);

      /** Get the number of contour levels
       *
       * \returns The number of contour levels
       * \exception Gtk::PLplot::Exception
       */
      unsigned int get_nlevels();

      /** Method to draw the dataset
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       */
      virtual void draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) override;
    };
  }
}


#endif
