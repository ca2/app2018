#pragma once


class CLASS_DECL_ACE temporary_exception : 
   virtual public ::exception::base
{
public:
   
   
   string m_strTip;


   temporary_exception(::ace::application * papp, const char * pszTip = NULL);
   temporary_exception(const temporary_exception & e);
   virtual ~temporary_exception();


};


