/*
Copyright (C) 2015 Tom Schoonjans

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "gtkmm-plplot/gtkmm-plplot.h"
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

namespace Test1 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::PlotData2D plot_data1;
    Gtk::PLplot::Plot2D plot;
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

  public:
    Window(std::valarray<double> &x, std::valarray<double> &y,
      Glib::ustring x_title = "X-axis", Glib::ustring y_title = "Y-axis",
      Glib::ustring plot_title = "", Gdk::RGBA color = Gdk::RGBA("red")) :
      plot_data1(x, y, color),
      plot(plot_data1, x_title, y_title, plot_title),
      canvas(plot),
      label1("Plot 1"), label2("Plot 2"),
      linewidth_adj1(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      linewidth_adj2(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      linewidth_spin1(linewidth_adj1, 0.1, 1.0),
      linewidth_spin2(linewidth_adj2, 0.1, 1.0) {

      //let's make this more interesting by adding more data
      std::valarray<double> x_va(1000), y_va(1000);
      for (unsigned int i = 0 ; i < 1000 ; i++) {
        x_va[i] = 8*M_PI*i/999;
      }
      y_va = 2*cos(x_va)-1;

      // Gtk::manage can be used for memory management here...
      plot.add_data(*Gtk::manage(new Gtk::PLplot::PlotData2D(x_va, y_va, Gdk::RGBA("blue"), Gtk::PLplot::LineStyle::LONG_DASH_LONG_GAP, 5.0)));
      plot.hide_legend();


      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test1");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      show_radio1.set_active(plot_data1.is_showing());
      show_radio2.set_active(plot.get_data(1)->is_showing());
      show_radio1.property_active().signal_changed().connect([this](){
        if (show_radio1.get_active()) {
          plot_data1.show();
          color_combo1.set_sensitive(true);
          linestyle_combo1.set_sensitive(true);
          linewidth_spin1.set_sensitive(true);
        }
        else {
          // another way to access our data, though this is really just plot_data1
          canvas.get_plot(0)->get_data(0)->hide();
          color_combo1.set_sensitive(false);
          linestyle_combo1.set_sensitive(false);
          linewidth_spin1.set_sensitive(false);
        }
      });
      show_radio2.property_active().signal_changed().connect([this](){
        if (show_radio2.get_active()) {
          canvas.get_plot(0)->get_data(1)->show();
          color_combo2.set_sensitive(true);
          linestyle_combo2.set_sensitive(true);
          linewidth_spin2.set_sensitive(true);
        }
        else {
          plot.get_data(1)->hide();
          color_combo2.set_sensitive(false);
          linestyle_combo2.set_sensitive(false);
          linewidth_spin2.set_sensitive(false);
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

      // unlike plot_data1, plot_data2 is a pointer!
      auto plot_data2 = dynamic_cast<Gtk::PLplot::PlotData2D *>(plot.get_data(1));

      color_combo1.set_rgba(plot_data1.get_color());
      color_combo2.set_rgba(plot_data2->get_color());

      color_combo1.set_use_alpha(true);
      color_combo2.set_use_alpha(true);

      linestyle_combo1.set_active(plot_data1.get_line_style()-1);
      linestyle_combo2.set_active(plot_data2->get_line_style()-1);

      color_combo1.signal_color_set().connect([this](){plot_data1.set_color(color_combo1.get_rgba());});
      color_combo2.signal_color_set().connect([this, plot_data2](){plot_data2->set_color(color_combo2.get_rgba());});

      linestyle_combo1.signal_changed().connect([this](){plot_data1.set_line_style(static_cast<Gtk::PLplot::LineStyle>(linestyle_combo1.get_active_row_number()+1));});
      linestyle_combo2.signal_changed().connect([this, plot_data2](){plot_data2->set_line_style(static_cast<Gtk::PLplot::LineStyle>(linestyle_combo2.get_active_row_number()+1));});

      linewidth_spin1.set_value(plot_data1.get_line_width());
      linewidth_spin2.set_value(plot_data2->get_line_width());

      linewidth_spin1.signal_value_changed().connect([this](){plot_data1.set_line_width(linewidth_spin1.get_value());});
      linewidth_spin2.signal_value_changed().connect([this, plot_data2](){plot_data2->set_line_width(linewidth_spin2.get_value());});

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

      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}


int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test1");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test1");

  //valarrays are underestimated IMHO
  std::valarray<double> x_va(1000), y_va(1000);
  for (unsigned int i = 0 ; i < 1000 ; i++) {
    x_va[i] = 4*M_PI*i/999;
  }
  y_va = sin(x_va);

  Glib::ustring x_title("x"), y_title("y");

  Test1::Window window(x_va, y_va, x_title, y_title);

	return app->run(window);
}
