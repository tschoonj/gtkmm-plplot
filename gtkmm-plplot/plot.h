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

#ifndef GTKMM_PLPLOT_PLOT_H
#define GTKMM_PLPLOT_PLOT_H

#include <vector>
#include <cairomm/cairomm.h>
#include <glibmm/ustring.h>
#include <gdkmm/rgba.h>
#include <gtkmm-plplot/object.h>
#include <gtkmm-plplot/plotdata.h>


class plstream;

namespace Gtk {
  namespace PLplot {
    /** \class Plot plot.h <gtkmm-plplot/plot.h>
     *  \brief an abstract base class for all plots
     *
     *  This is the base class from which all other plots classes are derived.
     *  It contains several pure virtual methods that have to be implemented by all derived classes.
     *  This class cannot be instantiated directly, meaning that instances returned by Canvas::get_plot() and
     *  Canvas::add_plot() need to be cast to access methods offered only by the derived class.
     */
     class Plot : public Object {
     private:
      Plot &operator=(const Plot &) = delete; ///< no assignment operator
      Plot(const Plot &) = delete; ///< no default copy constructor
      Plot() = delete;
     protected:
      std::vector<PlotData *> plot_data; ///< vector that contains pointers to the PlotData datasets
      Glib::ustring axis_title_x; ///< X-axis title
      Glib::ustring axis_title_y; ///< Y-axis title
      Glib::ustring plot_title;   ///< Plot title
      plstream *pls; ///< the \c plstream object that will ensure the connection with PLplot
      int canvas_width; ///< the width of the canvas in Cairo units
      int canvas_height; ///< the height of the canvas in Cairo units
      int plot_offset_x; ///< the offset of the plot with respect to the top left corner of the canvas, measured along the horizontal (X-) axis in Cairo units
      int plot_offset_y; ///< the offset of the plot with respect to the top left corner of the canvas, measured along the vertical (Y-) axis in Cairo units
      int plot_width; ///< the current width of the plot in Cairo units
      int plot_height; ///< the current height of the plot in Cairo units
      bool shown; ///< \c true means the plot is currently visible, \c false means it is not plotted
      Gdk::RGBA background_color; ///< the currently used background color of the plot (default = fully transparent white, meaning that the background will be determined by the canvas)
      const double plot_width_norm; ///< the normalized plot width, calculated relative to the canvas width
      const double plot_height_norm; ///< the normalized plot height, calculated relative to the canvas height
      const double plot_offset_horizontal_norm; ///< the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
      const double plot_offset_vertical_norm; ///< the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
      Gdk::RGBA axes_color; ///< the currently used color to draw the axes, the box and gridlines. Default is opaque black
      Gdk::RGBA titles_color; ///< the currently used color to draw the axes and plot titles. Default is opaque black

      sigc::signal<void> _signal_changed; ///< signal that gets emitted whenever any of the plot parameters, or any of the contained PlotData datasets is changed.
      sigc::signal<void, PlotData *> _signal_data_added; ///< signal emitted whenever a new PlotData dataset is added to the plot
      sigc::signal<void, PlotData *> _signal_data_removed; ///< signal emitted whenever data is removed from the plot.

      virtual void plot_data_modified() = 0; ///< a method that will update the \c _range variables when datasets are added, modified or removed.

      /** This is a default handler for signal_changed()
       *
       * This signal is emitted whenever any of the plot properties, or the properties of the PlotData datasets therein, is changed.
       * Currently it does nothing but the signal will get caught by Canvas, and will eventually trigger a redrawing of the entire widget.
       */
      virtual void on_changed();

      /** This is a default handler for signal_data_added()
       *
       * This signal is emitted whenever a new PlotData dataset is added to the plot.
       * It will call the private method plot_data_modified, which will update \c *_range_* properties, and emit signal_changed.
       * \param new_data a pointer to the newly added PlotData dataset, after it has been added to the \c plot_data vector.
       */
      virtual void on_data_added(PlotData *new_data);

      /** This is a default handler for signal_data_removed()
       *
       * This signal is emitted whenever a PlotData dataset is removed from the plot with remove_data.
       * It will first delete removed_data, provided its memory is managed, followed by a call to the
       * private method plot_data_modified, which will update \c *_range_* properties, and emit signal_changed.
       * \param removed_data a pointer to the PlotData dataset that was just removed. It should be deleted in this method or a memory leak will be created.
       */
      virtual void on_data_removed(PlotData *removed_data);

      /** Initialization method for draw_plot.
       *
       *  Should be called by all draw_plot() methods in derived classes.
       * \param cr Cairo context
       * \param width width of the plot in normalized coordinates
       * \param height height of the plot in normalized coordinates
       */
      virtual void draw_plot_init(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) final;

