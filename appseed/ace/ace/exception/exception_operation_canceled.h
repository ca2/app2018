#pragma once


class CLASS_DECL_ACE operation_canceled_exception : 
   virtual public simple_exception
{
public:


   operation_canceled_exception(::ace::application * papp);
   operation_canceled_exception(::ace::application * papp, const char * pszMessage);
   virtual ~operation_canceled_exception();


};
