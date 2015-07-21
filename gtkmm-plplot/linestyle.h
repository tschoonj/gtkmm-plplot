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

#ifndef GTKMMPLPLOTLINESTYLE_H
#define GTKMMPLPLOTLINESTYLE_H

namespace Gtk {
  namespace PLplot {
    /** Controls the line style of a Plot2DData dataset
     *
     * To be used in combination with Plot2DData constructors and the Plot2DData::set_line_style() and Plot2DData::get_line_style() methods.
     */
    enum LineStyle {
      NONE = 0, ///< No line will be plotted
      CONTINUOUS, ///< a continuous, uninterrupted line will be drawn
      SHORT_DASH_SHORT_GAP, ///< a line consisting of alternating short dashes and short gaps
      LONG_DASH_LONG_GAP, ///<  a line consisting of alternating long dashes and long gaps
      LONG_DASH_SHORT_GAP, ///<  a line consisting of alternating long dashes and short gaps
      LONG_DASH_SHORT_GAP_SHORT_DASH_SHORT_GAP, ///< a line consisting of repeating sequences of a long dash, a short gap, a short dash and a short gap
      LONG_DASH_SHORT_GAP_LONG_DASH_SHORT_GAP, ///< a line consisting of repeating sequences of a long dash, a short gap, a long dash and a short gap
      SHORT_DASH_SHORT_GAP_MEDIUM_DASH_MEDIUM_GAP_LONG_DASH_LONG_GAP, ///< a line consisting of repeating sequences of a short dash, short gap, medium dash, medium gap, long dash and a long gap
      SHORT_DASH_SHORT_GAP_MEDIUM_DASH_SHORT_GAP_LONG_DASH_SHORT_GAP ///< a line consisting of repeating sequences of a short dash, short gap, medium dash, short gap, long dash and a short gap
    };
  }
}
#endif
