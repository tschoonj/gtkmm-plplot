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

#include <gtkmm-plplot.h>
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


namespace Test6 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
  public:
    Window() {
      // general window and canvas settings
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test6");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      //add a plot to canvas
      //a cardioid
      std::valarray<double> theta1 = Gtk::PLplot::indgen_va(1000)*2.0*M_PI/999.0;
      std::valarray<double> r1 = cos(theta1) + double(1.0);

      auto plot = Gtk::manage(new Gtk::PLplot::PlotPolar(
        *Gtk::manage(new Gtk::PLplot::PlotDataPolar(r1, theta1, Gdk::RGBA("red"), Gtk::PLplot::LineStyle::CONTINUOUS, 2.0)),
        "",
        "",
        "Polar coordinate system"
      ));
      plot->get_data()->set_name("Cardiod");

      //Fermat's spiral
      std::valarray<double> theta2 = Gtk::PLplot::indgen_va(2000)*4.0*M_PI/1999.0;
      std::valarray<double> r2 = sqrt(theta2) * 2.0 / sqrt(M_PI) * 1.1;

      auto data2 = Gtk::manage(new Gtk::PLplot::PlotDataPolar(r2, theta2, Gdk::RGBA("green"), Gtk::PLplot::LineStyle::CONTINUOUS, 2.0));
      data2->set_name("Fermat's spiral");

      canvas.add_plot(*plot);
      plot->add_data(*data2);

      grid.attach(canvas, 0, 0, 1, 1);
      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test6");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test6");

  Test6::Window window;

	return app->run(window);
}
