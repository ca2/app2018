#pragma once


class CLASS_DECL_AURA range_error : 
   virtual public error_exception
{
   // ace class for resource-critical core API exceptions
   // handles ownership and initialization of an error message
public:
   
   
   range_error(::ace::application * papp, const char * pszError);
   virtual ~range_error();


};



