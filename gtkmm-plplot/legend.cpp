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

#include <sigc++/sigc++.h>
#include <gtkmm-plplot/legend.h>
#include <gtkmm-plplot/exception.h>
#include <gtkmm-plplot/utils.h>
#include <gtkmm-plplot/plotdata2d.h>
#include <gtkmm-plplot/plot.h>
#include <cstdlib>
#include <glib.h>
#include <plstream.h>

using namespace Gtk::PLplot;

static std::vector<int> _legend_corner_positions = {
  PL_POSITION_TOP | PL_POSITION_RIGHT,
  PL_POSITION_BOTTOM | PL_POSITION_RIGHT,
  PL_POSITION_TOP | PL_POSITION_LEFT,
  PL_POSITION_BOTTOM | PL_POSITION_LEFT
};

Legend::Legend(
  LegendCornerPosition _legend_corner_position,
  double _legend_pos_x,
  double _legend_pos_y,
  Gdk::RGBA _legend_background_color,
  Gdk::RGBA _legend_bounding_box_color) :
  showing_legend(true),
  legend_pos_x(_legend_pos_x),
  legend_pos_y(_legend_pos_y),
  legend_background_color(_legend_background_color),
  legend_bounding_box_color(_legend_bounding_box_color),
  legend_corner_position(_legend_corner_position) {

  if (_legend_pos_x < 0.0 || _legend_pos_x > 1.0 ||
      _legend_pos_y < 0.0 || _legend_pos_y > 1.0)
    throw Exception("Gtk::PLplot::Legend::set_legend_position -> position must be expressed in normalized coordinates!");
}

Legend::~Legend() {}

void Legend::set_legend_background_color(Gdk::RGBA _legend_background_color) {
  if (legend_background_color == _legend_background_color)
    return;
  legend_background_color = _legend_background_color;
  dynamic_cast<Plot *>(this)->signal_changed().emit();
}

Gdk::RGBA Legend::get_legend_background_color() {
  return legend_background_color;
}

void Legend::set_legend_bounding_box_color(Gdk::RGBA _legend_bounding_box_color) {
  if (legend_bounding_box_color == _legend_bounding_box_color)
    return;
  legend_bounding_box_color = _legend_bounding_box_color;
  dynamic_cast<Plot *>(this)->signal_changed().emit();
}

Gdk::RGBA Legend::get_legend_bounding_box_color() {
  return legend_bounding_box_color;
}

void Legend::set_legend_position(double _legend_pos_x, double _legend_pos_y) {
  if (_legend_pos_x == legend_pos_x && _legend_pos_y == legend_pos_y)
    return;

  if (_legend_pos_x < 0.0 || _legend_pos_x > 1.0 ||
      _legend_pos_y < 0.0 || _legend_pos_y > 1.0)
    throw Exception("Gtk::PLplot::Legend::set_legend_position -> position must be expressed in normalized coordinates!");

  legend_pos_x = _legend_pos_x;
  legend_pos_y = _legend_pos_y;
  dynamic_cast<Plot *>(this)->signal_changed().emit();
}

void Legend::get_legend_position(double &_legend_pos_x, double &_legend_pos_y) {
  _legend_pos_x = legend_pos_x;
  _legend_pos_y = legend_pos_y;
}

void Legend::set_legend_corner_position(LegendCornerPosition _legend_corner_position) {
  if (legend_corner_position == _legend_corner_position)
    return;
  legend_corner_position = _legend_corner_position;
  dynamic_cast<Plot *>(this)->signal_changed().emit();
}

LegendCornerPosition Legend::get_legend_corner_position() {
  return legend_corner_position;
}

void Legend::show_legend() {
  if (showing_legend)
    return;
  showing_legend = true;
  dynamic_cast<Plot *>(this)->signal_changed().emit();
}

void Legend::hide_legend() {
  if (!showing_legend)
    return;
  showing_legend = false;
  dynamic_cast<Plot *>(this)->signal_changed().emit();
}

bool Legend::is_showing_legend() {
  return showing_legend;
}

