#pragma once


class CLASS_DECL_c invalid_argument_exception :
   virtual public simple_exception
{
public:


   invalid_argument_exception(sp(::application) papp);
   invalid_argument_exception(sp(::application) papp, const char * pszMessage);
   virtual ~invalid_argument_exception();


};

