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

#ifndef GTKMM_PLPLOT_PLOTPOLAR_H
#define GTKMM_PLPLOT_PLOTPOLAR_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/legend.h>
#include <gtkmm-plplot/regionselection.h>
#include <gtkmm-plplot/plotdatapolar.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotPolar plotpolar.h <gtkmm-plplot/plotpolar.h>
     *  \brief a class for polar plots
     *
     *  A class for polar plots. Construction requires
     *  a single PlotDataPolar (which derives from PlotData2D) dataset, meaning it is not possible to generate an empty plot.
     *  Afterwards, other datasets may be added using the add_data() method.
     *  The \c x and \c y class variables of PlotData2D will when used with this class be interpreted
     *  to correspond to the radial coordinate (usually denoted as \c r) and the angular coordinate (usually denoted as \c θ), respectively.
     *  Several properties may be set such as the axes and plot titles.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  An example of this class is presented in \ref example6.
     */
    class PlotPolar : public Plot, public Legend, public RegionSelection {
    private:
      double max_r; ///< the maximum radial coordinate in the datasets
      PlotPolar() = delete; ///< no default constructor
      PlotPolar(const PlotPolar &) = delete; ///< no default copy constructor
      PlotPolar &operator=(const PlotPolar &) = delete; ///< no assignment operator
    protected:
      virtual void plot_data_modified() override;  ///< a method that will update the \c _range variables when datasets are added, modified or removed.

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

      /** Constructor
       *
       * This protected constructor is meant to be used in derived classes only to circumvent the problem of calling add_data() in a constructor which calls PlotPolar's public constructor.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotPolar(const Glib::ustring &axis_title_x,
                const Glib::ustring &axis_title_y,
                const Glib::ustring &plot_title,
                const double plot_width_norm,
                const double plot_height_norm,
                const double plot_offset_horizontal_norm,
                const double plot_offset_vertical_norm);

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
       * This class provides a single public constructor, which takes an existing PlotDataPolar dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas.
       * \param data a PlotDataPolar object containing a plot dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotPolar(PlotDataPolar &data,
                const Glib::ustring &axis_title_x = "",
                const Glib::ustring &axis_title_y = "",
                const Glib::ustring &plot_title = "",
                const double plot_width_norm = 1.0,
                const double plot_height_norm = 1.0,
                const double plot_offset_horizontal_norm = 0.0,
                const double plot_offset_vertical_norm = 0.0);

      /** Destructor
       *
       */
      virtual ~PlotPolar();

      /** Add a single PlotDataPolar dataset to the plot
       *
       * \param data dataset to be added to the plot
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_data(PlotDataPolar &data);

      /** Remove a single dataset from the plot
       *
       * \param plot_data_index index of the plotdata in the \c plot_data vector
       * \exception Gtk::PLplot::Exception
       * \since 2.2
       */
      virtual void remove_data(unsigned int plot_data_index) override;

      /** Remove a single dataset from the plot
       *
       * \param plot_data_member pointer to the plotdata in the \c plot_data vector
       * \exception Gtk::PLplot::Exception
       * \since 2.2
       */
      virtual void remove_data(PlotData &plot_data_member) override;

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
