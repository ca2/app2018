#pragma once


class CLASS_DECL_BASE operation_canceled_exception : 
   virtual public simple_exception
{
public:


   operation_canceled_exception(sp(base_application) papp);
   operation_canceled_exception(sp(base_application) papp, const char * pszMessage);
   virtual ~operation_canceled_exception();


};

