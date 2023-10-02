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
#include <gtkmm/aspectframe.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/grid.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>

#include <random>

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
    Gtk::Button remove_data_button;
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
      add_data_button("Append datapoint"),
      remove_data_button("Remove random datapoint") {

      // general window and canvas settings
      const int width = 1024, height = 580;
      set_default_size(width, height);
      set_title("Gtkmm-PLplot test5");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      canvas.set_focusable(true);
      Gtk::AspectFrame geometry(Gtk::Align::CENTER, Gtk::Align::CENTER, float(width)/float(height), false);
      geometry.set_child(canvas);

      // grid options
      grid.set_column_homogeneous(false);
      grid.set_column_spacing(5);
      grid.set_row_homogeneous(false);
      grid.set_row_spacing(5);

      //the labels
      line_label.set_hexpand(true);
      line_label.set_vexpand(false);
      line_label.set_valign(Gtk::Align::CENTER);
      line_label.set_halign(Gtk::Align::END);
      symbol_label.set_hexpand(true);
      symbol_label.set_vexpand(false);
      symbol_label.set_valign(Gtk::Align::CENTER);
      symbol_label.set_halign(Gtk::Align::END);

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
      line_color.set_halign(Gtk::Align::CENTER);
      line_color.set_valign(Gtk::Align::CENTER);
      symbol_color.set_halign(Gtk::Align::CENTER);
      symbol_color.set_valign(Gtk::Align::CENTER);

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
      linestyle_combo.set_halign(Gtk::Align::CENTER);
      linestyle_combo.set_valign(Gtk::Align::CENTER);

      grid.attach(linestyle_combo, 2, 0, 1, 1);

      //the symbol
      plot_data->set_symbol("√");
      symbol_entry.set_text(plot_data->get_symbol());

      symbol_entry.set_hexpand(false);
      symbol_entry.set_vexpand(false);
      symbol_entry.set_halign(Gtk::Align::FILL);
      symbol_entry.set_valign(Gtk::Align::CENTER);

      symbol_entry.signal_changed().connect([this, plot_data](){
        plot_data->set_symbol(
          symbol_entry.get_text()
        );
      });

      grid.attach(symbol_entry, 2, 1, 1, 1);

      //our spinbuttons, first one for the line thickness, second for symbol size
      linewidth_spin.set_hexpand(true);
      linewidth_spin.set_vexpand(false);
      linewidth_spin.set_halign(Gtk::Align::START);
      linewidth_spin.set_valign(Gtk::Align::CENTER);
      linewidth_spin.set_wrap(true);
      linewidth_spin.set_snap_to_ticks(true);
      linewidth_spin.set_numeric(true);
      symbol_scale_factor_spin.set_hexpand(true);
      symbol_scale_factor_spin.set_vexpand(false);
      symbol_scale_factor_spin.set_halign(Gtk::Align::START);
      symbol_scale_factor_spin.set_valign(Gtk::Align::CENTER);

      linewidth_spin.set_value(plot_data->get_line_width());
      symbol_scale_factor_spin.set_value(plot_data->get_symbol_height_scale_factor());

      linewidth_spin.signal_value_changed().connect([this, plot_data](){plot_data->set_line_width(linewidth_spin.get_value());});
      symbol_scale_factor_spin.signal_value_changed().connect([this, plot_data](){plot_data->set_symbol_height_scale_factor(symbol_scale_factor_spin.get_value());});

      grid.attach(linewidth_spin, 3, 0, 1, 1);
      grid.attach(symbol_scale_factor_spin, 3, 1, 1, 1);

      Gtk::Grid *buttons_grid = Gtk::manage(new Gtk::Grid());
      buttons_grid->set_column_homogeneous(true);
      buttons_grid->set_column_spacing(5);
      grid.attach(*buttons_grid, 0, 3, 4, 1);

      //the add datapoint button
      add_data_button.set_hexpand(false);
      add_data_button.set_vexpand(false);
      add_data_button.set_valign(Gtk::Align::CENTER);
      add_data_button.set_halign(Gtk::Align::END);
      buttons_grid->attach(add_data_button, 0, 0, 1, 1);
      add_data_button.signal_clicked().connect([this, plot_data](){
        //this lambda has a static variable that will keep our ever incrementing X-value
        static double new_x = 0.0;
        plot_data->add_datapoint(new_x, sqrt(new_x));
        new_x += 1.0;
	remove_data_button.set_sensitive(true);
      });

      remove_data_button.set_hexpand(false);
      remove_data_button.set_vexpand(false);
      remove_data_button.set_valign(Gtk::Align::CENTER);
      remove_data_button.set_halign(Gtk::Align::START);
      remove_data_button.set_sensitive(false);
      buttons_grid->attach(remove_data_button, 1, 0, 1, 1);
      remove_data_button.signal_clicked().connect([this, plot_data](){
        static std::default_random_engine rng;
        unsigned long int size = plot_data->size();
	    if (size == 1) {
          plot_data->remove_datapoint(0);
          remove_data_button.set_sensitive(false);
        }
        else {
          std::uniform_int_distribution<unsigned long int> dist(0, size - 1);
          plot_data->remove_datapoint(dist(rng));
        }
      });

      //finishing up
      grid.attach(geometry, 0, 2, 4, 1);
      grid.set_margin(10);
      set_child(grid);
    }
    virtual ~Window() {}
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test5");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("eu.tomschoonjans.gtkmm-plplot-test5");

  std::valarray<double> x_va, y_va;
  return app->make_window_and_run<Test5::Window>(argc, argv, x_va, y_va);
}
