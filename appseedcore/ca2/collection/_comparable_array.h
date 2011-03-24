#pragma once

#include "_base_array.h"
#include "_comparable_eq_array.h"


template < class TYPE, class ARG_TYPE, class ARRAY_TYPE>
class comparable_array :
   virtual public ARRAY_TYPE
{
public:
   comparable_array();
   comparable_array(const comparable_array & base_array);

   void QuickSort(bool bAsc = true)
   {
      if(bAsc)
      {
         sort::QuickSortAsc(*this);
      }
      else
      {
         sort::QuickSortDesc(*this);
      }
   }
};
template < class TYPE, class ARG_TYPE, class ARRAY_TYPE>
comparable_array<  TYPE,  ARG_TYPE,  ARRAY_TYPE>::
comparable_array()
{
}
template < class TYPE, class ARG_TYPE, class ARRAY_TYPE>
comparable_array<  TYPE,  ARG_TYPE,  ARRAY_TYPE>::
comparable_array(const comparable_array<  TYPE,  ARG_TYPE,  ARRAY_TYPE> & a) :
ARRAY_TYPE(a)
{
   operator = (a);
}




