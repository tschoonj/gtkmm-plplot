/*
Copyright (C) 2017 Tom Schoonjans

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
#include <gtkmm/notebook.h>
#include <functional>
#include <random>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

namespace Test12 {
  class HistogramTab : public Gtk::Grid {
  private:
    Gtk::Label line_label;
    Gtk::ComboBoxText linestyle_combo;
    Gtk::ColorButton line_color;
    Glib::RefPtr<Gtk::Adjustment> linewidth_adj;
    Gtk::SpinButton linewidth_spin;
    Gtk::Switch expand_bins;
    Gtk::Switch empty_bins;
    HistogramTab() = delete;
  protected:
    Gtk::PLplot::Canvas canvas;
    Gtk::PLplot::PlotDataHistogram *histogram_data;
  public:
    HistogramTab(Gtk::PLplot::PlotDataHistogram *_histogram_data, Glib::ustring x_title, Glib::ustring y_title, Glib::ustring plot_title) :
      line_label("Histogram"),
      linewidth_adj(Gtk::Adjustment::create(1.0, 0.1, 10.0, 0.1, 1.0, 0.0)),
      linewidth_spin(linewidth_adj, 0.1, 1.0),
      canvas(*Gtk::manage(new Gtk::PLplot::PlotHistogram(*_histogram_data, x_title, y_title, plot_title))),
      histogram_data(_histogram_data)
    {
      set_border_width(5);
      set_column_homogeneous(false);
      set_column_spacing(5);
      set_row_homogeneous(false);
      set_row_spacing(5);

      line_label.set_hexpand(true);
      line_label.set_vexpand(false);
      line_label.set_valign(Gtk::ALIGN_CENTER);
      line_label.set_halign(Gtk::ALIGN_END);

      attach(line_label, 0, 0, 1, 1);

      //the color buttons
      auto plot_data = dynamic_cast<Gtk::PLplot::PlotDataHistogram*>(canvas.get_plot(0)->get_data(0));
      line_color.set_rgba(plot_data->get_color());
      line_color.set_use_alpha(true);
      line_color.set_hexpand(false);
      line_color.set_vexpand(false);
      line_color.set_halign(Gtk::ALIGN_FILL);
      line_color.set_valign(Gtk::ALIGN_CENTER);

      line_color.signal_color_set().connect([this, plot_data](){plot_data->set_color(line_color.get_rgba());});

      attach(line_color, 1, 0, 1, 1);

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

      attach(linestyle_combo, 2, 0, 1, 1);

      //our spinbuttons, first one for the line thickness, second for symbol size
      linewidth_spin.set_hexpand(true);
      linewidth_spin.set_vexpand(false);
      linewidth_spin.set_halign(Gtk::ALIGN_START);
      linewidth_spin.set_valign(Gtk::ALIGN_CENTER);
      linewidth_spin.set_wrap(true);
      linewidth_spin.set_snap_to_ticks(true);
      linewidth_spin.set_numeric(true);

      linewidth_spin.set_value(plot_data->get_line_width());

      linewidth_spin.signal_value_changed().connect([this, plot_data](){plot_data->set_line_width(linewidth_spin.get_value());});

      attach(linewidth_spin, 3, 0, 1, 1);

      canvas.set_hexpand(true);
      canvas.set_vexpand(true);
      canvas.set_background_color(Gdk::RGBA("LightGrey"));
      attach(canvas, 0, 1, 4, 1);

      Gtk::Label *label;

      label = Gtk::manage(new Gtk::Label("Expand bins"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      attach(*label, 0, 2, 1, 1);
      expand_bins.set_hexpand(false);
      expand_bins.set_halign(Gtk::ALIGN_START);
      attach(expand_bins, 1, 2, 1, 1);
      label = Gtk::manage(new Gtk::Label("Show empty bins"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      attach(*label, 2, 2, 1, 1);
      empty_bins.set_hexpand(false);
      empty_bins.set_halign(Gtk::ALIGN_START);
      attach(empty_bins, 3, 2, 1, 1);

      expand_bins.set_active(histogram_data->get_expand_bins());
      expand_bins.property_active().signal_changed().connect([this](){
        histogram_data->set_expand_bins(expand_bins.get_active());
      });
      empty_bins.set_active(histogram_data->get_empty_bins());
      empty_bins.property_active().signal_changed().connect([this](){
        histogram_data->set_empty_bins(empty_bins.get_active());
      });


    }
  };

  class HistogramBinnedTab : public HistogramTab {
  private:
    Gtk::Switch centred;
    HistogramBinnedTab() = delete;
  public:
    HistogramBinnedTab(Gtk::PLplot::PlotDataHistogramBinned *_histogram_data, Glib::ustring x_title, Glib::ustring y_title, Glib::ustring plot_title) :
    HistogramTab(_histogram_data, x_title, y_title, plot_title) {
      Gtk::Label *label;

      label = Gtk::manage(new Gtk::Label("X-data is centred"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      label->set_vexpand(false);
      attach(*label, 0, 3, 1, 1);

      centred.set_halign(Gtk::ALIGN_START);
      centred.set_hexpand(false);
      centred.set_vexpand(false);
      centred.set_active(_histogram_data->get_centred());
      centred.property_active().signal_changed().connect([this, _histogram_data](){
        _histogram_data->set_centred(centred.get_active());
      });
      attach(centred, 1, 3, 1, 1);
    }
  };

  class HistogramUnbinnedTab : public HistogramTab {
  private:
  /*  Gtk::Button add_data_button; */
    Gtk::Switch ignore_outliers;
    Glib::RefPtr<Gtk::Adjustment> datmin_adj;
    Glib::RefPtr<Gtk::Adjustment> datmax_adj;
    Glib::RefPtr<Gtk::Adjustment> nbins_adj;
    Gtk::SpinButton datmin_spin;
    Gtk::SpinButton datmax_spin;
    Gtk::SpinButton nbins_spin;
    HistogramUnbinnedTab() = delete;
  public:
    HistogramUnbinnedTab(Gtk::PLplot::PlotDataHistogramUnbinned *_histogram_data, Glib::ustring x_title, Glib::ustring y_title, Glib::ustring plot_title) :
    HistogramTab(_histogram_data, x_title, y_title, plot_title),
    datmin_adj(Gtk::Adjustment::create(_histogram_data->get_data_minimum(), _histogram_data->get_data_minimum(), _histogram_data->get_data_maximum() - 0.01, 0.1)),
    datmax_adj(Gtk::Adjustment::create(_histogram_data->get_data_maximum(), _histogram_data->get_data_minimum() + 0.01, _histogram_data->get_data_maximum(), 0.1)),
    nbins_adj(Gtk::Adjustment::create(_histogram_data->get_nbins(), 3, 100, 1)),
    datmin_spin(datmin_adj, 0.1, 1),
    datmax_spin(datmax_adj, 0.1, 1),
    nbins_spin(nbins_adj, 1, 0)
    {
      Gtk::Label *label;

      label = Gtk::manage(new Gtk::Label("Ignore outliers"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      label->set_vexpand(false);
      attach(*label, 0, 3, 1, 1);

      ignore_outliers.set_halign(Gtk::ALIGN_START);
      ignore_outliers.set_hexpand(false);
      ignore_outliers.set_vexpand(false);
      ignore_outliers.set_active(_histogram_data->get_ignore_outliers());
      ignore_outliers.property_active().signal_changed().connect([this, _histogram_data](){
        _histogram_data->set_ignore_outliers(ignore_outliers.get_active());
      });
      attach(ignore_outliers, 1, 3, 1, 1);

      label = Gtk::manage(new Gtk::Label("Number of bins"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      label->set_vexpand(false);
      attach(*label, 2, 3, 1, 1);

      nbins_spin.set_halign(Gtk::ALIGN_START);
      nbins_spin.set_hexpand(false);
      nbins_spin.set_vexpand(false);
      nbins_spin.set_wrap(true);
      nbins_spin.set_snap_to_ticks(true);
      nbins_spin.set_numeric(true);
      nbins_spin.set_value(_histogram_data->get_nbins());
      nbins_spin.signal_value_changed().connect([this, _histogram_data](){
        _histogram_data->set_nbins(nbins_spin.get_value());
      });
      attach(nbins_spin, 3, 3, 1, 1);

      label = Gtk::manage(new Gtk::Label("Bin minimum"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      label->set_vexpand(false);
      attach(*label, 0, 4, 1, 1);

      datmin_spin.set_halign(Gtk::ALIGN_START);
      datmin_spin.set_hexpand(false);
      datmin_spin.set_vexpand(false);
      datmin_spin.set_wrap(true);
      datmin_spin.set_snap_to_ticks(true);
      datmin_spin.set_numeric(true);
      datmin_spin.set_value(_histogram_data->get_data_minimum());
      datmin_spin.signal_value_changed().connect([this, _histogram_data](){
        double value = datmin_spin.get_value();
        _histogram_data->set_data_minimum(value);
        double old_min, old_max;
        datmax_spin.get_range(old_min, old_max);
        datmax_spin.set_range(value + 0.01, old_max);
      });
      attach(datmin_spin, 1, 4, 1, 1);

      label = Gtk::manage(new Gtk::Label("Bin maximum"));
      label->set_halign(Gtk::ALIGN_END);
      label->set_hexpand(false);
      label->set_vexpand(false);
      attach(*label, 2, 4, 1, 1);

      datmax_spin.set_halign(Gtk::ALIGN_START);
      datmax_spin.set_hexpand(false);
      datmax_spin.set_vexpand(false);
      datmax_spin.set_wrap(true);
      datmax_spin.set_snap_to_ticks(true);
      datmax_spin.set_numeric(true);
      datmax_spin.set_value(_histogram_data->get_data_maximum());
      datmax_spin.signal_value_changed().connect([this, _histogram_data](){
        double value = datmax_spin.get_value();
        _histogram_data->set_data_maximum(value);
        double old_min, old_max;
        datmin_spin.get_range(old_min, old_max);
        datmin_spin.set_range(old_min, value - 0.01);
      });
      attach(datmax_spin, 3, 4, 1, 1);

    }
  };

  class HistogramUnbinnedWithAddButtonTab : public HistogramUnbinnedTab {
  private:
    Gtk::Button add_data_button;
    HistogramUnbinnedWithAddButtonTab() = delete;
  public:
    HistogramUnbinnedWithAddButtonTab(Gtk::PLplot::PlotDataHistogramUnbinned *_histogram_data, Glib::ustring x_title, Glib::ustring y_title, Glib::ustring plot_title, std::function<void()> add_data_function) :
    HistogramUnbinnedTab(_histogram_data, x_title, y_title, plot_title),
    add_data_button("Add 10 data points") {
      add_data_button.set_hexpand(false);
      add_data_button.set_halign(Gtk::ALIGN_CENTER);
      add_data_button.signal_clicked().connect(add_data_function);

      attach(add_data_button, 0, 5, 4, 1);
    }
  };

  class Window : public Gtk::Window {
  private:
    Gtk::Notebook notebook;
    std::mt19937 gen;
    std::normal_distribution<> d;
  public:
    Window() : notebook(), gen(1234), d(0, 1){
      // general window and canvas settings
      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test12");

      //add pages to notebook
      // page1 -> PLplot example
      std::valarray<double> data1 = sin(Gtk::PLplot::indgen_va(2047) * 2.0 * M_PI / 2047.0);
      HistogramUnbinnedTab *tab1 =
        Gtk::manage(
          new HistogramUnbinnedTab(
            Gtk::manage(
              new Gtk::PLplot::PlotDataHistogramUnbinned(
                data1, -1.1, 1.1, 44
              )
            ),
            "#frValue", // #fr triggers Roman font...
            "#frFrequency",
            "#frPLplot Example 5 - Probability function of Oscillator"
          )
        );

      // page2: Gaussian distribution
      std::vector<double> data2;
      for (size_t i = 0 ; i < 100 ; i++)
        data2.push_back(d(gen));
      Gtk::PLplot::PlotDataHistogramUnbinned *histogram_data2 = Gtk::manage(
              new Gtk::PLplot::PlotDataHistogramUnbinned(
                data2, -5.0, 5, 10
              )
            );
      HistogramUnbinnedTab *tab2 =
        Gtk::manage(
          new HistogramUnbinnedWithAddButtonTab(
            histogram_data2,
            "Value",
            "Frequency",
            "Normal distribution",
            [histogram_data2, this](){ /* function that will be called whenever the add_data_button is clicked */
              for (size_t i = 0 ; i < 10 ; i++)
                histogram_data2->add_datapoint(d(gen));
            }
          )
        );

      std::valarray<double> data3_x = Gtk::PLplot::indgen_va(21) - 10.0;
      std::valarray<double> data3_y = exp(-1.0 * data3_x * data3_x / 2.0) / sqrt(2.0 * M_PI);
      Gtk::PLplot::PlotDataHistogramBinned *histogram_data3 = Gtk::manage(
              new Gtk::PLplot::PlotDataHistogramBinned(
                data3_x, data3_y
              )
            );
      HistogramBinnedTab *tab3 =
        Gtk::manage(
          new HistogramBinnedTab(
            histogram_data3,
            "Value",
            "Frequency",
            "Normal distribution"
          )
        );

      // append pages
      notebook.append_page(*tab1, "Histogram 1");
      notebook.append_page(*tab2, "Histogram 2");
      notebook.append_page(*tab3, "Histogram 3");
      add(notebook);
      set_border_width(5);
      notebook.show_all();
    }
  };
}

int main(int argc, char **argv) {
  Glib::set_application_name("gtkmm-plplot-test12");
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.gtkmm-plplot-test12");

  Test12::Window *window = new Test12::Window();

  return app->run(*window);
}
