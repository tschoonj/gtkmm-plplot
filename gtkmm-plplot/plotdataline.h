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

#ifndef GTKMMPLPLOT_PLOT_DATA_LINE_H
#define GTKMMPLPLOT_PLOT_DATA_LINE_H

#include <gtkmm-plplot/plotdata.h>
#include <gdkmm/rgba.h>
#include <gtkmm-plplot/enums.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataLine plotdataline.h <gtkmm-plplot/plotdataline.h>
     *  \brief abstract base class that will hold a single dataset that can be rendered as a line
     *
     *  This class extends PlotData with support for drawing lines, defined by their color,
     *  style and width. Getters and setters are provided for each of these properties.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot
     */
    class PlotDataLine : public PlotData {
    private:
      PlotDataLine() = delete; ///< no default constructor
      PlotDataLine &operator=(const PlotDataLine &) = delete; ///< no assignment operator
      PlotDataLine(const PlotDataLine &source) = delete; ///< no default copy constructor
    protected:
      Gdk::RGBA color; ///< The color the dataset will be drawn in
      LineStyle line_style; ///< The linestyle that will be used for this dataset in the plot
      double line_width; ///< The line width of the dataset. Default is 1.0
      /** Constructor
       *
       * This constructor initializes a new PlotDataLine instance, by setting the properties
       * that will determine its appearance when rendered onto a plot
       * \param color the line color
       * \param line_style the line style
       * \param line_width the line width
       * \exception Gtk::PLplot::Exception
       */
      PlotDataLine(Gdk::RGBA color,
                   LineStyle line_style,
                   double line_width);
    public:
      /** Destructor
       *
       */
      virtual ~PlotDataLine();

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
       * \exception Gtk::PLplot::Exception
       */
      void set_line_width(double line_width);

      /** Get the current dataset line width
       *
       * \return the current dataset line width
       */
      double get_line_width();

    };
  }
}
#endif
