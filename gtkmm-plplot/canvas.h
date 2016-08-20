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

#ifndef GTKMMPLPLOTCANVAS_H
#define GTKMMPLPLOTCANVAS_H

#include <gtkmm/drawingarea.h>
#include <gtkmm-plplot/plot.h>
#include <vector>
#include <gdkmm/rgba.h>


/** \namespace Gtk::PLplot
 *  \brief The namespace provided by Gtkmm-PLplot
 *
 *  Gtkmm-PLplot provides only one namespace, which is used
 *  to contain all of its classes. As it name shows, the namespace
 *  name was chosen on purpose to make it clear that it acts as
 *  an extension of the Gtkmm provided `Gtk` namespace.
 *  When including Gtkmm-PLplot in your code, you will have to
 *  prefix the class names with our namespace, or alternatively,
 *  include `using namespace Gtk::PLplot;` in your file.
 */

namespace Gtk {
  namespace PLplot {

    /** \class Canvas canvas.h <gtkmm-plplot/canvas.h>
     *  \brief Principal plotting widget
     *
     *  Inheriting from `Gtk::DrawingArea`, this is by far the most important
     *  widget in PLplot, as it will contain the plots and the datasets.
     *  Depending on which constructor is chosen, either an empty canvas will be produced, or one containing a single plot.
     *  It is possible however, to add any number of plots on the canvas. The positions and dimensions of these plots are properties of the plots, that have to be provided
     *  when invoking their constructors. This is demonstrated in \ref example3 and \ref example4.
     *  \ref example1 shows how to write a canvas and its contents to a file as well as to send a canvas to a printer
     *  using the draw_plot method.
     */
    class Canvas : public Gtk::DrawingArea {
    private:
      std::vector<Plot *> plots; ///< A vector containing pointers to the plots
      sigc::signal<void> _signal_changed; ///< signal that will be emitted whenever one of the parameters of the canvas or the plots in contains changes
      double start_event[2]; ///< widget coordinates of the position where a \c button_press_event was generated
      double start_cairo[2]; ///< cairo coordinates of the position where a \c button_press_event was generated
      double end_event[2]; ///< widget coordinates of the position where a \c button_release_event was generated
      double end_cairo[2]; ///< cairo coordinates of the position where a \c button_release_event was generated
      bool selecting; ///< set to true if the user is currently dragging a selection box
      bool left_mouse_button_clicked; ///< set to true if the user is currently pressing the left mouse button
      bool shift_pressed; ///< set to true if the shift button is currently pressed in
      Plot *selected_plot; ///< \c pointer to the currently selected plot
      Plot *inside_plot; ///< \c pointer to the plot that currently contains the mouse cursor. It will be set to nullptr when it is not above a plot.
      double inside_plot_current_coords[2]; ///< coords of the current cursor position within inside_plot, updated in on_motion_notify_event();
      Gdk::RGBA background_color; ///< the currently used background color of the canvas (default = opaque White)
      Canvas(const Canvas &) = delete; ///< no copy constructor
      Canvas &operator=(const Canvas &) = delete; ///< no move assignment operator
    protected:
      /** This is a default handler for signal_draw().
       *
       * When deriving from Canvas and if overriding this method, you may still want to call it to ensure proper drawing of the plots provided by Gtkmm-PLplot
       * \param cr The cairo context to draw to.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       */
      virtual bool on_draw(const ::Cairo::RefPtr<::Cairo::Context>& cr) override;

      /** This is a default handler for signal_button_press_event().
       *
       * This method tracks the button press event that may lead to a selection box being dragged, as well as the double press event that will bring a previously zoomed-in range back to its default view.
       * When deriving from Canvas and if overriding this method, you may still want to call it to ensure that the selection box will still be drawn.
       * \param event The GdkEventButton which triggered this signal.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       */
      virtual bool on_button_press_event(GdkEventButton *event) override;

      /** This is a default handler for signal_button_release_event().
       *
       * This method tracks the button release event that leads to a selection box being completed and thus deleted, which is then followed by signal_select_region being emitted by the plot that had the selection box.
       * When deriving from Canvas and if overriding this method, you may still want to call it to ensure that the selection box will still be drawn.
       * \param event The GdkEventButton which triggered this signal.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       */
      virtual bool on_button_release_event(GdkEventButton *event) override;

