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

#ifndef GTKMMPLPLOT_PLOT_CONTOUR_H
#define GTKMMPLPLOT_PLOT_CONTOUR_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/regionselection.h>
#include <gtkmm-plplot/plotdatasurface.h>
#include <gtkmm-plplot/enums.h>


namespace Gtk {
  namespace PLplot {
    /** \class PlotContour plotcontour.h <gtkmm-plplot/plotcontour.h>
     *  \brief a class for contour plots
     *
     *  A class for conventional contour plots. Construction requires a
     *  single PlotDataSurface dataset, and no datasets may be added afterwards.
     *  For more beautiful contour plot with shaded regions, check out PlotContourShades.
     *  PlotContour features several properties that determine the appearance of the contour.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  For more information, the reader is referred to example \ref example7.
     */
    class PlotContour : public Plot, public RegionSelection {
    private:
      PlotContour() = delete; ///< no default constructor
      PlotContour(const PlotContour &) = delete; ///< no copy constructor
      PlotContour &operator=(const PlotContour &) = delete; ///< no assignment operator
    protected:
      unsigned int nlevels; ///< Number of contour levels to draw
      Gdk::RGBA edge_color; ///< Defines pen color used for contours defining edges.
      double edge_width; ///< Defines line width used for contours defining edges.
      bool showing_labels; ///< \c true will draw the contour labels, \c false hides them
      //TODO: add BoxStyle???
      double zmin; ///< Minimum of \c z, used to determine the contour edges
      double zmax; ///< Maximum of \c z, used to determine the contour edges
      std::vector<double> clevels; ///< Vector containing the data levels corresponding to the edges of each region that will be plotted. To work properly the levels should be monotonic.

      virtual void plot_data_modified() override; ///< a method that will update the \c _range variables when datasets are added, modified or removed.

      /** Constructor
       *
       * This protected constructor is meant to be used in derived classes only to circumvent the problem of calling add_data() in a constructor which calls PlotContour's public constructor.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotContour(const Glib::ustring &axis_title_x,
                  const Glib::ustring &axis_title_y,
                  const Glib::ustring &plot_title,
                  unsigned int nlevels,
                  Gdk::RGBA edge_color,
                  double edge_width,
                  const double plot_width_norm,
                  const double plot_height_norm,
                  const double plot_offset_horizontal_norm,
                  const double plot_offset_vertical_norm);

      /** Add a single PlotDataSurface dataset to the plot
       *
       * \param data dataset to be added to the plot
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_data(PlotDataSurface &data);

    public:
      /** Constructor
       *
       * This class provides a single public constructor, which takes an existing PlotDataSurface dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas.
       * The default corresponds to the plot taking up the full canvas space.
       * \param data a PlotDataSurface object containing a plot dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param nlevels the number of contour edges required for the plot
       * \param edge_color the contour edge color, default is red
       * \param edge_width the contour edge width, default is 1.0
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotContour(PlotDataSurface &data,
                  const Glib::ustring &axis_title_x = "X-axis",
                  const Glib::ustring &axis_title_y = "Y-axis",
                  const Glib::ustring &plot_title = "",
                  unsigned int nlevels = 6,
                  Gdk::RGBA edge_color = Gdk::RGBA("red"),
                  double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH,
                  const double plot_width_norm = 1.0,
                  const double plot_height_norm = 1.0,
                  const double plot_offset_horizontal_norm = 0.0,
                  const double plot_offset_vertical_norm = 0.0);

      /** Destructor
       *
       */
      virtual ~PlotContour();

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

      /** Show the contour labels
       *
       */
      void show_labels();

      /** Hides the contour labels
       *
       */
      void hide_labels();

      /** Returns whether or not the contour labels are currently showing
       *
       */
      bool is_showing_labels() const;

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) override;

      friend class Canvas;
    };
  }
}



#endif
