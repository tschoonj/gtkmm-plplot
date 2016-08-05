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

#ifndef GTKMM_PLPLOT_PLOT3D_H
#define GTKMM_PLPLOT_PLOT3D_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/plotdata3d.h>
#include <gtkmm-plplot/legend.h>
#include <gtkmm-plplot/enums.h>

namespace Gtk {
  namespace PLplot {
    /** \class Plot3D plot3d.h <gtkmm-plplot/plot3d.h>
     *  \brief a class for three-dimensional plots
     *
     *  A class for conventional three-dimensional plots. Construction requires
     *  one PlotData3D dataset, meaning it is not possible to generate an empty plot.
     *  Afterwards, other datasets may be added using the add_data() method.
     *  Several properties may be set such as the axes and plot titles.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  For more information, the reader is referred to the examples \ref example9.
     */
    class Plot3D : public Plot, public Legend {
    private:
      Glib::ustring axis_title_z; ///< Z-axis title
      double plotted_range_x[2]; ///< the current range shown on the plot for the X-axis
      double plotted_range_y[2]; ///< the current range shown on the plot for the Y-axis
      double plotted_range_z[2]; ///< the current range shown on the plot for the Z-axis
      double plot_data_range_x[2]; ///< the maximum range covered by the X-values of the datasets
      double plot_data_range_y[2];  ///< the maximum range covered by the Y-values of the datasets
      double plot_data_range_z[2];  ///< the maximum range covered by the Z-values of the datasets
      double altitude; ///< the viewing altitude
      double azimuth; ///< the viewing azimuth

      Plot3D() = delete; ///< no default constructor
      Plot3D(const Plot3D &) = delete; ///< no default copy constructor
      Plot3D &operator=(const Plot3D &) = delete; ///< no assignment operator
    protected:
      virtual void plot_data_modified() override; ///< a method that will update the \c _range variables when datasets are added, modified or removed.

      /** Constructor
       *
       * This protected constructor is meant to be used in derived classes only to circumvent the problem of calling add_data() in a constructor which calls Plot3D's public constructor.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param axis_title_z Z-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      Plot3D(const Glib::ustring &axis_title_x,
             const Glib::ustring &axis_title_y,
             const Glib::ustring &axis_title_z,
             const Glib::ustring &plot_title,
             const double plot_width_norm,
             const double plot_height_norm,
             const double plot_offset_horizontal_norm,
             const double plot_offset_vertical_norm);
    public:
      /** Constructor
       *
       * This class provides a single public constructor, which takes an existing PlotData3D dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas. The default corresponds to the plot taking up the full c
       * \param data a PlotData3D object containing a plot dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param axis_title_z Z-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      Plot3D(PlotData3D &data,
             const Glib::ustring &axis_title_x = "X-axis",
             const Glib::ustring &axis_title_y = "Y-axis",
             const Glib::ustring &axis_title_z = "Z-axis",
             const Glib::ustring &plot_title = "",
             const double plot_width_norm = 1.0,
             const double plot_height_norm = 1.0,
             const double plot_offset_horizontal_norm = 0.0,
             const double plot_offset_vertical_norm = 0.0);

      /** Destructor
       *
       */
      virtual ~Plot3D();

      /** Set the viewing altitude in degrees above the XY plane
       *
       * \param altitude the new viewing altitude
       */
      void set_altitude(double altitude);

      /** Get the current viewing altitude in degrees above the XY plane
       *
       * \return the current viewing altitude
       */
      double get_altitude();

      /** Set the viewing azimuth in degrees
       *
       * When the azimuth is zero, the observer is looking face onto the ZX plane, and as azimuth is increased, the observer moves clockwise around the box when viewed from above the XY plane.
       * \param azimuth the new viewing azimuth
       */
      void set_azimuth(double azimuth);

      /** Get the current viewing azimuth in degrees
       *
       * When the azimuth is zero, the observer is looking face onto the ZX plane, and as azimuth is increased, the observer moves clockwise around the box when viewed from above the XY plane.
       * \return the current viewing azimuth
       */
      double get_azimuth();

      /** Add a single PlotData3D dataset to the plot
       *
       * The dataset must be a PlotData3D instance: this will be verified and an exception will be thrown if the type is incorrect.
       * \param data dataset to be added to the plot
       * \return a pointer to the PlotData3D in the \c plot_data vector.
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_data(PlotData3D &data);

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
