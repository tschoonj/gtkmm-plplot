#include "gtkmm-plplot/canvas.h"
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtkmm/window.h>
#include <gtkmm/switch.h>
#include <gtkmm/label.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/grid.h>

namespace Test1 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    Gtk::Grid grid;
    Gtk::ComboBoxText color_combo1;
    Gtk::ComboBoxText color_combo2;
    Gtk::Switch show_radio1;
    Gtk::Switch show_radio2;
  public:
    Window(std::valarray<PLFLT> &x, std::valarray<PLFLT> &y,
      std::string x_title = "X-axis", std::string y_title = "Y-axis",
      std::string plot_title = "", Gtk::PLplot::Color color = Gtk::PLplot::Color::RED) :
      canvas(Gtk::PLplot::Plot2D(Gtk::PLplot::Plot2DData(x, y, color), x_title, y_title, plot_title)) {

      //let's make this more interesting by adding more data
      std::valarray<PLFLT> x_va(1000), y_va(1000);
      for (unsigned int i = 0 ; i < 1000 ; i++) {
        x_va[i] = 8*M_PI*i/999;
      }
      y_va = 2*cos(x_va)-1;
      Gtk::PLplot::Plot2D *plot = canvas.get_plot(0);
      plot->add_data(Gtk::PLplot::Plot2DData(x_va, y_va, Gtk::PLplot::Color::BLUE));


      set_default_size(720, 580);
      Gdk::Geometry geometry;
      geometry.min_aspect = geometry.max_aspect = double(720)/double(580);
      set_geometry_hints(*this, geometry, Gdk::HINT_ASPECT);
      set_title("Gtkmm-PLplot test1");
      canvas.set_hexpand(true);
      canvas.set_vexpand(true);

      show_radio1.set_active(canvas.get_plot(0)->get_data(0)->is_showing());
      show_radio2.set_active(canvas.get_plot(0)->get_data(1)->is_showing());
      show_radio1.property_active().signal_changed().connect([this](){
        if (show_radio1.get_active()) {
          canvas.get_plot(0)->get_data(0)->show();
          color_combo1.set_sensitive(true);
        }
        else {
          canvas.get_plot(0)->get_data(0)->hide();
          color_combo1.set_sensitive(false);

        }
      });
      show_radio2.property_active().signal_changed().connect([this](){
        if (show_radio2.get_active()) {
          canvas.get_plot(0)->get_data(1)->show();
          color_combo2.set_sensitive(true);
        }
        else {
          canvas.get_plot(0)->get_data(1)->hide();
          color_combo2.set_sensitive(false);

        }
      });

      //too lazy too make a class for this...
      color_combo1.append("black");
      color_combo1.append("red");
      color_combo1.append("yellow");
      color_combo1.append("green");
      color_combo1.append("aquamarine");
      color_combo1.append("pink");
      color_combo1.append("wheat");
      color_combo1.append("grey");
      color_combo1.append("brown");
      color_combo1.append("blue");
      color_combo1.append("blueviolet");
      color_combo1.append("cyan");
      color_combo1.append("turquoise");
      color_combo1.append("magenta");
      color_combo1.append("salmon");
      color_combo1.append("white");
      color_combo2.append("black");
      color_combo2.append("red");
      color_combo2.append("yellow");
      color_combo2.append("green");
      color_combo2.append("aquamarine");
      color_combo2.append("pink");
      color_combo2.append("wheat");
      color_combo2.append("grey");
      color_combo2.append("brown");
      color_combo2.append("blue");
      color_combo2.append("blueviolet");
      color_combo2.append("cyan");
      color_combo2.append("turquoise");
      color_combo2.append("magenta");
      color_combo2.append("salmon");
      color_combo2.append("white");

      color_combo1.set_hexpand(false);
      color_combo2.set_hexpand(false);
      show_radio1.set_hexpand(false);
      show_radio2.set_hexpand(false);

      grid.set_column_homogeneous(false);
      grid.set_column_spacing(5);

      color_combo1.set_active(canvas.get_plot(0)->get_data(0)->get_color());
      color_combo2.set_active(canvas.get_plot(0)->get_data(1)->get_color());

      color_combo1.signal_changed().connect([this](){canvas.get_plot(0)->get_data(0)->set_color(static_cast<Gtk::PLplot::Color>(color_combo1.get_active_row_number()));});
      color_combo2.signal_changed().connect([this](){canvas.get_plot(0)->get_data(1)->set_color(static_cast<Gtk::PLplot::Color>(color_combo2.get_active_row_number()));});

      grid.attach(*Gtk::manage(new Gtk::Label("Plot1")), 0, 0, 1, 1);
      grid.attach(show_radio1, 1, 0, 1, 1);
      grid.attach(color_combo1, 2, 0, 1, 1);
      grid.attach(*Gtk::manage(new Gtk::Label("Plot2")), 0, 1, 1, 1);
      grid.attach(show_radio2, 1, 1, 1, 1);
      grid.attach(color_combo2, 2, 1, 1, 1);
      grid.attach(canvas, 0, 2, 3, 1);

      add(grid);
      set_border_width(10);
      grid.show_all();
    }
    virtual ~Window() {}
  };
}


int main(int argc, char **argv) {
	Glib::set_application_name("gtkmm-plplot-test1");
#if defined(G_OS_WIN32)
  //windows requires a bit more work. This example sets the PLPLOT_LIB environment variable
  //to ensure the PLplot data files are found at runtime
	gchar *installation_dir = g_win32_get_package_installation_directory_of_module(NULL);
	std::string path_to_plplot(Glib::build_filename(installation_dir, "Share", "plplot"));
	std::cout << "path to plplot: " << path_to_plplot << std::endl;
	Glib::setenv("PLPLOT_LIB", path_to_plplot, true);
	g_free(installation_dir);
#endif
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "eu.tomschoonjans.plplot");

  //valarrays are underestimated IMHO
  std::valarray<PLFLT> x_va(1000), y_va(1000);
  for (unsigned int i = 0 ; i < 1000 ; i++) {
    x_va[i] = 4*M_PI*i/999;
  }
  y_va = sin(x_va);

  std::string x_title("x"), y_title("y");

  Test1::Window window(x_va, y_va, x_title, y_title);

	return app->run(window);
}
