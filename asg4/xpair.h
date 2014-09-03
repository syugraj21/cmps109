// $Id: xpair.h,v 1.1 2013-02-12 14:51:41-08 - - $

#ifndef __XPAIR_H__
#define __XPAIR_H__

#include "trace.h"

//
// Class xpair works like pair(3c++).
//

template <typename first_t, typename second_t>
struct xpair {
   xpair (const first_t &, const second_t &);
   first_t first;
   second_t second;
};

template <typename first_t, typename second_t>
ostream &operator<< (ostream &, const xpair <first_t, second_t> &);

//
// The following implicitly generated members will work,
// because they just send messages to the first and second
// fields, respectively.
//
// Caution:  xpair() does not initialize its fields unless
// first_t and second_t do so with their default ctors.
//
// xpair ();
// xpair (const xpair &);
// xpair &operator= (const xpair &);
// ~xpair ();
//

#endif


