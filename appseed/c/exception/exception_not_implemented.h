#pragma once


class CLASS_DECL_c not_implemented : 
   virtual public ::exception::base
{
public:


   string m_strTip;


   not_implemented(sp(base_application) papp, const char * pszTip = NULL);
   not_implemented(const not_implemented & e);
   virtual ~not_implemented();


};
