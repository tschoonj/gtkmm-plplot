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

#include <gtkmm-plplot/plotdataabstract.h>

using namespace Gtk::PLplot;

PlotDataAbstract::PlotDataAbstract() :
  shown(true) {

  this->signal_changed().connect(sigc::mem_fun(*this,
               &PlotDataAbstract::on_changed));
  this->signal_data_modified().connect(sigc::mem_fun(*this,
               &PlotDataAbstract::on_data_modified));

}

PlotDataAbstract::~PlotDataAbstract() {

}

void PlotDataAbstract::on_changed() {
  //do nothing
}

void PlotDataAbstract::on_data_modified() {
  //do nothing
}

void PlotDataAbstract::show() {
  shown = true;
  _signal_changed.emit();
}

void PlotDataAbstract::hide() {
  shown = false;
  _signal_changed.emit();
}

bool PlotDataAbstract::is_showing() const {
  return shown;
}
