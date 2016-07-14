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

#include <gtkmm-plplot/plotdata.h>

using namespace Gtk::PLplot;

PlotData::PlotData() :
  Glib::ObjectBase("GtkmmPLplotPlotData"),
  name(""),
  shown(true) {

  this->signal_changed().connect(sigc::mem_fun(*this,
               &PlotData::on_changed));
  this->signal_data_modified().connect(sigc::mem_fun(*this,
               &PlotData::on_data_modified));

}

PlotData::~PlotData() {}

void PlotData::on_changed() {
  //do nothing
}

void PlotData::on_data_modified() {
  //do nothing
}

void PlotData::set_name(Glib::ustring _name) {
  name = _name;
  _signal_changed.emit();
}

Glib::ustring PlotData::get_name() {
  return name;
}

void PlotData::show() {
  shown = true;
  _signal_changed.emit();
}

void PlotData::hide() {
  shown = false;
  _signal_changed.emit();
}

bool PlotData::is_showing() const {
  return shown;
}

void PlotData::draw_plot_data(const Cairo::RefPtr<Cairo::Context> &cr, plstream *pls) {
  //default is to do nothing -> only PlotMulti descendants should override this
}
