/*
Copyright (C) 2017 Tom Schoonjans

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

#ifndef GTKMMPLPLOT_PLOT_OBJECT2D_H
#define GTKMMPLPLOT_PLOT_OBJECT2D_H

#include <gtkmm-plplot/plotobject.h>
#include <gdkmm/rgba.h>

namespace Gtk {
  namespace PLplot {
    /** \struct PlotObject2DAuxData plotobject2d.h <gtkmm-plplot/plotobject2d.h>
     *  \brief class for auxiliary data to the drawing routine draw_plot_object() used from 2D plots
     *
     */
    struct PlotObject2DAuxData : public PlotObjectAuxData {
      double world_range_x[2]; ///< plot X-range, in world coordinates
      double world_range_y[2]; ///< plot Y-range, in world coordinates
      PlotObject2DAuxData(double _world_range_x[2], double _world_range_y[2]) :
        world_range_x{_world_range_x[0], _world_range_x[1]},
        world_range_y{_world_range_y[0], _world_range_y[1]} {}
    };

    /** \class PlotObject2D plotobject2d.h <gtkmm-plplot/plotobject2d.h>
     *  \brief the abstract base class that will hold a single plot object and its properties that can be added to a plot containing 2D data, such as Plot2D, PlotContour, PlotPolar and PlotHistogram
     *
     *  The base class that all other PlotObject classes ultimately derive from. Examples are PlotLine2D and PlotText2D
     */
    class PlotObject2D : public PlotObject {
    private:
      PlotObject2D() = delete; ///< no default constructor
      PlotObject2D &operator=(const PlotObject2D &) = delete; ///< no assignment operator
      PlotObject2D(const PlotObject2D &source) = delete; ///< no default copy constructor;
    protected:
      Gdk::RGBA color; ///< The color the dataset will be drawn in
      /** Constructor
       *
       * This constructor initializes a new PlotObject2D instance, by setting the properties
       * that will determine its appearance when rendered onto a plot
       * \param color the line color
       */
      PlotObject2D(Gdk::RGBA color);
    public:
      /** Destructor
       *
       */
      virtual ~PlotObject2D();

      /** Changes the color of the dataset
       *
       * \param color The new dataset color
       */
      void set_color(Gdk::RGBA color);

      /** Get the current dataset color
       *
       * \return the current dataset color
       */
      Gdk::RGBA get_color();

      /** Check if the object can be used in a plot with a logarithmic X-axis
       *
       * \return \c true if compatible, \c false if not
       */
      virtual bool is_logarithmic_x_compatible() = 0;

      /** Check if the object can be used in a plot with a logarithmic Y-axis
       *
       * \return \c true if compatible, \c false if not
       */
      virtual bool is_logarithmic_y_compatible() = 0;
    };
  }
}
#endif


