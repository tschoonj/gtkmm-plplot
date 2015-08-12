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

#ifndef GTKMMPLPLOT_PLOT_DATA_CONTOUR_SHADES_H
#define GTKMMPLPLOT_PLOT_DATA_CONTOUR_SHADES_H

#include <gtkmm-plplot/gtkmm-plplotconfig.h>
#include <gtkmm-plplot/plotdatacontour.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotDataContourShades plotdatacontourshades.h <gtkmm-plplot/plotdatacontourshades.h>
     *  \brief a class for PlotContourShades datasets
     *
     *  Instances of this class will hold a single (and the only possible) dataset for
     *  a PlotContourShades plot. Apart from the data necessary to generate a continous color shaded plot, it contains
     *  several properties that will determine the appearance of the plot. The input data can be provided
     *  as std::vector (X and Y), std::valarray (X and Y), PLFLT** (Z, array of arrays) and boost:multi_array (Z).
     *  Most of the methods provided are inherited from PlotDataContour.
     *  This datatype is demonstrated in \ref example8.
     */
     class PlotDataContourShades : public PlotDataContour {
     private:
       PlotDataContourShades() = delete; ///< no default constructor
       PlotDataContourShades &operator=(const PlotDataContourShades &) = delete; ///< no copy constructor
     protected:
       ColormapPalette colormap_palette; ///< The colormap that will be used to render the contourplot shades.
       AreaFillPattern area_fill_pattern; ///< The pattern that will be used to draw the shaded regions.
       double fill_width; ///< Defines line width used by the fill pattern.
       bool showing_edges; ///< \c true will draw contour lines at the edges, \c false will not.
     public:
      /** Constructor
       *
       * Initializes a new dataset for a PlotContourShades.
       * \param x the X-grid coordinates of the dataset, provided as a std::vector. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::vector. Have to match the second dimension of z
       * \param z the actual contour data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContourShades(const std::vector<PLFLT> &x,
                            const std::vector<PLFLT> &y,
                            PLFLT **z,
                            unsigned int nlevels = 6,
                            ColormapPalette colormap_palette = BLUE_YELLOW,
                            Gdk::RGBA edge_color = Gdk::RGBA("black"),
                            double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContourShades.
       * \param x the X-grid coordinates of the dataset, provided as a std::valarray. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::valarray. Have to match the second dimension of z
       * \param z the actual contour data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContourShades(const std::valarray<PLFLT> &x,
                            const std::valarray<PLFLT> &y,
                            PLFLT **z,
                            unsigned int nlevels = 6,
                            ColormapPalette colormap_palette = BLUE_YELLOW,
                            Gdk::RGBA edge_color = Gdk::RGBA("black"),
                            double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContourShades. The X- and Y-values will be
       * set to a vector of the appropriate size with elements set to correspond to their array subscript.
       * \param nx the extent of z along the first dimension
       * \param ny the extent of z along the second dimension
       * \param z the actual contour data, provided as a dynamically allocated array of dynamically allocated arrays. The constructor will initialize its own deep-copy of this variable.
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContourShades(unsigned int nx,
                            unsigned int ny,
                            PLFLT **z,
                            unsigned int nlevels = 6,
                            ColormapPalette colormap_palette = BLUE_YELLOW,
                            Gdk::RGBA edge_color = Gdk::RGBA("black"),
                            double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

#ifdef GTKMM_PLPLOT_BOOST_ENABLED
      /** Constructor
       *
       * Initializes a new dataset for a PlotContourShades.
       * \param x the X-grid coordinates of the dataset, provided as a std::vector. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::vector. Have to match the second dimension of z
       * \param z the actual contour data, provided as a Boost \c multi_array.
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContourShades(const std::vector<PLFLT> &x,
                            const std::vector<PLFLT> &y,
                            const boost::multi_array<PLFLT, 2> &z,
                            unsigned int nlevels = 6,
                            ColormapPalette colormap_palette = BLUE_YELLOW,
                            Gdk::RGBA edge_color = Gdk::RGBA("black"),
                            double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContourShades.
       * \param x the X-grid coordinates of the dataset, provided as a std::valarray. Have to match the first dimension of z
       * \param y the Y-grid coordinates of the dataset, provided as a std::valarray. Have to match the second dimension of z
       * \param z the actual contour data, provided as a Boost \c multi_array.
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContourShades(const std::valarray<PLFLT> &x,
                            const std::valarray<PLFLT> &y,
                            const boost::multi_array<PLFLT, 2> &z,
                            unsigned int nlevels = 6,
                            ColormapPalette colormap_palette = BLUE_YELLOW,
                            Gdk::RGBA edge_color = Gdk::RGBA("black"),
                            double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);

      /** Constructor
       *
       * Initializes a new dataset for a PlotContourShades.The X- and Y-values will be
       * set to a vector of the appropriate size with elements set to correspond to their array subscript.
       * \param z the actual contour data, provided as a Boost \c multi_array.
       * \param nlevels the number of contour edges required for the plot
       * \param colormap_palette the colormap palette that will be used to render the shaded regions of the contourplot
       * \param edge_color the contour edge color, default is black
       * \param edge_width the contour edge width, default is 1.0
       */
      PlotDataContourShades(const boost::multi_array<PLFLT, 2> &z,
                            unsigned int nlevels = 6,
                            ColormapPalette colormap_palette = BLUE_YELLOW,
                            Gdk::RGBA edge_color = Gdk::RGBA("black"),
                            double edge_width = PLOTDATA_DEFAULT_LINE_WIDTH);
#endif

      /** Copy constructor
       *
       * \param data dataset to be copied
       */
      PlotDataContourShades(const PlotDataContourShades &data);

      /** Destructor
       *
       */
      virtual ~PlotDataContourShades();



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
       * \param colorbar_title the text that will be added to the colorbar to reflect what type of data is rendered.
       * \param background_color the background color of the colorbar
       * \param bounding_box_color the plot bounding box color
       */
      virtual void draw_colorbar(const Cairo::RefPtr<Cairo::Context> &cr,
                                 plstream *pls,
                                 Glib::ustring colorbar_title,
                                 Gdk::RGBA background_color,
                                 Gdk::RGBA bounding_box_color);

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
