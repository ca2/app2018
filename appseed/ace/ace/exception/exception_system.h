// Common/Exception.h
// from 7-zip on 2012-12-23, dawn
#pragma once

class CLASS_DECL_ACE system_exception :
   virtual public simple_exception
{
public:
   
   
   HRESULT ErrorCode;


   system_exception(::ace::application * papp, HRESULT errorCode) :
      object(papp),
      ::call_stack(papp),
      ::exception::base(papp),
      ::simple_exception(papp),
      ErrorCode(errorCode)
   {

      
      debug_print(":system");


   }


};
