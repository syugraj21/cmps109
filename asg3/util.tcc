// $Id: util.tcc,v 1.1 2012-04-26 16:43:23-07 - - $

// 
// Explicit template instantiation.
//
// G++ is not always able to automatically instantiate certain
// templates, and so they must be instantiated explicitly.
// The errors that indicate that explicit instantiation is
// necessary occur at link time, not at run time, and produce
// messages like the following:
//
// object.cc:43: undefined reference to `std::basic_ostream<char,
// std::char_traits<char> >& operator<< <std::pair<inches,
// inches> >(std::basic_ostream<char, std::char_traits<char>
// >&, std::list<std::pair<inches, inches>,
// std::allocator<std::pair<inches, inches> > > const&)'
// collect2: ld returned 1 exit status
//
// Explicit instantiations must appear in the implementation file
// after the body of the instantiated entity.  In order to separate
// concerns, we put this information in a header file which is
// included at the *end* of the implementation file.
//

#include "numbers.h"

template ostream &operator<< (ostream &, const list<xycoords> &);
template double from_string <double> (const string &);


