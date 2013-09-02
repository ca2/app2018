#include "framework.h"


resource_exception::resource_exception(sp(::application) papp) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   simple_exception(papp)
{

}


resource_exception::~resource_exception()
{

}

