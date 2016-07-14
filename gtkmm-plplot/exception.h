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

#ifndef GTKMMPLPLOTEXCEPTION_H
#define GTKMMPLPLOTEXCEPTION_H

#include <string>
#include <exception>

namespace Gtk {
  namespace PLplot {
    /** \class Exception exception.h <gtkmm-plplot/exception.h>
     *  \brief Gtkmm-PLplot's exception handler class.
     *
     *  Gtkmm-PLplot has its own exception class: all methods that can throw exceptions, will always throw exceptions of this type.
     */
    class Exception: public std::exception {
    private:
      Glib::ustring Message; ///< The exception message
    public:
      /** Constructor
       *
       * Creates a new Gtkmm-PLplot exception.
       * \param s the exception message
       */
      explicit Exception(const Glib::ustring &s) : Message(s) {}

      /** destructor
       */
      virtual ~Exception() throw() {};
      // throw method
      /** Get the exception message
       *
       *  To be used in a catch block
       *  \return the exception message
       */
      virtual const char* what() const throw() {
        return Message.c_str();
      }
    };
  }
}
#endif
