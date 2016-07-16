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
#include <gtkmm/window.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glib.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <gtkmm/switch.h>
#include <gtkmm/comboboxtext.h>
#include <vector>

namespace Test2 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::Label x_log_label;
    Gtk::Label x_box_label;
    Gtk::Label y_log_label;
    Gtk::Label y_box_label;
    Gtk::Label box_label;
    Gtk::Switch x_log_switch;
    Gtk::Switch y_log_switch;
    Gtk::ComboBoxText box_combo;
    Gtk::Label legend_label;
    Gtk::Switch legend_switch;
    Gtk::Label corner_label;
    Gtk::ComboBoxText corner_combo;
    Gtk::PLplot::PlotData2D plot_data1;
  public:
    Window(std::vector<double> &x,
           std::vector<double> &y1,
           std::vector<double> &y2,
           std::vector<double> &y3,
           std::vector<double> &y4,
           Glib::ustring x_title,
           Glib::ustring y_title,
           Glib::ustring plot_title) :
           canvas(),
           x_log_label("X-axis logarithmic"),
           y_log_label("Y-axis logarithmic"),
           box_label("Box options"),
           legend_label("Show legend"),
           corner_label("Legend corner position"),
           plot_data1(x, y1, Gdk::RGBA("red")) {

        // The first dataset is a class member variable,
        // meaning that it will be destroyed along with
        // the Window when it is closed. It is is constructed in the
        // constructor initialization list.
        // Next, construct a plot with this dataset (mandatory).
        // Here I am using a managed pointer, but a class member variable would
        // in this case have been equally good. The fact that it is managed
        // means that we will not have to delete it when the canvas that will
        // own it gets destroyed.
        Gtk::PLplot::Plot2D *plot = Gtk::manage(new Gtk::PLplot::Plot2D(plot_data1, x_title, y_title, plot_title));

        // Additional datasets may be added using add_data
        // Again, I will be using managed pointers here, but class member
        // variables would have worked out just as well in this case.
        Gtk::PLplot::PlotData2D *plot_data2 = Gtk::manage(new Gtk::PLplot::PlotData2D(x, y2, Gdk::RGBA("blue")));
        plot->add_data(*plot_data2);
        plot->add_data(*Gtk::manage(new Gtk::PLplot::PlotData2D(x, y3, Gdk::RGBA("Blue Violet"))));
        Gtk::PLplot::PlotData2D *plot_data3 = dynamic_cast<Gtk::PLplot::PlotData2D *>(plot->get_data(2));
        // This next line creates and adds a dataset. However, the memory will
        // not be managed by Gtkmm-PLplot!!!
        // DO NOT DO THIS as it creates a memory leak unless handled for example
        // in on_delete_event or something like that...
        // Alternatively, plot_data3 could be turned into a class member variable,
        // and could then be deleted in the destructor...
        // So in this example we are naughty and create intentionally a memory leak...
        plot->add_data(*new Gtk::PLplot::PlotData2D(x, y4, Gdk::RGBA("Green")));
        Gtk::PLplot::PlotData2D *plot_data4 = dynamic_cast<Gtk::PLplot::PlotData2D *>(plot->get_data(3));

        canvas.add_plot(*plot);

        plot_data1.set_name("1 interaction");
        plot_data2->set_name("2 interactions");
        plot_data3->set_name("3 interactions");
        plot_data4->set_name("4 interactions");

        plot->set_axis_logarithmic_y();

        set_default_size(720, 580);
        Gdk::Geometry geometry;
        geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
        set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
        set_title("Gtkmm-PLplot test2");
        canvas.set_hexpand(true);
        canvas.set_vexpand(true);

        x_log_switch.set_active(plot->get_axis_logarithmic_x());
        y_log_switch.set_active(plot->get_axis_logarithmic_y());
        x_log_switch.property_active().signal_changed().connect([this, plot](){
          if (x_log_switch.get_active()) {
            plot->set_axis_logarithmic_x(true);
          }
          else {
            plot->set_axis_logarithmic_x(false);
          }
        });
        legend_switch.set_active(plot->is_showing_legend());
        legend_switch.property_active().signal_changed().connect([this, plot](){
          if (legend_switch.get_active()) {
            plot->show_legend();
          }
          else {
            plot->hide_legend();
          }
        });

        box_combo.append("No box, no ticks, no labels, no axes");
        box_combo.append("Box only");
        box_combo.append("Box, ticks and tick labels");
        box_combo.append("Box, ticks, tick labels, and main axes");
        box_combo.append("Box, ticks, tick labels, main axes and major tick grid");
        box_combo.append("Box, ticks, tick labels, main axes and major and minor tick grid");
        box_combo.signal_changed().connect(
          [this, plot](){
            plot->set_box_style(static_cast<Gtk::PLplot::BoxStyle>(
              box_combo.get_active_row_number() - 2
            ));
          }
        );
        box_combo.set_active(plot->get_box_style() + 2);

        corner_combo.append("Top-right");
        corner_combo.append("Bottom-right");
        corner_combo.append("Top-left");
        corner_combo.append("Bottom-left");
        corner_combo.signal_changed().connect(
          [this, plot](){
            plot->set_legend_corner_position(static_cast<Gtk::PLplot::LegendCornerPosition>(
              corner_combo.get_active_row_number()
            ));
          }
        );
        corner_combo.set_active(plot->get_legend_corner_position());

        y_log_switch.property_active().signal_changed().connect([this, plot](){
          if (y_log_switch.get_active()) {
            plot->set_axis_logarithmic_y(true);
          }
          else {
            plot->set_axis_logarithmic_y(false);
          }
        });
        x_log_label.set_hexpand(false);
        y_log_label.set_hexpand(false);
        box_label.set_hexpand(false);
        legend_label.set_hexpand(false);
        corner_label.set_hexpand(false);
        x_log_switch.set_hexpand(false);
        y_log_switch.set_hexpand(false);
        box_combo.set_hexpand(false);
        legend_switch.set_hexpand(false);
        corner_combo.set_hexpand(false);
        x_log_label.set_halign(Gtk::ALIGN_END);
        y_log_label.set_halign(Gtk::ALIGN_END);
        box_label.set_halign(Gtk::ALIGN_END);
        corner_label.set_halign(Gtk::ALIGN_END);
        legend_label.set_halign(Gtk::ALIGN_END);
        x_log_switch.set_halign(Gtk::ALIGN_START);
        y_log_switch.set_halign(Gtk::ALIGN_START);
        box_combo.set_halign(Gtk::ALIGN_START);
        corner_combo.set_halign(Gtk::ALIGN_START);
        legend_switch.set_halign(Gtk::ALIGN_START);

        grid.attach(x_log_label, 0, 0, 1, 1);
        grid.attach(y_log_label, 0, 1, 1, 1);
        grid.attach(legend_label, 0, 2, 1, 1);
        grid.attach(x_log_switch, 1, 0, 1, 1);
        grid.attach(y_log_switch, 1, 1, 1, 1);
        grid.attach(legend_switch, 1, 2, 1, 1);
        grid.attach(corner_label, 0, 3, 1, 1);
        grid.attach(corner_combo, 1, 3, 1, 1);
        grid.attach(box_label, 0, 4, 1, 1);
        grid.attach(box_combo, 1, 4, 1, 1);
        grid.attach(canvas, 0, 5, 2, 1);
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
  Glib::set_application_name("gtkmm-plplot-test2");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test2");

  //open our test file
  std::ifstream fs;
  fs.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
  try {
	   fs.open(TEST_CSV);
  }
  catch (std::exception &e) {
    std::cerr << "Error opening file " << TEST_CSV << " -> " << e.what() << std::endl;
    return 1;
  }

  std::vector<double> x;
  std::vector<double> y1, y2, y3, y4;

  while (1) {
    try {
      fs.clear();
      std::string line;
      std::getline(fs, line);
      gchar **splitted = g_strsplit(line.c_str(), ",", 0);

      x.push_back(g_ascii_strtod(splitted[1], NULL));
      y1.push_back(g_ascii_strtod(splitted[2], NULL));
      y2.push_back(g_ascii_strtod(splitted[3], NULL));
      y3.push_back(g_ascii_strtod(splitted[4], NULL));
      y4.push_back(g_ascii_strtod(splitted[5], NULL));
      g_strfreev(splitted);
    }
    catch (std::exception &e) {
      if (fs.eof()) {
        break;
      }
      std::cerr << "Error parsing " << TEST_CSV << " -> " << e.what() << std::endl;
      return 1;
    }
    catch (...) {
      std::cerr << "Unknown exception occurred" << std::endl;
      return 1;
    }
  }

  //ensure our y-values are greater than 1!
  //the x-values are already...
  std::for_each(std::begin(y1), std::end(y1), [](double &a) { if (a < 1.0 ) a = 1.0;});
  std::for_each(std::begin(y2), std::end(y2), [](double &a) { if (a < 1.0 ) a = 1.0;});
  std::for_each(std::begin(y3), std::end(y3), [](double &a) { if (a < 1.0 ) a = 1.0;});
  std::for_each(std::begin(y4), std::end(y4), [](double &a) { if (a < 1.0 ) a = 1.0;});

  Glib::ustring x_title("Energy (keV)"), y_title("Intensity (counts)"), plot_title("NIST SRM 1155 Stainless steel");
  Test2::Window window(x, y1, y2, y3, y4, x_title, y_title, plot_title);

	return app->run(window);
}
