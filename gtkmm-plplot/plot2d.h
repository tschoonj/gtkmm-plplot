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

#ifndef GTKMM_PLPLOT_PLOT2D_H
#define GTKMM_PLPLOT_PLOT2D_H

#include <gtkmm-plplot/plotabstract.h>
#include <gtkmm-plplot/plotdata2d.h>
#include <gtkmm-plplot/enums.h>


namespace Gtk {
  namespace PLplot {
    /** \class Plot2D plot2d.h <gtkmm-plplot/plot2d.h>
     *  \brief a class for two-dimensional plots
     *
     *  A class for conventional two-dimensional plots. Construction requires
     *  one Plot2DData dataset, meaning it is not possible to generate an empty plot.
     *  Afterwards, other datasets may be added using the add_data method.
     *  Several properties may be set such as the axes and plot titles, type of scaling (linear or logarithmic).
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     */
    class Plot2D : public PlotAbstract {
    private:
      bool log10_x; ///< \c true means X-axis logarithmic axis, \c false means linear
      bool log10_y; ///< \c true means Y-axis logarithmic axis, \c false means linear


      virtual void plot_data_modified(); ///< a private method that will update the \c _range variables when datasets are added, modified or removed.
      virtual void convert_plplot_to_cairo_coordinates(
        double x_pl, double y_pl,
        double &x_cr, double &y_cr); ///< method to calculate the Cairo coordinates corresponding to PLplot coordinates, mostly used after draw, which is necessary after Canvas widget resizing.
      virtual void convert_cairo_to_plplot_coordinates(
        double x_cr, double y_cr,
        double &x_pl, double &y_pl); ///< method to calculate the PLplot coordinates corresponding to Cairo coordinates, mostly used after draw, which is necessary after Canvas widget resizing.
      Plot2D() = delete; ///< no default constructor
      Plot2D &operator=(const Plot2D &) = delete; ///< no copy constructor
    protected:
      /** This is a default handler for signal_select_region()
       *
       * This function passes the plot (data) coordinates of the selection box to set_region, in order to set the plotted range corresponding to the selection box.
       * If this behavior is not desired, derive the class and implement your own on_select_region method.
       * \param xmin left X-coordinate
       * \param xmax right X-coordinate
       * \param ymin lower Y-coordinate
       * \param ymax upper Y-coordinate
       * \exception Gtk::PLplot::Exception
       */
      virtual void on_select_region(double xmin, double xmax, double ymin, double ymax);

      /** This is a default handler for signal_changed()
       *
       * This signal is emitted whenever any of the plot properties, or the properties of the Plot2DData datasets therein, is changed.
       * Currently it does nothing but the signal will get caught by Canvas, and will eventually trigger a redrawing of the entire widget.
       */
      virtual void on_changed();

      /** This is a default handler for signal_data_added()
       *
       * This signal is emitted whenever a new Plot2DData dataset is added to the plot.
       * It will call the private method plot_data_modified, which will update \c *_range_* properties, and emit signal_changed.
       * \param new_data a pointer to the newly added Plot2DData dataset, after it has been added to the \c plot_data vector.
       */
      virtual void on_data_added(PlotDataAbstract *new_data);

      /** This is a default handler for signal_data_removed()
       *
       * This signal is emitted whenever a Plot2DData dataset is removed from the plot.
       * It will call the private method plot_data_modified, which will update \c *_range_* properties, and emit signal_changed.
       */
      virtual void on_data_removed();

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
      void coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new);

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
      void coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT &x_new, PLFLT &y_new);

      /** This static method takes care of coordinate transformations when using non-linear axes
       *
       * When a plot has logarithmic axes or polar plot style, PLplot requires the user
       * to transform the dataset into the linear cartesian coordinate system which it uses internally.
       * The reason that this is static method is due to PLplot expecting a C function...
       * \param x_old the \c x world coordinate to be transformed
       * \param y_old the \c y world coordinate to be transformed
       * \param x_new the new \c x PLplot coordinate
       * \param y_new the new \c y PLplot coordinate
       * \param object the object we are dealing with
       */
      static void coordinate_transform_world_to_plplot(PLFLT x_old, PLFLT y_old, PLFLT *x_new, PLFLT *y_new, PLPointer object);

      /** This static method takes care of coordinate transformations when using non-linear axes
       *
       * When a plot has logarithmic axes or polar plot style, PLplot requires the user
       * to transform the dataset into the linear cartesian coordinate system which it uses internally.
       * The reason that this is static method is due to PLplot expecting a C function...
       * \param x_old the \c x PLplot coordinate to be transformed
       * \param y_old the \c y PLplot coordinate to be transformed
       * \param x_new the new \c x world coordinate
       * \param y_new the new \c y world coordinate
       * \param object the object we are dealing with
       */
      static void coordinate_transform_plplot_to_world(PLFLT x_old, PLFLT y_old, PLFLT *x_new, PLFLT *y_new, PLPointer object);
    public:
      /** Constructor
       *
       * This class provides a single constructor, which takes an existing Plot2DData dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas. The default corresponds to the plot taking up the full c
       * \param data a Plot2DData object containing a plot dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      Plot2D(const Plot2DData &data,
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
      Plot2D(const Plot2D &plot);

      /** Destructor
       *
       */
      virtual ~Plot2D();

      /** Add a single Plot2DData dataset to the plot
       *
       * \param data dataset to be added to the plot
       * \return a pointer to the Plot2DData in the \c plot_data vector.
       */
      void add_data(const PlotData2D &data);

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      //void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, plstream *_pls, int width, int height);
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height);

      /** Sets the scaling of the X-axis to logarithmic
       *
       * Before calling this method, the (default) scaling is linear.
       * The function will check if the X-values allow for a logarithmic axis. If not, an exception will be thrown.
       * \param log10 \c true sets scaling to logarithmic, \c false to linear.
       * \exception Gtk::PLplot::Exception
       */
      void set_axis_logarithmic_x(bool log10 = true);

      /** Sets the scaling of the Y-axis to logarithmic
       *
       * Before calling this method, the (default) scaling is linear.
       * The function will check if the Y-values allow for a logarithmic axis. If not, an exception will be thrown.
       * \param log10 \c true sets scaling to logarithmic, \c false to linear.
       * \exception Gtk::PLplot::Exception
       */
      void set_axis_logarithmic_y(bool log10 = true);

      /** Get the current scaling of the X-axis
       *
       * \return \c true indicates logarithmic scaling, \c false linear scaling.
       */
      bool get_axis_logarithmic_x();

      /** Get the current scaling of the Y-axis
       *
       * \return \c true indicates logarithmic scaling, \c false linear scaling.
       */
      bool get_axis_logarithmic_y();


      friend class Canvas;
    };
  }
}
#endif
