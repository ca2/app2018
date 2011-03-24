#pragma once


#undef new


#include "radix/radix_object.h"


template<class TYPE, class ARG_TYPE = const TYPE&>
class base_array : 
   virtual public ::radix::object
{
public:
   base_array();
   base_array(const base_array <TYPE, ARG_TYPE> & a);
   virtual ~base_array();

// Attributes
   count get_size() const;
   count get_count() const;
   bool is_empty(count countMinimum = 1) const;
   bool has_elements(count countMinimum = 1) const;
   index get_upper_bound() const;
   count set_size(count nNewSize, count nGrowBy = -1);

   void free_extra();



   // Accessing elements
   const TYPE& get_at(index nIndex) const;
   TYPE& get_at(index nIndex);
   void set_at(index nIndex, ARG_TYPE newElement);
   const TYPE& element_at(index nIndex) const;
   TYPE& element_at(index nIndex);

   // Direct Access to the element data (may return NULL)
   const TYPE* get_data() const;
   TYPE* get_data();

   // Potentially growing the base_array
   void set_at_grow(index nIndex, ARG_TYPE newElement);
   virtual index add_new(count count = 1);
   virtual index add(ARG_TYPE newElement);
   virtual count add(const base_array& src); // for disambiguation
   virtual count add_array(const base_array& src);
   void copy(const base_array& src);

   // overloaded operator helpers
   const TYPE& operator[](index nIndex) const;
   TYPE& operator[](index nIndex);

   // Operations that move elements around
   void insert_at(index nIndex, ARG_TYPE newElement, count nCount = 1);
   void remove_at(index nIndex, count nCount = 1);
   void insert_at(index nStartIndex, base_array* pNewArray);
   void insert_array_at(index nStartIndex, base_array* pNewArray); // for disambiguation
   void remove_last();

   
   void increment_size(count add_up = 1);
   
   TYPE & first_element();
   const TYPE & first_element() const;
   TYPE & last_element();
   const TYPE & last_element() const;
   count remove_all(bool bResize = false);

   index find_first(ARG_TYPE t, index (* lpfnCompare)(ARG_TYPE, ARG_TYPE), index start = 0, index last = -1) const;
   index raw_find_first(TYPE * pt, index first = 0, index last = -1) const;

   template < class DERIVED >
   index find_first(DERIVED * pt, index first = 0, index last = -1)
   {
      return raw_find_first(dynamic_cast < TYPE * > (pt), first, last);
   }

   void swap(index i1, index i2);

   base_array <TYPE, ARG_TYPE> & operator = (const base_array <TYPE, ARG_TYPE> & a);

   TYPE pop();
   void push(ARG_TYPE t);


// Implementation
protected:
   TYPE *   m_pData;   // the actual base_array of data
   count    m_nSize;     // # of elements (upperBound - 1)
   count    m_nMaxSize;  // max allocated
   count    m_nGrowBy;   // grow amount

public:
//   void Serialize(CArchive&);
#ifdef _DEBUG
   void dump(dump_context&) const;
   void assert_valid() const;
#endif
};

template<class TYPE, class ARG_TYPE>
base_array<TYPE, ARG_TYPE>::base_array(const base_array<TYPE, ARG_TYPE> & a)
{
   m_pData = NULL;
   m_nSize = m_nMaxSize = m_nGrowBy = 0;
   operator =(a);
}

template<class TYPE, class ARG_TYPE>
void base_array<TYPE, ARG_TYPE>::
increment_size(count iAddUp)
{
    set_size(get_size() + iAddUp);
}

template<class TYPE, class ARG_TYPE>
TYPE & base_array<TYPE, ARG_TYPE>::
last_element()
{
    ASSERT(get_size() > 0);
    return element_at(get_size() - 1);
}

template<class TYPE, class ARG_TYPE>
const TYPE & base_array<TYPE, ARG_TYPE>::
last_element() const
{
    ASSERT(get_size() > 0);
    return element_at(get_size() - 1);
}

template<class TYPE, class ARG_TYPE>
TYPE & base_array<TYPE, ARG_TYPE>::
first_element()
{
    ASSERT(get_size() > 0);
    return element_at(0);
}

template<class TYPE, class ARG_TYPE>
const TYPE & base_array<TYPE, ARG_TYPE>::
first_element() const
{
    ASSERT(get_size() > 0);
    return get_at(0);
}

template <class TYPE, class ARG_TYPE>
count base_array<TYPE, ARG_TYPE>::
remove_all(bool bResize)
{
   count countOld = get_count();
   if(bResize)
   {
      set_size(0, -1);
   }
   else
   {
      remove_at(0, m_nSize);
   }
   return countOld;
}

template <class TYPE, class ARG_TYPE>
void base_array<TYPE, ARG_TYPE>::swap(index i1, index i2)
{
    TYPE t;
    t = element_at(i1);
    element_at(i1) = element_at(i2);
    element_at(i2) = t;
}

template <class TYPE, class ARG_TYPE>
index base_array<TYPE, ARG_TYPE>::raw_find_first(TYPE *pt, index find, index last) const
{
   if(find < 0)
      find += get_count();
   if(last < 0)
      last += get_count();
   for(; find <= last; find++)
   {
      if(&element_at(find) == pt)
         return find;
   }
   return -1;
}
template <class TYPE, class ARG_TYPE>
index base_array<TYPE, ARG_TYPE>::find_first(ARG_TYPE t, index ( * lpfnCompare )(ARG_TYPE, ARG_TYPE), index find, index last) const
{
   if(find < 0)
      find += get_count();
   if(last < 0)
      last += get_count();
   for(; find <= last; find++)
   {
      if(lpfnCompare((ARG_TYPE)element_at(find), (ARG_TYPE)t) == 0)
         return find;
   }
   return -1;
}



template <class TYPE, class ARG_TYPE>
base_array <TYPE, ARG_TYPE> & 
base_array<TYPE, ARG_TYPE>::
operator = (const base_array <TYPE, ARG_TYPE> & a)
{
   if(&a == this)
      return *this;
   copy(a);
   return * this;
}

template <class TYPE, class ARG_TYPE>
inline TYPE base_array <TYPE, ARG_TYPE>::pop()
{
   ASSERT(get_size() > 0);
   TYPE t = last_element();
   remove_at(get_upper_bound());
   return t;
}

template <class TYPE, class ARG_TYPE>
inline void base_array <TYPE, ARG_TYPE>::
push(ARG_TYPE t)
{
   add(t);
}

#define new DEBUG_NEW