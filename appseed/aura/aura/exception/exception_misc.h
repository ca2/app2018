#pragma once


class CLASS_DECL_AURA misc_exception : 
   virtual public simple_exception
{
public:


   string m_strMessage;


   misc_exception(const misc_exception & e);
   misc_exception(::aura::application * papp, const char * pszMessage);
   virtual ~misc_exception();

   virtual bool get_error_message(string & str, PUINT pnHelpContext = NULL);

   
};
