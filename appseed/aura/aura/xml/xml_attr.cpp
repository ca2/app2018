#include "framework.h"


namespace xml
{


   attr_array::attr_array(::std::initializer_list < var > list) :
      property_set(list)
   {

   }


   attr_array::attr_array(::aura::application * papp) :
      property_set(papp)
   {

      m_pnodeParent = NULL;

   }


   ::xml::attr * attr_array::add_attr(const id & idName)
   {

      return &operator[](idName);

   }

} //namespace xml


