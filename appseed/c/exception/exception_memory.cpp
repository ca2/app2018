#include "framework.h"


memory_exception::memory_exception(sp(::application) papp) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   simple_exception(papp)
{

}


memory_exception::memory_exception(sp(::application) papp, const char * pszMessage) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   simple_exception(papp, pszMessage) 
{

}


memory_exception::~memory_exception()
{

}


