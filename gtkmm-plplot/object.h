/*
Copyright (C) 2016 Tom Schoonjans

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

#ifndef GTKMM_PLPLOT_OBJECT_H
#define GTKMM_PLPLOT_OBJECT_H

#include <glibmm/object.h>
#include <iostream>

namespace Gtk {
  namespace PLplot {
    /** \class Object object.h <gtkmm-plplot/object.h>
     *  \brief an abstract base class for all plots and plotdata's
     *
     *  This is the base class from which all other plots classes are derived.
     *  Through Glib::Object, it derives from libsigc++'s trackable to provide signalling support,
     *  and allows for memory management using Gtkmm's Gtk::manage method.
     */
     class Object : public Glib::Object {
     private:
       bool managed; ///< \c true means memory managed by us, \c false indicates otherwise
       //disable copy construction and assignment operator
       Object(const Object &); ///< disable copy constructor
       Object &operator=(const Object &) = delete; ///< disable assignment operator
     protected:
       /** Constructor
        *
        * Newly constructed instances have managed set to \c false
        */
       Object() : Glib::ObjectBase("GtkmmPLplotObject"), managed(false) {
         //std::cout << "Gtkmm::PLplot::Object constructor called" << std::endl;
       }
     public:
       /** Marks that this Object is managed by its Canvas or Plot container.
        *
        * This method should not be used directly: the user is strongly encouraged
        * to use Gtk::manage instead.
        */
       virtual void set_manage() {
         managed = true;
       }
       #ifndef DOXYGEN_SHOULD_SKIP_THIS
       bool is_managed_() const {
         return managed;
       }
       #endif
     };
  }
}
#endif
