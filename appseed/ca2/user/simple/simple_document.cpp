#include "framework.h"


simple_document::simple_document(sp(::application) papp) : 
   element(papp),
   ::ca2::data_container_base(papp),
   
   user::document(papp),
   m_set(papp)
{
}

simple_document::~simple_document()
{
}



