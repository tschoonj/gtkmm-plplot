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

#ifndef GTKMMPLPLOT_PLOT_OBJECT_H
#define GTKMMPLPLOT_PLOT_OBJECT_H

#include <cairomm/cairomm.h>
#include <gtkmm-plplot/object.h>

class plstream;

namespace Gtk {
  namespace PLplot {

    /** \struct PlotObjectAuxData plotobject.h <gtkmm-plplot/plotobject.h>
     *  \brief base class for auxiliary data to the drawing routine draw_plot_object()
     *
     *  It is empty by design: override this class and add variables that may be useful.
     */
    struct PlotObjectAuxData {
	
    };

    /** \class PlotObject plotobject.h <gtkmm-plplot/plotobject.h>
     *  \brief the abstract base class that will hold a single plot object and its properties
     *
     *  The base class that all other PlotObject classes ultimately derive from. Examples are PlotLine2D and PlotText2D
     */
    class PlotObject : public Object {
    private:
      bool shown; ///< \c true means the plot is currently visible, \c false means it is not plotted
      PlotObject &operator=(const PlotObject &) = delete; ///< no move assignment operator
      PlotObject(const PlotObject &source) = delete; ///< no default copy constructor;
    protected:
      sigc::signal<void> _signal_changed; ///< signal that gets emitted whenever any of the plot object properties is changed.

      /** This is a default handler for signal_changed()
       *
       * This signal is emitted whenever any of the object properties is changed.
       * Currently it does nothing but the signal will get caught by Plot, and will eventually trigger a redrawing of the entire widget.
       */
      virtual void on_changed();

      /** Constructor
       *
       */
      PlotObject();
    public:
      /** Destructor
       *
       */
      virtual ~PlotObject();

      /** Make the object visible in the plot
       *
       */
      void show();

      /** Hide the object in the plot
       *
       */
      void hide();

      /** Returns whether or not the object is showing in the plot
       *
       * \return \c true if the object is visible, \c false if not
       */
      bool is_showing() const;

      /** signal_changed is emitted whenever any of the object properties has changed.
       *
       * See default handler on_changed()
       */
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }

      /** Method to draw the object
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       * \param data additional data sent from the Plot to influence drawing
       */
      virtual void draw_plot_object(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls, PlotObjectAuxData &data) = 0;
    };
  }
}



#endif
