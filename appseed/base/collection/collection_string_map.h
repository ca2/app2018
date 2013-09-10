#pragma once


template < class VALUE, class ARG_VALUE = const VALUE &, class HASH = ::comparison::hash < const string & > , class EQUALS = ::comparison::equals_type_arg_type < string, const string & > >
class string_map :
   virtual public attrib_map < map < string, const string &, VALUE, ARG_VALUE, HASH, EQUALS > >
{
public:


   string_map(sp(base_application) papp = NULL, ::count nBlockSize = 10);
   string_map(const string_map & map);


   string_map & operator = (const string_map & map);


};

template < class VALUE, class ARG_VALUE, class HASH, class EQUALS >
string_map < VALUE, ARG_VALUE, HASH, EQUALS >::string_map(sp(base_application) papp, ::count nBlockSize) :
   base_element(papp),
   map < string, const string &, VALUE, ARG_VALUE, HASH, EQUALS > (papp, nBlockSize)
{
}

template < class VALUE, class ARG_VALUE, class HASH, class EQUALS >
string_map < VALUE, ARG_VALUE, HASH, EQUALS >::string_map(const string_map & map) :
   base_element(map.get_app()),
   attrib_map < ::map < string, const string &, VALUE, ARG_VALUE, HASH, EQUALS > > (map)
{
}

template < class VALUE, class ARG_VALUE, class HASH, class EQUALS >
string_map < VALUE, ARG_VALUE, HASH, EQUALS > & string_map < VALUE, ARG_VALUE, HASH, EQUALS >::operator = (const string_map & map)
{

   if(this != &map)
   {
      attrib_map < ::map < string, const string &, VALUE, ARG_VALUE, HASH, EQUALS > >::operator = (map);
   }

   return *this;

}


typedef CLASS_DECL_c string_map < int_ptr, int_ptr > string_to_intptr;

typedef CLASS_DECL_c string_map < void *, void * > string_to_ptr;


#define strmap(T) string_map < T >







