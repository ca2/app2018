#pragma once


class CLASS_DECL_ACE assert_exception : 
   virtual public simple_exception
{
public:


   string m_strFile;
   long m_lLine;


   assert_exception(const assert_exception & e);
   assert_exception(::ace::application * papp, const char * pszFile, long lLine);
   virtual ~assert_exception();

};