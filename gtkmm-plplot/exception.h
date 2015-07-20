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

#ifndef GTKMMPLPLOTEXCEPTION_H
#define GTKMMPLPLOTEXCEPTION_H

#include <string>
#include <exception>

namespace Gtk {
  namespace PLplot {
    class Exception: public std::exception {
    private:
      std::string Message;
    public:
      // constructors
      //explicit Exception(const char *ch) : Message(ch) {}
      explicit Exception(const std::string &s) : Message(s) {}
      virtual ~Exception() throw() {};
      // throw method
      virtual const char* what() const throw() {
        return Message.c_str();
      }
    };
  }
}
#endif
