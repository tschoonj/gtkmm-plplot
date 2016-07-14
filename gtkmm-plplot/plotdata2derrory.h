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

#ifndef GTKMM_PLPLOT_PLOTDATA2D_ERRORY_H
#define GTKMM_PLPLOT_PLOTDATA2D_ERRORY_H

#include <gtkmm-plplot/plotdata2d.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotData2DErrorY plotdata2derrory.h <gtkmm-plplot/plotdata2derrory.h>
     *  \brief a class that will hold a single XY dataset with Y errorbars and its properties for a Plot2D plot
     *
     *  Instances of this class contain a single dataset for a Plot2D two-dimensional plot,
     *  consisting of the raw X- and Y-data, as well as error values for the Y-data,
     *  along with a number of properties that will determine
     *  the appearance of the plot. Data may be represented as a line, symbols or both.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example11
     */
    class PlotData2DErrorY : virtual public PlotData2D {
    private:
      PlotData2DErrorY() = delete; ///< no default constructor
      PlotData2DErrorY &operator=(const PlotData2DErrorY &) = delete; ///< no assignment operator
      PlotData2DErrorY(const PlotData2DErrorY &source) = delete; ///< no default copy constructor;
      virtual void add_datapoint(double xval, double yval) override; ///< disable this method
      virtual void add_datapoint(std::pair<double, double> xy_pair) override; ///< disable this method;
    protected:
      std::vector<double> errory_low; ///< The lower error margins of the Y-data
      std::vector<double> errory_high; ///< The upper error margins of the Y-data
      Gdk::RGBA errory_color; ///< The color of the Y-data errorbars
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset using four vectors, one for the X-values, one for the Y-values,
       * and two vectors to define the errorsbar of the Y-data.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * By default, the color of the errorbars will be the same as the one chosen at construction time for the line.
       * Override this by calling set_error_y_color() after constructing the dataset.
       * \param x the X-values, as std::vector
       * \param y the Y-values, as std::vector
       * \param errory_low the lower values of the Y-data errorbars. Each value must be less than its Y-data counterpart.
       * \param errory_high the upper values of the Y-data errorbars. Each value must be greater than its Y-data counterpart.
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       * \exception Gtk::PLplot::Exception
       */
      PlotData2DErrorY(const std::vector<double> &x,
                       const std::vector<double> &y,
                       const std::vector<double> &errory_low,
                       const std::vector<double> &errory_high,
                       Gdk::RGBA color = Gdk::RGBA("red"),
                       LineStyle line_style = CONTINUOUS,
                       double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using four valarrays, one for the X-values, one for the Y-values,
       * and two vectors to define the errorsbar of the X-data.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * By default, the color of the errorbars will be the same as the one chosen at construction time for the line.
       * Override this by calling set_error_x_color() after constructing the dataset.
       * \param x the X-values, as std::valarray
       * \param y the Y-values, as std::valarray
       * \param errory_low the lower values of the Y-data errorbars. Each value must be less than its Y-data counterpart.
       * \param errory_high the upper values of the Y-data errorbars. Each value must be greater than its Y-data counterpart.
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotData2DErrorY(const std::valarray<double> &x,
                       const std::valarray<double> &y,
                       const std::valarray<double> &errory_low,
                       const std::valarray<double> &errory_high,
                       Gdk::RGBA color = Gdk::RGBA("red"),
                       LineStyle line_style = CONTINUOUS,
                       double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Destructor
       *
       */
      virtual ~PlotData2DErrorY();

      /**
       *
       * \returns a copy of the dataset Y-values lower error values
       */
      std::vector<double> get_vector_error_y_low();

      /**
       *
       * \returns a copy of the dataset Y-values higher error values
       */
      std::vector<double> get_vector_error_y_high();

      /** Get the data extremes: minima and maxima along both X- and Y-axes
       *
       * \param xmin X minimum value
       * \param xmax X maximum value
       * \param ymin Y minimum value
       * \param ymax Y minimum value
       */
      virtual void get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) override;

      /** Set the Y-dataset errorbar color
       *
       * \param color the color that will used for plotting the Y-data errorbar.
       */
      void set_error_y_color(Gdk::RGBA color);

      /** Get the Y-dataset errorbar color
       *
       * \return the color that is currently used for plotting the Y-dataset errorbars.
       */
      Gdk::RGBA get_error_y_color();

      /** Add a single datapoint, consisting of an \c xval and \c yval value, as well as the errors associated with the \c yval value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xval an X-value
       * \param yval an Y-value
       * \param yval_error_low lower endpoint of the Y-data errorbar
       * \param yval_error_high upper endpoint of the Y-data errorbar
       */
      virtual void add_datapoint(double xval, double yval, double yval_error_low, double yval_error_high);

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
