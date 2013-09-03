#pragma once










#include "collection_pair.h"


#include "collection_array.h"
#include "collection_raw_array.h"
#include "collection_smart_pointer_array.h"
#include "collection_comparable_eq_array.h"
#include "collection_comparable_array.h"
#include "collection_comparable_raw_array.h"
#include "collection_comparable_primitive_array.h"

#include "base/ca2/ca2_byte_serializable.h"

#include "collection_primitive_array.h"
#include "collection_numeric_array.h"

#include "collection_logic_array.h"

#include "collection_list.h"
#include "collection_comparable_eq_list.h"
#include "collection_comparable_list.h"


#include "collection_typed_ptr_array.h"
#include "collection_typed_pointer_array.h"


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
   // Arrays
   class byte_array;           // array of BYTE
   class uint16_array;           // array of WORD
   class uint32_array;          // array of uint32_t
   class CUIntArray;           // array of UINT
   class CPtrArray;            // array of void *
   class CObArray;             // array of object*

   // Lists
   class pointer_list;             // list of void *
   class object_list;              // list of object*

   // Maps (aka Dictionaries)
   class CMapWordToOb;         // map from WORD to object*
   class CMapWordToPtr;        // map from WORD to void *
   class CMapPtrToWord;        // map from void * to WORD
   class map_ptr_to_ptr;         // map from void * to void *

   // Special string variants
   class string_list;          // list of Strings
   class CMapStringToPtr;      // map from string to void *
   class CMapStringToOb;       // map from string to object*
   class string_to_string_map;   // map from string to string







#include "collection_pointer_list.h"
#include "collection_object_list.h"
#include "collection_typed_ptr_list.h"
#include "collection_typed_pointer_list.h"
#include "collection_string_list.h"


class CLASS_DECL_c map_word_to_ptr :
   virtual public map < WORD, WORD, void *, void * >
{
public:
   map_word_to_ptr(sp(base_application) papp = NULL, ::count nBlockSize = 10);
};

class CLASS_DECL_c map_ptr_to_word :
   virtual public map < void *, void *, WORD, WORD >
{
public:
   map_ptr_to_word(sp(base_application) papp = NULL, ::count nBlockSize = 10);
};


class CLASS_DECL_c map_ptr_to_ptr :
   virtual public map < void *, void *, void *, void * >
{
public:
   map_ptr_to_ptr(sp(base_application) papp = NULL, ::count nBlockSize = 10);
};

class CLASS_DECL_c map_word_to_ob :
   virtual public map < WORD, WORD, object *, object * >
{
public:
   map_word_to_ob(sp(base_application) papp = NULL, ::count nBlockSize = 10);
};


class CLASS_DECL_c map_string_to_ptr :
   virtual public map < string, const string &, void *, void * >
{
public:
   map_string_to_ptr(sp(base_application) papp = NULL, ::count nBlockSize = 10);
};

class CLASS_DECL_c map_string_to_ob :
   virtual public map < string, const string &, object *, object * >
{
public:
   map_string_to_ob(sp(base_application) papp = NULL, ::count nBlockSize = 10);
};





#include "collection_string_to_string_map.h"
#include "collection_bit_array.h"
#include "collection_string_array.h"

#include "c/datetime/datetime_time.h"

#include "primitive/primitive_var.h"

#include "collection_stringa.h"


#include "collection_var_array.h"
#include "c/primitive/primitive_property.h"




#include "c/primitive/primitive_signal.h"
#include "c/primitive/primitive_property_set.h"


#include "collection_point_array.h"
#include "collection_rect_array.h"


#include "collection_string_to_string_map.h"


#include "collection_typed_pointer_map.h"

// inline
#include "collection.inl"


class CLASS_DECL_c dword_2darray :
   public spa(uint32_array)
{
public:
};

class CLASS_DECL_c index_2darray :
   public spa(index_array)
{
public:
};



#include "collection_sort.h"

#include "collection_id_array.h"

#include "collection_array.h"

#include "collection_bitset.h"

#include "collection_packset.h"

#include "collection_spreadset.h"

#include "collection_deque.h"

#include "collection_queue.h"

#include "collection_priority_queue.h"

class CLASS_DECL_c time_array :
   public array < ::datetime::time, ::datetime::time & >
{
};


#include "collection_buffer.h"

#include "collection_stringl.h"
#include "collection_string_sort_array.h"
#include "collection_stringa.h"

#include "collection_numeric_array.h"

#include "collection_lemon_array.h"







#include "c/collection/collection_base_enum.h"


