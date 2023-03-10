#pragma once


template < class TYPE, class ARG_TYPE, class T >
class map_to_smart_pointer :
   virtual public map < TYPE, ARG_TYPE, smart_pointer < T >, const smart_pointer < T > & >
{
public:


   map_to_smart_pointer(::aura::application * papp = NULL) :
      object(papp),
      map < TYPE, ARG_TYPE, smart_pointer < T >, const smart_pointer < T > & > (papp)
   {
   }



   virtual ~map_to_smart_pointer()
   {
   }


} ;

template < class T >
class string_to_smart_pointer :
   virtual public string_map < smart_pointer < T >, const smart_pointer < T > & >
{
public:


   string_to_smart_pointer(::aura::application * papp = NULL) :
      object(papp),
      string_map < smart_pointer < T >, const smart_pointer < T > & > (papp)
   {

   }


   virtual ~string_to_smart_pointer()
   {
   }


} ;

template < class T >
class int_to_smart_pointer:
   virtual public int_ptr_map < smart_pointer < T >,const smart_pointer < T > & >
{
public:


   int_to_smart_pointer(::aura::application * papp = NULL,::count nBlockSize = 10):
      object(papp),
      int_ptr_map < smart_pointer < T >,const smart_pointer < T > & >(papp,nBlockSize)
   {

   }


   virtual ~int_to_smart_pointer()
   {
   }


} ;


#define mapsp(TYPE, ARG_TYPE, SPTYPE) map_to_smart_pointer < TYPE, ARG_TYPE, SPTYPE >
#define strsp(SPTYPE) string_to_smart_pointer < SPTYPE >
#define isp(SPTYPE) int_to_smart_pointer < SPTYPE >
#define idsp(SPTYPE) mapsp(id, id, SPTYPE)






