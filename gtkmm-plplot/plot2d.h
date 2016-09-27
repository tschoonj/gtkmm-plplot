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

#ifndef GTKMM_PLPLOT_PLOT2D_H
#define GTKMM_PLPLOT_PLOT2D_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/plotdata2d.h>
#include <gtkmm-plplot/legend.h>
#include <gtkmm-plplot/regionselection.h>
#include <gtkmm-plplot/enums.h>

namespace Gtk {
  namespace PLplot {
    /** \class Plot2D plot2d.h <gtkmm-plplot/plot2d.h>
     *  \brief a class for two-dimensional plots
     *
     *  A class for conventional two-dimensional plots.
     *  New plots can be created with either none or a single dataset.
     *  Afterwards, additional datasets may be added using the add_data() method.
     *  Several properties may be set such as the axes and plot titles, type of scaling (linear or logarithmic).
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  For more information, the reader is referred to the examples \ref example1, \ref example2, \ref example3, \ref example4
     *  and \ref example5.
     */
    class Plot2D : public Plot, public Legend, public RegionSelection {
    private:
      bool log10_x; ///< \c true means X-axis logarithmic axis, \c false means linear
      bool log10_y; ///< \c true means Y-axis logarithmic axis, \c false means linear
      BoxStyle box_style; ///< the currently used box style to draw the box, axes and grid

      Plot2D() = delete; ///< no default constructor
      Plot2D(const Plot2D &) = delete; ///< no default copy constructor
      Plot2D &operator=(const Plot2D &) = delete; ///< no assignment operator
    protected:
      virtual void plot_data_modified() override; ///< a method that will update the \c _range variables when datasets are added, modified or removed.

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
      static void coordinate_transform_world_to_plplot(double x_old, double y_old, double *x_new, double *y_new, void *object);

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
      static void coordinate_transform_plplot_to_world(double x_old, double y_old, double *x_new, double *y_new, void *object);

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
      virtual void coordinate_transform_world_to_plplot(double x_old, double y_old, double &x_new, double &y_new) override;

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
      virtual void coordinate_transform_plplot_to_world(double x_old, double y_old, double &x_new, double &y_new) override;
    public:

      /** Constructor
       *
       * This constructor produces an empty PlotData2D plot. Add data to it using add_data.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas. The default corresponds to the plot taking up the full c
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      Plot2D(const Glib::ustring &axis_title_x = "X-axis",
             const Glib::ustring &axis_title_y = "Y-axis",
             const Glib::ustring &plot_title = "",
             const double plot_width_norm = 1.0,
             const double plot_height_norm = 1.0,
             const double plot_offset_horizontal_norm = 0.0,
             const double plot_offset_vertical_norm = 0.0);

      /** Constructor
       *
       * This constructor takes an existing PlotData2D dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas. The default corresponds to the plot taking up the full c
       * \param data a PlotData2D object containing a plot dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      Plot2D(PlotData2D &data,
             const Glib::ustring &axis_title_x = "X-axis",
             const Glib::ustring &axis_title_y = "Y-axis",
             const Glib::ustring &plot_title = "",
             const double plot_width_norm = 1.0,
             const double plot_height_norm = 1.0,
             const double plot_offset_horizontal_norm = 0.0,
             const double plot_offset_vertical_norm = 0.0);

      /** Destructor
       *
       */
      virtual ~Plot2D();

      /** Add a single PlotData2D dataset to the plot
       *
       * \param data dataset to be added to the plot
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_data(PlotData2D &data);

      /** Remove a single dataset from the plot
       *
       * \param plot_data_index index of the plotdata in the \c plot_data vector
       * \exception Gtk::PLplot::Exception
       */
      virtual void remove_data(unsigned int plot_data_index) override;

      /** Remove a single dataset from the plot
       *
       * \param plot_data_member pointer to the plotdata in the \c plot_data vector
       * \exception Gtk::PLplot::Exception
       */
      virtual void remove_data(PlotData &plot_data_member) override;

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) override;

      /** Set the box style
       *
       * \param style the new box style (default is BOX_TICKS_TICK_LABELS)
       */
      void set_box_style(BoxStyle style = BOX_TICKS_TICK_LABELS);

      /** Get the box style
       *
       * \return the currently selected box style
       */
      BoxStyle get_box_style();


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
