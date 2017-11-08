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

#ifndef GTKMMPLPLOT_PLOT_OBJECT_2D_LINE_H
#define GTKMMPLPLOT_PLOT_OBJECT_2D_LINE_H

#include <gtkmm-plplot/plotobject2d.h>
#include <gtkmm-plplot/enums.h>
#include <gtkmm/orientable.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotObject2DLine plotobject2dline.h <gtkmm-plplot/plotobject2dline.h>
     *  \brief Line object for 2D plots 
     *
     */
    class PlotObject2DLine : public PlotObject2D {
    private:
      PlotObject2DLine() = delete; ///< no default constructor
      PlotObject2DLine &operator=(const PlotObject2DLine &) = delete; ///< no assignment operator
      PlotObject2DLine(const PlotObject2DLine &source) = delete; ///< no default copy constructor;
    protected:
      LineStyle line_style; ///< The linestyle that will be used to draw this line object in the plot
      double line_width; ///< The line width of the line object. Default is 1.0
      double coords_begin[2]; ///< coordinates of starting point (WORLD coordinates!)
      double coords_end[2]; ///< coordinates of end point (WORLD coordinates!)
    public:
      /** Constructor
       *
       * This constructor initializes a new PlotObject2DLine instance, by setting the properties
       * that will determine its appearance when rendered onto a plot
       * \param coord_x_begin X coordinate of the starting point
       * \param coord_y_begin Y coordinate of the starting point
       * \param coord_x_end X coordinate of the end point
       * \param coord_y_end Y coordinate of the end point
       * \param color the line color
       * \param line_style the line style
       * \param line_width the line width
       * \exception Gtk::PLplot::Exception
       */
      PlotObject2DLine(double coord_x_begin,
		       double coord_y_begin,
		       double coord_x_end,
		       double coord_y_end,
		       Gdk::RGBA color = Gdk::RGBA("black"),
                       LineStyle line_style = LineStyle::CONTINUOUS,
                       double line_width = 1.0);

      /** Constructor
       *
       * This constructor initializes a new PlotObject2DLine instance, with either a horizontal or vertical orientation, by setting the properties
       * that will determine its appearance when rendered onto a plot
       * \param orientation Must be either Gtk::ORIENTATION_HORIZONTAL or Gtk::ORIENTATION_VERTICAL
       * \param offset For horizontal lines, the vertical offset and vice-versa for vertical lines
       * \param color the line color
       * \param line_style the line style
       * \param line_width the line width
       * \exception Gtk::PLplot::Exception
       */
      PlotObject2DLine(Gtk::Orientation orientation,
		       double offset,
		       Gdk::RGBA color = Gdk::RGBA("black"),
                       LineStyle line_style = LineStyle::CONTINUOUS,
                       double line_width = 1.0);

      /** Destructor
       *
       */
      virtual ~PlotObject2DLine();

      /** Changes the line style
       *
       * \param line_style The new line style
       */
      void set_line_style(LineStyle line_style);

      /** Get the current line style
       *
       * \return the current line style
       */
      LineStyle get_line_style();

      /** Changes the line width
       *
       * \param line_width The new line width
       * \exception Gtk::PLplot::Exception
       */
      void set_line_width(double line_width);

      /** Get the current line width
       *
       * \return the current line width
       */
      double get_line_width();

      /** Check if the object can be used in a plot with a logarithmic X-axis
       *
       * \return \c true if compatible, \c false if not
       */
      virtual bool is_logarithmic_x_compatible() override;

      /** Check if the object can be used in a plot with a logarithmic Y-axis
       *
       * \return \c true if compatible, \c false if not
       */
      virtual bool is_logarithmic_y_compatible() override;

      /** Method to draw the object
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       * \param data additional data sent from the Plot to influence drawing
       */
      virtual void draw_plot_object(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, PlotObjectAuxData &data) override;

    };
  }
}
#endif
