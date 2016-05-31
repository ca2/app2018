#pragma once


class CLASS_DECL_ACE istring
{
public:


   ::ace::application *   m_paceapp;
   string                  m_str;
   string                  m_strTemplate;


   istring(::ace::application * papp);
   ~istring();


   istring & operator = (const char * psz);


   inline operator const char * () { return m_str; }
   inline operator const char * () const { return m_str; }


   void update_string();

   string & to_string(string & str) const { return str = m_str; };
   string to_string() const { return m_str; };

};



