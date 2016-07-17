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

#include "gtkmm-plplot.h"
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/button.h>
#include <iostream>
#include <glib.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace Test3 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::Button add_plot;
    Gtk::Button remove_plot;
    Gtk::Button show_plot;
    Gtk::Button hide_plot;
    Gtk::PLplot::Plot2D *plot;

    void add_plot_clicked() {
      add_plot.set_sensitive(false);

      std::valarray<double> x_va(1000), y_va(1000);
      for (unsigned int i = 0 ; i < 1000 ; i++) {
        x_va[i] = 4*M_PI*i/999;
      }
      y_va = sin(x_va);
      auto plot_data = Gtk::manage(new Gtk::PLplot::PlotData2D(x_va, y_va, Gdk::RGBA("blue"), Gtk::PLplot::LineStyle::LONG_DASH_LONG_GAP, 5.0));
      plot = Gtk::manage(new Gtk::PLplot::Plot2D(*plot_data));
      canvas.add_plot(*plot);
      plot->hide_legend();

      remove_plot.set_sensitive(true);
      show_plot.set_sensitive(false);
      hide_plot.set_sensitive(true);
    }
  public:
    Window() : canvas(), add_plot("Add"), remove_plot("Remove"),
      show_plot("Show"), hide_plot("Hide") {
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test3");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      add_plot.set_hexpand(true);
      add_plot.set_vexpand(false);
      add_plot.set_halign(Gtk::ALIGN_END);
      remove_plot.set_hexpand(false);
      remove_plot.set_vexpand(false);
      show_plot.set_hexpand(false);
      show_plot.set_vexpand(false);
      hide_plot.set_hexpand(true);
      hide_plot.set_vexpand(false);
      hide_plot.set_halign(Gtk::ALIGN_START);

      remove_plot.set_sensitive(false);
      show_plot.set_sensitive(false);
      hide_plot.set_sensitive(false);

      add_plot.signal_clicked().connect(sigc::mem_fun(*this, &Window::add_plot_clicked));
      remove_plot.signal_clicked().connect([this](){
        canvas.remove_plot(*plot);
        add_plot.set_sensitive(true);
        remove_plot.set_sensitive(false);
        show_plot.set_sensitive(false);
        hide_plot.set_sensitive(false);
      });
      show_plot.signal_clicked().connect([this](){
        plot->show();
        show_plot.set_sensitive(false);
        hide_plot.set_sensitive(true);
      });
      hide_plot.signal_clicked().connect([this](){
        plot->hide();
        show_plot.set_sensitive(true);
        hide_plot.set_sensitive(false);
      });

      grid.attach(add_plot, 0, 0, 1, 1);
      grid.attach(remove_plot, 1, 0, 1, 1);
      grid.attach(show_plot, 2, 0, 1, 1);
      grid.attach(hide_plot, 3, 0, 1, 1);
      grid.attach(canvas, 0, 1, 4, 1);
      grid.set_row_spacing(5);
      grid.set_column_spacing(5);
      grid.set_column_homogeneous(false);


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
