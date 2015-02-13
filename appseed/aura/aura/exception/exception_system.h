// Common/Exception.h
// from 7-zip on 2012-12-23, dawn
#pragma once

class CLASS_DECL_AURA system_exception :
   virtual public simple_exception
{
public:
   HRESULT ErrorCode;
   system_exception(::aura::application * papp, HRESULT errorCode) :
      object(papp),
      ::call_stack(papp),
      ::exception::base(papp),
      ::simple_exception(papp),
      ErrorCode(errorCode)
   {
         printf(":system");
   }
};
