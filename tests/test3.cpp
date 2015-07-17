#include "gtkmm-plplot/canvas.h"
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <iostream>
#include <glib.h>

namespace Test3 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::Button add_data;
    sigc::connection handler;
    void add_data_clicked() {
      handler.block();
      std::valarray<PLFLT> x_va(1000), y_va(1000);
      for (unsigned int i = 0 ; i < 1000 ; i++) {
        x_va[i] = 4*M_PI*i/999;
      }
      y_va = sin(x_va);
      canvas.add_plot(Gtk::PLplot::Plot2D(Gtk::PLplot::Plot2DData(x_va, y_va, Gdk::RGBA("blue"), Gtk::PLplot::LineStyle::LONG_DASH_LONG_GAP, 5.0)));
    }
  public:
    Window() : canvas(), add_data("Add data") {
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test3");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      add_data.set_hexpand(false);
      add_data.set_vexpand(false);
      add_data.set_halign(Gtk::ALIGN_CENTER);
      add_data.set_valign(Gtk::ALIGN_CENTER);

      handler = add_data.signal_clicked().connect(sigc::mem_fun(*this, &Window::add_data_clicked));

      grid.attach(add_data, 0, 0, 1, 1);
      grid.attach(canvas, 0, 1, 1, 1);
      grid.set_row_spacing(5);


      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}

int main(int argc, char *argv[]) {
  Glib::set_application_name("gtkmm-plplot-test3");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test3");
  Test3::Window window;

  return app->run(window);
}
