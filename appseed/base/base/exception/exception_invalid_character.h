#pragma once




class CLASS_DECL_BASE invalid_character : 
   virtual public simple_exception
{
public:
   

   invalid_character(sp(base_application) papp, const char * pszMessage);
   virtual ~invalid_character();


};
