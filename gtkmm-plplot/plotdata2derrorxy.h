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

#ifndef GTKMM_PLPLOT_PLOTDATA2D_ERRORXY_H
#define GTKMM_PLPLOT_PLOTDATA2D_ERRORXY_H

#include <gtkmm-plplot/plotdata2derrorx.h>
#include <gtkmm-plplot/plotdata2derrory.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotData2DErrorXY plotdata2derrorxy.h <gtkmm-plplot/plotdata2derrorxy.h>
     *  \brief a class that will hold a single XY dataset with X and Y errorbars and its properties for a Plot2D plot
     *
     *  Instances of this class contain a single dataset for a Plot2D two-dimensional plot,
     *  consisting of the raw X- and Y-data, as well as error values for the X-data and Y-data,
     *  along with a number of properties that will determine
     *  the appearance of the plot. Data may be represented as a line, symbols or both.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example11
     */
    class PlotData2DErrorXY : public PlotData2DErrorX, public PlotData2DErrorY {
    private:
      PlotData2DErrorXY() = delete; ///< no default constructor
      PlotData2DErrorXY &operator=(const PlotData2DErrorXY &) = delete; ///< no assignment operator
      PlotData2DErrorXY(const PlotData2DErrorXY &source) = delete; ///< no default copy constructor;
      virtual void add_datapoint(double xval, double yval) final; ///< disable this method
      virtual void add_datapoint(std::pair<double, double> xy_pair) final; ///< disable this method;
      virtual void add_datapoint(double xval, double yval, double xval_error_low, double xval_error_high) final; ///< disable this method
    protected:
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset using six vectors, one for the X-values, one for the Y-values,
       * and four vectors to define the errorsbars of the X-data and Y-data.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * By default, the color of the errorbars will be the same as the one chosen at construction time for the line.
       * Override this by calling set_error_x_color() and set_error_y_color() after constructing the dataset.
       * \param x the X-values, as std::vector
       * \param y the Y-values, as std::vector
       * \param errorx_low the lower values of the X-data errorbars. Each value must be less than its X-data counterpart.
       * \param errorx_high the upper values of the X-data errorbars. Each value must be greater than its X-data counterpart.
       * \param errory_low the lower values of the Y-data errorbars. Each value must be less than its Y-data counterpart.
       * \param errory_high the upper values of the Y-data errorbars. Each value must be greater than its Y-data counterpart.
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       * \exception Gtk::PLplot::Exception
       */
      PlotData2DErrorXY(const std::vector<double> &x,
                        const std::vector<double> &y,
                        const std::vector<double> &errorx_low,
                        const std::vector<double> &errorx_high,
                        const std::vector<double> &errory_low,
                        const std::vector<double> &errory_high,
                        Gdk::RGBA color = Gdk::RGBA("red"),
                        LineStyle line_style = CONTINUOUS,
                        double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using four valarrays, one for the X-values, one for the Y-values,
       * and four valarrays to define the errorsbars of the X-data and Y-data.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * By default, the color of the errorbars will be the same as the one chosen at construction time for the line.
       * Override this by calling set_error_x_color() and set_error_y_color() after constructing the dataset.
       * \param x the X-values, as std::valarray
       * \param y the Y-values, as std::valarray
       * \param errorx_low the lower values of the X-data errorbars. Each value must be less than its X-data counterpart.
       * \param errorx_high the upper values of the X-data errorbars. Each value must be greater than its X-data counterpart.
       * \param errory_low the lower values of the Y-data errorbars. Each value must be less than its Y-data counterpart.
       * \param errory_high the upper values of the Y-data errorbars. Each value must be greater than its Y-data counterpart.
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotData2DErrorXY(const std::valarray<double> &x,
                        const std::valarray<double> &y,
                        const std::valarray<double> &errorx_low,
                        const std::valarray<double> &errorx_high,
                        const std::valarray<double> &errory_low,
                        const std::valarray<double> &errory_high,
                        Gdk::RGBA color = Gdk::RGBA("red"),
                        LineStyle line_style = CONTINUOUS,
                        double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Destructor
       *
       */
      virtual ~PlotData2DErrorXY();

      /** Get the data extremes: minima and maxima along both X- and Y-axes
       *
       * \param xmin X minimum value
       * \param xmax X maximum value
       * \param ymin Y minimum value
       * \param ymax Y minimum value
       */
      virtual void get_extremes(double &xmin, double &xmax, double &ymin, double &ymax) override;

      /** Add a single datapoint, consisting of an \c xval and \c yval value, as well as the errors associated with the \c xval and \c yval values, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xval an X-value
       * \param yval an Y-value
       * \param xval_error_low lower endpoint of the X-data errorbar
       * \param xval_error_high upper endpoint of the X-data errorbar
       * \param yval_error_low lower endpoint of the Y-data errorbar
       * \param yval_error_high upper endpoint of the Y-data errorbar
       */
      virtual void add_datapoint(double xval, double yval, double xval_error_low, double xval_error_high, double yval_error_low, double yval_error_high);

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
