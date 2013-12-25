#pragma once

template < int t_iTimeout = ((1984 + 1977) * 2) >
class string_timeout
{
public:


   string      m_str;
   DWORD       m_dwExpire;

   bool expired();
   bool valid();

   string_timeout & operator = (const string & str);

   operator const string & () const
   {
      return m_str;
   }

   strsize get_length()
   {
      return m_str.get_length();
   }

};

template < int t_iTimeout >
bool string_timeout < t_iTimeout >::expired()
{

   int iCur = (int)GetTickCount();

   int iExp = (int)m_dwExpire;

   return (iCur - iExp) > 0;

}

template < int t_iTimeout >
bool string_timeout < t_iTimeout >::valid()
{

   return !expired();

}

template < int t_iTimeout >
string_timeout< t_iTimeout > & string_timeout < t_iTimeout >::operator = (const string & str)
{

   m_str = str;

   m_dwExpire = GetTickCount() + < t_iTimeout >;

   return *this;

}




namespace fontopus
{

   typedef CLASS_DECL_BASE ::string_timeout < > string_timeout;
   typedef CLASS_DECL_BASE string_map < string_timeout > string_string_timeout;


   class CLASS_DECL_BASE user_authentication :
      public object
   {
   public:


      string_string_timeout                  m_mapFontopus;
      string_string_timeout                  m_mapServer;


   };


} // namespace fontopus

