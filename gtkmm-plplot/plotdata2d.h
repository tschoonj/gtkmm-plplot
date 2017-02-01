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

#ifndef GTKMMPLPLOT_PLOTDATA_2D_H
#define GTKMMPLPLOT_PLOTDATA_2D_H

#include <vector>
#include <valarray>
#include <glibmm/ustring.h>
#include <gtkmm-plplot/plotdataline.h>
#include <gdkmm/rgba.h>


namespace Gtk {
  namespace PLplot {
    /** \class PlotData2D plotdata2d.h <gtkmm-plplot/plotdata2d.h>
     *  \brief a class that will hold a single dataset and its properties for a Plot2D plot
     *
     *  Instances of this class contain a single dataset for a Plot2D two-dimensional plot,
     *  consisting of the raw X- and Y-data, along with a number of properties that will determine
     *  the appearance of the plot. Data may be represented as a line, symbols or both.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  Several of the methods that are offered by this class are demonstrated in \ref example5
     */
    class PlotData2D : public PlotDataLine {
    private:
      PlotData2D() = delete; ///< no default constructor
      PlotData2D &operator=(const PlotData2D &) = delete; ///< no assignment operator
      PlotData2D(const PlotData2D &source) = delete; ///< no default copy constructor;
    protected:
      std::vector<double> x; ///< The X-values of the dataset
      std::vector<double> y; ///< The Y-values of the dataset
      Glib::ustring symbol; ///< If not an empty string, the symbol will be plotted at each of the data points from \c x and \c y.
      Gdk::RGBA symbol_color; ///< The color the symbol will be plotted in
      double symbol_scale_factor; ///< Scale factor that will determine the size of the symbols. Default is 1.
    public:
      /** Constructor
       *
       * This constructor initializes a new dataset using two vectors, one for the X-values, and one for the Y-values.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param x the X-values, as std::vector
       * \param y the Y-values, as std::vector
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       * \exception Gtk::PLplot::Exception
       */
      PlotData2D(const std::vector<double> &x,
                 const std::vector<double> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using two valarrays, one for the X-values, and one for the Y-values.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param x the X-values, as std::valarray
       * \param y the Y-values, as std::valarray
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotData2D(const std::valarray<double> &x,
                 const std::valarray<double> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using a single vector for the Y-values. The X-values will be
       * set to a vector of the same size with elements set to correspond to their array subscript.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param y the Y-values, as std::vector
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotData2D(const std::vector<double> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * This constructor initializes a new dataset using a single valarray for the Y-values. The X-values will be
       * set to a vector of the same size with elements set to correspond to their array subscript.
       * Optionally, one can also use the constructor to override the default color, line style and line width.
       * \param y the Y-values, as std::valarray
       * \param color the line color, default is red
       * \param line_style the line style, default is CONTINUOUS
       * \param line_width the line width, default is 1.0
       */
      PlotData2D(const std::valarray<double> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Destructor
       *
       */
      virtual ~PlotData2D();

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

      /** Get the data extremes: minima and maxima along both X- and Y-axes
       *
       * \param xmin X minimum value
       * \param xmax X maximum value
       * \param ymin Y minimum value
       * \param ymax Y minimum value
       */
      virtual void get_extremes(double &xmin, double &xmax, double &ymin, double &ymax);

      /** Set the plot symbol
       *
       * It is possible to plot a symbol (string) at the datapoints,
       * either replacing (if \c line_style is NONE) or complementing the line connecting the points.
       * The symbol will be sent 'as-is' to PLplot, meaning that it supports the
       * many escape-sequences that PLplot provides. For more information, the reader is referred to
       * the PLplot manual, section 3.8.4 Escape sequences in text.
       * If the plot symbol is no longer desired, disable it by passing an emptry string.
       * \param symbol the symbol to be used for plotting
       */
      void set_symbol(Glib::ustring symbol);

      /** Get the plot symbol
       *
       * See set_symbol() for more information
       * \return the symbol currently to be used for plotting
       */
      Glib::ustring get_symbol();

      /** Set the plot symbol color
       *
       * See set_symbol() for more information about plotting symbols
       * \param color the color that will used for plotting the symbols
       */
      void set_symbol_color(Gdk::RGBA color);

      /** Get the plot symbol color
       *
       * See set_symbol() for more information about plotting symbols
       * \return the color that is currently used for plotting symbols
       */
      Gdk::RGBA get_symbol_color();

      /** Set the plot symbol height scale factor
       *
       * See set_symbol() for more information about plotting symbols
       * \param scale_factor the scale factor that will used to determine the height of the plotting symbols.
       */
      void set_symbol_height_scale_factor(double scale_factor = 1.0);

      /** Get the plot symbol height scale factor
       *
       * See set_symbol() for more information about plotting symbols
       * \return the current scale factor that will used to determine the height of the plotting symbols.
       * \exception Gtk::PLplot::Exception
       */
      double get_symbol_height_scale_factor();

      /** Add a single datapoint, consisting of an \c xval and \c yval value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xval an X-value
       * \param yval an Y-value
       */
      virtual void add_datapoint(double xval, double yval);

      /** Add a single datapoint, consisting of a std::pair with an X and Y value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xy_pair a std::pair containing both an X- and a Y- value
       */
      virtual void add_datapoint(std::pair<double, double> xy_pair);

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
