#pragma once


class CLASS_DECL_c operation_canceled_exception : 
   virtual public simple_exception
{
public:


   operation_canceled_exception(sp(::application) papp);
   operation_canceled_exception(sp(::application) papp, const char * pszMessage);
   virtual ~operation_canceled_exception();


};

