/*
Copyright (C) 2018 Rémi BERTHO

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
#include <gtkmm/box.h>
#include <gtkmm/radiobutton.h>
#include <iostream>
#include <glib.h>
#include <glibmm/date.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace Test13 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::PLplot::Plot2D *plot = nullptr;
    Gtk::Box box;
    Gtk::RadioButton button_1;
    Gtk::RadioButton button_2;
    Gtk::RadioButton button_3;

    void add_plot_1() {
      if (plot != nullptr){
		canvas.remove_plot(*plot);
		plot = nullptr;
	  }

      // Data points every 10 minutes for 1 day
	  int npts = 73;
      std::valarray<double> x_va(npts), y_va(npts);
      double xmin = 0;
      double xmax = 60.0 * 60.0 * 24.0; // Number of seconds in a day
      double ymin = 10.0;
      double ymax = 20.0;

      for (unsigned int i = 0 ; i < npts ; i++) {
        x_va[i] = xmax * ( (double) i / (double) npts );
        y_va[i] = 15.0 - 5.0 * cos( 2 * M_PI * ( (double) i / (double) npts ) );
      }
      auto plot_data = Gtk::manage(new Gtk::PLplot::PlotData2D(x_va, y_va, Gdk::RGBA("blue"), Gtk::PLplot::LineStyle::LONG_DASH_LONG_GAP, 5.0));
      plot = Gtk::manage(new Gtk::PLplot::Plot2D(*plot_data));
      plot->set_axis_time_format("%H:%M");
      canvas.add_plot(*plot);
      plot->hide_legend();
    }

    void add_plot_2() {
      if (plot != nullptr){
		canvas.remove_plot(*plot);
		plot = nullptr;
	  }

	  int npts = 365;
      std::valarray<double> y_va(npts);
      std::valarray<double> x_va(npts);
      Glib::DateTime first_date = Glib::DateTime::create_local(2010, 1,1,0,0,0);
      for (unsigned int i = 0 ; i < npts ; i++) {
        x_va[i] = first_date.add_days(i).to_unix();
        y_va[i] = 15.0 - 5.0 * cos( 2 * M_PI * ( (double) i / (double) (npts/12) ) );
      }
      auto plot_data = Gtk::manage(new Gtk::PLplot::PlotData2D(x_va, y_va));
      plot = Gtk::manage(new Gtk::PLplot::Plot2D(*plot_data));
      plot->set_axis_time_format("%D");
      canvas.add_plot(*plot);
      plot->hide_legend();
    }

    void add_plot_3() {
      if (plot != nullptr){
		canvas.remove_plot(*plot);
		plot = nullptr;
	  }

	  int npts = 2018;
      std::valarray<double> y_va(npts);
      std::valarray<double> x_va(npts);
      for (unsigned int i = 0 ; i < npts ; i++) {
        x_va[i] = i;
        y_va[i] = 15.0 - 5.0 * cos( 2 * M_PI * ( (double) i / (double) (npts/10) ) );
      }
      auto plot_data = Gtk::manage(new Gtk::PLplot::PlotData2D(x_va, y_va));
      plot = Gtk::manage(new Gtk::PLplot::Plot2D(*plot_data));
      plot->set_axis_time_format("%Y");
      plot->config_time(365.25, Glib::DateTime::create_local(1, 1,1,0,0,0));
      canvas.add_plot(*plot);
      plot->hide_legend();
    }
  public:
    Window() : canvas() {
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test13");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      box.set_homogeneous ();
      box.set_halign(Gtk::ALIGN_CENTER);
      box.get_style_context()->add_class("linked");

      button_1.set_label("Test 1");
      button_1.set_mode(false);
      button_1.signal_clicked().connect(sigc::mem_fun(*this, &Window::add_plot_1));

      button_2.set_label("Test 2");
      button_2.set_mode(false);
      button_2.join_group(button_1);
      button_2.signal_clicked().connect(sigc::mem_fun(*this, &Window::add_plot_2));

      button_3.set_label("Test 3");
      button_3.set_mode(false);
      button_3.join_group(button_1);
      button_3.signal_clicked().connect(sigc::mem_fun(*this, &Window::add_plot_3));

      box.pack_start(button_1);
      box.pack_start(button_2);
      box.pack_start(button_3);
      grid.attach(box, 0, 0, 1, 1);

      add_plot_1();

      grid.attach(canvas, 0, 1, 1, 1);
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
  Glib::set_application_name("gtkmm-plplot-test13");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test13");
  Test13::Window window;

  return app->run(window);
}
