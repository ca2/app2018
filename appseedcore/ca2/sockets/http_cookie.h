#pragma once

class http_socket;

namespace http
{

class CLASS_DECL_ca cookie
{
public:
   cookie();
   cookie(const cookie & cookie);
   virtual ~cookie();

   string         m_strName;
   var  m_varValue;
   string         m_strExpire;
   string         m_strPath;
   string         m_strDomain;
   bool           m_bSecure;

   string get_cookie_string();

   cookie & operator = (var var)
   {
      m_varValue = var;
      return *this;
   }


   cookie & operator = (const cookie & cookie);

   bool is_set()
   {
      return m_varValue.is_set();
   }

   bool is_new()
   {
      return m_varValue.is_new();
   }

   operator var &()
   {
      return m_varValue;
   }
};

class CLASS_DECL_ca cookies :
   public array_ptr_alloc < cookie, cookie & >
{
public:
   cookies();
   virtual ~cookies();

   int find_cookie(const char * name);
   http::cookie & cookie(const char * name);
   using array_ptr_alloc < http::cookie, http::cookie & >::add;
   void add(const char * psz);
   void parse_header(const char * psz);
   string get_cookie_header();
   int get_length(const char * name);
   string set_cookie(
      const char * pszKey,
      var var,
      int iExpire = 0,
      const char * pszDomain = NULL, 
      const char * pszPath = NULL,
      bool bSecure = false);
   static string expire(time_t iExpire = 0);


   using array_ptr_alloc < class http::cookie, class http::cookie & >::operator [];
   http::cookie & operator [](const char * name);

};

} // namespace http