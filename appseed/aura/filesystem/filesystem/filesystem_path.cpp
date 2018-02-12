﻿#include "framework.h"


namespace file
{

   path::path(e_context_switcher_null)
   {

      m_epath = path_file;

   }


   path::path(e_path epath)
   {

      m_epath = epath;

   }


   path::path(const unichar * pwsz, strsize iCount, e_path epath, int iDir, bool bNormalizePath, int64_t iSize) :
      string(pwsz, iCount),
      path_meta(epath, iSize, iDir)
   {

      if (m_epath == path_none)
      {

         m_epath = get_path_type(*this, epath);

      }

      if (bNormalizePath)
      {

         bool bCertainlySyntathicallyDir = normalize_path_inline(*this, m_epath);

         if (bCertainlySyntathicallyDir)
         {

            m_iDir = 1;

         }
         else
         {

            m_iDir = iDir;

         }

      }

   }

   path::path(const string & str, e_path epath, int iDir, bool bNormalizePath, int64_t iSize) :
      string(str)
   {

      m_iSize = iSize;

      if (epath == path_none)
      {

         m_epath = get_path_type(str, epath);

      }
      else
      {

         m_epath = epath;

      }

      if (bNormalizePath)
      {

         bool bCertainlySyntathicallyDir = normalize_path_inline(*this, m_epath);

         if (bCertainlySyntathicallyDir)
         {

            m_iDir = 1;

         }
         else
         {

            m_iDir = iDir;

         }

      }
      else
      {

         m_iDir = iDir;

      }

   }


   path::path(const id & id,e_path epath, int iDir) :
      path(string(id), epath, iDir)
   {

   }

   path::path(const var & var, e_path epath, int iDir) :
      path(var.get_string(), epath, iDir)
   {


   }

   path::path(const path & path) :
      ::string((const string &) path),
      path_meta((path_meta &) path)
   {

   }


   path::path(path && path) :
      string(::move(path)),
      path_meta((path_meta &) path)
   {

   }


   path::path(const char * psz, e_path epath, int iDir):
      path(string(psz), epath, iDir)
   {

   }

   path::path(const unichar * psz, e_path epath, int iDir) :
      path(string(psz), epath, iDir)
   {

   }


   path::path(const wstring & wstr, e_path epath, int iDir):
      path(string(wstr), epath, iDir)
   {

   }

   path::~path() NOTHROW
   {


   }


//   bool path::is_equal(const path & path) const
//   {
//
//
//#ifdef WINDOWS
//
//      if (((const string &)*this).compare_ci((const string &)path) == 0) // undoubtely eaqual...
//         return true;
//
//#else
//
//      if (((const string &)*this).operator == ((const string &)path)) // undoubtely eaqual...
//         return true;
//
//#endif
//
//      return false;
//
//      // find extend equalitys..
//
//      ::file::patha patha1;
//
//      split(patha1);
//
//      ::file::patha patha2;
//
//      path.split(patha2);
//
//      if(patha1.get_size() == patha2.get_size())
//      {
//
//         for(index i = 0; i < patha1.get_size(); i++)
//         {
//
//            if(patha1[i].::string::operator!=(patha2[i]))
//            {
//
//               goto there_s_difference_in_this_step_1;
//
//            }
//
//         }
//
//         return true;
//
//      }
//
//      there_s_difference_in_this_step_1:
//
//
//      return false;
//
//   }


   void path::split(patha & patha) const
   {

      ascendants_name(patha);

   }


   patha & path::ascendants_path(patha & straParam) const
   {

      ::file::patha stra;

      ascendants_name(stra);

      ::file::path str;

      if(stra.get_count() > 0)
      {
         str = stra[0];
         straParam.add(str);
      }

      for(int32_t i = 1; i < stra.get_size(); i++)
      {
         str /= stra[i];
         straParam.add(str);
      }

      return straParam;

   }


   string path::name(index i) const
   {

      patha patha;

      ascendants_name(patha);

      if (i < 0)
      {

         i += patha.get_size();

      }


      if (i < 0)
      {

         return "";

      }

      if (i >= patha.get_size())
      {

         return "";

      }

      return patha[i];

   }


