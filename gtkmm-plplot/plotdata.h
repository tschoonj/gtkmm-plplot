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

#ifndef GTKMMPLPLOT_PLOT_DATA_H
#define GTKMMPLPLOT_PLOT_DATA_H

#include <cairomm/cairomm.h>
#include <gtkmm-plplot/object.h>

class plstream;

#define PLOTDATA_DEFAULT_LINE_WIDTH 1.0

namespace Gtk {
  namespace PLplot {
    /** \class PlotData plotdata.h <gtkmm-plplot/plotdata.h>
     *  \brief the abstract base class that will hold a single dataset and its properties
     *
     *  The base class that all other PlotData classes ultimately derive from. Due to the wide range
     *  of plot data corresponding to the different plot types, this class offers only a few methods, meaning that
     *  the derived classes will have to contain most of their functionality themselves, as opposed to the plots.
     */
    class PlotData : public Object {
    private:
      Glib::ustring name; ///< the name that will be assigned to the dataset. Will be used to generate legends if supported by the Plot type
      bool shown; ///< \c true means the plot is currently visible, \c false means it is not plotted
      PlotData &operator=(const PlotData &) = delete; ///< no move assignment operator
      PlotData(const PlotData &source) = delete; ///< no default copy constructor;
    protected:
      sigc::signal<void> _signal_changed; ///< signal that gets emitted whenever any of the dataset proprties is changed.
      sigc::signal<void> _signal_data_modified; ///< signal that gets emitted whenever the X- and Y-datasets have been modified.
      /** This is a default handler for signal_changed()
       *
       * This signal is emitted whenever any of the dataset properties is changed.
       * Currently it does nothing but the signal will get caught by Plot, and will eventually trigger a redrawing of the entire widget.
       */
      virtual void on_changed();

      /** This is a default handler for signal_data_modified()
       *
       * This signal is emitted whenever any of the dataset proprties is changed.
       * Currently it does nothing but the signal will get caught by Plot, and will eventually trigger a redrawing of the entire widget, taking into account the new dataset.
       */
      virtual void on_data_modified();

      /** Constructor
       *
       */
      PlotData();
    public:
      /** Destructor
       *
       */
      virtual ~PlotData();

      /** Sets the name of the plot dataset
       *
       * This name will be used to construct a legend if supported by the Plot type.
       * If set to an empty string, a default name will be used.
       */
      void set_name(Glib::ustring name);

      /** Get the current name of the plot dataset
       *
       */
      Glib::ustring get_name();

      /** Make the dataset visible in the plot
       *
       */
      void show();

      /** Hide the dataset in the plot
       *
       */
      void hide();

      /** Returns whether or not the dataset is showing in the plot
       *
       * \return \c true if the dataset is visible, \c false if not
       */
      bool is_showing() const;

      /** signal_changed is emitted whenever any of the dataset properties has changed.
       *
       * See default handler on_changed()
       */
      sigc::signal<void> signal_changed() {
        return _signal_changed;
      }

      /** signal_data_modified is emitted whenever the X- and Y- arrays are modified.
       *
       * See default handler on_data_modified()
       */
      sigc::signal<void> signal_data_modified() {
        return _signal_data_modified;
      }

      /** Method to draw the dataset
       *
       * This method is virtual allowing inheriting classes to implement their own method with the same signature.
       * \param cr the cairo context to draw to.
       * \param pls the PLplot plstream object that will do the actual plotting on the Cairo context
       */
      virtual void draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls);
    };
  }
}


#endif
