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
#include <glib.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/colorbutton.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace Test9 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::Label label1;
    Gtk::Label label2;
    Gtk::ColorButton color_combo1;
    Gtk::ColorButton color_combo2;
    Gtk::ComboBoxText linestyle_combo1;
    Gtk::ComboBoxText linestyle_combo2;
    Gtk::Switch show_radio1;
    Gtk::Switch show_radio2;
    Glib::RefPtr<Gtk::Adjustment> linewidth_adj1;
    Glib::RefPtr<Gtk::Adjustment> linewidth_adj2;
    Gtk::SpinButton linewidth_spin1;
    Gtk::SpinButton linewidth_spin2;
    Glib::RefPtr<Gtk::Adjustment> altitude_adj;
    Glib::RefPtr<Gtk::Adjustment> azimuth_adj;
    Gtk::SpinButton altitude_spin;
    Gtk::SpinButton azimuth_spin;
    Gtk::Label altitude_label;
    Gtk::Label azimuth_label;

  public:
    Window() :
      label1("Plot 1"), label2("Plot 2"),
      linewidth_adj1(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      linewidth_adj2(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      linewidth_spin1(linewidth_adj1, 0.1, 1.0),
      linewidth_spin2(linewidth_adj2, 0.1, 1.0),
      altitude_adj(Gtk::Adjustment::create(45.0, 0, 90.0, 1.0, 5.0, 0.0)),
      azimuth_adj(Gtk::Adjustment::create(45.0, 0, 360.0, 1.0, 5.0, 0.0)),
      altitude_spin(altitude_adj, 0.1, 1.0),
      azimuth_spin(azimuth_adj, 0.1, 1.0),
      altitude_label("Altitude"),
      azimuth_label("Azimuth") {

      //generate our data
      //first a helix
      std::valarray<double> x_va1 = cos(Gtk::PLplot::indgen_va(1000)*8.0*M_PI/999.0);
      std::valarray<double> y_va1 = sin(Gtk::PLplot::indgen_va(1000)*8.0*M_PI/999.0);
      std::valarray<double> z_va1 = Gtk::PLplot::indgen_va(1000)*10.0/999.0 - 5.0;

      Gtk::PLplot::PlotData3D *data1 = Gtk::manage(new Gtk::PLplot::PlotData3D(x_va1, y_va1, z_va1));
      data1->set_name("Helix");

      //followed by a simple sine
      std::valarray<double> x_va2 = Gtk::PLplot::indgen_va(1000)*6.0/999.0 - 3.0;
      std::valarray<double> y_va2 = Gtk::PLplot::indgen_va(1000)*6.0/999.0 - 3.0;
      std::valarray<double> z_va2 = cos(sqrt(x_va2*x_va2 + y_va2*y_va2));

      Gtk::PLplot::PlotData3D *data2 = Gtk::manage(new Gtk::PLplot::PlotData3D(x_va2, y_va2, z_va2, Gdk::RGBA("blue"), Gtk::PLplot::LineStyle::LONG_DASH_LONG_GAP));
      data2->set_name("Cosine");

      Gtk::PLplot::Plot3D *plot = Gtk::manage(new Gtk::PLplot::Plot3D(*data1));
      plot->add_data(*data2);

      plot->set_plot_title("Plot3D demo");

      canvas.add_plot(*plot);

      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test9");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      show_radio1.set_active(data1->is_showing());
      show_radio2.set_active(data2->is_showing());
      show_radio1.property_active().signal_changed().connect([this, data1](){
        if (show_radio1.get_active()) {
          data1->show();
          color_combo1.set_sensitive(true);
        }
        else {
          data1->hide();
          color_combo1.set_sensitive(false);
        }
      });
      show_radio2.property_active().signal_changed().connect([this, data2](){
        if (show_radio2.get_active()) {
          data2->show();
          color_combo2.set_sensitive(true);
        }
        else {
          data2->hide();
          color_combo2.set_sensitive(false);
        }
      });
      linestyle_combo1.append("continuous");
      linestyle_combo1.append("short dash short gap");
      linestyle_combo1.append("long dash long gap");
      linestyle_combo1.append("long dash short gap");
      linestyle_combo1.append("long dash short gap short dash short gap");
      linestyle_combo1.append("long dash short gap long dash short gap");
      linestyle_combo2.append("continuous");
      linestyle_combo2.append("short dash short gap");
      linestyle_combo2.append("long dash long gap");
      linestyle_combo2.append("long dash short gap");
      linestyle_combo2.append("long dash short gap short dash short gap");
      linestyle_combo2.append("long dash short gap long dash short gap");

      linewidth_spin1.set_wrap(true);
      linewidth_spin2.set_wrap(true);
      linewidth_spin1.set_snap_to_ticks(true);
      linewidth_spin2.set_snap_to_ticks(true);
      linewidth_spin1.set_numeric(true);
      linewidth_spin2.set_numeric(true);

      color_combo1.set_hexpand(false);
      color_combo2.set_hexpand(false);
      linewidth_spin1.set_halign(Gtk::ALIGN_START);
      linewidth_spin2.set_halign(Gtk::ALIGN_START);
      show_radio1.set_hexpand(false);
      show_radio2.set_hexpand(false);
      label1.set_hexpand(true);
      label2.set_hexpand(true);
      linewidth_spin1.set_hexpand(true);
      linewidth_spin2.set_hexpand(true);
      linestyle_combo1.set_hexpand(false);
      linestyle_combo2.set_hexpand(false);
      label1.set_halign(Gtk::ALIGN_END);
      label2.set_halign(Gtk::ALIGN_END);

      grid.set_column_homogeneous(false);
      grid.set_column_spacing(5);
      grid.set_row_spacing(5);

      color_combo1.set_rgba(data1->get_color());
      color_combo2.set_rgba(data2->get_color());

      color_combo1.set_use_alpha(true);
      color_combo2.set_use_alpha(true);

      linestyle_combo1.set_active(data1->get_line_style()-1);
      linestyle_combo2.set_active(data2->get_line_style()-1);

      color_combo1.signal_color_set().connect([this, data1](){data1->set_color(color_combo1.get_rgba());});
      color_combo2.signal_color_set().connect([this, data2](){data2->set_color(color_combo2.get_rgba());});

      linestyle_combo1.signal_changed().connect([this, data1](){data1->set_line_style(static_cast<Gtk::PLplot::LineStyle>(linestyle_combo1.get_active_row_number()+1));});
      linestyle_combo2.signal_changed().connect([this, data2](){data2->set_line_style(static_cast<Gtk::PLplot::LineStyle>(linestyle_combo2.get_active_row_number()+1));});

      linewidth_spin1.set_value(data1->get_line_width());
      linewidth_spin2.set_value(data2->get_line_width());

      linewidth_spin1.signal_value_changed().connect([this, data1](){data1->set_line_width(linewidth_spin1.get_value());});
      linewidth_spin2.signal_value_changed().connect([this, data2](){data2->set_line_width(linewidth_spin2.get_value());});

      altitude_spin.set_wrap(true);
      azimuth_spin.set_wrap(true);
      altitude_spin.set_snap_to_ticks(true);
      azimuth_spin.set_snap_to_ticks(true);
      altitude_spin.set_numeric(true);
      azimuth_spin.set_numeric(true);
      altitude_label.set_halign(Gtk::ALIGN_END);
      azimuth_label.set_halign(Gtk::ALIGN_END);
      altitude_spin.set_halign(Gtk::ALIGN_START);
      azimuth_spin.set_halign(Gtk::ALIGN_START);
      altitude_label.set_hexpand(false);
      azimuth_label.set_hexpand(false);
      altitude_label.set_vexpand(false);
      azimuth_label.set_vexpand(false);
      altitude_spin.set_hexpand(false);
      azimuth_spin.set_hexpand(false);
      altitude_spin.set_vexpand(false);
      azimuth_spin.set_vexpand(false);

      altitude_spin.set_value(plot->get_altitude());
      azimuth_spin.set_value(plot->get_azimuth());

      altitude_spin.signal_value_changed().connect(
        [this, plot]() {
          plot->set_altitude(altitude_spin.get_value());
        }
      );
      azimuth_spin.signal_value_changed().connect(
        [this, plot]() {
          plot->set_azimuth(azimuth_spin.get_value());
        }
      );

      grid.attach(label1, 0, 0, 1, 1);
      grid.attach(show_radio1, 1, 0, 1, 1);
      grid.attach(color_combo1, 2, 0, 1, 1);
      grid.attach(linestyle_combo1, 3, 0, 1, 1);
      grid.attach(linewidth_spin1, 4, 0, 1, 1);
      grid.attach(label2, 0, 1, 1, 1);
      grid.attach(show_radio2, 1, 1, 1, 1);
      grid.attach(color_combo2, 2, 1, 1, 1);
      grid.attach(linestyle_combo2, 3, 1, 1, 1);
      grid.attach(linewidth_spin2, 4, 1, 1, 1);
      grid.attach(canvas, 0, 2, 5, 1);
      grid.attach(altitude_label, 0, 3, 3, 1);
      grid.attach(altitude_spin, 3, 3, 2, 1);
      grid.attach(azimuth_label, 0, 4, 3, 1);
      grid.attach(azimuth_spin, 3, 4, 2, 1);

      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test9");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test9");

  Test9::Window window;

	return app->run(window);
}
