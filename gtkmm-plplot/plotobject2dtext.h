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

#ifndef GTKMMPLPLOT_PLOT_OBJECT_2D_TEXT_H
#define GTKMMPLPLOT_PLOT_OBJECT_2D_TEXT_H

#include <gtkmm-plplot/plotobject2d.h>
#include <gtkmm/orientable.h>

namespace Gtk {
  namespace PLplot {
    /** \class PlotObject2DText plotobject2dtext.h <gtkmm-plplot/plotobject2dtext.h>
     *  \brief Text object for 2D plots 
     *
     * This object is used in \ref example2
     */
    class PlotObject2DText : public ::Gtk::Orientable, public PlotObject2D {
    private:
      PlotObject2DText() = delete; ///< no default constructor
      PlotObject2DText &operator=(const PlotObject2DText &) = delete; ///< no assignment operator
      PlotObject2DText(const PlotObject2DText &source) = delete; ///< no default copy constructor;
    protected:
      double justification; ///< text justification. Must be [0,1]
      Glib::ustring text; ///< text that should be plotted
      double coords[2]; ///< text coordinates
      double scale_factor; ///< text scale factor
    public:
      /** Constructor
       *
       * This constructor initializes a new PlotObject2DText instance, defined by its contents (text) as well as its position
       * and color. More properties can be set using the other methods that are offered by this class.
       * \param text What will be printed on the plot
       * \param coord_x X-coordinate of the text
       * \param coord_y Y-coordinate of the text
       * \param color The color text color
       */
      PlotObject2DText(Glib::ustring text,
                       double coord_x,
                       double coord_y,
                       Gdk::RGBA color = Gdk::RGBA("black"));

      /** Destructor
       *
       */
      virtual ~PlotObject2DText();

      /** Get current text justification
       *
       * 0.0 corresponds to left justified, 1.0 to right justified. Only values in [0.0, 1.0] are valid
       * \return Current text justification
       */
      double get_justification();

      /** Set text justification
       *
       * 0.0 corresponds to left justified, 1.0 to right justified. Only values in [0.0, 1.0] are valid
       * \param justification New text justification
       * \exception Gtk::PLplot::Exception
       */
      void set_justification(double justification);

      /** Get current text
       *
       * \return Current text
       */
      Glib::ustring get_text();

      /** Set new text
       *
       * \param text New text
       */
      void set_text(Glib::ustring text);

      /** Get current text coordinates
       *
       * \param coord_x X-coordinate
       * \param coord_y Y-coordinate
       */
      void get_coordinates(double &coord_x, double &coord_y);

      /** Set new text coordinates
       *
       * \param coord_x X-coordinate
       * \param coord_y Y-coordinate
       */
      void set_coordinates(double coord_x, double coord_y);

      /** Get current text scale factor
       *
       * Use this parameter to get the current text font size
       * \return Text scale factor
       */
      double get_scale_factor();

      /** Set text scale factor
       *
       * Use this parameter to increase or decrease the text font size
       * \param scale_factor New text scale factor
       */
      void set_scale_factor(double scale_factor);

      /** Check if the object can be used in a plot with a logarithmic X-axis
       *
       * \return \c true if compatible, \c false if not
       */
      virtual bool is_logarithmic_x_compatible() override;

      /** Check if the object can be used in a plot with a logarithmic Y-axis
       *
       * \return \c true if compatible, \c false if not
       */
      virtual bool is_logarithmic_y_compatible() override;

      /** Method to draw the object
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       * \param data additional data sent from the Plot to influence drawing
       */
      virtual void draw_plot_object(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, PlotObjectAuxData &data) override;
    };
  }
}
#endif
