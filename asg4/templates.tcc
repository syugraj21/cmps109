// $Id: templates.tcc,v 1.1 2013-02-12 15:05:06-08 - - $

// Explicit template instantiations.

#ifdef __LISTMAP_H__
template class listmap <string, string>;
#endif

#ifdef __UTIL_H__
template string to_string <int> (const int &);
#endif

#ifdef __XPAIR_H__
template class xpair <string, string>;
#endif

#ifdef __XCOMPARE_H__
template class xcompare<string>;
#endif

