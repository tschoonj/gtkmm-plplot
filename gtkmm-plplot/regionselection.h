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

#ifndef GTKMM_PLPLOT_REGIONSELECTION_H
#define GTKMM_PLPLOT_REGIONSELECTION_H

#include <sigc++/sigc++.h>
#include <gdk/gdk.h>
#include <gdkmm/rgba.h>

namespace Gtk {
  namespace PLplot {
    /** \class RegionSelection regionselection.h <gtkmm-plplot/regionselection.h>
     *  \brief a class providing support for selecting a region in a plot
     *
     * Several plot classes (Plot2D, PlotPolar, PlotContour and PlotContourShades)
     * that represent data in a two-dimensional grid inherit from this class, allowing
     * users to drag a selection box using a mouse gesture (mouse button press, drag, mouse button release).
     * The default signal handler will initiate a redrawing of the plot, with the visible region set to
     * correspond to the initally drawn box. This is shown in \ref example1 and others.
     */
    class RegionSelection : public sigc::trackable {
    private:
      bool region_selectable; ///< \c true indicates that a region on the plot can be selected by dragging a box with the mouse button pressed in when showing, or if double mouse button pressed event zooms out, \c false means not possible. The default is \c true
      bool region_zoomable; ///< \c true indicates that one can zoom in on a region on the plot by using the mouse scroll wheel (or trackpad), \c false means not possible. The default is \c true
      double region_zoom_scale_factor; ///< scale factor that will be used for scroll wheel based zooming
      Gdk::RGBA region_selection_color; ///< color that will be used to draw the selection box
      double region_selection_width; ///< width of the selection box frame
    protected:
      double cairo_range_x[2]; ///< the current range shown on the plot for the X-axis in Cairo coordinates
      double cairo_range_y[2]; ///< the current range shown on the plot for the Y-axis in Cairo coordinates
      double plotted_range_x[2]; ///< the current range shown on the plot for the X-axis
      double plotted_range_y[2]; ///< the current range shown on the plot for the Y-axis
      double plot_data_range_x[2]; ///< the maximum range covered by the X-values of the datasets
      double plot_data_range_y[2];  ///< the maximum range covered by the Y-values of the datasets
      sigc::signal<void, double, double, double, double > _signal_select_region; ///< signal that gets emitted whenever a new region was selected using the mouse pointer in Canvas::on_button_release_event()
      sigc::signal<void, double, double, GdkScrollDirection > _signal_zoom_region; ///< signal that gets emitted whenever one zooms in on the plot using the mouse scroll wheel in Canvas::on_scroll_event()
      sigc::signal<void, double, double > _signal_cursor_motion; ///< signal that will be emitted whenever the cursor (usually the mouse) is moved.
      sigc::signal<void, double, double> _signal_double_press; ///< signal that will emitted whenever a double mouse-click event was recorded within the plot box. Default response will be to reset the region to a range determined by the minima and maxima of the X- and Y- datasets.

      void coordinate_transform_plplot_to_cairo(
        double x_pl, double y_pl,
        double &x_cr, double &y_cr); ///< method to calculate the Cairo coordinates corresponding to PLplot coordinates, mostly used after draw, which is necessary after Canvas widget resizing.

      void coordinate_transform_cairo_to_plplot(
        double x_cr, double y_cr,
        double &x_pl, double &y_pl); ///< method to calculate the PLplot coordinates corresponding to Cairo coordinates, mostly used after draw, which is necessary after Canvas widget resizing.

      /** This is a default handler for signal_select_region()
       *
       * This method passes the plot (data) coordinates of the selection box to set_region, in order to set the plotted range corresponding to the selection box.
       * If this behavior is not desired, derive the class and implement your own on_select_region method.
       * \param xmin left X-coordinate
       * \param xmax right X-coordinate
       * \param ymin lower Y-coordinate
       * \param ymax upper Y-coordinate
       * \exception Gtk::PLplot::Exception
       */
      virtual void on_select_region(double xmin, double xmax, double ymin, double ymax);

      /** This is a default handler for signal_zoom_region()
       *
       * This method passes the plot (data) coordinates corresponding to the position where the user produced a scroll event.
       * Depending on the direction of the scroll movement, the region will be zoomed in or out.
       * If this behavior is not desired, derive the class and implement your own on_zoom_region method.
       * \param x X-coordinate
       * \param y Y-coordinate
       * \param direction Movement direction
       * \exception Gtk::PLplot::Exception
       * \since 2.2
       */
      virtual void on_zoom_region(double x, double y, GdkScrollDirection direction);

      /** This is a default handler for signal_cursor_motion()
       *
       * This signal is emitted whenever the cursor (usually the mouse) is moved within the plot. x and y correspond to the new data coordinates.
       * Currently this method does nothing, and users are encouraged to write their own signal handler or override the method in a new class.
       * \param x The X-value corresponding to the current cursor position
       * \param y The Y-value corresponding to the current cursor position
       */
      virtual void on_cursor_motion(double x, double y);

      /** This is a default handler for signal_double_press()
       *
       * This signal is emitted whenever a double mouse-click event is recorded within the plot box.
       * Default response will be the resetting of the plot region to a range determined by the minima and maxima of the X- and Y- datasets
       * \param x X-plot-coordinate of the position where the double click event occurred.
       * \param y Y-plot-coordinate of the position where the double click event occurred.
       */
      virtual void on_double_press(double x, double y);

