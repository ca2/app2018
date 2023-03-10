#pragma once



#define forall(set) for(auto item : set)
#define forallref(set) for(auto & item : set)



//#include "collection_iterator.h"

#include "collection_pair.h"


// Range-based for loop support // Thank you PUGIXML
template < typename CONST_ITERATOR >
class const_range
{
public:

   typedef CONST_ITERATOR const_iterator;

   CONST_ITERATOR m_begin;
   CONST_ITERATOR m_end;

   const_range(CONST_ITERATOR b,CONST_ITERATOR e): m_begin(b),m_end(e) {}

   CONST_ITERATOR begin() const { return m_begin; }
   CONST_ITERATOR end() const { return m_end; }


   typename CONST_ITERATOR::TYPE & element_at(::index iIndex)
   {

      CONST_ITERATOR it = begin();

      while(iIndex > 0 && it != end())
      {

         iIndex--;

      }

      if(iIndex == 0)
      {

         return (*it);

      }
      else
      {

         _throw(invalid_argument_exception(get_app()));

      }

   }
};


// Range-based for loop support // Thank you PUGIXML
template < typename ITERATOR >
class range
{
public:

   typedef ITERATOR const_iterator;

   ITERATOR m_begin;
   ITERATOR m_end;

   range(ITERATOR b,ITERATOR e): m_begin(b),m_end(e) { }

   ITERATOR begin() const { return m_begin; }
   ITERATOR end() const { return m_end; }

   typename ITERATOR::TYPE& element_at(::index iIndex)
   {

      ITERATOR it = begin();

      while(iIndex > 0 && it != end())
      {

         iIndex--;

      }

      if(iIndex == 0)
      {

         return (*it);

      }
      else
      {

         _throw(invalid_argument_exception(get_app()));

      }

   }

};


//#include "collection_comparable_eq_array.h"
//#include "collection_comparable_array.h"
#include "collection_comparable_raw_array.h"
#include "collection_comparable_primitive_array.h"


#include "collection_primitive_array.h"
#include "collection_numeric_array_range.h"
#include "collection_numeric_array.h"

#include "collection_logic_array.h"

#include "collection_list.h"
#include "collection_comparable_eq_list.h"
#include "collection_comparable_list.h"


#include "collection_map_association.h"


#include "collection_map.h"
#include "collection_attrib_map.h"
#include "collection_int_map.h"
#include "collection_string_map.h"
#include "collection_strid_map.h"
#include "collection_map_to_smart_pointer.h"
#include "collection_fifo_map.h"
#include "collection_sort_map.h"


#include "collection_sort_array.h"


/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file

//object

// Lists
class pointer_list;             // list of void *
class object_list;              // list of object*

//   // Maps (aka Dictionaries)
//   class CMapWordToOb;         // map from WORD to object*
//   class CMapWordToPtr;        // map from WORD to void *
//   class CMapPtrToWord;        // map from void * to WORD
//   class map_ptr_to_ptr;         // map from void * to void *
//
//   // Special string variants
//   class string_list;          // list of Strings
//   class CMapStringToPtr;      // map from string to void *
//   class CMapStringToOb;       // map from string to object*
////   class string_to_string;   // map from string to string




#include "collection_string_list.h"


class CLASS_DECL_AURA map_word_to_ptr :
   virtual public map < WORD, WORD, void *, void * >
{
public:
   map_word_to_ptr(::aura::application * papp = NULL);
};

class CLASS_DECL_AURA map_ptr_to_word :
   virtual public map < void *, void *, WORD, WORD >
{
public:
   map_ptr_to_word(::aura::application * papp = NULL);
};


class CLASS_DECL_AURA map_ptr_to_ptr :
   virtual public map < void *, void *, void *, void * >
{
public:
   map_ptr_to_ptr(::aura::application * papp = NULL);
};

class CLASS_DECL_AURA map_word_to_ob :
   virtual public map < WORD, WORD, object *, object * >
{
public:
   map_word_to_ob(::aura::application * papp = NULL);
};


class CLASS_DECL_AURA map_string_to_ptr :
   virtual public map < string, const string &, void *, void * >
{
public:
   map_string_to_ptr(::aura::application * papp = NULL);
};

class CLASS_DECL_AURA map_string_to_ob :
   virtual public map < string, const string &, object *, object * >
{
public:
   map_string_to_ob(::aura::application * papp = NULL);
};





#include "collection_string_to_string_map.h"
#include "collection_bit_array.h"
#include "collection_string_array.h"

#include "aura/primitive/primitive_var.h"

#include "collection_stringa.h"


#include "collection_var_array.h"
#include "aura/primitive/primitive_property.h"




#include "aura/aura/message/message.h"
#include "aura/primitive/primitive_property_set.h"
#include "aura/primitive/primitive_relation_set.h"


#include "collection_point_array.h"
#include "collection_rect_array.h"





typedef CLASS_DECL_AURA spa(uint_array) dword_2darray;
typedef CLASS_DECL_AURA spa(index_array) index_2darray;



#include "collection_sort.h"

#include "collection_id_array.h"

#include "collection_bitset.h"

#include "collection_packset.h"

#include "collection_spreadset.h"

#include "collection_deque.h"

#include "collection_queue.h"

#include "collection_priority_queue.h"

class CLASS_DECL_AURA time_array :
   public array < ::datetime::time, ::datetime::time & >
{
};


#include "collection_stringl.h"
//#include "collection_string_sort_array.h"
//#include "collection_stringa.h"

#include "collection_lemon_array.h"







#include "collection_unique_array.h"

#include "collection_stack.h"

#include "collection_set.h"


#include "collection_base_2array.h"





