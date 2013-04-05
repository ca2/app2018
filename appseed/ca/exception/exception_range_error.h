#pragma once


class CLASS_DECL_ca range_error : 
   virtual public error_exception
{
   // base class for resource-critical ca API exceptions
   // handles ownership and initialization of an error message
public:
   
   
   range_error(::ca::applicationsp papp, const char * pszError);
   virtual ~range_error();


};



