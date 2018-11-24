// $Id: listmap.tcc,v 1.11 2018-01-25 14:19:14-08 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   while (!(empty()))
      erase(begin());
}

//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   Less less;
   listmap<Key,Value,Less>::iterator beg = begin();
   listmap<Key,Value,Less>::iterator en = end();
   
   if (empty())
   {
      node* new_node = new node(en.where, en.where, pair);
      end().where->next = new_node;
      end().where->prev = new_node;
      return begin();
   }
   while (beg != en && less(beg->first, pair.first))
   {
      ++beg;
   }
   if (beg != en && beg->first == pair.first)
   {
      beg->second = pair.second;
      return beg;
   }
   
   node* new_node = new node(beg.where, beg.where->prev, pair);
   new_node->prev->next = new_node;
   new_node->next->prev = new_node;
   return --beg;
}

//
// iterator listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   DEBUGF ('l', that);
   listmap<Key,Value,Less>::iterator beg = begin();
   listmap<Key,Value,Less>::iterator en = end();
   while (beg != en)
   {
      if (beg.where->value.first == that)
      {
         return beg;
      }
      ++beg;
   }
   return beg;
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   DEBUGF ('l', &*position);
   //iterator temp = end();
   if (position.where != nullptr)
   {
      //temp = ++position;
      position.where->prev->next = position.where->next;
      position.where->next->prev = position.where->prev;
      delete position.where;
   }
   return iterator();
}

//
//void listmap::print_map()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator 
listmap<Key, Value, Less>::print_map()
{
   listmap<Key,Value,Less>::iterator beg = begin();
   listmap<Key,Value,Less>::iterator en = end();
   while (beg != en)
   {
      cout << beg.where->value.first << " = " 
         << beg.where->value.second << endl;
      ++beg;
   }
   return beg;
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
//listmap::node* listmap::iterator::get_where()
//
template <typename Key, typename Value, class Less>
typename listmap<Key, Value, Less>::node*
listmap<Key, Value, Less>::iterator::get_where()
{
   return where;
}

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   DEBUGF ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   DEBUGF ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   DEBUGF ('l', where);
   
   //if (en.where == where)
   //{}
   //else
   //{
      where = where->next;
   //}
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   DEBUGF ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

//
// void listmap::iterator::erase()
//
template <typename Key, typename Value, class Less>
void listmap<Key, Value, Less>::iterator::erase()
{
   if (where != nullptr)
   {
      where->prev->next = where->next;
      where->next->prev = where->prev;
      delete where;
   }
}
