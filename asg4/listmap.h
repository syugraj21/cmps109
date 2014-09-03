// $Id: listmap.h,v 1.1 2013-02-12 14:51:41-08 - - $

#ifndef __LISTMAP_H__
#define __LISTMAP_H__

#include "xcompare.h"
#include "xpair.h"

template <typename key_t, typename value_t>
class listmap {
   private:
      struct node {
         xpair <key_t, value_t> pair;
         node *prev;
         node *next;
         node (const xpair <key_t, value_t> &);
      };
      node *head;
      node *tail;
   public:
      typedef xpair <key_t, value_t> mapxpair;
      class iterator;
      listmap ();
      listmap (const listmap &);
      listmap &operator= (const listmap &);
      ~listmap ();
      void insert (const xpair <key_t, value_t> &);
      iterator find (const key_t &) const;
      iterator begin ();
      iterator rbegin ();
      iterator end ();
      iterator rend();
      bool empty () const;
};

template <typename key_t, typename value_t>
class listmap<key_t, value_t>::iterator {
      friend class listmap<key_t, value_t>;
   private:
      iterator (listmap *map, node *where);
      listmap *map;
      node *where;
   public:
      mapxpair &operator* ();
      mapxpair *operator-> ();
      iterator &operator++ (); //++itor
      iterator &operator-- (); //--itor
      bool operator== (const iterator &) const;
      bool operator!= (const iterator &) const;
      void erase ();
      iterator ();
};

#endif


