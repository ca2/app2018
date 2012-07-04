#include "framework.h"

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
         m_strNameLow   = cookie.m_strNameLow;
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



index cookies::find_cookie(const char * name)
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

index cookies::lowfind_cookie(const char * name)
{
   for(int i = 0; i < this->get_size(); i++)
   {
      if(this->element_at(i).m_strNameLow == name)
      {
         return i;
      }
   }
   return -1;
}

http::cookie & cookies::cookie(const char * name)
{
   index iFind = find_cookie(name);
   if(iFind < 0)
   {
      class cookie c;
      c.m_strName = name;
      c.m_strNameLow = name;
      add(c);
      iFind = find_cookie(name);
      if(iFind < 0)
      {
         return *((http::cookie *) NULL);
      }
      return this->element_at(iFind);;
   }
   return this->element_at(iFind);
}

http::cookie & cookies::lowcookie(const char * name)
{
   index iFind = lowfind_cookie(name);
   if(iFind < 0)
   {
      class cookie c;
      c.m_strName = name;
      c.m_strNameLow = name;
      add(c);
      iFind = find_cookie(name);
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
   //stringa stra;
   //stra.add_tokens(psz, ";", TRUE);
   bool bRun = true;
   int i = 0;
   while(bRun)
   {
      const char * pszEnd = strchr(psz, ';');
      bRun = pszEnd != NULL;
      if(!bRun)
         pszEnd = psz + strlen(psz);
      
      const char * pszEqual = strchr(psz, '=');
      if(pszEqual > pszEnd)
         pszEqual = NULL;
      if(i == 0)
      {
         if(pszEqual != NULL)
         {
            cookie.m_strName = string(psz, pszEqual - psz);
            cookie.m_strNameLow = cookie.m_strName;
            cookie.m_strNameLow.make_lower();
            cookie.m_varValue = string(psz + 1, pszEnd - pszEqual - 1);
         }
         else
         {
            return;
         }
      }
      else if(pszEqual != NULL)
      {
         string strKey = string(psz, pszEqual - psz);
         string strValue = string(psz + 1, pszEnd - pszEqual - 1);
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
         if(string(psz, pszEnd - psz) == "secure")
         {
            cookie.m_bSecure = true;
         }
      }
      i++;
      psz = pszEnd + 1;
   }
   index iFind = find_cookie(cookie.m_strName);
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
   for(index i = 0; i < this->get_size(); i++)
   {
      strCookie += (const char *) (this->element_at(i).m_strName + "=" + this->element_at(i).m_varValue.get_string());
      strCookie += ";";
   }
   return strCookie;
}


strsize cookies::get_length( const char * name)
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
   
   if(psz == NULL)
      return;

   stringa stra;
   const char * pszParam = psz;
   const char * pszParamEnd;
   const char * pszKeyEnd;
   class cookie c;
   while(true)
   {
      while(*pszParam != '\0' && isspace((unsigned char ) *pszParam))
      {
         pszParam++;
      }
      if(*pszParam == '\0')
         break;
      pszParamEnd = strchr(pszParam, ';');
      pszKeyEnd = strchr(pszParam, '=');
      if(pszParamEnd == NULL)
      {
         if(pszKeyEnd == NULL)
         {
            c.m_strName = string(pszParam);
            c.m_strNameLow = c.m_strName;
            c.m_strNameLow.make_lower();
            c.m_varValue.set_type(var::type_empty);
            add(c);
         }
         else
         {
            c.m_strName = string(pszParam, pszKeyEnd - pszParam);
            c.m_strNameLow = c.m_strName;
            c.m_strNameLow.make_lower();
            c.m_varValue = string(pszKeyEnd + 1);
            add(c);
         }
         return;
      }
      else
      {
         if(pszKeyEnd == NULL || pszKeyEnd > pszParamEnd)
         {
            c.m_strName = string(pszParam, pszParamEnd - pszKeyEnd);
            c.m_strNameLow = c.m_strName;
            c.m_strNameLow.make_lower();
            c.m_varValue.set_type(var::type_empty);
            add(c);
         }
         else
         {
            c.m_strName = string(pszParam, pszKeyEnd - pszParam);
            c.m_strNameLow = c.m_strName;
            c.m_strNameLow.make_lower();
            c.m_varValue = string(pszKeyEnd + 1, pszParamEnd - (pszKeyEnd + 1));
            add(c);
         }
      }
      pszParam = pszParamEnd + 1;
   }
}

http::cookie & cookies::operator [](const char * name)
{
   return cookie(name);
}


} // namespace http
