#pragma once


class CLASS_DECL_BASE invalid_index_exception : 
   virtual public invalid_argument_exception
{
public:


   invalid_index_exception(sp(base_application) papp);
   invalid_index_exception(sp(base_application) papp, const char * pszMessage);
   virtual ~invalid_index_exception();


};