   patha & path::ascendants_name(patha & straParam) const
   {

      string strPath = *this;

      if(strPath == "/")
      {

         straParam.add("/");

         return straParam;

      }

      strsize iFirstColon = find(':');
      strsize iFirstSlash = find('/');
      strsize iSecondSlash = find('/', iFirstSlash + 1);

      if (iFirstColon > 0 && iFirstSlash == iFirstColon + 1 && iSecondSlash == iFirstSlash + 1)
      {

         ::file::path path = Left(iSecondSlash + 1);

         straParam.add(path);

         strPath = Mid(iSecondSlash + 1);

      }

      stringa straSeparator;

      straSeparator.add("/");

      straSeparator.add("\\");

      straParam.add_smallest_tokens(strPath,straSeparator,FALSE);

      if(strPath.begins_ci("/"))
      {
         straParam[0] = "/" + straParam[0];
      }
      else if(strPath.begins_ci("\\\\"))
      {
         straParam[0] = "\\\\" + straParam[0];
      }

      return straParam;

   }


   path path::folder(int i) const
   {

      ::file::path path(*this);

      while(i > 0)
      {

         path = path.folder();

         i--;

      }

      return path;

   }


   path & path::go_up()
   {

      return *this = folder();

   }


   path & path::go_up(int i)
   {

      while(i > 0)
      {

         go_up();

         i--;

      }

      return *this;

   }


   path & path::operator += (const path & path)
   {

      string str = path;

      str.trim_left("\\/");

      string::operator += (str);

      return *this;

   }

   path & path::operator += (const string & strParam)
   {

      string str = strParam;

      str.trim_left("\\/");

      string strThis = *this;

      stdstring < simple_string >::operator = (normalize_path(strThis + str, m_epath));

      return *this;

   }

   patha path::operator / (const stringa & stra) const
   {

      patha patha;

      for (auto str : stra)
      {

         patha.add(*this / str);

      }

      return patha;

   }

   path path::operator / (const path & path) const
   {

      string strPath;

      strPath = c_str();

      if (strPath.is_empty())
      {

         return path;

      }

      string str = path;

      if (strPath.Right(3) == "://")
      {

         return ::file::path(strPath + str, m_epath, path.m_iDir);

      }

      strPath += sep();

      str.trim_left("\\/");

      return ::file::path(strPath + str, m_epath, path.m_iDir);

   }

   path path::operator / (const string & str) const
   {

      return operator /(::file::path(str));

   }


   path path::operator / (const char * psz) const
   {

      return operator /(::file::path(psz));

   }

   path & path::operator /= (const path & path)
   {

      string strPath;

      strPath = string(*this);

      string str = path;

      if (strPath.Right(3) == "://")
      {

         return operator = (::file::path(strPath + str, m_epath));

      }

      if(strPath.is_empty())
      {

         strPath = sep();

      }
      else if(strPath.get_length() == 1 && strPath[0] == sep())
      {

      }
      else
      {

         strPath += sep();

         str.trim_left("\\/");

      }

      return operator = (::file::path(strPath + str, m_epath));

   }

   path & path::operator /= (const string & str)
   {

      return operator /=(::file::path(str));

   }

   path & path::operator /= (const char * psz)
   {

      return operator /=(::file::path(psz));

   }


   ::file::path path::replace_file_extension(const char * pszNewExtension, const char * pszOldExtension)
   {

      string strNewExtension(pszNewExtension);

      ::str::begins_eat_ci(strNewExtension, ".");

      string strOldExtension(pszOldExtension);

      ::str::begins_eat_ci(strOldExtension, ".");

      ::file::path path(*this);

      ::str::ends_eat_ci(path, strOldExtension);

      path += strNewExtension;

      return path;

   }


   ::file::path path::replace_file_extension(const char * pszNewExtension)
   {

      string strNewExtension(pszNewExtension);

      ::str::begins_eat_ci(strNewExtension, ".");

      ::file::path pathFolder = folder();

      string strTitle = title();

      string strName = strTitle + "." + strNewExtension;

      return pathFolder / strName;

   }


   void path::set_type(e_path epath)
   {

      if(epath != m_epath)
      {

         m_epath = epath;

         ::stdstring < simple_string >::operator = (normalize_path(*this, m_epath));
         ;
      }

   }





