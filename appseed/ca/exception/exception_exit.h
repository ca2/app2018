#pragma once


class CLASS_DECL_ca exit_exception : 
   virtual public simple_exception
{
public:


   exit_exception(::ca::applicationsp papp, const char * pszMessage = ::null());
   exit_exception(const exit_exception & e);
   virtual ~exit_exception();


};
