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

#ifndef GTKMMPLPLOTCANVAS_H
#define GTKMMPLPLOTCANVAS_H

#include <gtkmm/drawingarea.h>
#include <gtkmm-plplot/plot2d.h>
#include <vector>
#include <plstream.h>
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
     *  Plotting widget
     *  Inheriting from `Gtk::DrawingArea`, this is the by far the most important
     *  widget in PLplot.
     */
    class Canvas : public Gtk::DrawingArea {
    private:
      std::vector<Plot2D *> plots;
      sigc::signal<void> _signal_changed;
      double start_event[2];
      double start_cairo[2];
      double end_event[2];
      double end_cairo[2];
      bool selecting;
      unsigned int selected_plot;
      double x_pl_range[2];
      double y_pl_range[2];
      double x_cr_range[2];
      double y_cr_range[2];
      Gdk::RGBA background_color;
      Canvas(const Canvas &) = delete;
      Canvas &operator=(const Canvas &) = delete;
    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
      virtual bool on_button_press_event(GdkEventButton *event);
      virtual bool on_button_release_event(GdkEventButton *event);
      virtual bool on_motion_notify_event (GdkEventMotion *event);
      virtual void on_changed();
    public:
      Canvas(Gdk::RGBA background_color = Gdk::RGBA("White"));
      Canvas(const Plot2D &plot, Gdk::RGBA background_color = Gdk::RGBA("White"));
      virtual ~Canvas();
      Plot2D *add_plot(const Plot2D &plot);
      void remove_plot(unsigned int plot_index);
      void remove_plot(Plot2D *plot);
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }
      Plot2D *get_plot(unsigned int plot_index);
      Gdk::RGBA get_background_color();
      void set_background_color(Gdk::RGBA);
    };
  }
}


#endif
