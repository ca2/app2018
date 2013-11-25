#pragma once

// primitive array is a special array and should be used with care
// it uses operations like memmove and memcopy to move objects and does not
// call constructors and destructors for the elements
// it is faster than the more generic arrays, can be faster than raw array
// for many "primitive" types because the arguments are passed as the own
// "primitive" type data and it is proper for use with primitive data types.

template < class TYPE >
class comparable_primitive_array
{
public:


   typedef comparable_raw_array < TYPE, const TYPE & > type;


};


