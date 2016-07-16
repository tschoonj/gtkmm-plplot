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

namespace Test5 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::Label line_label;
    Gtk::Label symbol_label;
    Gtk::ComboBoxText linestyle_combo;
    Gtk::Entry symbol_entry;
    Gtk::ColorButton line_color;
    Gtk::ColorButton symbol_color;
    Glib::RefPtr<Gtk::Adjustment> linewidth_adj;
    Glib::RefPtr<Gtk::Adjustment> symbol_scale_factor_adj;
    Gtk::SpinButton linewidth_spin;
    Gtk::SpinButton symbol_scale_factor_spin;
    Gtk::Button add_data_button;
  public:
    Window(std::valarray<double> &x, std::valarray<double> &y,
      Glib::ustring x_title = "X-axis", Glib::ustring y_title = "Y-axis",
      Glib::ustring plot_title = "y = sqrt(x)", Gdk::RGBA color = Gdk::RGBA("Blue")) :
      canvas(*Gtk::manage(new Gtk::PLplot::Plot2D(*Gtk::manage(new Gtk::PLplot::PlotData2D(x, y, color)), x_title, y_title, plot_title))),
      line_label("Line"), symbol_label("Symbols"),
      linewidth_adj(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      symbol_scale_factor_adj(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      linewidth_spin(linewidth_adj, 0.1, 1.0),
      symbol_scale_factor_spin(symbol_scale_factor_adj, 0.1, 1.0),
      add_data_button("Add datapoint") {

      // general window and canvas settings
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test5");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      // grid options
      grid.set_column_homogeneous(false);
      grid.set_column_spacing(5);
      grid.set_row_homogeneous(false);
      grid.set_row_spacing(5);

      //the labels
      line_label.set_hexpand(true);
      line_label.set_vexpand(false);
      line_label.set_valign(Gtk::ALIGN_CENTER);
      line_label.set_halign(Gtk::ALIGN_END);
      symbol_label.set_hexpand(true);
      symbol_label.set_vexpand(false);
      symbol_label.set_valign(Gtk::ALIGN_CENTER);
      symbol_label.set_halign(Gtk::ALIGN_END);

      grid.attach(line_label, 0, 0, 1, 1);
      grid.attach(symbol_label, 0, 1, 1, 1);

      //the color buttons
      auto plot_data = dynamic_cast<Gtk::PLplot::PlotData2D *>(canvas.get_plot(0)->get_data(0));
      line_color.set_rgba(plot_data->get_color());
      symbol_color.set_rgba(plot_data->get_symbol_color());
      line_color.set_use_alpha(true);
      symbol_color.set_use_alpha(true);
      line_color.set_hexpand(false);
      line_color.set_vexpand(false);
      symbol_color.set_hexpand(false);
      symbol_color.set_vexpand(false);
      line_color.set_halign(Gtk::ALIGN_CENTER);
      line_color.set_valign(Gtk::ALIGN_CENTER);
      symbol_color.set_halign(Gtk::ALIGN_CENTER);
      symbol_color.set_valign(Gtk::ALIGN_CENTER);

      line_color.signal_color_set().connect([this, plot_data](){plot_data->set_color(line_color.get_rgba());});
      symbol_color.signal_color_set().connect([this, plot_data](){plot_data->set_symbol_color(symbol_color.get_rgba());});

      grid.attach(line_color, 1, 0, 1, 1);
      grid.attach(symbol_color, 1, 1, 1, 1);

      //the linestyle comboboxtext
      linestyle_combo.append("none");
      linestyle_combo.append("continuous");
      linestyle_combo.append("short dash short gap");
      linestyle_combo.append("long dash long gap");
      linestyle_combo.append("long dash short gap");
      linestyle_combo.append("long dash short gap short dash short gap");
      linestyle_combo.append("long dash short gap long dash short gap");

      linestyle_combo.set_active(plot_data->get_line_style());
      linestyle_combo.signal_changed().connect([this, plot_data](){plot_data->set_line_style(static_cast<Gtk::PLplot::LineStyle>(linestyle_combo.get_active_row_number()));});

      linestyle_combo.set_hexpand(false);
      linestyle_combo.set_vexpand(false);
      linestyle_combo.set_halign(Gtk::ALIGN_CENTER);
      linestyle_combo.set_valign(Gtk::ALIGN_CENTER);

      grid.attach(linestyle_combo, 2, 0, 1, 1);

      //the symbol
      plot_data->set_symbol("√");
      symbol_entry.set_text(plot_data->get_symbol());

      symbol_entry.set_hexpand(false);
      symbol_entry.set_vexpand(false);
      symbol_entry.set_halign(Gtk::ALIGN_FILL);
      symbol_entry.set_valign(Gtk::ALIGN_CENTER);

      symbol_entry.signal_changed().connect([this, plot_data](){
        plot_data->set_symbol(
          symbol_entry.get_text()
        );
      });

      grid.attach(symbol_entry, 2, 1, 1, 1);

      //our spinbuttons, first one for the line thickness, second for symbol size
      linewidth_spin.set_hexpand(true);
      linewidth_spin.set_vexpand(false);
      linewidth_spin.set_halign(Gtk::ALIGN_START);
      linewidth_spin.set_valign(Gtk::ALIGN_CENTER);
      linewidth_spin.set_wrap(true);
      linewidth_spin.set_snap_to_ticks(true);
      linewidth_spin.set_numeric(true);
      symbol_scale_factor_spin.set_hexpand(true);
      symbol_scale_factor_spin.set_vexpand(false);
      symbol_scale_factor_spin.set_halign(Gtk::ALIGN_START);
      symbol_scale_factor_spin.set_valign(Gtk::ALIGN_CENTER);

      linewidth_spin.set_value(plot_data->get_line_width());
      symbol_scale_factor_spin.set_value(plot_data->get_symbol_height_scale_factor());

      linewidth_spin.signal_value_changed().connect([this, plot_data](){plot_data->set_line_width(linewidth_spin.get_value());});
      symbol_scale_factor_spin.signal_value_changed().connect([this, plot_data](){plot_data->set_symbol_height_scale_factor(symbol_scale_factor_spin.get_value());});

      grid.attach(linewidth_spin, 3, 0, 1, 1);
      grid.attach(symbol_scale_factor_spin, 3, 1, 1, 1);

      //the add datapoint button
      add_data_button.set_hexpand(false);
      add_data_button.set_vexpand(false);
      add_data_button.set_valign(Gtk::ALIGN_CENTER);
      add_data_button.set_halign(Gtk::ALIGN_CENTER);
      grid.attach(add_data_button, 0, 3, 4, 1);
      add_data_button.signal_clicked().connect([this, plot_data](){
        //this lambda has a static variable that will keep our ever incrementing X-value
        static double new_x = 11;
        plot_data->add_datapoint(new_x, sqrt(new_x));
        new_x += 1.0;
      });

      //finishing up
      grid.attach(canvas, 0, 2, 4, 1);
      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test5");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test5");

  std::valarray<double> x_va = Gtk::PLplot::indgen_va(10)+double(1.0), y_va = sqrt(x_va);

  Test5::Window window(x_va, y_va);

	return app->run(window);
}
