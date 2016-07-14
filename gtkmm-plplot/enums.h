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

#ifndef GTKMMPLPLOTENUMS_H
#define GTKMMPLPLOTENUMS_H

namespace Gtk {
  namespace PLplot {
    /** Controls the line style of a PlotData2D dataset
     *
     * To be used in combination with PlotData2D constructors and the PlotData2D::set_line_style() and PlotData2D::get_line_style() methods.
     */
    enum LineStyle {
      NONE = 0, ///< no line will be plotted
      CONTINUOUS, ///< a continuous, uninterrupted line will be drawn
      SHORT_DASH_SHORT_GAP, ///< a line consisting of alternating short dashes and short gaps
      LONG_DASH_LONG_GAP, ///<  a line consisting of alternating long dashes and long gaps
      LONG_DASH_SHORT_GAP, ///<  a line consisting of alternating long dashes and short gaps
      LONG_DASH_SHORT_GAP_SHORT_DASH_SHORT_GAP, ///< a line consisting of repeating sequences of a long dash, a short gap, a short dash and a short gap
      LONG_DASH_SHORT_GAP_LONG_DASH_SHORT_GAP, ///< a line consisting of repeating sequences of a long dash, a short gap, a long dash and a short gap
      SHORT_DASH_SHORT_GAP_MEDIUM_DASH_MEDIUM_GAP_LONG_DASH_LONG_GAP, ///< a line consisting of repeating sequences of a short dash, short gap, medium dash, medium gap, long dash and a long gap
      SHORT_DASH_SHORT_GAP_MEDIUM_DASH_SHORT_GAP_LONG_DASH_SHORT_GAP ///< a line consisting of repeating sequences of a short dash, short gap, medium dash, short gap, long dash and a short gap
    };

    /** Controls the appearance of the box around the plot
     *
     * To be used in combination with Plot2D::set_box_style() and Plot2D::get_box_style()
     */
    enum BoxStyle {
      NO_BOX_NO_TICKS_NO_LABELS_NO_AXES = -2, ///< draw no box, no tick marks, no numeric tick labels, no axes
      BOX_ONLY = -1, ///< draw box only
      BOX_TICKS_TICK_LABELS = 0, ///< draw box, ticks, and numeric tick labels. This is the default
      BOX_TICKS_TICK_LABELS_MAIN_AXES = 1, ///< also draw coordinate axes at x=0 and y=0
      BOX_TICKS_TICK_LABELS_MAIN_AXES_MAJOR_TICK_GRID = 2, ///< also draw a grid at major tick positions in both coordinates
      BOX_TICKS_TICK_LABELS_MAIN_AXES_MAJOR_MINOR_TICK_GRID = 3, ///< also draw a grid at minor tick positions in both coordinates
    };

    /** Provides access to the different colormaps supported by PLplot
     *
     * Currently supported by PlotDataContour
     */
    enum ColormapPalette {
      DEFAULT = 0, ///< the default colormap
      BLUE_RED, ///< a gradient from blue to red
      BLUE_YELLOW, ///< a gradient from blue to yellow
      GRAY, ///< fifty shades of gray
      HIGHFREQ, ///< to check
      LOWFREQ, ///< to check
      RADAR, ///< to check
    };

    /** Provides access to the different area fill patterns supported by PlotDataContour
     *
     */
    enum AreaFillPattern {
      SOLID = 0, ///< solid fill of the area
      HORIZONTAL_LINES, ///< horizontal lines
      VERTICAL_LINES, ///< vertical lines
      UPWARD_LINES_45_DEGREES, ///< lines at 45 degrees angle upward
      DOWNWARD_LINES_45_DEGREES, ///< lines at 45 degrees angle downward
      UPWARD_LINES_30_DEGREES, ///< lines at 30 degrees angle upward
      DOWNWARD_LINES_30_DEGREES, ///< lines at 30 degrees angle downward
      HORIZONTAL_AND_VERTICAL_LINES, ///< both horizontal and vertical lines
      UPWARD_AND_DOWNWARD_LINES_45_DEGREES, ///< lines at 45 degrees angle upward and downward
    };

    /** Determines which corner of the Legend box that will be used for the normalized coordinate system.
     *
     */
    enum LegendCornerPosition {
      TOP_RIGHT = 0, ///< top right corner
      BOTTOM_RIGHT, ///< bottom right corner
      TOP_LEFT, ///< top left corner
      BOTTOM_LEFT, ///< bottom left corner
    };

  }
}
#endif
