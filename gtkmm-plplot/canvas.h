#ifndef GTKMMPLPLOTCANVAS_H
#define GTKMMPLPLOTCANVAS_H

#include <gtkmm/drawingarea.h>
#include "gtkmm-plplot/plot2d.h"
#include <vector>
#include <plstream.h>
#include <gdkmm/rgba.h>

namespace Gtk {
  namespace PLplot {
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
      Canvas();
    protected:
      virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
      virtual bool on_button_press_event(GdkEventButton *event);
      virtual bool on_button_release_event(GdkEventButton *event);
      virtual bool on_motion_notify_event (GdkEventMotion *event);
      virtual void on_changed();
    public:
      Canvas(const Plot2D &plot);
      virtual ~Canvas();
      //void add_plot(const Plot2D &plot);
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }
      Plot2D *get_plot(unsigned int);
      Gdk::RGBA get_background_color();
      void set_background_color(Gdk::RGBA);
    };
  }
}


#endif
