// $Id: xpair.cpp,v 1.2 2013-02-12 15:05:06-08 - - $

#include <iostream>
#include <string>

using namespace std;

#include "xpair.h"
#include "trace.h"

template <typename first_t, typename second_t>
xpair <first_t, second_t>::xpair (const first_t &_first,
            const second_t &_second): first (_first), second (_second) {
   TRACE ('p', *this);
}

template <typename first_t, typename second_t>
ostream &operator<< (ostream &out,
                     const xpair <first_t, second_t> &that) {
   out << "[" << that.first << "," << that.second << "]";
   return out;
}

#include "templates.tcc"


