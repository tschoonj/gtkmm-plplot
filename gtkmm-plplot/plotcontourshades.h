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

#ifndef GTKMMPLPLOT_PLOT_CONTOUR_SHADES_H
#define GTKMMPLPLOT_PLOT_CONTOUR_SHADES_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/plotcontour.h>
#include <gtkmm-plplot/enums.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotContourShades plotcontourshades.h <gtkmm-plplot/plotcontourshades.h>
     *  \brief a class for contour plots with shaded regions.
     *
     *  A class for contour plots with shaded regions. Construction requires a
     *  single PlotDataSurface dataset, and no datasets may be added afterwards.
     *  This class offers one property on top of those offered by parent class Plot: whether to draw a colorbar or not.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  For more information, the reader is referred to example \ref example8.
     */
     class PlotContourShades : public PlotContour {
     private:
       bool showing_colorbar; ///< \c true will draw a colorbar, \c false will not.
       bool showing_edges; ///< \c true will draw contour lines at the edges, \c false will not.
       Glib::ustring colorbar_title; ///< the title of the colorbar
       ColormapPalette colormap_palette; ///< The colormap that will be used to render the contourplot shades.
       AreaFillPattern area_fill_pattern; ///< The pattern that will be used to draw the shaded regions.
       double fill_width; ///< Defines line width used by the fill pattern.
       PlotContourShades() = delete; ///< no default constructor
       PlotContourShades(const PlotContourShades &) = delete; ///< no default copy constructor
       PlotContourShades &operator=(const PlotContourShades &) = delete; ///< no assignment operator
     protected:
      /** Constructor
       *
       * This protected constructor is meant to be used in derived classes only to circumvent the problem of calling add_data() in a constructor which calls PlotContourShades's public constructor.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotContourShades(const Glib::ustring &axis_title_x,
                        const Glib::ustring &axis_title_y,
                        const Glib::ustring &plot_title,
                        unsigned int nlevels,
                        ColormapPalette colormap_palette,
                        Gdk::RGBA edge_color,
                        double edge_width,
                        const double plot_width_norm,
                        const double plot_height_norm,
                        const double plot_offset_horizontal_norm,
                        const double plot_offset_vertical_norm);

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
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotContourShades(PlotDataSurface &data,
                        const Glib::ustring &axis_title_x = "X-axis",
                        const Glib::ustring &axis_title_y = "Y-axis",
                        const Glib::ustring &plot_title = "",
                        unsigned int nlevels = 6,
                        ColormapPalette colormap_palette = BLUE_YELLOW,
                        Gdk::RGBA edge_color = Gdk::RGBA("black"),
                        double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH,
                        const double plot_width_norm = 1.0,
                        const double plot_height_norm = 1.0,
                        const double plot_offset_horizontal_norm = 0.0,
                        const double plot_offset_vertical_norm = 0.0);

      /** Destructor
       *
       */
      virtual ~PlotContourShades();

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

      /** Changes the colormap palette of the contour shades
       *
       * \param colormap The new colormap palette
       */
      void set_colormap_palette(ColormapPalette colormap);

      /** Get the current contour shades colormap palette
       *
       * \returns the current contour shades colormap palette
       */
      ColormapPalette get_colormap_palette();

      /** Changes the area fill pattern of the contour shades
       *
       * \param area_fill_pattern The new contour shades area fill pattern
       */
      void set_area_fill_pattern(AreaFillPattern area_fill_pattern);

      /** Get the current contour shades area fill pattern
       *
       * \returns the current contour shades area fill pattern
       */
      AreaFillPattern get_area_fill_pattern();

      /** Set the width of the lines used by the fill pattern
       *
       * Relevant only if get_area_fill_pattern() is not set to AreaFillPattern::SOLID.
       * \param width the new width
       * \exception Gtk::PLplot::Exception
       */
      void set_area_lines_width(double width);

      /** Get the current width of fill pattern lines
       *
       * Relevant only if get_area_fill_pattern() is not set to AreaFillPattern::SOLID.
       * \returns the current width of the fill pattern lines
       */
      double get_area_lines_width();

      /** Marks the contour edges for showing
       *
       */
      void show_edges();

      /** Marks the contour edges for hiding
       *
       */
      void hide_edges();

      /** Gets whether or not the contour edge lines are showing
       *
       * \returns \c true means the contour edge lines are showing, \c false means not
       */
      bool is_showing_edges();

      /** Method to draw the colorbar
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       */
      virtual void draw_colorbar(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls);

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      //void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, plstream *_pls, int width, int height);
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) override;

      friend class Canvas;

     };
  }
}
#endif
