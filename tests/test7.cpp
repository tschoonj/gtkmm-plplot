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
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>


namespace Test7 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::Label edge_label;
    Gtk::ColorButton edge_color;
    Glib::RefPtr<Gtk::Adjustment> edge_width_adj;
    Gtk::SpinButton edge_width_spin;

    Gtk::Label nlevels_label;
    Glib::RefPtr<Gtk::Adjustment> nlevels_adj;
    Gtk::SpinButton nlevels_spin;


  public:
    Window() :
      edge_label("Contour edge properties"),
      edge_color(Gdk::RGBA("Red")),
      edge_width_adj(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      edge_width_spin(edge_width_adj, 0.1, 1),
      nlevels_label("Number of contour edges"),
      nlevels_adj(Gtk::Adjustment::create(7, 3, 20, 1, 5)),
      nlevels_spin(nlevels_adj, 1, 0)
      {

      Glib::ustring x_title = "X-axis";
      Glib::ustring y_title = "Y-axis";
      Glib::ustring plot_title = "Intensity vs detector position";

      // general window and canvas settings
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test7");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      //read in our dataset
      std::ifstream fs;
      fs.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

      // 11 x 11 dataset (I know the file layout and the data dimensions already)
      const int nx = 11;
      const int ny = 11;
      std::vector<double> x(nx);
      std::vector<double> y(ny);
#ifdef GTKMM_PLPLOT_BOOST_ENABLED
      boost::multi_array<double, 2> z(boost::extents[nx][ny]);
      std::cout << "Using Boost multi_array!" << std::endl;
#else
      double **z = Gtk::PLplot::calloc_array2d(nx, ny);
#endif

      fs.open(TEST_CSV);
      std::string line;
      std::getline(fs, line);
      gchar **splitted = g_strsplit(line.c_str(), ",", 0);

      //first line contains the x values
      for (int i = 1 ; i < nx + 1 ; i++) {
        x[i-1] = g_ascii_strtod(splitted[i], NULL);
      }

      g_strfreev(splitted);

      for (int i = 0 ; i < ny ; i++) {
        line.clear();
        std::getline(fs, line);
        splitted = g_strsplit(line.c_str(), ",", 0);
        y[i] = g_ascii_strtod(splitted[0], NULL);
        for (int j = 1 ; j < nx + 1 ; j++) {
          z[j-1][i] = g_ascii_strtod(splitted[j], NULL);
        }
        g_strfreev(splitted);
      }

      //construct the plot
      auto plot = Gtk::manage(new Gtk::PLplot::PlotContour(
        *Gtk::manage(new Gtk::PLplot::PlotDataSurface(x, y, z)),
        x_title,
        y_title,
        plot_title,
        7,
        edge_color.get_rgba(),
        1.0
      ));

      canvas.add_plot(*plot);

      //now let's set up the grid
      grid.set_column_homogeneous(false);
      grid.set_column_spacing(5);
      grid.set_row_homogeneous(false);
      grid.set_row_spacing(5);

      edge_label.set_hexpand(true);
      edge_label.set_vexpand(false);
      edge_label.set_valign(Gtk::ALIGN_CENTER);
      edge_label.set_halign(Gtk::ALIGN_END);

      grid.attach(edge_label, 0, 0, 1, 1);

      //color button
      edge_color.set_rgba(plot->get_edge_color());
      edge_color.set_use_alpha(true);
      edge_color.set_hexpand(false);
      edge_color.set_vexpand(false);
      edge_color.set_halign(Gtk::ALIGN_CENTER);
      edge_color.set_valign(Gtk::ALIGN_CENTER);
      edge_color.signal_color_set().connect([this, plot](){plot->set_edge_color(edge_color.get_rgba());});

      grid.attach(edge_color, 1, 0, 1, 1);

      //the spinbutton
      edge_width_spin.set_hexpand(true);
      edge_width_spin.set_vexpand(false);
      edge_width_spin.set_halign(Gtk::ALIGN_START);
      edge_width_spin.set_valign(Gtk::ALIGN_CENTER);
      edge_width_spin.set_wrap(true);
      edge_width_spin.set_snap_to_ticks(true);
      edge_width_spin.set_numeric(true);
      edge_width_spin.set_value(plot->get_edge_width());
      edge_width_spin.signal_value_changed().connect([this, plot](){
        plot->set_edge_width(edge_width_spin.get_value());
      });
      grid.attach(edge_width_spin, 2, 0, 1, 1);

      //add canvas to grid
      grid.attach(canvas, 0, 1, 4, 1);

      //nlevels
      nlevels_label.set_hexpand(true);
      nlevels_label.set_vexpand(false);
      nlevels_label.set_valign(Gtk::ALIGN_CENTER);
      nlevels_label.set_halign(Gtk::ALIGN_END);
      nlevels_spin.set_hexpand(true);
      nlevels_spin.set_vexpand(false);
      nlevels_spin.set_halign(Gtk::ALIGN_START);
      nlevels_spin.set_valign(Gtk::ALIGN_CENTER);
      nlevels_spin.set_wrap(true);
      nlevels_spin.set_snap_to_ticks(true);
      nlevels_spin.set_numeric(true);
      nlevels_spin.set_value(plot->get_nlevels());
      nlevels_spin.signal_value_changed().connect([this, plot](){
        plot->set_nlevels(nlevels_spin.get_value());
      });

      grid.attach(nlevels_label, 0, 2, 2, 1);
      grid.attach(nlevels_spin, 2, 2, 2, 1);

      //finishing up
      add(grid);
      set_border_width(10);
      grid.show_all();

    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test7");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test7");

  Test7::Window window;

	return app->run(window);
}
