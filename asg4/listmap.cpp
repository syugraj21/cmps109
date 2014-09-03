// $Id: listmap.cpp,v 1.2 2013-02-12 15:05:06-08 - - $

#include "listmap.h"

xcompare <string> compare;

template <typename key_t, typename value_t>
listmap <key_t, value_t>::node::node (const mapxpair &pair):
            pair(pair), prev(NULL), next(NULL) {
}

template <typename key_t, typename value_t>
listmap <key_t, value_t>::listmap (): head(NULL), tail (NULL) {
}

template <typename key_t, typename value_t>
listmap <key_t, value_t>::~listmap () {
   TRACE ('l', (void*) this);
   //iterator itor = begin ();
   //iterator iend = end ();
   listmap<key_t, value_t>::iterator itor = this->begin();
   listmap<key_t, value_t>::iterator end = this->begin();
   while (itor != iend) itor.erase ();
}

template <typename key_t, typename value_t>
xpair <key_t, value_t> &
listmap <key_t, value_t>::iterator::operator* () {
   TRACE ('l', where->pair);
   return where->pair;
}

template <typename key_t, typename value_t>
xpair <key_t, value_t> *
listmap <key_t, value_t>::iterator::operator-> () {
   TRACE ('l', where->pair);
   return &(where->pair);
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator &
listmap <key_t, value_t>::iterator::operator++ () {
   TRACE ('l', "First: " << map << ", " << where);
   TRACE ('l', "Second: " << map->head << ", " << map->tail);
   if (where == NULL){
      throw range_error ("cannot increment over NULL element"); 
   }
   where = where->next;
   return *this;
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator &
listmap <key_t, value_t>::iterator::operator-- () {
   if (where == NULL){
      throw range_error ("cannot decrement over NULL element");
   }
   where = where->prev;
   return *this;
}

template <typename key_t, typename value_t>
bool listmap <key_t, value_t>::iterator::operator==
            (const iterator &that) const {
   return this->where == that.where;
}


template <typename key_t, typename value_t>
bool listmap <key_t, value_t>::iterator::operator!=
            (const iterator &that) const {
   return this->where != that.where;
}

template <typename key_t, typename value_t>
listmap <key_t, value_t>::iterator::iterator ():
            map (NULL), where (NULL){
}

template <typename key_t, typename value_t>
listmap <key_t, value_t>::iterator::iterator (listmap *map,
            node *where): map (map), where (where){
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator
listmap <key_t, value_t>::begin () {
   return iterator (this, head);
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator
listmap <key_t, value_t>::rbegin() {
   return iterator (this, NULL);
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator
listmap <key_t, value_t>::end () {
   return iterator (this, NULL);
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator
listmap <key_t, value_t>::rend(){
   return iterator (this, tail);
}

template <typename key_t, typename value_t>
typename listmap <key_t, value_t>::iterator
listmap <key_t, value_t>::find (const key_t &key){
   listmap <key_t, value_t>::iterator itor = this->begin();
   listmap <key_t, value_t>::iterator end = this->end();
   while (itor != end){
      if (itor->first == key){
         return iterator (this,itor.where);
      }
   }
   return iterator (NULL, NULL);
}


template <typename key_t, typename value_t>
bool listmap <key_t, value_t>::empty () const {
   return head == NULL;
}

template <typename key_t, typename value_t>
void listmap <key_t, value_t>::iterator::erase () {
   if (where == NULL){
      throw range_error ("cannot delete empty listmap key");
   //if only one node
   } else if (where == map->head && where == map->tail){
      delete where;
      where = NULL;
      map->head = NULL;
      map->tail = NULL;
   //delete first node only  
   } else if (where == map->head){
      node temp = where->next;
      temp->prev = NULL;
      delete where;
      where = temp;
      map->head = temp;
   //delete last node only
   } else if (where == map->tail){
      node *temp = where;
      temp->next = NULL;
      delete where;
      where = temp;
      map->tail = temp;
   } else {
      node *ptemp = where->prev;
      node *ntemp = where->next;
      ptemp->next = ntemp;
      ntemp->prev = ptemp;
      delete where;
      where = ptemp;
   }
}

template <typename key_t, typename value_t>
void listmap<key_t, value_t>::insert
            (const xpair <key_t, value_t> &pair) {
   node *temp = new node (pair);
   if (empty ()) {
      head = tail = tmp;
   }else {
      node *itor = head;
      while (itor != NULL){
         int cmp = compare (itor->pair.first, pair.first);
         if (cmp == 1 && itor == head){
	    temp->next = itor;
	    temp->prev = NULL;
	    head->prev = temp;
	    head = temp;
	 } else if (cmps == 1){
	    node *prev_itor = itor->prev;
	    temp->next = itor;
	    itor->prev = temp;
	    temp->prev = prev_itor;
	    prev_itor->next = temp;
	 }
	 itor = itor->next;
      }
      /*cout << tail->pair.first << " cmp " << pair.first << " = "
           << cmp << endl;*/
      tail->next = tmp;
      tmp->prev = tail;
      tail = tmp;
   }
   TRACE ('l', &pair << "->" << pair);
}

#include "templates.tcc"


