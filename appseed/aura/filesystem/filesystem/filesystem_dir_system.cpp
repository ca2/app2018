#include "framework.h"


#ifdef WINDOWS

#define DIR_SEPARATOR "\\"

#else

#define DIR_SEPARATOR "/"

#endif


namespace file
{


   bool path::is_equal(const char * lpcsz1, const char * lpcsz2)
   {
      UNREFERENCED_PARAMETER(lpcsz1);
      UNREFERENCED_PARAMETER(lpcsz2);
      throw interface_only_exception(get_app(), "this is an interface");
   }


   namespace dir
   {


      system::system(::aura::application * papp) :
         ::element(papp),
         m_mutex(papp),
         m_isdirmap(papp)
      {
//         m_pziputil = new zip::Util;
         m_isdirmap.m_dwTimeOut = 15000;
      }

      system::~system()
      {
//         if(m_pziputil != NULL)
//         {
////            delete m_pziputil;
//         }
      }



      string system::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl)
      {
         UNREFERENCED_PARAMETER(pszFolder);
         UNREFERENCED_PARAMETER(iLenFolder);
         UNREFERENCED_PARAMETER(pszRelative);
         UNREFERENCED_PARAMETER(iLenRelative);
         UNREFERENCED_PARAMETER(psz2);
         UNREFERENCED_PARAMETER(iLen2);
         UNREFERENCED_PARAMETER(bUrl);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, bool bUrl)
      {

         return path(pszFolder, iLenFolder, pszRelative, iLenRelative, NULL, 0, bUrl);

      }

      string system::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2)
      {

         return path(pszFolder, iLenFolder, pszRelative, iLenRelative, psz2, iLen2, ::url::is_url(pszFolder));

      }

      string system::path(const string & strFolder, const string & strRelative)
      {

         return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), NULL, 0, ::url::is_url(strFolder));

      }


      string system::path(const string & strFolder, const string & strRelative, const string & str2)
      {

         return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), str2, str2.get_length(), ::url::is_url(strFolder));

      }

      string system::path(const string & strFolder, const string & strRelative, const char * psz2)
      {

         if(psz2 == NULL)
         {

            return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), ::url::is_url(strFolder));

         }
         else
         {

            return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), psz2, strlen(psz2), ::url::is_url(strFolder));

         }
      }

      string system::sibling(const string & strPath,const string & strRelative,bool bUrl)
      {

         return path(name(strPath),strRelative,bUrl);

      }


      string system::sibling(const string & strPath,const string & strRelative,const string & str2,bool bUrl)
      {

         return path(name(strPath),strRelative,str2,bUrl);

      }


      string system::sibling(const string & strPath,const string & strRelative)
      {

         return sibling(strPath,strRelative,::url::is_url(strPath));

      }


      string system::sibling(const string & strPath,const string & strRelative,const string & str2)
      {

         return sibling(strPath,strRelative, str2,::url::is_url(strPath));

      }


      string system::path(const string & strFolder, const string & strRelative, bool bUrl)
      {

         return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), NULL, 0, bUrl);

      }

      string system::path(const string & strFolder, const string & strRelative, const string & str2, bool bUrl)
      {

         return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), str2, str2.get_length(), bUrl);

      }

      inline bool myspace(char ch)
      {
         return ch == ' ' ||
                ch == '\t' ||
                ch == '\r' ||
                ch == '\n';
      }

