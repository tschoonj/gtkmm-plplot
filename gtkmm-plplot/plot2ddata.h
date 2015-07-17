#ifndef GTKMMPLPLOTPLOT2DDATA_H
#define GTKMMPLPLOTPLOT2DDATA_H

#include <sigc++/sigc++.h>
#include <vector>
#include <valarray>
#include "gtkmm-plplot/linestyle.h"
#include <plstream.h>
#include <glibmm/ustring.h>
#include <cairomm/cairomm.h>
#include <gdkmm/rgba.h>


#define PLOT2DDATA_DEFAULT_LINE_WIDTH 1.0

namespace Gtk {
  namespace PLplot {
    class Plot2DData : public sigc::trackable {
    private:
      std::vector<PLFLT> x;
      std::vector<PLFLT> y;
      Gdk::RGBA color;
      LineStyle line_style;
      double line_width;
      bool shown;
      sigc::signal<void> _signal_changed;
      Plot2DData();
    protected:
      virtual void on_changed();
    public:
      Plot2DData(const std::vector<PLFLT> &x,
                 const std::vector<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);
      Plot2DData(const std::valarray<PLFLT> &x,
                 const std::valarray<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);
      Plot2DData(const std::vector<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);
      Plot2DData(const std::valarray<PLFLT> &y,
                 Gdk::RGBA color = Gdk::RGBA("red"),
                 LineStyle line_style = CONTINUOUS,
                 double line_width = PLOT2DDATA_DEFAULT_LINE_WIDTH);
      Plot2DData(const Plot2DData &data);
      virtual ~Plot2DData() {}
      void set_color(Gdk::RGBA color);
      Gdk::RGBA get_color();
      void set_line_style(LineStyle line_style);
      LineStyle get_line_style();
      void set_line_width(double line_width);
      double get_line_width();
      void show();
      void hide();
      bool is_showing() const;
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }
      void draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, bool log10_x, bool log10_y);
      friend class Plot2D;
    };
  }
}

#endif
