/*
Copyright (C) 2016 Tom Schoonjans

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
#include <gtkmm/aspectframe.h>
#include <glibmm/miscutils.h>
#include <gtkmm/window.h>
#include <valarray>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace Test11 {
  class Window : public Gtk::Window {
  private:
    std::valarray<double> x;
    std::valarray<double> xmin;
    std::valarray<double> xmax;
    std::valarray<double> y;
    std::valarray<double> ymin;
    std::valarray<double> ymax;
    Gtk::PLplot::Canvas canvas;
    Gtk::PLplot::PlotData2D plot_data_2d;
    Gtk::PLplot::PlotData2DErrorX plot_data_2d_error_x;
    Gtk::PLplot::PlotData2DErrorY plot_data_2d_error_y;
    Gtk::PLplot::PlotData2DErrorXY plot_data_2d_error_xy;
    Gtk::PLplot::Plot2D plot_2d;
    Gtk::PLplot::Plot2D plot_2d_error_x;
    Gtk::PLplot::Plot2D plot_2d_error_y;
    Gtk::PLplot::Plot2D plot_2d_error_xy;
  public:
    Window() :
        x(Gtk::PLplot::indgen_va(40) * 2.0 * M_PI / 39.0),
        xmin(x - 2.0 * M_PI / 80.0),
        xmax(x + 2.0 * M_PI / 80.0),
        y(sin(x)),
        ymin(y - 0.1 - Gtk::PLplot::indgen_va(40) / 39.0),
        ymax(y + 0.1 + Gtk::PLplot::indgen_va(40) / 39.0),
        canvas(),
        plot_data_2d(x, y),
        plot_data_2d_error_x(x, y, xmin, xmax),
        plot_data_2d_error_y(x, y, ymin, ymax),
        plot_data_2d_error_xy(x, y, xmin, xmax, ymin, ymax),
        plot_2d(plot_data_2d, "X-axis", "Y-axis", "No errorbars", 0.5, 0.5, 0.0, 0.0),
        plot_2d_error_x(plot_data_2d_error_x, "X-axis", "Y-axis", "X-data errorbars", 0.5, 0.5, 0.5, 0.0),
        plot_2d_error_y(plot_data_2d_error_y, "X-axis", "Y-axis", "Y-data errorbars", 0.5, 0.5, 0.0, 0.5),
        plot_2d_error_xy(plot_data_2d_error_xy, "X-axis", "Y-axis", "X- and Y-data errorbars", 0.5, 0.5, 0.5, 0.5)
    {
      // no need to show legends
      plot_2d.hide_legend();
      plot_2d_error_x.hide_legend();
      plot_2d_error_y.hide_legend();
      plot_2d_error_xy.hide_legend();

      // a some color
      plot_data_2d_error_x.set_error_x_color(Gdk::RGBA("blue"));
      plot_data_2d_error_xy.set_error_x_color(Gdk::RGBA("blue"));
      plot_data_2d_error_y.set_error_y_color(Gdk::RGBA("green"));
      plot_data_2d_error_xy.set_error_y_color(Gdk::RGBA("green"));

      // add all plots to the canvas
      canvas.add_plot(plot_2d);
      canvas.add_plot(plot_2d_error_x);
      canvas.add_plot(plot_2d_error_y);
      canvas.add_plot(plot_2d_error_xy);

      const int width = 1024, height = 825;
      set_default_size(width, height);
      set_title("Gtkmm-PLplot test11");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      canvas.set_focusable(true);
      Gtk::AspectFrame geometry(Gtk::Align::CENTER, Gtk::Align::CENTER, float(width)/float(height), false);
      geometry.set_child(canvas);
      
      geometry.set_margin(10);
      set_child(geometry);
    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test11");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("eu.tomschoonjans.gtkmm-plplot-test11");

  return app->make_window_and_run<Test11::Window>(argc, argv);
}
