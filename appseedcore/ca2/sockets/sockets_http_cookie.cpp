#include "StdAfx.h"

namespace http
{

   cookie::cookie()
   {
      m_bSecure = false;
   }

   cookie::cookie(const cookie & cookie)
   {
      operator = (cookie);
   }

   cookie::~cookie()
   {
   }

   cookie & cookie::operator = (const cookie & cookie)
   {
      if(&cookie != this)
      {
         m_strName      = cookie.m_strName;
         m_varValue     = cookie.m_varValue;
         m_strExpire    = cookie.m_strExpire;
         m_strPath      = cookie.m_strPath;
         m_strDomain    = cookie.m_strDomain;
         m_bSecure      = cookie.m_bSecure;
      }
      return *this;
   }

   string cookie::get_cookie_string()
   {
      string str;
      str = m_strName;
      str += "=";
      str += m_varValue.get_string();
      if(m_strExpire.has_char())
      {
         str += "; expires=";
         str += m_strExpire;
      }
      if(m_strPath.has_char())
      {
         str += "; path=";
         str += m_strPath;
      }
      if(m_strDomain.has_char())
      {
         str += "; domain=";
         str += m_strDomain;
      }
      if(m_bSecure)
      {
         str += "; secure";
      }
      return str;
   }

cookies::cookies()
{

}

cookies::~cookies()
{

}



int cookies::find_cookie(const char * name)
{
   for(int i = 0; i < this->get_size(); i++)
   {
      if(this->element_at(i).m_strName == name)
      {
         return i;
      }
   }
   return -1;
}

http::cookie & cookies::cookie(const char * name)
{
   int iFind = find_cookie(name);
   if(iFind < 0)
   {
      class cookie c;
      c.m_strName = name;
      add(c);
      int iFind = find_cookie(name);
      if(iFind < 0)
      {
         return *((http::cookie *) NULL);
      }
      return this->element_at(iFind);;
   }
   return this->element_at(iFind);
}

void cookies::add(const char * psz)
{
   class cookie cookie;
   cookie.m_bSecure = false;
   stringa stra;
   stra.add_tokens(psz, ";", TRUE);
   for(int i = 0; i < stra.get_size(); i++)
   {
      stringa stra2;
      stra2.add_tokens(stra[i], "=", TRUE);
      if(i == 0)
      {
         if(stra2.get_size() >= 2)
         {
            cookie.m_strName = stra2[0].trim();
            cookie.m_varValue = stra2[1];
         }
         else
         {
            return;
         }
      }
      else if(stra2.get_size() >= 2)
      {
         string strKey = stra2[0].trim();
         string strValue = stra2[1].trim();
         if(strKey == "expires")
         {
            cookie.m_strExpire = strValue;
         }
         else if(strKey == "domain")
         {
            cookie.m_strDomain = strValue;
         }
         else if(strKey == "path")
         {
            cookie.m_strPath = strValue;
         }
      }
      else
      {
         if(stra2[0] == "secure")
         {
            cookie.m_bSecure = true;
         }
      }
   }
   int iFind = find_cookie(cookie.m_strName);
   if(iFind < 0)
   {
      add(cookie);
      return;
   }
   this->element_at(iFind) = cookie;
   return;
}


string cookies::get_cookie_header()
{
   string strCookie;
   for(int i = 0; i < this->get_size(); i++)
   {
      strCookie += (const char *) (this->element_at(i).m_strName + "=" + this->element_at(i).m_varValue.get_string());
      strCookie += ";";
   }
   return strCookie;
}


int cookies::get_length( const char * name)
{
   return cookie(name).m_varValue.get_string().get_length();
}

string cookies::set_cookie(
   const char * name,
   var var,
   int iExpire,
   const char * path,
   const char * domain,
   bool bSecure)
{
   string str;
   str = name;
   str += "=";
   str += var.get_string();
   str += "; ";
   if(iExpire > 0)
   {
      str += "expires";
      str += "=";
      str += expire(iExpire);
   }
   else if(iExpire < 0)
   {
      str += "expires";
      str += "=";
      str += expire(0);
   }
   if(path != NULL && strlen(path) > 0)
   {
      str += "; ";
      str += "path";
      str += "=";
      str += path;
   }
   if(domain != NULL && strlen(domain) > 0)
   {
      str += "; ";
      str += "domain";
      str += "=";
      str += domain;
   }
   if(bSecure)
   {
      str += "; ";
      str += "secure";
   }
   str += ";";
   add(str);
   return str;
}


string cookies::expire(time_t t)
{
   if(t == 0)
      t = time(NULL);

   //time_t t = time((time_t *)&iExpire);
   struct tm tp;
#ifdef _WIN32
   memcpy(&tp, gmtime(&t), sizeof(tp));
#else
   gmtime_r(&t, &tp);
#endif
   const char *days[7] = {"Sunday", "Monday",
    "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
   const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May",
    "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
   string strDateTime;

   strDateTime.Format("%s, %02d-%s-%04d %02d:%02d:%02d GMT",
    days[tp.tm_wday],
    tp.tm_mday,
    months[tp.tm_mon],
    tp.tm_year + 1900,
    tp.tm_hour,
    tp.tm_min,
    tp.tm_sec);

   return strDateTime;
}




void cookies::parse_header(const char * psz)
{
   stringa stra;
   stra.add_tokens(psz, ";", FALSE);
   class cookie c;
   for(int i = 0; i < stra.get_size(); i++)
   {
      string str = stra[i];
      str = str.trim();
      stringa stra2;
      stra2.add_tokens(str, "=", TRUE);
      if(stra2.get_size() > 0)
      {
         c.m_strName = stra2[0].trim();
      }
      else
      {
         c.m_strName.Empty();
      }
      if(stra2.get_size() > 1)
      {
         c.m_varValue = stra2[1].trim();
      }
      else
      {
         c.m_varValue.set_type(var::type_empty);
      }
      add(c);
   }
}

http::cookie & cookies::operator [](const char * name)
{
   return cookie(name);
}


} // namespace http
