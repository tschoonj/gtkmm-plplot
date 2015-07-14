#ifndef GTKMMPLPLOTUTILS_H
#define GTKMMPLPLOTUTILS_H

#include <vector>
#include <plstream.h>

namespace Gtk {
  namespace PLplot {
    std::vector<PLFLT> indgen(unsigned int n) {
      std::vector<PLFLT> rv(n);
      for (unsigned int i = 0 ; i < n ; i++)
        rv[i] = (PLFLT) i;
      return rv;
    }
  }
}


#endif