      /** Constructor
       *
       */
      RegionSelection();

      /** Copy constructor
       *
       * \param region_selection copy source
       */
      RegionSelection(const RegionSelection &region_selection) = default;
    public:
      /** Destructor
       *
       */
      virtual ~RegionSelection();

      /** Changes the visible plotted region
       *
       * Sets the axes range of the plotted box to the provided world coordinates.
       * \param xmin left X-coordinate
       * \param xmax right X-coordinate
       * \param ymin lower Y-coordinate
       * \param ymax upper Y-coordinate
       * \exception Gtk::PLplot::Exception
       */
      void set_region(double xmin, double xmax, double ymin, double ymax);

      /** Get the currently visible plotted region
       *
       * \param xmin left X-coordinate
       * \param xmax right X-coordinate
       * \param ymin lower Y-coordinate
       * \param ymax upper Y-coordinate
       * \exception Gtk::PLplot::Exception
       */
      void get_region(double &xmin, double &xmax, double &ymin, double &ymax);

      /** Get whether regions can be selected on the plot by dragging the mouse while the button is clicked in.
       *
       * \return \c true if a region is selectable in the plot, \c false if not
       */
      bool get_region_selectable();

      /** Sets whether regions can be selected on the plot by dragging the mouse while the button is clicked in.
       *
       * \param selectable pass \c true if a region has to be selectable, \c false if not
       */
      void set_region_selectable(bool selectable = true);

      /** Get whether regions can be selected on the plot by scrolling the mouse wheel (or trackpad)
       *
       * \return \c true if a region is zoomable in the plot, \c false if not
       * \since 2.2
       */
      bool get_region_zoomable();

      /** Sets whether regions can be selected on the plot by scrolling the mouse wheel (or trackpad)
       *
       * \param zoomable pass \c true if a region has to be selectable, \c false if not
       * \since 2.2
       */
      void set_region_zoomable(bool zoomable = true);

      /** Gets the currently used zoom scale factor
       *
       * The default value is 2.0. If a value lower than 1.0 is chosen, zoom-in and zoom-out will exchange behavior.
       * \return the currently used zoom scale factor
       * \since 2.2
       */
      double get_region_zoom_scale_factor();

      /** Sets a new zoom scale factor
       *
       * \param scale_factor the new zoom scale factor
       * \exception Gtk::PLplot::Exception
       * \since 2.2
       */
      void set_region_zoom_scale_factor(double scale_factor);

      /** Gets the currently used selection box color
       *
       * The default color is black
       * \return the currently used selection box color
       * \since 2.2
       */
      Gdk::RGBA get_region_selection_color();

      /** Sets the selection box color
       *
       * \param color the new region selection box color
       * \since 2.2
       */
      void set_region_selection_color(Gdk::RGBA color);

      /** Gets the currently used selection box line width
       *
       * The default width is 2.0
       * \return the currently used selection box width
       * \since 2.2
       */
      double get_region_selection_width();

      /** Sets the selection box line width
       *
       * \param line_width the new region selection box line width
       * \exception Gtk::PLplot::Exception
       * \since 2.2
       */
      void set_region_selection_width(double line_width);

      /** This method takes care of coordinate transformations when using non-linear axes
       *
       * When a plot has logarithmic axes or polar plot style, PLplot requires the user
       * to transform the dataset into the linear cartesian coordinate system which it uses internally.
       * This method is a wrapper around the static function with the same name.
       * \param x_old the \c x world coordinate to be transformed
       * \param y_old the \c y world coordinate to be transformed
       * \param x_new the new \c x PLplot coordinate
       * \param y_new the new \c y PLplot coordinate
       */
      virtual void coordinate_transform_world_to_plplot(double x_old, double y_old, double &x_new, double &y_new);

      /** This method takes care of coordinate transformations when using non-linear axes
       *
       * When a plot has logarithmic axes or polar plot style, PLplot requires the user
       * to transform the dataset into the linear cartesian coordinate system which it uses internally.
       * This method is a wrapper around the static function with the same name.
       * \param x_old the \c x PLplot coordinate to be transformed
       * \param y_old the \c y PLplot coordinate to be transformed
       * \param x_new the new \c x world coordinate
       * \param y_new the new \c y world coordinate
       */
      virtual void coordinate_transform_plplot_to_world(double x_old, double y_old, double &x_new, double &y_new);

      /** signal_select_region is emitted whenever a selection box is dragged across a plot
       *
       * See default handler on_select_region()
       */
      sigc::signal<void, double, double, double, double > signal_select_region() {
        return _signal_select_region;
      }

      /** signal_cursor_motion is emitted whenever the cursor is moved within the plot
       *
       * See default handler on_select_region()
       */
      sigc::signal<void, double, double > signal_cursor_motion() {
        return _signal_cursor_motion;
      }

      /** signal_double_press is emitted whenever a double-click event on the plot is recorded.
       *
       * See default handler on_double_press()
       */
      sigc::signal<void, double, double > signal_double_press() {
        return _signal_double_press;
      }

      /** signal_zoom_region is emitted whenever a mouse scroll event on the plot is recorded.
       *
       * See default handler on_zoom_region()
       * \since 2.2
       */
      sigc::signal<void, double, double, GdkScrollDirection> signal_zoom_region() {
        return _signal_zoom_region;
      }

      friend class Canvas;
    };
  }
}
#endif