      /** Constructor
       *
       * This class provides a single constructor, which takes an existing PlotData dataset to construct a plot.
       * Optionally, the constructor takes additional arguments to set the axes and plot titles, as well as normalized coordinates that will determine the position and dimensions of the plot within the canvas. The default corresponds to the plot taking up the full space.
       * \param axis_title_x X-axis title
       * \param axis_title_y Y-axis title
       * \param plot_title plot title
       * \param plot_width_norm the normalized plot width, calculated relative to the canvas width
       * \param plot_height_norm the normalized plot height, calculated relative to the canvas height
       * \param plot_offset_horizontal_norm the normalized horizontal offset from the canvas top left corner, calculated relative to the canvas width
       * \param plot_offset_vertical_norm the normalized vertical offset from the canvas top left corner, calculated relative to the canvas height
       */
      Plot(const Glib::ustring &axis_title_x,
           const Glib::ustring &axis_title_y,
           const Glib::ustring &plot_title,
           const double plot_width_norm,
           const double plot_height_norm,
           const double plot_offset_horizontal_norm,
           const double plot_offset_vertical_norm);

    public:
      /** Destructor
       *
       */
      virtual ~Plot();

      /** Remove a single dataset from the plot
       *
       * \param plot_data_index index of the plotdata in the \c plot_data vector
       * \exception Gtk::PLplot::Exception
       */
      virtual void remove_data(unsigned int plot_data_index);

      /** Remove a single dataset from the plot
       *
       * \param plot_data_member pointer to the plotdata in the \c plot_data vector
       * \exception Gtk::PLplot::Exception
       */
      virtual void remove_data(PlotData &plot_data_member);

      /** Get a pointer to a dataset included in the plot
       *
       * Throws an exception when \c data_index is invalid.
       * \param data_index index of the dataset in the \c plot_data vector
       * \return a pointer to the PlotData in the \c plot_data vector.
       * \exception Gtk::PLplot::Exception
       */
      virtual PlotData *get_data(unsigned int data_index = 0) final;

      /** Method to draw the plot with all of its datasets
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param width the width of the Canvas widget
       * \param height the height of the Canvas widget
       */
      //void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, plstream *_pls, int width, int height);
      virtual void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height) = 0;
      /** Set the X-axis title
       *
       * \param title the new X-axis title
       */
      void set_axis_title_x(Glib::ustring title);

      /** Set the Y-axis title
       *
       * \param title the new Y-axis title
       */
      void set_axis_title_y(Glib::ustring title);

      /** Get the X-axis title
       *
       * \return the current X-axis title
       */
      Glib::ustring get_axis_title_x();

      /** Get the Y-axis title
       *
       * \return the current Y-axis title
       */
      Glib::ustring get_axis_title_y();

      /** Set the plot title
       *
       * \param title the new plot title (may be an empty string)
       */
      void set_plot_title(Glib::ustring title);

      /** Get the plot title
       *
       * \return the current plot title
       */
      Glib::ustring get_plot_title();

      /** Make the plot visible on the canvas
       *
       */
      void show();

      /** Hide the plot on the canvas
       *
       */
      void hide();

      /** Returns whether or not the plot is showing on the canvas
       *
       * \return \c true if the plot is visible, \c false if not
       */
      bool is_showing() const;

      /** Get the plot background color
       *
       * \return The currently selected plot background color.
       */
      Gdk::RGBA get_background_color();

      /** Set the background color
       *
       * \param color Set a new plot background color.
       */
      void set_background_color(Gdk::RGBA color);

      /** Get the color to draw the axes, the box and gridlines
       *
       * \return The currently selected axes color.
       */
      Gdk::RGBA get_axes_color();

      /** Set the color to draw the axes, the box and gridlines
       *
       * \param color Set a new axes color.
       */
      void set_axes_color(Gdk::RGBA color);

      /** Get the color to draw the plot and axes titles
       *
       * \return The currently selected plot and axes color.
       */
      Gdk::RGBA get_titles_color();

      /** Set the color to draw the plot and axes titles
       *
       * \param color Set a new titles color.
       */
      void set_titles_color(Gdk::RGBA color);

      /** signal_changed is emitted whenever any of the plot properties or any of the dataset properties has changed.
       *
       * See default handler on_changed()
       */
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }

      /** signal_data_added is emitted whenever a new dataset is added to the plot
       *
       * See default handler on_data_added()
       */
      sigc::signal<void, PlotData *> signal_data_added() {
        return _signal_data_added;
      }

      /** signal_data_removed is emitted whenever a dataset is removed from the plot
      *
      * See default handler on_data_removed()
      */
      sigc::signal<void, PlotData *> signal_data_removed() {
        return _signal_data_removed;
      }

      friend class Canvas;
      friend class RegionSelection;
    };
  }
}



#endif
