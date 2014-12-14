#pragma once


#include "aura/primitive/collection/collection_iterator.h"


template < class TYPE, class ARG_TYPE >
inline bool LessCompareElements(ARG_TYPE element1, ARG_TYPE element2)
{
   return element1 < element2;
}


namespace comparison
{

   template < class TYPE, class ARG_TYPE = const TYPE & >
   class less
   {
   public:

      inline bool operator() (ARG_TYPE element1, ARG_TYPE element2) const
      {
         return ::LessCompareElements < TYPE, ARG_TYPE > (element1, element2);
      }


   };


} // namespace comparison

template <bool b> struct StaticAssertionFailed;
template <> struct StaticAssertionFailed <true> {};

#define STATIC_ASSERT(x) \
    {StaticAssertionFailed <x> ();}