      /** This is a default handler for signal_motion_notify_event().
       *
       * This method tracks the mouse movement event that leads to a selection box being drawn within the plot box.
       * When deriving from Canvas and if overriding this method, you may still want to call it to ensure that the selection box will still be drawn.
       * \param event The GdkEventMotion which triggered this signal.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       */
      virtual bool on_motion_notify_event(GdkEventMotion *event) override;

      /** This is a default handler for signal_scroll_event().
       *
       * This method tracks the scroll movement that may be used to trigger a zoom-in or zoom-out event on a particular plot.
       * When deriving from Canvas and if overriding this method, you may still want to call it to ensure that the selection box will still be drawn.
       * \param event The GdkEventScroll which triggered this signal.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       * \since 2.2
       */
      virtual bool on_scroll_event(GdkEventScroll *event) override;

      /** This is a default handler for signal_key_press_event().
       *
       * This method gets called whenever a key is pressed.
       * In this class, the method will only be useful whenever the SHIFT key is pressed, as this will lead to the plot being moved around within its box.
       * \param event The GdkEventKey which triggered this signal.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       * \since 2.2
       */
      virtual bool on_key_press_event(GdkEventKey *event) override;

      /** This is a default handler for signal_key_release_event().
       *
       * This method gets called whenever a key is released.
       * In this class, the method will only be useful whenever the SHIFT key is released, as this will lead to the plot being moved around within its box.
       * \param event The GdkEventKey which triggered this signal.
       * \return \c true to stop other handlers from being invoked for the event. \c false to propagate the event further
       * \since 2.2
       */
      virtual bool on_key_release_event(GdkEventKey *event) override;

      /** This is a default handler for signal_changed().
       *
       * This signal is emitted whenever any of the canvas properties changes, or any of the properties of the plots contained therein.
       * This default handler does one thing only: asking for a redraw of the complete canvas.
       */
      virtual void on_changed();
    public:
      /** Canvas constructor
       *
       * This is the default class constructor, which will produce only a widget with the \c background_color specified.
       * Add plots using add_plot.
       * \param background_color The color of the background for the canvas.
       * \exception Gtk::PLplot::Exception
       */
      Canvas(Gdk::RGBA background_color = Gdk::RGBA("White"));

      /** Canvas constructor
       *
       * This is a Canvas constructor that takes one plot as argument, resulting in a box with data when shown.
       * More plots may be added using add_plot.
       * \param plot The initial plot that will be added to the canvas.
       * \param background_color The color of the background for the canvas.
       */
      Canvas(Plot &plot, Gdk::RGBA background_color = Gdk::RGBA("White"));

      /** Canvas destructor
       *
       */
      virtual ~Canvas();

      /** Draw onto a given cairo context
       *
       * Enables drawing the canvas and its plot(s) onto a particular cairo context,
       * whose surface is not necessarily connected to a widget.
       * Use this method if you want to write to PNG, EPS, PDF etc. files or if
       * you would like to send this to a print job
       */
      void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);

      /** Add a single plot to the canvas
       *
       * \param plot Plot to be added to the canvas
       */
      void add_plot(Plot &plot);

      /** Remove a single plot from the canvas
       *
       * \param plot_index index of the plot in the \c plots vector
       * \exception Gtk::PLplot::Exception
       */
      void remove_plot(unsigned int plot_index);

      /** Remove a single plot from the canvas
       *
       * \param plot pointer to the plot in the \c plots vector
       * \exception Gtk::PLplot::Exception
       */
      void remove_plot(Plot &plot);

      /** signal_changed is emitted whenever any of the canvas properties or any of the plot properties has changed.
       *
       * See default handler on_changed()
       * \exception Gtk::PLplot::Exception
       * \return signal
       */
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }

      /** Get a pointer to a plot included in the canvas
       *
       * \param plot_index index of the plot in the \c plots vector
       * \exception Gtk::PLplot::Exception
       * \return a pointer to the Plot in the \c plots vector.
       */
      Plot *get_plot(unsigned int plot_index);

      /** Get the background color
       *
       * \return The currently selected background color.
       */
      Gdk::RGBA get_background_color();

      /** Set the background color
       *
       * \param color Set a new background color.
       */
      void set_background_color(Gdk::RGBA color);
    };
  }
}


#endif
