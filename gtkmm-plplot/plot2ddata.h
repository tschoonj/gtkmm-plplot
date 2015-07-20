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

#ifndef GTKMMPLPLOTPLOT2DDATA_H
#define GTKMMPLPLOTPLOT2DDATA_H

#include <sigc++/sigc++.h>
#include <vector>
#include <valarray>
#include <gtkmm-plplot/linestyle.h>
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
      Plot2DData() = delete;
      Plot2DData &operator=(const Plot2DData &) = delete;
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
