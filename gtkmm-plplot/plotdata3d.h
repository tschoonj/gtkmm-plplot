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

#ifndef GTKMMPLPLOT_PLOTDATA_3D_H
#define GTKMMPLPLOT_PLOTDATA_3D_H

#include <gtkmm-plplot/plotdata2d.h>
#include <tuple>

namespace Gtk {
  namespace PLplot {
    /** \class PlotData3D plotdata3d.h <gtkmm-plplot/plotdata3d.h>
     *  \brief a class that will hold a single dataset and its properties for a Plot3D plot
     *
     *  Instances of this class contain a single dataset for a Plot3D three-dimensional plot,
     *  consisting of the raw X-, Y- and Z-data, along with a number of properties that will determine
     *  the appearance of the plot. Data may be represented as a line, symbols or both.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example9.
     */
    class PlotData3D : public PlotData2D {
    private:
      PlotData3D() = delete; ///< no default constructor
      PlotData3D(const PlotData3D &) = delete; ///< no copy constructor
      PlotData3D &operator=(const PlotData3D &) = delete; ///< no assignment operator

      /** Add a single datapoint, consisting of an \c xval and \c yval value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * **Do not use this method for PlotData3D objects, an exception will be thrown!**
       * \param xval an X-value
       * \param yval an Y-value
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_datapoint(double xval, double yval) final;

      /** Add a single datapoint, consisting of a std::pair with an X and Y value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * **Do not use this method for PlotData3D objects, an exception will be thrown!**
       * \param xy_pair a std::pair containing both an X- and a Y- value
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_datapoint(std::pair<double, double> xy_pair) final;
    protected:
      std::vector<double> z; ///< The Z-values of the dataset
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset using two vectors, one for the X-values, and one for the Y-values.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param x the X-values, as std::vector
       * \param y the Y-values, as std::vector
       * \param z the Z-values, as std::vector
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       * \exception Gtk::PLplot::Exception
       */
      PlotData3D(const std::vector<double> &x,
                 const std::vector<double> &y,
                 const std::vector<double> &z,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using two valarrays, one for the X-values, and one for the Y-values.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param x the X-values, as std::valarray
       * \param y the Y-values, as std::valarray
       * \param z the Z-values, as std::valarray
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotData3D(const std::valarray<double> &x,
                 const std::valarray<double> &y,
                 const std::valarray<double> &z,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Destructor
       *
       */
      virtual ~PlotData3D();

      /**
       *
       * \returns a copy of the dataset Z-values
       */
      std::vector<double> get_vector_z();

      /** Add a single datapoint, consisting of an \c xval, \c yval and \c zval value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x, \c y and \c z.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xval an X-value
       * \param yval an Y-value
       * \param zval an Z-value
       */
      virtual void add_datapoint(double xval, double yval, double zval);

      /** Add a single datapoint, consisting of a std::tuple with an X, Y and Z value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x, \c y and \c z.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xyz_tuple a std::tuple containing 3 elements (X, Y and Z).
       */
      virtual void add_datapoint(std::tuple<double, double, double> xyz_tuple);

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