#define string_STRCAT2_beg_char_end(strCat, ch, str1, str2, beg1, end1, beg2, end2) \
         { \
            char * psz = strCat.GetBufferSetLength(end1 - beg1 + 1 + end2 - beg2 + 1 + 1); \
            strncpy(psz, &((const char *)str1)[beg1], end1 - beg1 + 1); \
            psz[end1 - beg1 + 1] = ch; \
            strncpy(&psz[end1 - beg1 + 2], &((const char *)str2)[beg2], end2 - beg2 + 1); \
            strPath.ReleaseBuffer(end1 - beg1 + 1 + end2 - beg2 + 1 + 1); \
         }

      string system::simple_path(const string & strFolder, const string & strRelative)
      {

         if(strRelative.is_empty())
         {
            if(strFolder.is_empty())
               return "";
            return strFolder;
         }

         strsize iFolderBeg = 0;

         strsize iFolderEnd = strFolder.get_length() - 1;

         if(iFolderEnd >= iFolderBeg)
         {
            //strFolder.trim();
            // passive left trimming
            while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderBeg]))
               iFolderBeg++;
            // passive right trimming
            while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderEnd]))
               iFolderEnd--;
            //better than following 2 together
            //::str::ends_eat(strFolder, "\\");
            //::str::ends_eat(strFolder, "/");
            while(iFolderBeg <= iFolderEnd && (strFolder[iFolderEnd] == '/' || strFolder[iFolderEnd] == '\\'))
               iFolderEnd--;
         }

         strsize iRelativeBeg = 0;

         strsize iRelativeEnd = strRelative.get_length() - 1;

         if(iRelativeEnd >= iRelativeBeg)
         {
            //strFolder.trim();
            // passive left trimming
            while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeBeg]))
               iFolderBeg++;
            // passive right trimming
            while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeEnd]))
               iFolderEnd--;
            //better than following 2 together
            //::str::ends_eat(strFolder, "\\");
            //::str::ends_eat(strFolder, "/");
            while(iRelativeBeg <= iRelativeEnd && (strRelative[iRelativeBeg] == '/' || strRelative[iRelativeBeg] == '\\'))
               iRelativeBeg++;
         }

         string strPath;
         if(iFolderBeg > iFolderEnd)
         {
            strPath = strRelative;
         }
         else
         {
            char * psz = strPath.GetBufferSetLength(iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1);
            strncpy(psz, &((const char *)strFolder)[iFolderBeg], iFolderEnd - iFolderBeg + 1);
            #if defined(LINUX) || defined(APPLEOS)
            psz[iFolderEnd - iFolderBeg + 1] = '/';
            #else
            psz[iFolderEnd - iFolderBeg + 1] = '\\';
            #endif
            strncpy(&psz[iFolderEnd - iFolderBeg + 2], &((const char *)strRelative)[iRelativeBeg], iRelativeEnd - iRelativeBeg + 1);
            strPath.ReleaseBuffer(iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1);
         }


         return strPath;
      }

      string system::simple_path(const string & strFolder, const string & strRelative, const string & str2)
      {

         if(strRelative.is_empty())
         {
            if(str2.is_empty())
            {
               if(strFolder.is_empty())
                  return "";
               return strFolder;
            }
            else
            {
               if(strFolder.is_empty())
                  return str2;
               return simple_path(strFolder, str2);
            }
         }
         else if(strFolder.is_empty())
         {
            if(str2.is_empty())
            {
               return strRelative;
            }
            else
            {
               return simple_path(strRelative, str2);
            }
         }
         else if(str2.is_empty())
         {
            return simple_path(strFolder, strRelative);
         }

         // none of them - 3 - are empty

         strsize iFolderBeg = 0;

         strsize iFolderEnd = strFolder.get_length() - 1;

         if(iFolderEnd >= iFolderBeg)
         {
            //strFolder.trim();
            // passive left trimming
            while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderBeg]))
               iFolderBeg++;
            // passive right trimming
            while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderEnd]))
               iFolderEnd--;
            //better than following 2 together
            //::str::ends_eat(strFolder, "\\");
            //::str::ends_eat(strFolder, "/");
            while(iFolderBeg <= iFolderEnd && (strFolder[iFolderEnd] == '/' || strFolder[iFolderEnd] == '\\'))
               iFolderEnd--;
         }

         strsize iRelativeBeg = 0;

         strsize iRelativeEnd = strRelative.get_length() - 1;

         if(iRelativeEnd >= iRelativeBeg)
         {
            //strFolder.trim();
            // passive left trimming
            while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeBeg]))
               iFolderBeg++;
            // passive right trimming
            while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeEnd]))
               iFolderEnd--;
            //better than following 2 together
            //::str::ends_eat(strFolder, "\\");
            //::str::ends_eat(strFolder, "/");
            while(iRelativeBeg <= iRelativeEnd && (strRelative[iRelativeBeg] == '/' || strRelative[iRelativeBeg] == '\\'))
               iRelativeBeg++;
         }


         strsize iBeg2 = 0;

         strsize iEnd2 = str2.get_length() - 1;

         if(iEnd2 >= iBeg2)
         {
            //strFolder.trim();
            // passive left trimming
            while(iBeg2 <= iEnd2 && myspace(str2.m_pszData[iBeg2]))
               iBeg2++;
            // passive right trimming
            while(iBeg2 <= iEnd2 && myspace(str2.m_pszData[iEnd2]))
               iEnd2--;
            //better than following 2 together
            //::str::ends_eat(strFolder, "\\");
            //::str::ends_eat(strFolder, "/");
            while(iBeg2 <= iEnd2 && (str2[iBeg2] == '/' || str2[iBeg2] == '\\'))
               iBeg2++;
         }

         if(iRelativeBeg > iRelativeEnd)
         {
            if(iBeg2 > iEnd2)
            {
               if(iFolderBeg > iFolderEnd)
                  return "";
               return strFolder.Mid(iFolderBeg, iFolderEnd - iFolderBeg + 1);
            }
            else
            {
               if(iFolderBeg > iFolderEnd)
                  return str2.Mid(iBeg2, iEnd2 - iBeg2 + 1);
               string strPath;
               string_STRCAT2_beg_char_end(strPath, '\\', strFolder, str2, iFolderBeg, iFolderEnd, iBeg2, iEnd2);
               return strPath;
            }
         }
         else if(strFolder.is_empty())
         {
            if(str2.is_empty())
            {
               return strRelative;
            }
            else
            {
               string strPath;
               #if defined(LINUX) || defined(APPLEOS)
               string_STRCAT2_beg_char_end(strPath, '/', strRelative, str2, iRelativeBeg, iRelativeEnd, iBeg2, iEnd2);
               #else
               string_STRCAT2_beg_char_end(strPath, '\\', strRelative, str2, iRelativeBeg, iRelativeEnd, iBeg2, iEnd2);
               #endif
               return strPath;
            }
         }
         else if(str2.is_empty())
         {
            string strPath;
            #if defined(LINUX) || defined(APPLEOS)
            string_STRCAT2_beg_char_end(strPath, '\\', strFolder, strRelative, iFolderBeg, iFolderEnd, iRelativeBeg, iRelativeEnd);
            #else
            string_STRCAT2_beg_char_end(strPath, '/', strFolder, strRelative, iFolderBeg, iFolderEnd, iRelativeBeg, iRelativeEnd);
            #endif
            return strPath;
         }

         string strPath;

         char * psz = strPath.GetBufferSetLength(iEnd2 - iBeg2 + 1 + iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1 + 1);
         strncpy(psz, &((const char *)strFolder)[iFolderBeg], iFolderEnd - iFolderBeg + 1);
         #if defined(LINUX) || defined(APPLEOS)
         psz[iFolderEnd - iFolderBeg + 1] = '/';
         #else
         psz[iFolderEnd - iFolderBeg + 1] = '\\';
         #endif
         strncpy(&psz[iFolderEnd - iFolderBeg + 2], &((const char *)strRelative)[iRelativeBeg], iRelativeEnd - iRelativeBeg + 1);
         #if defined(LINUX) || defined(APPLEOS)
         psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1] = '/';
         #else
         psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1] = '\\';
         #endif
         strncpy(&psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1 + 1], &((const char *)str2)[iBeg2], iEnd2 - iBeg2 + 1);
         strPath.ReleaseBuffer(iEnd2 - iBeg2 + 1 + iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1 + 1);

         return strPath;

      }


      string system::relpath(const char * lpcszSource, const char * lpcszRelative, const char * psz2)
      {
         UNREFERENCED_PARAMETER(lpcszSource);
         UNREFERENCED_PARAMETER(lpcszRelative);
         UNREFERENCED_PARAMETER(psz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      void system::root_ones(stringa & straPath, stringa & straTitle, ::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(straPath);
         UNREFERENCED_PARAMETER(straTitle);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::rls_pattern(::aura::application * papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, bool_array * pbaIsDir, int64_array * piaSize, e_extract eextract)
      {

         return false;
         //UNREFERENCED_PARAMETER(pstraRelative);
         //UNREFERENCED_PARAMETER(pszPattern);
         //throw not_implemented(get_app(), "is really a directory or compressed directory/file??");

      }


      void system::ls_pattern(::aura::application * papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, bool_array * pbaIsDir, int64_array * piaSize)
      {
         UNREFERENCED_PARAMETER(pszPattern);

         throw not_implemented(get_app(), "is really a directory or compressed directory/file??");

      }

      void system::ls(::aura::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, bool_array * pbaIsDir, int64_array * piaSize)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(pstraPath);
         UNREFERENCED_PARAMETER(pstraTitle);
         UNREFERENCED_PARAMETER(pbaIsDir);
         UNREFERENCED_PARAMETER(piaSize);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      
      bool system::rls(::aura::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, e_extract eextract)
      {
         
         return false;
      
         //throw interface_only_exception(get_app(),"this is an interface");

      }


      void system::rls_dir(::aura::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(pstraPath);
         UNREFERENCED_PARAMETER(pstraTitle);
         UNREFERENCED_PARAMETER(pstraRelative);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      void system::ls_dir(::aura::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
      {
         throw interface_only_exception(get_app(),"this is an interface");
      }

      bool system::has_subdir(::aura::application * papp, const char * lpcsz)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      void system::ls_file(::aura::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(pstraPath);
         UNREFERENCED_PARAMETER(pstraTitle);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::is_or_definitively_not(bool & bIs, const char * lpcszPath, ::aura::application * papp)
      {

         bIs = false;



#ifdef WINDOWSEX
#ifdef WINDOWSEX
         if(::str::ends_ci(lpcszPath,".lnk"))
         {
#endif

            string strTarget;

            if(System.file().resolve_link(strTarget,lpcszPath))
            {

               bIs = is(strTarget,papp);

               return true;

            }

#ifdef WINDOWSEX
         }
#endif
#endif

         return false;

      }


      bool system::is(const char * lpcszPath, ::aura::application * papp)
      {

         bool bIs;

         if (!is_or_definitively_not(bIs, lpcszPath, papp) || !bIs)
            return false;

         return true;

      }

      bool system::is(const string & strPath, ::aura::application * papp)
      {

         return false;
      }

      bool system::is(const var & var, ::aura::application * papp)
      {
         return is((const string &) var, papp);
      }

      bool system::name_is(const char * lpcszPath, ::aura::application * papp)
      {
         return name_is((const string &) lpcszPath, papp);
      }

      bool system::name_is(const string & strPath, ::aura::application * papp)
      {
         //OutputDebugString(strPath);

         return false;
      }

      bool system::name_is(const var & var, ::aura::application * papp)
      {
         return name_is((const string &) var, papp);
      }


      system::is_dir_map::is_dir_map(::aura::application * papp) :
         string_map < is_dir >(papp, 256), // block size
         m_mutex(papp)
      {
         InitHashTable(16384, TRUE);
      }

      bool system::is_dir_map::lookup(const char * pszPath, bool &bIsDir, uint32_t & dwLastError)
      {
         return lookup(string(pszPath), bIsDir, dwLastError);
      }

      bool system::is_dir_map::lookup(const string & strPath, bool &bIsDir, uint32_t & dwLastError)
      {

         if(strPath.get_length() <= 0)
         {
            bIsDir = false;
            return true;
         }




         string strLookup(strPath);

#if defined(LINUX) || defined(APPLEOS)
         if(strLookup.last_char() != '\\' && strLookup.last_char() != '/' )
            strLookup += "/";
#else
         if(strLookup.last_char() != '\\' && strLookup.last_char() != '/' )
            strLookup += "\\";
#endif

         single_lock sl(&m_mutex, TRUE);

         string_map < is_dir >::pair * ppair = this->PLookup(strLookup);

         if(ppair == NULL)
            return false;

         if(::get_tick_count() > ppair->m_element2.m_dwLastCheck + m_dwTimeOut)
         {
            return false;
         }

         bIsDir = ppair->m_element2.m_bIsDir;

         dwLastError = ppair->m_element2.m_dwError;

         return true;

      }

      bool system::is_dir_map::lookup(const string & strPath, bool &bIsDir, uint32_t &dwLastError, int32_t iLast)
      {

         if(iLast < 0)
         {
            bIsDir = true; // root_ones dir
            return true;
         }

         string strLookup(strPath, iLast + 1);

         single_lock sl(&m_mutex, TRUE);

         string_map < is_dir >::pair * ppair = this->PLookup(strLookup);

         if(ppair == NULL)
            return false;

         if(::get_tick_count() > ppair->m_element2.m_dwLastCheck + m_dwTimeOut)
         {
            return false;
         }

         bIsDir = ppair->m_element2.m_bIsDir;

         dwLastError = ppair->m_element2.m_dwError;

         return true;

      }

      void system::is_dir_map::set(const char * pszPath, bool bIsDir, uint32_t dwLastError)
      {
#if defined(LINUX) || defined(APPLEOS)
         static const char * strSep = "/";
#else
         static const char * strSep = "\\";
#endif
         is_dir isdir;
         isdir.m_bIsDir = bIsDir;
         isdir.m_dwError = dwLastError;
         isdir.m_dwLastCheck = ::get_tick_count();
         string strPath(pszPath);
         if(!::str::ends(strPath, strSep))
            strPath += strSep;
         single_lock sl(&m_mutex, TRUE);
         set_at(strPath, isdir);
      }

      void system::is_dir_map::set(const string & strPath, bool bIsDir, uint32_t dwLastError)
      {
#if defined(LINUX) || defined(APPLEOS)
         static const char * strSep = "/";
#else
         static const char * strSep = "\\";
#endif
         is_dir isdir;
         isdir.m_bIsDir = bIsDir;
         isdir.m_dwError = dwLastError;
         isdir.m_dwLastCheck = ::get_tick_count();
         if(::str::ends(strPath, strSep))
         {
            single_lock sl(&m_mutex, TRUE);
            set_at(strPath, isdir);
         }
         else
         {
            string strPath2(strPath);
            strPath2 += strSep;
            single_lock sl(&m_mutex, TRUE);
            set_at(strPath2, isdir);
         }
      }

      string system::time(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::stage(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::stageapp(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::netseed(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::element(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::element(const string & str, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(str);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::element(const char * lpcsz, const string & str2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(str2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::element(const string & str, const string & str2)
      {
         UNREFERENCED_PARAMETER(str);
         UNREFERENCED_PARAMETER(str2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::element(const string & str)
      {
         UNREFERENCED_PARAMETER(str);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::element()
      {
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::module(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::ca2module(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      void system::time_square(string &str)
      {
         UNREFERENCED_PARAMETER(str);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::time_log(const char * pszId)
      {
         UNREFERENCED_PARAMETER(pszId);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::mk(const char * lpcsz, ::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::rm(::aura::application * papp, const char * psz, bool bRecursive)
      {
         UNREFERENCED_PARAMETER(psz);
         UNREFERENCED_PARAMETER(bRecursive);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::name(const char * path1)
      {
         UNREFERENCED_PARAMETER(path1);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::locale_schema(::aura::application * papp, const string & strLocale, const string & strSchema)
      {

         return papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema);

      }

      stringa system::locale_schema_matter(::aura::application * papp, const string & strLocale, const string & strSchema, const char * pszRoot, const char * pszApp)
      {

         //single_lock sl(&papp->m_mutexMatterLocator, true);
         if (pszRoot != NULL && pszApp != NULL)
         {

            string strRoot(pszRoot);

            string strDomain(pszApp);

            if(Session.m_bMatterFromHttpCache)
            {

               return path(simple_path(strRoot,"appmatter",strDomain),papp->m_paurasession->get_locale_schema_dir(strLocale,strSchema));
            }
            else
            {
               return path(element(simple_path(strRoot,"appmatter",strDomain)),papp->m_paurasession->get_locale_schema_dir(strLocale,strSchema));

            }

         }
         else
         {

            stringa stra;

            for (index i = 0; i < papp->m_straMatterLocator.get_count(); i++)
            {

               stra.add_unique(path(papp->m_straMatterLocator[i], papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema)));

            }

            return stra;

         }


      }

      stringa system::locale_schema_matter(const string & strLocator, const string & strLocale, const string & strSchema)
      {

         return path(strLocator, simple_path(strLocale, strSchema));

      }

      void system::matter_ls(::aura::application * papp, const string & str, stringa & stra)
      {

         string strDir = matter(papp, str, true);

         ls(papp, strDir, &stra);

      }

      void system::matter_ls_file(::aura::application * papp, const string & str, stringa & stra)
      {

         string strDir = matter(papp, str, true);

         if(Sess(papp).m_bMatterFromHttpCache)
         {

            property_set set(get_app());

            set["raw_http"] = true;

            string strFile = System.dir().commonappdata(path("cache", strDir, "list_dir.list_dir"));

            strsize iFind = strFile.find(DIR_SEPARATOR);

            if (iFind > 0)
            {

               strFile.replace(":", "_", iFind + 1);

            }

            strFile.replace("////", "//");
            strFile.replace("\\\\","\\", 1);

            string strLs;

            if(Application.file().exists(strFile))
            {

               strLs = Application.file().as_string(strFile);

            }


            stringa straLs;

            stringa straSep;

            straSep.add("\r");
            straSep.add("\n");
            straSep.add("\r\n");

            straLs.add_smallest_tokens(strLs,straSep,false);

            for(index i = 0; i < straLs.get_count(); i++)
            {

               if(!::str::ends(straLs[i],"/"))
               {
                  string strPath  = System.dir().path(strDir,straLs[i]);
                  stra.add(strPath);
               }

            }

         }
         else
         {

            ls_file(papp,strDir,&stra);

         }


      }

      string system::matter(::aura::application * papp,const stringa & stra,bool bDir,const char * pszRoot,const char * pszApp)
      {
         throw interface_only_exception(get_app());

         return "";

      }

      string system::matter(::aura::application * papp, const char * psz, const char * psz2, bool bDir, const char * pszRoot, const char * pszApp)
      {
         return matter(papp, string(psz), string(psz2), bDir, pszRoot, pszApp);
      }

      string system::matter(::aura::application * papp, const string & str, const char * psz, bool bDir, const char * pszRoot, const char * pszApp)
      {
         return matter(papp, str, string(psz), bDir, pszRoot, pszApp);
      }

      string system::matter(::aura::application * papp, const char * psz, const string & str, bool bDir, const char * pszRoot, const char * pszApp)
      {
         return matter(papp, string(psz), str, bDir, pszRoot, pszApp);
      }


      string system::matter(::aura::application * papp, const string & str, const string & str2, bool bDir, const char * pszRoot, const char * pszApp)
      {
         throw interface_only_exception(get_app());

         return "";

      }


      string system::matter(::aura::application * papp, const char * psz, bool bDir, const char * pszRoot, const char * pszApp)
      {
         string str(psz);
         string str2;
         return matter(papp, str, str2, bDir, pszRoot, pszApp);
      }

      string system::matter(::aura::application * papp, const string & str, bool bDir, const char * pszRoot, const char * pszApp)
      {
         string str2;
         return matter(papp, str, str2, bDir, pszRoot, pszApp);
      }

      string system::matter(::aura::application * papp)
      {
         string str;
         string str2;
         return matter(papp, str, str2);
      }

      string system::matter_from_locator(::aura::str_context * pcontext, const string & strLocator)
      {
         string str;


         string str2;

         return matter_from_locator(pcontext, strLocator, str, str2);

      }

      string system::matter_from_locator(::aura::str_context * pcontext, const string & strLocator, const string & str)
      {

         string str2;

         return matter_from_locator(pcontext, strLocator, str, str2);

      }

      /* static const string strEn("en");
         static const string strStd("_std");
         static const string strEmpty("");

         string strPath;
         string strLs = locale_schema_matter(strLocator, App(papp).get_locale(), App(papp).get_style());
         strPath = path(strLs, str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strLs = locale_schema_matter(strLocator, strEn, App(papp).get_style());
         strPath = path(strLs, str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strPath = path(locale_schema_matter(strLocator, strStd, App(papp).get_style()), str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strPath = path(locale_schema_matter(strLocator, App(papp).get_locale(), App(papp).get_locale()), str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strPath = path(locale_schema_matter(strLocator, App(papp).get_locale(), strEn), str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strPath = path(locale_schema_matter(strLocator, App(papp).get_locale(), strStd), str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strLs = locale_schema_matter(strLocator, strEn, strEn);
         strPath = path(strLs, str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strPath = path(locale_schema_matter(strLocator, strStd, strStd), str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
      */

      string system::matter_from_locator(::aura::str_context * pcontext, const string & strLocator,  const string & str, const string & str2)
      {

         string strPath;

         stringa straLs;

         string strLs;

         string strLocale  = pcontext->m_plocaleschema->m_idLocale;
         string strSchema   = pcontext->m_plocaleschema->m_idSchema;
         straLs      = locale_schema_matter(strLocator, strLocale, strSchema);
         for (index l = 0; l < straLs.get_count(); l++)
         {
            strLs = straLs[l];

            strPath = path(strLs, str, str2);
            if (System.file().exists(strPath, get_app()))
               return strPath;

         }


         for(int32_t i = 0; i < pcontext->m_plocaleschema->m_idaLocale.get_count(); i++)
         {

            strLocale         = pcontext->m_plocaleschema->m_idaLocale[i];
            strSchema          = pcontext->m_plocaleschema->m_idaSchema[i];
            straLs             = locale_schema_matter(strLocator, strLocale, strSchema);

            for (index l = 0; l < straLs.get_count(); l++)
            {
               strLs = straLs[l];

               strPath = path(strLs, str, str2);
               if (System.file().exists(strPath, get_app()))
                  return strPath;

            }

         }


         straLs             = locale_schema_matter(strLocator, "en", "en");

         for (index l = 0; l < straLs.get_count(); l++)
         {

            strLs = straLs[l];

            strPath = path(strLs, str, str2);

            if (System.file().exists(strPath, get_app()))
               return strPath;

         }


         straLs = locale_schema_matter(strLocator, "se", "se");

         for (index l = 0; l < straLs.get_count(); l++)
         {

            strLs = straLs[l];

            strPath = path(strLs, str, str2);

            if (System.file().exists(strPath, get_app()))
               return strPath;

         }

         return path(strLs, str, str2);


      }

      void system::appmatter_locators(string & strRoot, string & strDomain, ::aura::application * papp)
      {

         if(papp->is_system())
         {
            strRoot     = "app";
            strDomain   = "main";
         }
         else
         {

            appmatter_locators(strRoot, strDomain, papp->m_strLibraryName, papp->m_strAppId);


            if(strRoot.is_empty() || strDomain.is_empty())
            {

               strRoot     = "app";
               strDomain   = "main";

            }


         }

      }

      void system::appmatter_locators(string & strRoot, string & strDomain, const string & strAppName)
      {

         appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

      }

      void system::appmatter_locators(string & strRoot, string & strDomain, const string & strLibraryNameParam, const string & strAppNameParam)
      {

         strsize iFind = strAppNameParam.find('/');

         if(iFind < 0)
            return;

         strRoot = strAppNameParam.Left(iFind);

         strsize iFind2 = strAppNameParam.find('/', iFind + 1);

         if(iFind2 > iFind + 1)
         {
            strDomain = "_" + strAppNameParam.Mid(iFind + 1);
            return;
         }
         else
         {
            strDomain = strAppNameParam.Mid(iFind + 1);
         }


         /*string strLibraryRoot;
         string strLibraryName;
         if(strLibraryNameParam.has_char() && strLibraryNameParam != "app_" + strAppNameParam
            && ::str::begins_ci(strLibraryNameParam, "app_") && strLibraryNameParam.find("_", strlen("app_")) > 4)
         {
            stringa stra2;
            stra2.add_tokens(strLibraryNameParam, "_", FALSE);
            strLibraryRoot = stra2[1];
            strLibraryName = stra2.implode("_", 2);
         }
         else
         {
            strLibraryName = strLibraryNameParam;
         }

         int32_t iFind = strAppNameParam.find("/");

         if(iFind > 0)
         {

            strRoot = strAppNameParam.Left(iFind);

         }
         else if(strLibraryRoot.has_char())
         {

            strRoot = "app-" + strLibraryRoot;

         }
         else
         {

            strRoot = "app";

         }

         if(iFind > 0)
         {

            int32_t iFind2 = strAppNameParam.find('/', iFind + 1);

            if(iFind2 < 0)
            {

               strDomain = strAppNameParam.Mid(iFind + 1);

            }
            else
            {

               strDomain = "_" + strAppNameParam.Mid(iFind + 1);

            }


         }
         else
         {

            strDomain += strAppNameParam;

         }

         */
      }

      string system::appmatter_locator(::aura::application * papp)
      {

         string strRoot;
         string strDomain;

         appmatter_locators(strRoot,strDomain,papp);

         if(Session.m_bMatterFromHttpCache)
         {
            return simple_path(strRoot, "appmatter", strDomain);

         }
         else
         {
            return element(simple_path(strRoot,"appmatter",strDomain));

         }

      }

      string system::appmatter_locator(const string & strLibraryName, const string & strAppName)
      {

         string strRoot;
         string strDomain;

         appmatter_locators(strRoot, strDomain, strLibraryName, strAppName);

         return element(simple_path(strRoot, "appmatter", strDomain));

      }

      string system::appmatter_locator(const string & strAppName)
      {

         string strRoot;
         string strDomain;

         appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

#ifdef CUBE

         return simple_path(strRoot, "appmatter", strDomain);

#else

         return element(simple_path(strRoot, "appmatter", strDomain));

#endif

      }

      string system::axis_appmatter_locator(const string & strBase, const string & strLibraryName, const string & strAppName)
      {

         string strRoot;
         string strDomain;

         appmatter_locators(strRoot, strDomain, strLibraryName, strAppName);

         return path(strBase, simple_path(strRoot, "appmatter", strDomain));

      }

      string system::axis_appmatter_locator(const string & strBase, const string & strAppName)
      {

         string strRoot;
         string strDomain;

         appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

         return path(strBase, simple_path(strRoot, "appmatter", strDomain));

      }


      class ::file::path & system::path()
      {
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::initialize()
      {
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::trash_that_is_not_trash(const char * lpcsz)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::appdata(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }


      string system::commonappdata()
      {

            return "/var/lib";

      }

      string system::commonappdata(const char * lpcsz,const char * lpcsz2)
      {

         return element_commonappdata(element(),lpcsz,lpcsz2);

      }

      string system::element_commonappdata(const string & strElement,const char * lpcsz,const char * lpcsz2)
      {

      string strRelative;

      strRelative = strElement;

      index iFind = strRelative.find(':');

      if(iFind >= 0)
      {

         strsize iFind1 = strRelative.reverse_find("\\",iFind);

         strsize iFind2 = strRelative.reverse_find("/",iFind);

         strsize iStart = MAX(iFind1 + 1,iFind2 + 1);

         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart,iFind - iStart) + strRelative.Mid(iFind + 1);

      }

      return path(path(commonappdata(),"ca2",strRelative),lpcsz,lpcsz2);

      }


      string system::usersystemappdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcszPrefix);
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::userappdata(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::userdata(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::userfolder(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::default_os_user_path_prefix(::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(papp);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::default_userappdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcszPrefix);
         UNREFERENCED_PARAMETER(lpcszLogin);
         UNREFERENCED_PARAMETER(pszRelativePath);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::default_userdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcszPrefix);
         UNREFERENCED_PARAMETER(lpcszLogin);
         UNREFERENCED_PARAMETER(pszRelativePath);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::default_userfolder(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcszPrefix);
         UNREFERENCED_PARAMETER(lpcszLogin);
         UNREFERENCED_PARAMETER(pszRelativePath);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::userquicklaunch(::aura::application * papp, const char * lpcszRelativePath, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcszRelativePath);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::userprograms(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(papp);
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::commonprograms(const char * lpcsz, const char * lpcsz2)
      {
         UNREFERENCED_PARAMETER(lpcsz);
         UNREFERENCED_PARAMETER(lpcsz2);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::is_inside_time(const char * pszPath, ::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(pszPath);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      bool system::is_inside(const char * pszDir, const char * pszPath, ::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(pszDir);
         UNREFERENCED_PARAMETER(pszPath);
         throw interface_only_exception(get_app(), "this is an interface");
      }

      string system::pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode, ::aura::application * papp)
      {

         stringa stra;

         stra.add_tokens(pszEnv, ":", false);

         string strCandidate;

         for(int32_t i = 0; i < stra.get_count(); i++)
         {

            strCandidate = path(stra[i], pszTopic);

            if(Sess(papp).file().exists(strCandidate))
            {
               return strCandidate;
            }

         }

         return "";

      }

      string system::get_api_cc()
      {

         if(m_strApiCc.has_char())
            return m_strApiCc;

         synch_lock sl(&m_mutex);



         return m_strApiCc;

      }


   }  // namespace dir


} // namespace file