   path & path::operator = (const ::file::path & path)
   {

      if(&path != this)
      {

         string::operator  = ((const string &) path);
         *((path_meta *)this) = (const path_meta &)path;

      }

      return *this;


   }


   path & path::operator = (const string & str)
   {

      *((path_meta*)this) = path_meta();

      m_epath = is_url_dup(str) ? path_url : path_file;

      stdstring < simple_string >::operator  = (normalize_path(str, m_epath));

      return *this;

   }


   path path::operator + (const path & path) const
   {

      return ::file::path((const string &)*this  + string((const string &)path), m_epath);

   }


   path path::operator + (const string & str) const
   {

      return ::file::path((const string &)*this + (m_iDir > 0 ? string(sep()) : "" ) + string((const string &)str),m_epath);

   }


   path path::operator + (const char * psz) const
   {

      return operator + (string(psz));

   }



   path & path::operator = (const char * psz)
   {

      return operator = (string(psz));

   }


   path & path::operator += (const char * psz)
   {

      return operator += (string(psz));

   }

   path & path::operator = (const wstring & wstr)
   {

      return operator = (string(wstr));

   }

   path & path::operator += (const wstring & wstr) { return operator += (string(wstr)); }

   path & path::operator = (const unichar * psz) { return operator = (wstring(psz)); }
   path & path::operator += (const unichar * psz) { return operator += (wstring(psz)); }

   string & path::to_string(string & str) const
   {
      return str = (const string &)*this;
   }

   path path::operator * (const path & path) const
   {

      return sibling(path);

   }

   path path::operator * (const string & str) const { return operator * (::file::path(str)); }
   path path::operator * (const char * psz) const { return operator * (::file::path(psz)); }

   path & path::operator *= (const path & path)
   {

      if(this != &path)
      {

         *this = sibling(path);

      }

      return *this;

   }

   path & path::operator *= (const string & str) { return operator *= (::file::path(str)); }
   path & path::operator *= (const char * psz) { return operator *= (::file::path(psz)); }

   ::file::path path::title() const
   {
      return ::file_title_dup(operator const char*());
   }

   //void path::calc_name() const
   //{
   //   m_iName = find_file_name();
   //   m_strName = Mid(m_iName);
   //}





   index path::find_file_name() const
   {
      return MAX(0, reverse_find(sep()) + 1);
   }

   string path::ext() const
   {
      return extension();
   }

   bool path::is_relative()
   {
      return file_path_is_relative_dup(*this) != FALSE;
   }

   path path::relative() const
   {

      return Mid(MAX(0,m_iRelative));

   }


   path path::up() const
   {

      return folder();

   }


   path path::up(int i) const
   {

      return folder(i);

   }


   path & path::operator -= (int i)
   {

      return go_up(i);

   }





   e_path get_path_type(const string & str, e_path epathForce)
   {

      if(epathForce != path_none)
      {

         return epathForce;

      }
      else if(is_url_dup(str))
      {

         return path_url;

      }
      else
      {

         return path_file;

      }

   }

   string normalize_path(string strPath, e_path epath)
   {

      normalize_path_inline(strPath, epath);

      return strPath;

   }

   bool normalize_path_inline(string & strPath, e_path & epath)
   {

      bool bUrl;

      bool bOnlyNativeFileSep;

      strsize iaSlash[512];

      int iSlashCount;

      bool bCertainlySyntathicallyDir = solve_relative_compressions_inline(strPath, bUrl, bOnlyNativeFileSep, iaSlash, &iSlashCount);

      if(bUrl)
      {

         epath = path_url;

      }
      else
      {

         epath = path_file;

      }

#ifdef WINDOWS
      if(!bOnlyNativeFileSep && epath == path_file)
#else
      if(!bOnlyNativeFileSep)
#endif
      {

#ifdef WINDOWS
         if(strPath == "\\\\" || strPath == "\\")
         {

         }
         else
#endif
            if (strPath.has_char())
            {

               char * psz = strPath.GetBuffer();

               char chSep = path_sep(epath);

               for(int i = 0; i < iSlashCount; i++)
               {

                  psz[iaSlash[i]] = chSep;

               }

               strPath.ReleaseBuffer();

            }

      }

      return bCertainlySyntathicallyDir;

   }


} // namespace file



