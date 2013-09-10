// Common/Exception.h
// from 7-zip on 2012-12-23, dawn
#pragma once

class CLASS_DECL_c system_exception :
   virtual public simple_exception
{
public:
   HRESULT ErrorCode;
   system_exception(sp(base_application) papp, HRESULT errorCode) :
      base_element(papp),
      ::call_stack(papp),
      ::exception::base(papp),
      ::simple_exception(papp),
      ErrorCode(errorCode)
   {
   }
};
