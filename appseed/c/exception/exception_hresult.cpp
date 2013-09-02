#include "framework.h"



hresult_exception::hresult_exception(sp(::application) papp, HRESULT hr) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp)
{
   m_hresult = hr;
}
   

hresult_exception::~hresult_exception()
{
}
