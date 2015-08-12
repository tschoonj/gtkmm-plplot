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

#ifndef GTKMMPLPLOT_PLOT_CONTOUR_SHADES_H
#define GTKMMPLPLOT_PLOT_CONTOUR_SHADES_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/plotcontour.h>
#include <gtkmm-plplot/plotdatacontourshades.h>
#include <gtkmm-plplot/enums.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotContourShades plotcontourshades.h <gtkmm-plplot/plotcontourshades.h>
     *  \brief a class for contour plots with shaded regions.
     *
     *  A class for contour plots with shaded regions. Construction requires a
     *  single PlotDataContourShades dataset, and no datasets may be added afterwards.
     *  This class offers one property on top of those offered by parent class Plot: whether to draw a colorbar or not.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  For more information, the reader is referred to example \ref example8.
     */
     class PlotContourShades : public PlotContour {
     private:
       bool showing_colorbar; ///< \c true will draw a colorbar, \c false will not.
       Glib::ustring colorbar_title; ///< the title of the colorbar
       PlotContourShades() = delete; ///< no default constructor
       PlotContourShades &operator=(const PlotContourShades &) = delete; ///< no copy constructor
     protected:
      /** Constructor
       *
       * This protected constructor is meant to be used in derived classes only to circumvent the problem of calling add_data() in a constructor which calls PlotContourShades's public constructor.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotContourShades(const Glib::ustring &axis_title_x,
                        const Glib::ustring &axis_title_y,
                        const Glib::ustring &plot_title,
                        const double plot_width_norm,
                        const double plot_height_norm,
                        const double plot_offset_horizontal_norm,
                        const double plot_offset_vertical_norm);

     public:
      /** Constructor
       *
       * This class provides a single public constructor, which takes an existing PlotDataContourShades dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas.
       * The default corresponds to the plot taking up the full canvas space.
       * \param data a PlotDataContourShades object containing a plot dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotContourShades(const PlotDataContourShades &data,
                        const Glib::ustring &axis_title_x = "X-axis",
                        const Glib::ustring &axis_title_y = "Y-axis",
                        const Glib::ustring &plot_title = "",
                        const double plot_width_norm = 1.0,
                        const double plot_height_norm = 1.0,
                        const double plot_offset_horizontal_norm = 0.0,
                        const double plot_offset_vertical_norm = 0.0);

      /** Copy constructor
       *
       * \param plot plot to be copied
       */
      PlotContourShades(const PlotContourShades &plot);

      /** Destructor
       *
       */
      virtual ~PlotContourShades();

      /** Add a single PlotDataContourShades dataset to the plot
       *
       * The dataset must be a PlotDataContourShades instance: this will be verified and an exception will be thrown if the type is incorrect.
       * This will only work if the initial data set has been removed from the \c plot_data vector! PlotDataContourShades allows just one dataset...
       * \param data dataset to be added to the plot
       * \return a pointer to the PlotDataContourShades in the \c plot_data vector.
       * \exception Gtk::PLplot::Exception
       */
      virtual PlotDataContourShades *add_data(const PlotData &data);

      /** Show the colorbar
       *
       */
      void show_colorbar();

      /** Hides the colorbar
       *
       */
      void hide_colorbar();

      /** Returns whether or not the colorbar is currently showing
       *
       */
      bool is_showing_colorbar();

      /** Set the colorbar title
       *
       */
      void set_colorbar_title(Glib::ustring colorbar_title);

      /** Get the currently used colorbar title
       *
       */
      Glib::ustring get_colorbar_title();

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      //void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, plstream *_pls, int width, int height);
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) override;

      /** Freshly allocate a clone of the instance
       *
       * This very important method allows Canvas::add_plot() to add new plots to its internal array.
       * Since the canvas keeps its own copies of the plots, every Plot derived class needs to provide
       * an implementation of this method, to ensure a proper copy can be provided.
       */
      virtual PlotContourShades *clone() const;

      friend class Canvas;

     };
  }
}
#endif
