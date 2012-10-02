#include "framework.h"



hresult_exception::hresult_exception(::ca::application * papp, HRESULT hr) :
   ca(papp),
   ::call_stack(papp),
   ::base_exception(papp)
{
   m_hresult = hr;
}
   

hresult_exception::~hresult_exception()
{
}
