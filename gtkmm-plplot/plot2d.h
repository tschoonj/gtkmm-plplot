#ifndef GTKMMPLPLOTPLOT2D_H
#define GTKMMPLPLOTPLOT2D_H

#include "gtkmm-plplot/plot2ddata.h"
#include <sigc++/sigc++.h>
#include <vector>
#include <plstream.h>
#include <cairomm/cairomm.h>
#include <glibmm/ustring.h>


namespace Gtk {
  namespace PLplot {
    class Plot2D : public sigc::trackable {
    private:
      std::vector<Plot2DData *> plot_data;
      bool log10_x;
      bool log10_y;
      Glib::ustring axis_title_x;
      Glib::ustring axis_title_y;
      Glib::ustring plot_title;
      plstream *pls;
      double plot_data_range_x[2];
      double plot_data_range_y[2];
      double plotted_range_x[2];
      double plotted_range_y[2];
      double cairo_range_x[2];
      double cairo_range_y[2];
      int plot_width;
      int plot_height;
      bool shown;
      sigc::signal<void, double, double, double, double > _signal_select_region;
      sigc::signal<void> _signal_changed;
      sigc::signal<void, Plot2DData *> _signal_data_added;
      sigc::signal<void> _signal_data_removed;
      void plot_data_modified();
      Plot2D();
    protected:
      virtual void on_select_region(double xmin, double xmax, double ymin, double ymax);
      virtual void on_changed();
      virtual void on_data_added(Plot2DData *);
      virtual void on_data_removed();
    public:
      Plot2D(const Plot2DData &data, const Glib::ustring &axis_title_x = "X-axis", const Glib::ustring &axis_title_y = "Y-axis", const Glib::ustring &plot_title = "");
      Plot2D(const Plot2D &);
      virtual ~Plot2D();

      void add_data(const Plot2DData &data);
      Plot2DData *get_data(unsigned int index);
      void set_axis_logarithmic_x(bool log10 = true);
      void set_axis_logarithmic_y(bool log10 = true);
      bool get_axis_logarithmic_x();
      bool get_axis_logarithmic_y();
      void set_axis_title_x(Glib::ustring &title);
      void set_axis_title_y(Glib::ustring &title);
      Glib::ustring get_axis_title_x();
      Glib::ustring get_axis_title_y();
      void set_plot_title(Glib::ustring &title);
      Glib::ustring get_plot_title();

      //void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, plstream *_pls, int width, int height);
      void draw_plot(const Cairo::RefPtr<Cairo::Context> &cr, const int width, const int height);

      void convert_plplot_to_cairo_coordinates(
        double x_pl, double y_pl,
        double &x_cr, double &y_cr);

      void set_region(double xmin, double xmax, double ymin, double ymax);
      
      void show();
      void hide();
      bool is_showing() const;

      sigc::signal<void, double, double, double, double > signal_select_region() {
        return _signal_select_region;
      }

      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }

      sigc::signal<void, Plot2DData *> signal_data_added() {
        return _signal_data_added;
      }

      sigc::signal<void> signal_data_removed() {
        return _signal_data_removed;
      }
      friend class Canvas;
    };
  }
}
#endif
