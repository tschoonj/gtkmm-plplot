#ifndef GTKMMPLPLOTCANVAS_H
#define GTKMMPLPLOTCANVAS_H

#include <gtkmm/drawingarea.h>
#include <gtkmm-plplot/plot2d.h>
#include <vector>
#include <plstream.h>
#include <gdkmm/rgba.h>

/** \mainpage
 *  \section introduction Introduction
 *  Scientific plotting and GTK+ have never been good friends.
 *  Over the years there have been several attempts like [GtkExtra](http://gtkextra.sourceforge.net/cms/),
 *  [GtkDataBox](http://sourceforge.net/projects/gtkdatabox/), [GoatPlot](https://github.com/drahnr/goatplot),
 *  [GtkGraph](http://gtkgraph.sourceforge.net), [Plotmm](http://plotmm.sourceforge.net/index.html)...
 *  All of these packages unfortunately appear to share a lack of development and
 *  maintenance, which is most clearly shown by the fact that they do not support
 *  the latest release series GTK+3!
 *
 *  Now it is clear that writing a plotting library for GTK+ is a huge and cumbersome
 *  task, and the lack of support for GTK+3 can be most certainly explained by the transition
 *  of the GTK+ drawing routines to use Cairo instead of GDK, which essentially required developers
 *  to rewrite all their code from scratch.
 *
 *  Due to the nature of my scientific work, I found myself in need of scientific plotting
 *  widget support in GTK+ and for some time I have been able to do what I needed with
 *  GtkExtra. Although certainly not perfect, it comes with a wide range of plotting routines,
 *  but as I said before, it only supports GTK+2. In order to address this shortcoming, I have joined
 *  the development team some time ago but I found myself lacking the knowledge of the GTK+ and Cairo
 *  internals to produce a working version that supports GTK+3.
 *
 *  Despite the lack of attention by the other developers of GtkExtra, I found myself still in need
 *  of a scientific plotting widget for GTK+3, so I decided to find a different solution.
 *  Since I certainly do not have the skill (or time for that matter) to write an entire class of GTK+ widgets for
 *  scientific plotting, I had to find a different approach. The idea that eventually culminated
 *  into this project was the following: first I rediscovered the [PLplot](http://plplot.sourceforge.net) library, which
 *  I had used many years ago to produce simple plots from within command-line programs. I went through their (excellent)
 *  documentation and discovered that they had introduced support for a Cairo plotting driver, along with an `extcairo` device
 *  that allowed PLplot to draw onto a Cairo context. An example is included in the source where they demonstrate
 *  this feature by plotting onto a Cairo context connected to a PostScript surface. Based on this example I wrote
 *  my own Gtkmm widget that inherits from `Gtkmm::DrawingArea` with a custom `on_draw` method as explained in the
 *  [GTK+](https://developer.gnome.org/gtk3/stable/GtkDrawingArea.html) and [Gtkmm](https://developer.gnome.org/gtkmm/stable/classGtk_1_1DrawingArea.html) manuals.
 *  With surprisingly little work, I managed to produce a nice looking plot.
 *
 *  I followed up with a blogpost [about this](http://tschoonj.github.io/blog/2015/07/05/embedding-plplot-in-gtk-drawingareas/) and
 *  a couple of days later I decided to start this project after it was clear that it could be useful to myself and my colleages.
 *
 *  I think that the approach followed by Gtkmm-PLplot has the following advantages:
 *  - Depends on a mature toolkit (PLplot) to the bulk of the work. No need for me to write the low-level plotting routines.
 *  - Relies on Cairo for rendering, which looks like it will remain the GTK+ workhorse for a long time to come. This means low maintenance.
 *  - PLplot itself has a lot of useful features, but suffers (IMHO) from its obscure, acronym based hard to remember API. Gtkmm-PLplot hides this API
 *    and offers an interface that should be familiar to anyone with GTK+/Gtkmm experience.
 *
 *  There may be people wondering why I didn't write this in plain C using GTK+ instead of Gtkmm. I acknowledge that this project would
 *  benefit from this as it makes the API easily available in other languages through introspection. However, writing GTK+ widgets is a real pain
 *  (remember: nobody is maintaining those other plotting widgets...), while writing in C++ produces nice, streamlined and compact code.
 *  If anyone would be interested in producing native C bindings on the same idea, I will gladly help out if I can...
 *
 *  That's it! Enjoy using Gtkmm-PLplot and let me know if you run into bugs...
 *  \author Tom Schoonjans
 *  \section installation_instructions Installation instructions
 *
 *  \section usage Usage
 *
 *  \section examples Examples
 */


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
