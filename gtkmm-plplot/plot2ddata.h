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

#ifndef GTKMMPLPLOTPLOT2DDATA_H
#define GTKMMPLPLOTPLOT2DDATA_H

#include <sigc++/sigc++.h>
#include <vector>
#include <valarray>
#include <gtkmm-plplot/enums.h>
#include <plstream.h>
#include <glibmm/ustring.h>
#include <cairomm/cairomm.h>
#include <gdkmm/rgba.h>


#define PLOT2DDATA_DEFAULT_LINE_WIDTH 1.0

namespace Gtk {
  namespace PLplot {
    /** \class Plot2DData plot2ddata.h <gtkmm-plplot/plot2ddata.h>
     *  \brief a class that will hold a single dataset and its properties for a Plot2D plot
     *
     *  Instances of this class contain a single dataset for a Plot2D two-dimensional plot,
     *  consisting of the raw X- and Y-data, along with a number of properties that will determine
     *  the appearance in the plot.
     *  The constructors of this class allow to use either std::vector or std::valarray as sources of data,
     *  for added flexibility. Internally they are stored only as std::vector though. The datatype is PLplot's \c PLFLT, which is typedef'ed to \c double.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     */
    class Plot2DData : public sigc::trackable {
    private:
      std::vector<PLFLT> x; ///< The X-values of the dataset
      std::vector<PLFLT> y; ///< The Y-values of the dataset
      Gdk::RGBA color; ///< The color the dataset will be drawn in
      LineStyle line_style; ///< The linestyle that will be used for this dataset in the plot
      double line_width; ///< The line width of the dataset. Default is 1.0
      bool shown; ///< \c true means the plot is currently visible, \c false means it is not plotted
      Glib::ustring symbol; ///< If not an empty string, the symbol will be plotted at each of the data points from \c x and \c y.
      Gdk::RGBA symbol_color; ///< The color the symbol will be plotted in
      double symbol_scale_factor; ///< Scale factor that will determine the size of the symbols. Default is 1.
      sigc::signal<void> _signal_changed; ///< signal that gets emitted whenever any of the dataset proprties is changed.
      sigc::signal<void> _signal_data_modified; ///< signal that gets emitted whenever the X- and Y-datasets have been modified.
      Plot2DData() = delete; ///< no default constructor
      Plot2DData &operator=(const Plot2DData &) = delete; ///< no copy constructor
    protected:
      /** This is a default handler for signal_changed()
       *
       * This signal is emitted whenever any of the dataset properties is changed.
       * Currently it does nothing but the signal will get caught by Plot2D, and will eventually trigger a redrawing of the entire widget.
       */
      virtual void on_changed();

      /** This is a default handler for signal_data_modified()
       *
       * This signal is emitted whenever any of the dataset proprties is changed.
       * Currently it does nothing but the signal will get caught by Plot2D, and will eventually trigger a redrawing of the entire widget, taking into account the new dataset.
       */
      virtual void on_data_modified();
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
       */
      Plot2DData(const std::vector<PLFLT> &x,
                 const std::vector<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);

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
      Plot2DData(const std::valarray<PLFLT> &x,
                 const std::valarray<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);

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
      Plot2DData(const std::vector<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);

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
      Plot2DData(const std::valarray<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);

      /** Copy constructor
       *
       * \param data dataset to be copied
       */
      Plot2DData(const Plot2DData &data);

      /** Destructor
       *
       */
      virtual ~Plot2DData() {}

      /** Changes the color of the dataset
       *
       * \param color The new dataset color
       */
      void set_color(Gdk::RGBA color);

      /** Get the current dataset color
       *
       * \return the current dataset color
       */
      Gdk::RGBA get_color();

      /** Changes the line style of the dataset
       *
       * \param line_style The new dataset line style
       */
      void set_line_style(LineStyle line_style);

      /** Get the current dataset line style
       *
       * \return the current dataset line style
       */
      LineStyle get_line_style();

      /** Changes the line width of the dataset
       *
       * \param line_width The new dataset line width
       */
      void set_line_width(double line_width);

      /** Get the current dataset line width
       *
       * \return the current dataset line width
       */
      double get_line_width();

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
      void add_datapoint(PLFLT xval, PLFLT yval);

      /** Add a single datapoint, consisting of a std::pair with an X and Y value, to the dataset
       *
       * This datapoint will be added at the end of the std::vector's \c x and \c y.
       * After this method is called, the plot will be automatically updated to reflect the changes.
       * \param xy_pair a std::pair containing both an X- and a Y- value
       */
      void add_datapoint(std::pair<PLFLT, PLFLT> xy_pair);

      /** Make the dataset visible in the plot
       *
       */
      void show();

      /** Hide the dataset in the plot
       *
       */
      void hide();

      /** Returns whether or not the dataset is showing in the plot
       *
       * \return \c true if the dataset is visible, \c false if not
       */
      bool is_showing() const;

      /** signal_changed is emitted whenever any of the dataset properties has changed.
       *
       * See default handler on_changed()
       */
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }

      /** signal_data_modified is emitted whenever the X- and Y- arrays are modified.
       *
       * See default handler on_data_modified()
       */
      sigc::signal<void> signal_data_modified() {
        return _signal_data_modified;
      }

      /** Method to draw the dataset
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       * \param log10_x \c true means logarithmic X- axis, \c false means linear
       * \param log10_y \c true means logarithmic y- axis, \c false means linear
       */
      virtual void draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, bool log10_x, bool log10_y);
      friend class Plot2D;
    };
  }
}

#endif
