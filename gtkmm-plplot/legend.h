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

#ifndef GTKMMPLPLOT_LEGEND_H
#define GTKMMPLPLOT_LEGEND_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/plotdata2d.h>
#include <vector>

namespace Gtk {
  namespace PLplot {
    /** \class Legend legend.h <gtkmm-plplot/legend.h>
     *  \brief a class providing legends for plot classes with multiple datasets
     *
     * Plot2D, PlotPolar and other plot classes can handle several datasets simultaneously.
     * To facilitate distinction between the different datasets, this class extends these plots
     * by adding support for a legend that shows a short string with a description of each dataset.
     * Such a legend consists of a box which contains a list of these descrptions, along with a small plot
     * of the line and/or symbol that was used to render the corresponding dataset.
     * This is demonstrated in examples \ref example2, \ref example5 and \ref example6.
     */
    class Legend : public virtual Plot {
    private:
      bool showing_legend; ///< \c true will render the legend, while \c false will hide it
      double legend_pos_x; ///< the position of the right top corner of the legend along the X-axis in normalized coordinates
      double legend_pos_y; ///< the position of the right top corner of the legend along the Y-axis in normalized coordinates
      Gdk::RGBA legend_background_color; ///< the background color of the legend
      Gdk::RGBA legend_bounding_box_color; ///< the color of the bounding box of the legend
      Legend &operator=(const Legend &) = delete; ///< no move constructor
    protected:
      /** Copy constructor
       *
       * \param legend copy source
       */
      Legend(const Legend &legend) = default;

      /** Constructor
       *
       * This class offers a single, protected constructor. To be called from the constructors of classes
       * that derive from this class through multiple inheritance.
       * \param legend_pos_x The position of the right top corner of the legend along the X-axis in normalized coordinates
       * \param legend_pos_y The position of the right top corner of the legend along the Y-axis in normalized coordinates
       * \param legend_background_color The default background color of the legend
       * \param legend_bounding_box_color The default color of the bounding box color of the legend
       */
      Legend(double legend_pos_x = 0.0,
             double legend_pos_y = 0.0,
             Gdk::RGBA legend_background_color = Gdk::RGBA("White"),
             Gdk::RGBA legend_bounding_box_color = Gdk::RGBA("Black"));

      /** Method to draw the legend
       *
       * To be called from within draw_plot().
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       */
      virtual void draw_legend(const Cairo::RefPtr<Cairo::Context> &cr);

    public:
      /** Set the background color of the legend
       *
       * \param legend_background_color the new color
       */
      void set_legend_background_color(Gdk::RGBA legend_background_color);

      /** Get the currently selected background color of the legend
       *
       * \returns the legend background color
       */
      Gdk::RGBA get_legend_background_color();

      /** Set the bounding box color of the legend
       *
       * \param legend_bounding_box_color the legend bounding color
       */
      void set_legend_bounding_box_color(Gdk::RGBA legend_bounding_box_color);

      /** Get the currently selected legend bounding box color
       *
       * \returns the legend bounding box color
       */
      Gdk::RGBA get_legend_bounding_box_color();

      /** Sets the new position of the legend
       *
       * This position corresponds to the normalized coordinates
       * of the top right corner of the legend, measured with respect to the
       * top right corner of the plot box.
       * \param legend_pos_x the new position of the right top corner of the legend along the X-axis in normalized coordinates
       * \param legend_pos_y the new position of the right top corner of the legend along the Y-axis in normalized coordinates
       * \exception Gtk::PLplot::Exception
       */
      void set_legend_position(double legend_pos_x, double legend_pos_y);

      /** Gets the current position of the legend
       *
       * See set_legend_position() for more information.
       * \param legend_pos_x the current position of the right top corner of the legend along the X-axis in normalized coordinates
       * \param legend_pos_y the current position of the right top corner of the legend along the Y-axis in normalized coordinates
       */
      void get_legend_position(double &legend_pos_x, double &legend_pos_y);

      /** Show the legend
       *
       */
      void show_legend();

      /** Hides the legend
       *
       */
      void hide_legend();

      /** Returns whether or not the legend is currently showing
       *
       */
      bool is_showing_legend();

    };
  }
}
#endif