void Legend::draw_legend(
  const Cairo::RefPtr<Cairo::Context> &cr,
  std::vector<PlotData *> &plot_data,
  plstream *pls) {

  double legend_width, legend_height;

  int opt = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX;
  std::vector<int> opt_array;
  std::vector<int> text_colors;
  int line_style;
  double line_width;
  std::vector<int> line_colors;
  std::vector<int> line_styles;
  std::vector<double> line_widths;
  double symbol_scale;
  std::vector<int> symbol_colors;
  std::vector<double> symbol_scales;
  std::vector<int> symbol_numbers;


  char **text = NULL; /* let's use plain old C for this */
  char **symbols = NULL; /* let's use plain old C for this */

  int index = 0;

  for (auto &iter : plot_data) {
    auto iter2 = dynamic_cast<PlotData2D *>(iter);

    if (iter2 == nullptr)
      throw Exception("Gtk::PLplot::Legend::draw_legend -> could not perform dynamic_cast to PlotData2D");

    /* no need to add it to the legend if it's not visible! */
    if (!iter2->is_showing() || (
        iter2->get_line_style() == NONE &&
        iter2->get_symbol().empty()))
      continue;

    int my_opt_array = 0;

    symbols = (char **) g_realloc(symbols, sizeof(char *) * (opt_array.size() + 2));

    if (iter2->get_line_style() != NONE) {
      my_opt_array |= PL_LEGEND_LINE;
      change_plstream_color(pls, iter2->get_color(), false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX + 2 + (2 * index));
      line_style = iter2->get_line_style();
      line_width = iter2->get_line_width();
    }
    if (!iter2->get_symbol().empty()) {
      my_opt_array |= PL_LEGEND_SYMBOL;
      change_plstream_color(pls, iter2->get_symbol_color(), false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX + 2 + (2 * index) + 1);
      symbol_scale = iter2->get_symbol_height_scale_factor();
      symbols[index] = g_strdup(iter2->get_symbol().c_str());
      symbols[index + 1] = NULL;
    }
    else {
      symbols[index] = g_strdup("");
      symbols[index + 1] = NULL;
    }

    opt_array.push_back(my_opt_array);
    line_colors.push_back(GTKMM_PLPLOT_DEFAULT_COLOR_INDEX + 2 + (2 * index));
    line_styles.push_back(line_style);
    line_widths.push_back(line_width);
    symbol_colors.push_back(GTKMM_PLPLOT_DEFAULT_COLOR_INDEX + 2 + (2 * index) + 1);
    symbol_scales.push_back(symbol_scale);

    text = (char **) realloc(text, sizeof(char *) * (opt_array.size() + 1));
    if (iter2->get_name().empty()) {
      text[index] = g_strdup_printf("Data %i", index);
    }
    else {
      text[index] = g_strdup(iter2->get_name().c_str());
    }
    text[index + 1] = NULL;

    index++;
  }

  auto nlegend = opt_array.size();

  if (nlegend == 0) {
    //no data was found
    //cleanup
    g_strfreev(text);
    g_strfreev(symbols);
    return;
  }

  text_colors.assign(nlegend, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX +1);
  symbol_numbers.assign(nlegend, 3);


  change_plstream_color(pls, legend_background_color, false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX);
  change_plstream_color(pls, legend_bounding_box_color, false, GTKMM_PLPLOT_DEFAULT_COLOR_INDEX + 1);

  pls->width(1.0);

  pls->legend(
    &legend_width,
    &legend_height,
    opt,
    _legend_corner_positions[legend_corner_position],
    legend_pos_x,
    legend_pos_y,
    0.1, /* plot width */
    GTKMM_PLPLOT_DEFAULT_COLOR_INDEX,
    GTKMM_PLPLOT_DEFAULT_COLOR_INDEX + 1,
    1, /* bounding box line style */
    0, /* nrow */
    0, /* ncolumn */
    nlegend,
    &opt_array[0],
    1.0, /* text offset */
    1.0, /* text scale */
    2.0, /* text spacing */
    0.0, /* text justification */
    &text_colors[0],
    (const char * const *) text,
    NULL, NULL, NULL, NULL, /* box stuff that we are not using */
    &line_colors[0], &line_styles[0], &line_widths[0],
    &symbol_colors[0], &symbol_scales[0], &symbol_numbers[0], (const char * const *) symbols);


  //cleanup
  g_strfreev(text);
  g_strfreev(symbols);
}
