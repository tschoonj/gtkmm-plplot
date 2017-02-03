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

#ifndef GTKMM_PLPLOT_PLOT_HISTOGRAM_H
#define GTKMM_PLPLOT_PLOT_HISTOGRAM_H

#include <gtkmm-plplot/plot.h>
#include <gtkmm-plplot/plotdatahistogram.h>
#include <gtkmm-plplot/regionselection.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotHistogram plothistogram.h <gtkmm-plplot/plothistogram.h>
     *  \brief a class for plotting histograms
     *
     *  A class for histogram plots.
     *
     *  Construction requires a single PlotDataHistogram dataset
     *  (currently implemented by PlotDataHistogramUnbinned and PlotDataHistogramBinned),
     *  and no datasets may be added afterwards,
     *  Several properties may be set such as the axes and plot titles.
     *  Important is that whenever a property is changed, \c signal_changed() is emitted, which will eventually
     *  be picked up by the \c canvas that will hold the plot.
     *  For more information, the reader is referred to example \ref example12.
     */
    class PlotHistogram : public Plot, public RegionSelection {
    private:
      PlotHistogram() = delete; ///< no default constructor
      PlotHistogram(const PlotHistogram &) = delete; ///< no copy constructor
      PlotHistogram &operator=(const PlotHistogram &) = delete; ///< no assignment operato
    protected:
      virtual void plot_data_modified() override; ///< a method that will update the \c _range variables when datasets are added, modified or removed.

      /** Constructor
       *
       * This protected constructor is meant to be used in derived classes only to circumvent the problem of calling add_data() in a constructor which calls PlotHistogram's public constructor.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotHistogram(const Glib::ustring &axis_title_x,
                    const Glib::ustring &axis_title_y,
                    const Glib::ustring &plot_title,
                    const double plot_width_norm,
                    const double plot_height_norm,
                    const double plot_offset_horizontal_norm,
                    const double plot_offset_vertical_norm);

      /** Add a single PlotDataHistogram dataset to the plot
       *
       * \param data dataset to be added to the plot
       * \exception Gtk::PLplot::Exception
       */
      virtual void add_data(PlotDataHistogram &data);

      public:
      /** Constructor
       *
       * This class provides a single public constructor, which takes an existing PlotDataHistogram dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas.
       * The default corresponds to the plot taking up the full canvas space.
       * \param data a PlotDataHistogram object containing a histogram dataset
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      PlotHistogram(PlotDataHistogram &data,
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
      virtual ~PlotHistogram();

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) override;
    };
  }
}
#endif
