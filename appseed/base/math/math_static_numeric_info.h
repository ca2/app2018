#pragma once

#include <limits.h>

template < class T >
class static_numeric_info
{
public:

}; // numeric_info




template <>
class CLASS_DECL_c static_numeric_info < uint32_t >
{
public:

   static const bool is_signed = false;
   static const bool is_integer = true;

}; // numeric_info



