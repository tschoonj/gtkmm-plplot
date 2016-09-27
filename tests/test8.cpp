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
#include <gtkmm/switch.h>
#include <gtkmm/paned.h>
#include <gtkmm/aspectframe.h>
#include <gtkmmconfig.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>


namespace Test8 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;

    Gtk::Label show_edges_label;
    Gtk::Switch show_edges_switch;

    Gtk::Label show_labels_label;
    Gtk::Switch show_labels_switch;

    Gtk::Label edge_color_label;
    Gtk::ColorButton edge_color;

    Gtk::Label edge_width_label;
    Glib::RefPtr<Gtk::Adjustment> edge_width_adj;
    Gtk::SpinButton edge_width_spin;

    Gtk::Label nlevels_label;
    Glib::RefPtr<Gtk::Adjustment> nlevels_adj;
    Gtk::SpinButton nlevels_spin;

    Gtk::Label colormap_palette_label;
    Gtk::ComboBoxText colormap_palette_combo;

    Gtk::Label area_fill_pattern_label;
    Gtk::ComboBoxText area_fill_pattern_combo;

    Gtk::Label area_lines_width_label;
    Glib::RefPtr<Gtk::Adjustment> area_lines_width_adj;
    Gtk::SpinButton area_lines_width_spin;

    Gtk::Label colorbar_label;
    Gtk::Switch colorbar_switch;

    Gtk::Paned paned;

    Gtk::AspectFrame aspect_frame;


  public:
    Window() :
      show_edges_label("Show edges"),
      show_labels_label("Show contour labels"),
      edge_color_label("Contour edge color"),
      edge_color(Gdk::RGBA("Red")),
      edge_width_label("Contour edge width"),
      edge_width_adj(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      edge_width_spin(edge_width_adj, 0.1, 1),
      nlevels_label("Number of contour edges"),
      nlevels_adj(Gtk::Adjustment::create(7, 3, 20, 1, 5)),
      nlevels_spin(nlevels_adj, 1, 0),
      colormap_palette_label("Colormap palette"),
      area_fill_pattern_label("Fill pattern"),
      area_lines_width_label("Fill pattern width"),
      area_lines_width_adj(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      area_lines_width_spin(area_lines_width_adj, 0.1, 1),
      colorbar_label("Show colorbar"),
      paned(Gtk::ORIENTATION_VERTICAL),
      aspect_frame(Glib::ustring(), Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 1.5, false)
      {

      Glib::ustring x_title = "X-axis";
      Glib::ustring y_title = "Y-axis";
      Glib::ustring plot_title = "Intensity vs detector position";

      // general window and canvas settings
      set_default_size(720, 720);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(720);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test8");
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
      auto plot = Gtk::manage(new Gtk::PLplot::PlotContourShades(
        *Gtk::manage(new Gtk::PLplot::PlotDataSurface(
          x,
          y,
          z
        )),
        x_title,
        y_title,
        plot_title,
        7,
        Gtk::PLplot::ColormapPalette::BLUE_RED,
        edge_color.get_rgba(),
        1.0
      ));

      canvas.add_plot(*plot);

      plot->set_background_color(Gdk::RGBA("Yellow Green"));

      //now let's set up the grid
      grid.set_column_homogeneous(true);
      grid.set_column_spacing(5);
      grid.set_row_homogeneous(false);
      grid.set_row_spacing(5);

      int row_counter = 0;

      show_edges_label.set_hexpand(true);
      show_edges_label.set_vexpand(false);
      show_edges_label.set_valign(Gtk::ALIGN_CENTER);
      show_edges_label.set_halign(Gtk::ALIGN_END);
      show_edges_switch.set_hexpand(true);
      show_edges_switch.set_vexpand(false);
      show_edges_switch.set_halign(Gtk::ALIGN_START);
      show_edges_switch.set_valign(Gtk::ALIGN_CENTER);
      show_edges_switch.set_active(plot->is_showing_edges());
      show_edges_switch.property_active().signal_changed().connect([this, plot](){
        if (show_edges_switch.get_active()) {
          edge_color.set_sensitive();
          edge_width_spin.set_sensitive();
          show_labels_switch.set_sensitive();
          plot->show_edges();
        }
        else {
          edge_color.set_sensitive(false);
          edge_width_spin.set_sensitive(false);
          show_labels_switch.set_sensitive(false);
          plot->hide_edges();
        }
      });

      grid.attach(show_edges_label, 0, row_counter, 1, 1);
      grid.attach(show_edges_switch, 1, row_counter++, 1, 1);

      // show contour labels
      show_labels_label.set_hexpand(true);
      show_labels_label.set_vexpand(false);
      show_labels_label.set_valign(Gtk::ALIGN_CENTER);
      show_labels_label.set_halign(Gtk::ALIGN_END);
      show_labels_switch.set_hexpand(true);
      show_labels_switch.set_vexpand(false);
      show_labels_switch.set_halign(Gtk::ALIGN_START);
      show_labels_switch.set_valign(Gtk::ALIGN_CENTER);
      show_labels_switch.set_active(plot->is_showing_labels());
      show_labels_switch.property_active().signal_changed().connect([this, plot](){
        if (show_labels_switch.get_active()) {
          plot->show_labels();
        }
        else {
          plot->hide_labels();
        }
      });

      grid.attach(show_labels_label, 0, row_counter, 1, 1);
      grid.attach(show_labels_switch, 1, row_counter++, 1, 1);

      //color button
      edge_color_label.set_hexpand(true);
      edge_color_label.set_vexpand(false);
      edge_color_label.set_valign(Gtk::ALIGN_CENTER);
      edge_color_label.set_halign(Gtk::ALIGN_END);

      edge_color.set_rgba(plot->get_edge_color());
      edge_color.set_use_alpha(true);
      edge_color.set_hexpand(true);
      edge_color.set_vexpand(false);
      edge_color.set_halign(Gtk::ALIGN_START);
      edge_color.set_valign(Gtk::ALIGN_CENTER);
      edge_color.signal_color_set().connect([this, plot](){plot->set_edge_color(edge_color.get_rgba());});

      grid.attach(edge_color_label, 0, row_counter, 1, 1);
      grid.attach(edge_color, 1, row_counter++, 1, 1);

      //the edge width spinbutton
      edge_width_label.set_hexpand(true);
      edge_width_label.set_vexpand(false);
      edge_width_label.set_valign(Gtk::ALIGN_CENTER);
      edge_width_label.set_halign(Gtk::ALIGN_END);

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

      grid.attach(edge_width_label, 0, row_counter, 1, 1);
      grid.attach(edge_width_spin, 1, row_counter++, 1, 1);

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

      grid.attach(nlevels_label, 0, row_counter, 1, 1);
      grid.attach(nlevels_spin, 1, row_counter++, 1, 1);

      // colormap palette
      colormap_palette_label.set_hexpand(true);
      colormap_palette_label.set_vexpand(false);
      colormap_palette_label.set_valign(Gtk::ALIGN_CENTER);
      colormap_palette_label.set_halign(Gtk::ALIGN_END);
      colormap_palette_combo.set_hexpand(true);
      colormap_palette_combo.set_vexpand(false);
      colormap_palette_combo.set_halign(Gtk::ALIGN_START);
      colormap_palette_combo.set_valign(Gtk::ALIGN_CENTER);

      colormap_palette_combo.append("Default");
      colormap_palette_combo.append("Blue → Red");
      colormap_palette_combo.append("Blue → Yellow");
      colormap_palette_combo.append("Gray");
      colormap_palette_combo.append("High frequencies");
      colormap_palette_combo.append("Low frequencies");
      colormap_palette_combo.append("Radar");

      colormap_palette_combo.set_active(plot->get_colormap_palette());
      colormap_palette_combo.signal_changed().connect([this, plot](){
        plot->set_colormap_palette(static_cast<Gtk::PLplot::ColormapPalette>(colormap_palette_combo.get_active_row_number()));
      });

      grid.attach(colormap_palette_label, 0, row_counter, 1, 1);
      grid.attach(colormap_palette_combo, 1, row_counter++, 1, 1);

      //area fill pattern
      area_fill_pattern_label.set_hexpand(true);
      area_fill_pattern_label.set_vexpand(false);
      area_fill_pattern_label.set_valign(Gtk::ALIGN_CENTER);
      area_fill_pattern_label.set_halign(Gtk::ALIGN_END);
      area_fill_pattern_combo.set_hexpand(true);
      area_fill_pattern_combo.set_vexpand(false);
      area_fill_pattern_combo.set_halign(Gtk::ALIGN_START);
      area_fill_pattern_combo.set_valign(Gtk::ALIGN_CENTER);

      area_fill_pattern_combo.append("Solid");
      area_fill_pattern_combo.append("Horizontal lines");
      area_fill_pattern_combo.append("Vertical lines");
      area_fill_pattern_combo.append("Upward lines at 45 degrees");
      area_fill_pattern_combo.append("Downward lines at 45 degrees");
      area_fill_pattern_combo.append("Upward lines at 30 degrees");
      area_fill_pattern_combo.append("Downward lines at 30 degrees");
      area_fill_pattern_combo.append("Horizontal and vertical lines");
      area_fill_pattern_combo.append("Upward and downward lines at 45 degrees");
      area_fill_pattern_combo.set_active(plot->get_area_fill_pattern());
      area_fill_pattern_combo.signal_changed().connect([this, plot](){
        plot->set_area_fill_pattern(static_cast<Gtk::PLplot::AreaFillPattern>(area_fill_pattern_combo.get_active_row_number()));
        if (area_fill_pattern_combo.get_active_row_number() == 0 /* SOLID */) {
          area_lines_width_spin.set_sensitive(false);
        }
        else {
          area_lines_width_spin.set_sensitive();
        }
      });

      grid.attach(area_fill_pattern_label, 0, row_counter, 1, 1);
      grid.attach(area_fill_pattern_combo, 1, row_counter++, 1, 1);

      //the area lines width spinbutton
      area_lines_width_label.set_hexpand(true);
      area_lines_width_label.set_vexpand(false);
      area_lines_width_label.set_valign(Gtk::ALIGN_CENTER);
      area_lines_width_label.set_halign(Gtk::ALIGN_END);

      area_lines_width_spin.set_hexpand(true);
      area_lines_width_spin.set_vexpand(false);
      area_lines_width_spin.set_halign(Gtk::ALIGN_START);
      area_lines_width_spin.set_valign(Gtk::ALIGN_CENTER);
      area_lines_width_spin.set_wrap(true);
      area_lines_width_spin.set_snap_to_ticks(true);
      area_lines_width_spin.set_numeric(true);
      area_lines_width_spin.set_value(plot->get_area_lines_width());
      area_lines_width_spin.signal_value_changed().connect([this, plot](){
        plot->set_area_lines_width(area_lines_width_spin.get_value());
      });

      area_lines_width_spin.set_sensitive(false);

      grid.attach(area_lines_width_label, 0, row_counter, 1, 1);
      grid.attach(area_lines_width_spin, 1, row_counter++, 1, 1);

      //colorbar
      colorbar_label.set_hexpand(true);
      colorbar_label.set_vexpand(false);
      colorbar_label.set_valign(Gtk::ALIGN_CENTER);
      colorbar_label.set_halign(Gtk::ALIGN_END);
      colorbar_switch.set_hexpand(true);
      colorbar_switch.set_vexpand(false);
      colorbar_switch.set_halign(Gtk::ALIGN_START);
      colorbar_switch.set_valign(Gtk::ALIGN_CENTER);
      colorbar_switch.set_active(plot->is_showing_colorbar());
      colorbar_switch.property_active().signal_changed().connect([this, plot](){
        if (colorbar_switch.get_active()) {
          plot->show_colorbar();
        }
        else {
          plot->hide_colorbar();
        }
      });

      grid.attach(colorbar_label, 0, row_counter, 1, 1);
      grid.attach(colorbar_switch, 1, row_counter++, 1, 1);
      paned.add1(grid);

      //add canvas to grid
      aspect_frame.add(canvas);
      paned.add2(aspect_frame);

      //finishing up
      add(paned);
      set_border_width(10);
#if GTKMM_MAJOR_VERSION == 3 && GTKMM_MINOR_VERSION >= 18
      paned.set_wide_handle(true);
#endif
      paned.show_all();

    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test8");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test8");

  Test8::Window window;

	return app->run(window);
}
