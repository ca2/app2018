#pragma once


class CLASS_DECL_AURA not_implemented : 
   virtual public ::exception::base
{
public:


   string m_strTip;


   not_implemented(::aura::application * papp, const char * pszTip = NULL);
   not_implemented(const not_implemented & e);
   virtual ~not_implemented();


};
