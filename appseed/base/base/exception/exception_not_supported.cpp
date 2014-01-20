#include "framework.h"


not_supported_exception::not_supported_exception(const not_supported_exception & e) :
   element(e.get_app()),
   ::call_stack(e.get_app()),
   ::exception::base(e.get_app()),
   not_implemented(e),
   interface_only_exception(e)
{

}

not_supported_exception::not_supported_exception(sp(base_application) papp, const char * pszTip) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   not_implemented(papp, pszTip),
   interface_only_exception(papp, pszTip)
{

}

not_supported_exception::~not_supported_exception()
{

}


