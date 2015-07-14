#include "gtkmm-plplot/canvas.h"
#include <gtkmm/application.h>
#include <glibmm/miscutils.h>
#include <glib.h>
#include <gtkmm/window.h>

namespace Test1 {
  class Window : public Gtk::Window {
  private:
    Gtk::PLplot::Canvas canvas;
    //bool on_plplot_drawing_area_double_click(GdkEventButton *event,
    //  double xmin, double xmax, double ymin, double ymax);
    void on_quit_button_clicked() {
      get_application()->remove_window(*this);
      return;
    }
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
      add(canvas);
      set_border_width(10);
      canvas.show_all();
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
