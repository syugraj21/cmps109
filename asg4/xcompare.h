// $Id: xcompare.h,v 1.1 2013-02-12 14:51:41-08 - - $

#ifndef __XCOMPARE_H__
#define __XCOMPARE_H__

#include "trace.h"

//
// We assume that the type type_t has a compare member function.
//

template <typename type_t>
struct xcompare {
   int operator() (const type_t &left, const type_t &right) const;
};

#endif


