// $Id: xcompare.cpp,v 1.2 2013-02-12 15:05:06-08 - - $

#include <string>
using namespace std;

#include "xcompare.h"
#include "trace.h"

template <typename type_t>
int xcompare<type_t>::operator() (const type_t &left,
                                  const type_t &right) const {
   int result = left.compare (right);
   TRACE ('c', "compare (" << left << ", " << right
          << ") = " << result << endl);
   return result;
}

#include "templates.tcc"


