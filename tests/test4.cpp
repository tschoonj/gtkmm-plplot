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
#include <glibmm/stringutils.h>
#include <gtkmm/window.h>
#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <iostream>
#include <glib.h>
#include <cmath>

namespace Test4 {
  class CheckButton : public Gtk::CheckButton {
  private:
    //we are not using any methods specific to Plot2D, so we can safely use Plot here
    Gtk::PLplot::Plot2D *plot;
  public:
    CheckButton(Glib::ustring text) : Gtk::CheckButton(text), plot(nullptr) {}
    void on_toggled() final {
      if (!plot)
        return;
      if (get_active())
        plot->show();
      else
        plot->hide();
    }
    void connect_plot(Gtk::PLplot::Plot2D *_plot) {
      plot = _plot;
      plot->hide_legend();
    }
  };

  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Test4::CheckButton checkbutton1;
    Test4::CheckButton checkbutton2;
    Test4::CheckButton checkbutton3;
    Gtk::Grid coordinates_grid;
    Gtk::Label label_x;
    Gtk::Label label_y;
    Gtk::Entry entry_x;
    Gtk::Entry entry_y;
  public:
    Window() : canvas(), checkbutton1("Plot 1"),
      checkbutton2("Plot 2"), checkbutton3("Plot 3"),
      label_x("X:"),
      label_y("Y:") {
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test4");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      canvas.set_background_color(Gdk::RGBA("Light Gray"));
      grid.set_column_homogeneous(true);
      checkbutton1.set_vexpand(false);
      checkbutton1.set_hexpand(false);
      checkbutton1.set_halign(Gtk::ALIGN_CENTER);
      checkbutton2.set_vexpand(false);
      checkbutton2.set_hexpand(false);
      checkbutton2.set_halign(Gtk::ALIGN_CENTER);
      checkbutton3.set_vexpand(false);
      checkbutton3.set_hexpand(false);
      checkbutton3.set_halign(Gtk::ALIGN_CENTER);

      std::valarray<double> x_va = Gtk::PLplot::indgen_va(1000)/50.0 - 10.0;
      std::valarray<double> y_va1 = sinh(x_va);
      std::valarray<double> y_va2 = cosh(x_va);
      std::valarray<double> y_va3 = tanh(x_va);

      //generate the data, the plot, add them to the canvas and use the return value to pass it to the checkbutton
      Gtk::PLplot::Plot2D *plot1 = Gtk::manage(
        new Gtk::PLplot::Plot2D(
          *Gtk::manage(
            new Gtk::PLplot::PlotData2D(
              x_va,
              y_va1,
              Gdk::RGBA("blue"),
              Gtk::PLplot::LineStyle::CONTINUOUS,
              3.0
            )
          ),
          "X-axis",
          "Y-axis",
          "Hyperbolic sine",
          0.5,
          0.5,
          0.0,
          0.0
        )
      );
      canvas.add_plot(*plot1);
      checkbutton1.connect_plot(plot1);

      Gtk::PLplot::Plot2D *plot2 = Gtk::manage(
        new Gtk::PLplot::Plot2D(
          *Gtk::manage(
            new Gtk::PLplot::PlotData2D(
              x_va,
              y_va2,
              Gdk::RGBA("red"),
              Gtk::PLplot::LineStyle::CONTINUOUS,
              3.0
            )
          ),
          "X-axis",
          "Y-axis",
          "Hyperbolic cosine",
          0.5,
          0.5,
          0.5,
          0.0
        )
      );
      canvas.add_plot(*plot2);
      checkbutton2.connect_plot(plot2);

      Gtk::PLplot::Plot2D *plot3 = Gtk::manage(
        new Gtk::PLplot::Plot2D(
          *Gtk::manage(
            new Gtk::PLplot::PlotData2D(
              x_va,
              y_va3,
              Gdk::RGBA("green"),
              Gtk::PLplot::LineStyle::CONTINUOUS,
              3.0
            )
          ),
          "X-axis",
          "Y-axis",
          "Hyperbolic tangent",
          0.4,
          0.4,
          0.2,
          0.55
        )
      );
      canvas.add_plot(*plot3);
      checkbutton3.connect_plot(plot3);

      checkbutton1.set_active();
      checkbutton2.set_active(false);
      checkbutton3.set_active();

      //let's make the first plot unpannable
      plot1->set_region_pannable(false);

      //let's override the background color of the second plot and hide it to make the surprise complete!!
      plot2->set_background_color(Gdk::RGBA("Yellow Green"));
      plot2->hide();

      //also let's disable the region selection, for no reason whatsoever!
      plot2->set_region_selectable(false);

      //let's override the default region of this plot, this is NOT influenced by set_region_selectable()!!!
      plot2->set_region(-10, 10, -10, 10);

      //we can also change some other colors
      plot2->set_axes_color(Gdk::RGBA("Blue"));
      plot2->set_titles_color(Gdk::RGBA("Purple"));
      plot2->set_box_style(Gtk::PLplot::BOX_TICKS_TICK_LABELS_MAIN_AXES_MAJOR_MINOR_TICK_GRID);

      //lets'give the third plot a nice opaque background color
      plot3->set_background_color(Gdk::RGBA("White"));

      //but disable zooming by scrolling
      plot3->set_region_zoomable(false);

      //and give it an unusual selection box
      plot3->set_region_selection_color(Gdk::RGBA("Purple"));
      plot3->set_region_selection_width(4.0);

      //hook up signal_cursor_motion to the entries
      unsigned int plotnr = 0;
      while (1) {
        try {
          Gtk::PLplot::Plot2D *plot = dynamic_cast<Gtk::PLplot::Plot2D *>(canvas.get_plot(plotnr++));
          plot->signal_cursor_motion().connect([this](double x, double y){
            entry_x.set_text(Glib::Ascii::dtostr(x));
            entry_y.set_text(Glib::Ascii::dtostr(y));
          });
        }
        catch (Gtk::PLplot::Exception &e) {
          break;
        }
      }

      grid.attach(checkbutton1, 0, 0, 1, 1);
      grid.attach(checkbutton2, 1, 0, 1, 1);
      grid.attach(checkbutton3, 2, 0, 1, 1);
      grid.attach(canvas, 0, 1, 3, 1);

      coordinates_grid.set_column_homogeneous(false);
      coordinates_grid.attach(label_x, 0, 0, 1, 1);
      coordinates_grid.attach(entry_x, 1, 0, 1, 1);
      coordinates_grid.attach(label_y, 2, 0, 1, 1);
      coordinates_grid.attach(entry_y, 3, 0, 1, 1);
      label_x.set_vexpand(false);
      label_x.set_hexpand(true);
      label_x.set_halign(Gtk::ALIGN_END);
      label_y.set_vexpand(false);
      label_y.set_hexpand(false);
      label_y.set_halign(Gtk::ALIGN_CENTER);
      entry_x.set_vexpand(false);
      entry_x.set_hexpand(false);
      entry_x.set_halign(Gtk::ALIGN_END);
      entry_y.set_vexpand(false);
      entry_y.set_hexpand(true);
      entry_y.set_halign(Gtk::ALIGN_START);
      coordinates_grid.set_column_spacing(5);
      grid.attach(coordinates_grid, 0, 2, 3, 1);
      grid.set_row_spacing(5);

      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}

int main(int argc, char *argv[]) {
  Glib::set_application_name("gtkmm-plplot-test4");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test4");
  Test4::Window window;

  return app->run(window);
}
