#include "framework.h"


#ifdef WINDOWS

#define DIR_SEPARATOR "\\"

#else

#define DIR_SEPARATOR "/"

#endif


namespace file
{


   namespace dir
   {


      namespace axis
      {


         system::system(::aura::application * papp) :
            ::object(papp),
            ::file::dir::system(papp),
            file_watcher(papp)
         {
            ////         m_pziputil = new zip::Util;
            //         m_isdirmap.m_dwTimeOut = 15000;


            System.http().m_pmutexPac = &m_mutex;
            System.http().m_pmutexProxy = &m_mutex;
            System.http().m_pmutexDownload = &m_mutex;

         }

         system::~system()
         {
            //if(m_pziputil != NULL)
            //{
            //   delete m_pziputil;
            //}
         }



         //string system::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl)
         //{
         //   UNREFERENCED_PARAMETER(pszFolder);
         //   UNREFERENCED_PARAMETER(iLenFolder);
         //   UNREFERENCED_PARAMETER(pszRelative);
         //   UNREFERENCED_PARAMETER(iLenRelative);
         //   UNREFERENCED_PARAMETER(psz2);
         //   UNREFERENCED_PARAMETER(iLen2);
         //   UNREFERENCED_PARAMETER(bUrl);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, bool bUrl)
         //{

         //   return path(pszFolder, iLenFolder, pszRelative, iLenRelative, NULL, 0, bUrl);

         //}

         //string system::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2)
         //{

         //   return path(pszFolder, iLenFolder, pszRelative, iLenRelative, psz2, iLen2, ::url::is_url(pszFolder));

         //}

         //string system::path(const string & strFolder, const string & strRelative)
         //{

         //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), NULL, 0, ::url::is_url(strFolder));

         //}


         //string system::path(const string & strFolder, const string & strRelative, const string & str2)
         //{

         //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), str2, str2.get_length(), ::url::is_url(strFolder));

         //}

         //string system::path(const string & strFolder, const string & strRelative, const char * psz2)
         //{
         //   if(psz2 == NULL)
         //   {
         //      return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), ::url::is_url(strFolder));
         //   }
         //   else
         //   {
         //      return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), psz2, strlen(psz2), ::url::is_url(strFolder));
         //   }

         //}

         //string system::path(const string & strFolder, const string & strRelative, bool bUrl)
         //{

         //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), NULL, 0, bUrl);

         //}

         //string system::path(const string & strFolder, const string & strRelative, const string & str2, bool bUrl)
         //{

         //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), str2, str2.get_length(), bUrl);

         //}

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

         //string system::simple_path(const string & strFolder, const string & strRelative)
         //{

         //   if(strRelative.is_empty())
         //   {
         //      if(strFolder.is_empty())
         //         return "";
         //      return strFolder;
         //   }

         //   strsize iFolderBeg = 0;

         //   strsize iFolderEnd = strFolder.get_length() - 1;

         //   if(iFolderEnd >= iFolderBeg)
         //   {
         //      //strFolder.trim();
         //      // passive left trimming
         //      while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderBeg]))
         //         iFolderBeg++;
         //      // passive right trimming
         //      while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderEnd]))
         //         iFolderEnd--;
         //      //better than following 2 together
         //      //::str::ends_eat(strFolder, "\\");
         //      //::str::ends_eat(strFolder, "/");
         //      while(iFolderBeg <= iFolderEnd && (strFolder[iFolderEnd] == '/' || strFolder[iFolderEnd] == '\\'))
         //         iFolderEnd--;
         //   }

         //   strsize iRelativeBeg = 0;

         //   strsize iRelativeEnd = strRelative.get_length() - 1;

         //   if(iRelativeEnd >= iRelativeBeg)
         //   {
         //      //strFolder.trim();
         //      // passive left trimming
         //      while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeBeg]))
         //         iFolderBeg++;
         //      // passive right trimming
         //      while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeEnd]))
         //         iFolderEnd--;
         //      //better than following 2 together
         //      //::str::ends_eat(strFolder, "\\");
         //      //::str::ends_eat(strFolder, "/");
         //      while(iRelativeBeg <= iRelativeEnd && (strRelative[iRelativeBeg] == '/' || strRelative[iRelativeBeg] == '\\'))
         //         iRelativeBeg++;
         //   }

         //   string strPath;
         //   if(iFolderBeg > iFolderEnd)
         //   {
         //      strPath = strRelative;
         //   }
         //   else
         //   {
         //      char * psz = strPath.GetBufferSetLength(iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1);
         //      strncpy(psz, &((const char *)strFolder)[iFolderBeg], iFolderEnd - iFolderBeg + 1);
         //      #if defined(LINUX) || defined(APPLEOS)
         //      psz[iFolderEnd - iFolderBeg + 1] = '/';
         //      #else
         //      psz[iFolderEnd - iFolderBeg + 1] = '\\';
         //      #endif
         //      strncpy(&psz[iFolderEnd - iFolderBeg + 2], &((const char *)strRelative)[iRelativeBeg], iRelativeEnd - iRelativeBeg + 1);
         //      strPath.ReleaseBuffer(iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1);
         //   }


         //   return strPath;
         //}

         //string system::simple_path(const string & strFolder, const string & strRelative, const string & str2)
         //{

         //   if(strRelative.is_empty())
         //   {
         //      if(str2.is_empty())
         //      {
         //         if(strFolder.is_empty())
         //            return "";
         //         return strFolder;
         //      }
         //      else
         //      {
         //         if(strFolder.is_empty())
         //            return str2;
         //         return simple_path(strFolder, str2);
         //      }
         //   }
         //   else if(strFolder.is_empty())
         //   {
         //      if(str2.is_empty())
         //      {
         //         return strRelative;
         //      }
         //      else
         //      {
         //         return simple_path(strRelative, str2);
         //      }
         //   }
         //   else if(str2.is_empty())
         //   {
         //      return simple_path(strFolder, strRelative);
         //   }

         //   // none of them - 3 - are empty

         //   strsize iFolderBeg = 0;

         //   strsize iFolderEnd = strFolder.get_length() - 1;

         //   if(iFolderEnd >= iFolderBeg)
         //   {
         //      //strFolder.trim();
         //      // passive left trimming
         //      while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderBeg]))
         //         iFolderBeg++;
         //      // passive right trimming
         //      while(iFolderBeg <= iFolderEnd && myspace(strFolder[iFolderEnd]))
         //         iFolderEnd--;
         //      //better than following 2 together
         //      //::str::ends_eat(strFolder, "\\");
         //      //::str::ends_eat(strFolder, "/");
         //      while(iFolderBeg <= iFolderEnd && (strFolder[iFolderEnd] == '/' || strFolder[iFolderEnd] == '\\'))
         //         iFolderEnd--;
         //   }

         //   strsize iRelativeBeg = 0;

         //   strsize iRelativeEnd = strRelative.get_length() - 1;

         //   if(iRelativeEnd >= iRelativeBeg)
         //   {
         //      //strFolder.trim();
         //      // passive left trimming
         //      while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeBeg]))
         //         iFolderBeg++;
         //      // passive right trimming
         //      while(iRelativeBeg <= iRelativeEnd && myspace(strRelative[iRelativeEnd]))
         //         iFolderEnd--;
         //      //better than following 2 together
         //      //::str::ends_eat(strFolder, "\\");
         //      //::str::ends_eat(strFolder, "/");
         //      while(iRelativeBeg <= iRelativeEnd && (strRelative[iRelativeBeg] == '/' || strRelative[iRelativeBeg] == '\\'))
         //         iRelativeBeg++;
         //   }


         //   strsize iBeg2 = 0;

         //   strsize iEnd2 = str2.get_length() - 1;

         //   if(iEnd2 >= iBeg2)
         //   {
         //      //strFolder.trim();
         //      // passive left trimming
         //      while(iBeg2 <= iEnd2 && myspace(str2.m_pszData[iBeg2]))
         //         iBeg2++;
         //      // passive right trimming
         //      while(iBeg2 <= iEnd2 && myspace(str2.m_pszData[iEnd2]))
         //         iEnd2--;
         //      //better than following 2 together
         //      //::str::ends_eat(strFolder, "\\");
         //      //::str::ends_eat(strFolder, "/");
         //      while(iBeg2 <= iEnd2 && (str2[iBeg2] == '/' || str2[iBeg2] == '\\'))
         //         iBeg2++;
         //   }

         //   if(iRelativeBeg > iRelativeEnd)
         //   {
         //      if(iBeg2 > iEnd2)
         //      {
         //         if(iFolderBeg > iFolderEnd)
         //            return "";
         //         return strFolder.Mid(iFolderBeg, iFolderEnd - iFolderBeg + 1);
         //      }
         //      else
         //      {
         //         if(iFolderBeg > iFolderEnd)
         //            return str2.Mid(iBeg2, iEnd2 - iBeg2 + 1);
         //         string strPath;
         //         string_STRCAT2_beg_char_end(strPath, '\\', strFolder, str2, iFolderBeg, iFolderEnd, iBeg2, iEnd2);
         //         return strPath;
         //      }
         //   }
         //   else if(strFolder.is_empty())
         //   {
         //      if(str2.is_empty())
         //      {
         //         return strRelative;
         //      }
         //      else
         //      {
         //         string strPath;
         //         #if defined(LINUX) || defined(APPLEOS)
         //         string_STRCAT2_beg_char_end(strPath, '/', strRelative, str2, iRelativeBeg, iRelativeEnd, iBeg2, iEnd2);
         //         #else
         //         string_STRCAT2_beg_char_end(strPath, '\\', strRelative, str2, iRelativeBeg, iRelativeEnd, iBeg2, iEnd2);
         //         #endif
         //         return strPath;
         //      }
         //   }
         //   else if(str2.is_empty())
         //   {
         //      string strPath;
         //      #if defined(LINUX) || defined(APPLEOS)
         //      string_STRCAT2_beg_char_end(strPath, '\\', strFolder, strRelative, iFolderBeg, iFolderEnd, iRelativeBeg, iRelativeEnd);
         //      #else
         //      string_STRCAT2_beg_char_end(strPath, '/', strFolder, strRelative, iFolderBeg, iFolderEnd, iRelativeBeg, iRelativeEnd);
         //      #endif
         //      return strPath;
         //   }

         //   string strPath;

         //   char * psz = strPath.GetBufferSetLength(iEnd2 - iBeg2 + 1 + iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1 + 1);
         //   strncpy(psz, &((const char *)strFolder)[iFolderBeg], iFolderEnd - iFolderBeg + 1);
         //   #if defined(LINUX) || defined(APPLEOS)
         //   psz[iFolderEnd - iFolderBeg + 1] = '/';
         //   #else
         //   psz[iFolderEnd - iFolderBeg + 1] = '\\';
         //   #endif
         //   strncpy(&psz[iFolderEnd - iFolderBeg + 2], &((const char *)strRelative)[iRelativeBeg], iRelativeEnd - iRelativeBeg + 1);
         //   #if defined(LINUX) || defined(APPLEOS)
         //   psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1] = '/';
         //   #else
         //   psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1] = '\\';
         //   #endif
         //   strncpy(&psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1 + 1], &((const char *)str2)[iBeg2], iEnd2 - iBeg2 + 1);
         //   strPath.ReleaseBuffer(iEnd2 - iBeg2 + 1 + iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1 + 1);

         //   return strPath;

         //}


         //string system::relpath(const char * lpcszSource, const char * lpcszRelative, const char * psz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcszSource);
         //   UNREFERENCED_PARAMETER(lpcszRelative);
         //   UNREFERENCED_PARAMETER(psz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //void system::root_ones(stringa & patha, stringa & straTitle, ::aura::application * papp)
         //{
         //   UNREFERENCED_PARAMETER(patha);
         //   UNREFERENCED_PARAMETER(straTitle);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}


         ::file::listing & system::ls(::aura::application * papp, ::file::listing & l)
         {


            return ::file::dir::system::ls(papp, l);

         }

         ::file::listing & system::ls_relative_name(::aura::application * papp, ::file::listing & l)
         {


            return ::file::dir::system::ls_relative_name(papp, l);

         }

         //bool system::rls(::aura::application * papp,const ::file::path & lpcsz,::file::patha & patha,e_extract eextract)
         //{
         //
         //   if(eextract != extract_none && ::get_thread() != NULL && get_thread()->m_bZipIsDir && (::str::ends_ci(lpcsz,".zip") || ::str::find_file_extension("zip:",lpcsz) >= 0))
         //   {

         //      return m_pziputil->ls(papp, lpcsz, false, patha, NULL, NULL, eextract == extract_all ? extract_all : extract_none);

         //   }

         //   return false;
         //
         //}


         //bool system::rls_dir(::aura::application * papp, const ::file::path & lpcsz, ::file::patha & patha)
         //{
         //
         //   return false;

         //}


         //bool system::ls_dir(::aura::application * papp,const ::file::path & lpcsz,::file::patha & patha)
         //{

         //   if(::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::ends_ci(lpcsz,".zip") || ::str::find_file_extension("zip:",lpcsz) >= 0))
         //   {
         //
         //      return m_pziputil->ls_dir(papp, lpcsz, patha);

         //   }

         //   return false;


         //}


         //bool system::ls_file(::aura::application * papp, const ::file::path & lpcsz, ::file::patha & patha, ::file::patha * ppathaName)
         //{

         //   return false;
         //
         //}


         bool system::is_cached(bool & bIs, const ::file::path & lpcszPath, ::aura::application * papp)
         {

            return ::file::dir::system::is_cached(bIs, lpcszPath, papp);

         }


         bool system::is_impl(const ::file::path & strPath, ::aura::application * papp, bool bOptional, bool bNoCache)
         {

            return false;

         }


         bool system::name_is(const ::file::path & strPath, ::aura::application * papp)
         {

            return ::file::dir::system::system::name_is(strPath, papp);

         }


         //      system::is_dir_map::is_dir_map(::aura::application * papp) :
         //         string_map < is_dir >(papp, 256), // block size
         //         m_mutex(papp)
         //      {
         //         InitHashTable(16384, TRUE);
         //      }
         //
         //      bool system::is_dir_map::lookup(const char * pszPath, bool &bIsDir, uint32_t & dwLastError)
         //      {
         //         return lookup(string(pszPath), bIsDir, dwLastError);
         //      }
         //
         //      bool system::is_dir_map::lookup(const string & strPath, bool &bIsDir, uint32_t & dwLastError)
         //      {
         //
         //         if(strPath.get_length() <= 0)
         //         {
         //            bIsDir = false;
         //            return true;
         //         }
         //
         //
         //
         //
         //         string strLookup(strPath);
         //
         //#if defined(LINUX) || defined(APPLEOS)
         //         if(strLookup.last_char() != '\\' && strLookup.last_char() != '/' )
         //            strLookup += "/";
         //#else
         //         if(strLookup.last_char() != '\\' && strLookup.last_char() != '/' )
         //            strLookup += "\\";
         //#endif
         //
         //         single_lock sl(&m_mutex, TRUE);
         //
         //         string_map < is_dir >::pair * ppair = this->PLookup(strLookup);
         //
         //         if(ppair == NULL)
         //            return false;
         //
         //         if(::get_tick_count() > ppair->m_element2.m_dwLastCheck + m_dwTimeOut)
         //         {
         //            return false;
         //         }
         //
         //         bIsDir = ppair->m_element2.m_bIsDir;
         //
         //         dwLastError = ppair->m_element2.m_dwError;
         //
         //         return true;
         //
         //      }
         //
         //      bool system::is_dir_map::lookup(const string & strPath, bool &bIsDir, uint32_t &dwLastError, int32_t iLast)
         //      {
         //
         //         if(iLast < 0)
         //         {
         //            bIsDir = true; // root_ones dir
         //            return true;
         //         }
         //
         //         string strLookup(strPath, iLast + 1);
         //
         //         single_lock sl(&m_mutex, TRUE);
         //
         //         string_map < is_dir >::pair * ppair = this->PLookup(strLookup);
         //
         //         if(ppair == NULL)
         //            return false;
         //
         //         if(::get_tick_count() > ppair->m_element2.m_dwLastCheck + m_dwTimeOut)
         //         {
         //            return false;
         //         }
         //
         //         bIsDir = ppair->m_element2.m_bIsDir;
         //
         //         dwLastError = ppair->m_element2.m_dwError;
         //
         //         return true;
         //
         //      }
         //
         //      void system::is_dir_map::set(const char * pszPath, bool bIsDir, uint32_t dwLastError)
         //      {
         //#if defined(LINUX) || defined(APPLEOS)
         //         static const char * strSep = "/";
         //#else
         //         static const char * strSep = "\\";
         //#endif
         //         is_dir isdir;
         //         isdir.m_bIsDir = bIsDir;
         //         isdir.m_dwError = dwLastError;
         //         isdir.m_dwLastCheck = ::get_tick_count();
         //         string strPath(pszPath);
         //         if(!::str::ends(strPath, strSep))
         //            strPath += strSep;
         //         single_lock sl(&m_mutex, TRUE);
         //         set_at(strPath, isdir);
         //      }
         //
         //      void system::is_dir_map::set(const string & strPath, bool bIsDir, uint32_t dwLastError)
         //      {
         //#if defined(LINUX) || defined(APPLEOS)
         //         static const char * strSep = "/";
         //#else
         //         static const char * strSep = "\\";
         //#endif
         //         is_dir isdir;
         //         isdir.m_bIsDir = bIsDir;
         //         isdir.m_dwError = dwLastError;
         //         isdir.m_dwLastCheck = ::get_tick_count();
         //         if(::str::ends(strPath, strSep))
         //         {
         //            single_lock sl(&m_mutex, TRUE);
         //            set_at(strPath, isdir);
         //         }
         //         else
         //         {
         //            string strPath2(strPath);
         //            strPath2 += strSep;
         //            single_lock sl(&m_mutex, TRUE);
         //            set_at(strPath2, isdir);
         //         }
         //      }

         //string system::time(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::stage(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::stageapp(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::netseed(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::element(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::element(const string & str, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(str);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::element(const char * lpcsz, const string & str2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(str2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::element(const string & str, const string & str2)
         //{
         //   UNREFERENCED_PARAMETER(str);
         //   UNREFERENCED_PARAMETER(str2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::element(const string & str)
         //{
         //   UNREFERENCED_PARAMETER(str);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::install()
         //{
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::module(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::ca2module(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //void system::time_square(string &str)
         //{
         //   UNREFERENCED_PARAMETER(str);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::time_log(const char * pszId)
         //{
         //   UNREFERENCED_PARAMETER(pszId);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //bool system::mk(const char * lpcsz, ::aura::application * papp)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //bool system::rm(::aura::application * papp, const char * psz, bool bRecursive)
         //{
         //   UNREFERENCED_PARAMETER(psz);
         //   UNREFERENCED_PARAMETER(bRecursive);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::name(const char * path1)
         //{
         //   UNREFERENCED_PARAMETER(path1);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //::file::path system::locale_schema(::aura::application * papp, const string & strLocale, const string & strSchema)
         //{

         //   return papp->m_psession->get_locale_schema_dir(strLocale, strSchema);

         //}



//         void system::matter_ls(::aura::application * papp, const ::file::path & str, ::file::patha & stra)
//         {
//
//            ::file::dir::system::matter_ls(papp, str, stra);
//
//            //::file::path strDir = matter(papp, str, true);
//
//            //if (Sess(papp).m_bMatterFromHttpCache)
//            //{
//
//            //   string strMatter = strDir;
//
//            //   strsize iFind1 = strMatter.find_ci("/matter/");
//
//            //   strsize iFind2 = strMatter.find_ci("\\matter\\");
//
//            //   strsize iFind = min_non_neg(iFind1, iFind2);
//
//            //   if(iFind > 0)
//            //   {
//
//            //      strMatter = strMatter.Mid(iFind);
//
//            //   }
//
//            //   property_set set(get_app());
//
//            //   set["raw_http"] = true;
//
//            //   ::file::path strFile = System.dir().cache() / strMatter / "list_dir.list_dir";
//
//            //   iFind = strFile.find(DIR_SEPARATOR);
//
//            //   if (iFind > 0)
//            //   {
//
//            //      strFile.replace(":", "_", iFind + 1);
//
//            //   }
//
//            //   strFile.replace("////", "//");
//            //   strFile.replace("\\\\", "\\", 1);
//
//            //   ::file::path strLs;
//
//            //   if (Application.file().exists(strFile))
//            //   {
//
//            //      strLs = Application.file().as_string(strFile);
//
//            //   }
//            //   else
//            //   {
//
//            //      // todo: keep cache timeout information;
//            //      strLs = Sess(papp).http().get("http://"+get_api_cc()+"/api/matter/list_dir?dir=" + System.url().url_encode(strMatter), set);
//
//            //      Application.file().put_contents(strFile, strLs);
//
//            //   }
//
//
//
//            //   stringa straLs;
//
//            //   stringa straSep;
//
//            //   straSep.add("\r");
//            //   straSep.add("\n");
//            //   straSep.add("\r\n");
//
//            //   straLs.add_smallest_tokens(strLs, straSep, false);
//
//            //   for (index i = 0; i < straLs.get_count(); i++)
//            //   {
//
//            //      ::file::path strPath = strDir / straLs[i];
//
//            //      if (::str::ends(straLs[i], "/"))
//            //      {
//
//            //         strPath.m_iDir = 1;
//
//            //      }
//
//            //      stra.add(strPath);
//
//            //   }
//
//            //}
//            //else
//            //{
//
////               stra = listing(papp).ls(strDir);
//
//            //}
//
//         }


         //void system::matter_ls_file(::aura::application * papp, const ::file::path & str, ::file::patha & stra)
         //{

         //   ::file::path strDir = matter(papp, str, true);

         //   if (Sess(papp).m_bMatterFromHttpCache)
         //   {

         //      string strMatter = strDir;

         //      strsize iFind1 = strMatter.find_ci("/matter/");

         //      strsize iFind2 = strMatter.find_ci("\\matter\\");

         //      strsize iFind = min_non_neg(iFind1, iFind2);

         //      if(iFind > 0)
         //      {

         //         strMatter = strMatter.Mid(iFind);

         //      }

         //      property_set set(get_app());

         //      set["raw_http"] = true;

         //      ::file::path strFile = System.dir().cache() / strDir / "list_dir.list_dir";

         //      iFind = strFile.find(DIR_SEPARATOR);

         //      if (iFind > 0)
         //      {

         //         strFile.replace(":", "_", iFind + 1);

         //      }

         //      strFile.replace("////", "//");
         //      strFile.replace("\\\\", "\\", 1);

         //      ::file::path strLs;

         //      if (Application.file().exists(strFile))
         //      {

         //         strLs = Application.file().as_string(strFile);

         //      }
         //      else
         //      {

         //         // todo: keep cache timeout information;
         //         strLs = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/list_dir?dir=" + System.url().url_encode(strMatter), set);

         //         Application.file().put_contents(strFile, strLs);

         //      }



         //      stringa straLs;

         //      stringa straSep;

         //      straSep.add("\r");
         //      straSep.add("\n");
         //      straSep.add("\r\n");

         //      straLs.add_smallest_tokens(strLs, straSep, false);

         //      for (index i = 0; i < straLs.get_count(); i++)
         //      {

         //         if (!::str::ends(straLs[i], "/"))
         //         {
         //            ::file::path strPath = strDir / straLs[i];
         //            stra.add(strPath);
         //         }

         //      }

         //   }
         //   else
         //   {
         //      stra = listing(papp).ls_file(strDir);
         //   }
         //}


//         ::file::path system::matter(::aura::application * papp, const ::file::patha & stra, bool bDir, const ::file::path & root, const ::file::path & domain)
//         {
//
//            ::aura::str_context * pcontext = Sess(papp).str_context();
//
//            ::index j;
//
//            ::count ca = stra.get_count();
//
//            ::file::path strPath;
//
//            string strLocale;
//
//            string strSchema;
//
//            ::file::patha straLs;
//
//            ::file::path strLs;
//
//            ::file::path strFile;
//
//            if (root.has_char() && domain.has_char())
//            {
//
//               ::index j;
//
//               ::count ca = stra.get_count();
//
//               if (ca <= 0)
//                  return "";
//
//               ::aura::str_context * pcontext = Sess(papp).str_context();
//
//               ::file::path strFile;
//
//               if (Session.m_bMatterFromHttpCache)
//               {
//
//
//                  ::file::patha patha;
//
//                  strLocale = pcontext->m_plocaleschema->m_idLocale;
//
//                  strSchema = pcontext->m_plocaleschema->m_idSchema;
//
//                  straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
//
//                  for (index l = 0; l < straLs.get_count(); l++)
//                  {
//
//                     strLs = straLs[l];
//
//                     strFile = System.dir().cache() / papp->m_psession->get_locale_schema_dir(strLocale, strSchema) / stra.implode(",") + ".map_question";
//
//                     strsize iFind = strFile.find(DIR_SEPARATOR);
//
//                     if (iFind > 0)
//                     {
//
//                        strFile.replace(":", "_", iFind + 1);
//
//                     }
//
//                     strPath = Application.file().as_string(strFile);
//
//                     if (strPath.has_char())
//                     {
//                        // todo: keep cache timeout information;
//                        return strPath;
//                     }
//
//                     for (j = 0; j < ca; j++)
//                     {
//
//                        patha.add(strLs / stra[j]);
//
//                     }
//
//                     for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
//                     {
//
//                        strLocale = pcontext->localeschema().m_idaLocale[i];
//                        strSchema = pcontext->localeschema().m_idaSchema[i];
//                        straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
//
//                        for (int k = 0; k < straLs.get_count(); k++)
//                        {
//
//                           ::file::path strLd = straLs[k];
//
//                           for (j = 0; j < ca; j++)
//                           {
//
//                              patha.add(strLs / stra[j]);
//
//                           }
//
//                        }
//
//                     }
//
//                     straLs = locale_schema_matter(papp, "en", "en");
//                     for (int k = 0; k < straLs.get_count(); k++)
//                     {
//
//                        ::file::path strLd = straLs[k];
//
//                        patha.add(strLs / stra[0]);
//
//                     }
//
//                     property_set set(papp);
//
//                     string strCandidate = patha.implode("|");
//
//                     string strParam = System.url().url_encode(strCandidate);
//
//                     if (bDir)
//                     {
//                        strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_dir?candidate=" + strParam, set);
//                     }
//                     else
//                     {
//                        strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_file?candidate=" + strParam, set);
//                     }
//
//                     if (strPath.has_char())
//                        goto ret;
//
//                  }
//
//               }
//
//               else
//               {
//
//                  strLocale = pcontext->m_plocaleschema->m_idLocale;
//                  strSchema = pcontext->m_plocaleschema->m_idSchema;
//                  straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
//
//                  for (index l = 0; l < straLs.get_count(); l++)
//                  {
//                     strLs = straLs[l];
//
//                     for (j = 0; j < ca; j++)
//                     {
//
//                        strPath = strLs / stra[j];
//
//                        if (bDir)
//                        {
//                           if (System.dir().is(strPath, get_app()))
//                              return strPath;
//                        }
//                        else
//                        {
//                           if (System.file().exists(strPath, get_app()))
//                              return strPath;
//                        }
//
//                     }
//
//
//                     for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
//                     {
//
//                        strLocale = pcontext->localeschema().m_idaLocale[i];
//                        strSchema = pcontext->localeschema().m_idaSchema[i];
//                        straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
//
//                        for (index l = 0; l < straLs.get_count(); l++)
//                        {
//                           strLs = straLs[l];
//
//                           for (j = 0; j < ca; j++)
//                           {
//
//                              strPath = strLs / stra[j];
//
//                              if (bDir)
//                              {
//                                 if (System.dir().is(strPath, get_app()))
//                                    return strPath;
//                              }
//                              else
//                              {
//                                 if (System.file().exists(strPath, get_app()))
//                                    return strPath;
//                              }
//
//                           }
//
//                        }
//
//                     }
//
//                  }
//
//               }
//
//
//
//
//               straLs = locale_schema_matter(papp, "en", "en", root, domain);
//
//               for (index l = 0; l < straLs.get_count(); l++)
//               {
//
//                  strLs = straLs[l];
//
//                  for (j = 0; j < ca; j++)
//                  {
//
//                     strPath = strLs / stra[j];
//
//                     if (bDir)
//                     {
//                        if (System.dir().is(strPath, get_app()))
//                           goto ret;
//                     }
//                     else
//                     {
//                        if (System.file().exists(strPath, get_app()))
//                           goto ret;
//                     }
//
//                  }
//
//
//               }
//
//
//               if (ca <= 0)
//                  return "";
//
//            }
//
//
//            if (Session.m_bMatterFromHttpCache)
//
//            {
//
//
//               ::file::patha patha;
//
//               strLocale = pcontext->m_plocaleschema->m_idLocale;
//               strSchema = pcontext->m_plocaleschema->m_idSchema;
//               straLs = locale_schema_matter(papp, strLocale, strSchema);
//
//               for (index l = 0; l < straLs.get_count(); l++)
//               {
//                  strLs = straLs[l];
//
//                  strFile = System.dir().cache() / papp->m_psession->get_locale_schema_dir(strLocale, strSchema) / stra.implode(",") + ".map_question";
//
//                  strsize iFind = strFile.find(DIR_SEPARATOR);
//
//                  if (iFind > 0)
//                  {
//
//                     strFile.replace(":", "_", iFind + 1);
//
//                  }
//
//                  strPath = Application.file().as_string(strFile);
//
//                  if (strPath.has_char())
//                  {
//                     // todo: keep cache timeout information;
//                     return strPath;
//                  }
//
//                  for (j = 0; j < ca; j++)
//                  {
//
//                     patha.add(strLs / stra[j]);
//
//                  }
//
//                  for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
//                  {
//
//                     strLocale = pcontext->localeschema().m_idaLocale[i];
//                     strSchema = pcontext->localeschema().m_idaSchema[i];
//                     straLs = locale_schema_matter(papp, strLocale, strSchema);
//                     for (int k = 0; k < straLs.get_count(); k++)
//                     {
//
//                        strLs = straLs[k];
//
//                        for (j = 0; j < ca; j++)
//                        {
//
//                           patha.add(strLs / stra[j]);
//
//                        }
//
//                     }
//
//                  }
//
//                  straLs = locale_schema_matter(papp, "en", "en");
//                  for (int k = 0; k < straLs.get_count(); k++)
//                  {
//
//                     strLs = straLs[k];
//                     patha.add(strLs / stra[0]);
//                  }
//
//                  property_set set(papp);
//
//                  string strCandidate = patha.implode("|");
//
//                  string strParam = System.url().url_encode(strCandidate);
//
//                  if (bDir)
//                  {
//                     strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_dir?candidate=" + strParam, set);
//                  }
//                  else
//                  {
//                     strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_file?candidate=" + strParam, set);
//                  }
//
//                  if (strPath.has_char())
//                     goto ret;
//
//               }
//
//            }
//
//            else
//
//            {
//
//               strLocale = pcontext->m_plocaleschema->m_idLocale;
//               strSchema = pcontext->m_plocaleschema->m_idSchema;
//               straLs = locale_schema_matter(papp, strLocale, strSchema);
//
//
//               for (index l = 0; l < straLs.get_count(); l++)
//               {
//
//                  strLs = straLs[l];
//
//                  for (j = 0; j < ca; j++)
//                  {
//
//                     strPath = strLs / stra[j];
//
//                     if (bDir)
//                     {
//                        if (System.dir().is(strPath, get_app()))
//                           return strPath;
//                     }
//                     else
//                     {
//                        if (System.file().exists(strPath, get_app()))
//                           return strPath;
//                     }
//
//                  }
//
//               }
//
//
//               for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
//               {
//
//                  strLocale = pcontext->localeschema().m_idaLocale[i];
//                  strSchema = pcontext->localeschema().m_idaSchema[i];
//                  straLs = locale_schema_matter(papp, strLocale, strSchema);
//
//                  for (index l = 0; l < straLs.get_count(); l++)
//                  {
//
//                     strLs = straLs[l];
//
//                     for (j = 0; j < ca; j++)
//                     {
//
//                        strPath = strLs / stra[j];
//
//                        if (bDir)
//                        {
//                           if (System.dir().is(strPath, get_app()))
//                              return strPath;
//                        }
//                        else
//                        {
//                           if (System.file().exists(strPath, get_app()))
//                              return strPath;
//                        }
//
//                     }
//
//                  }
//
//               }
//
//            }
//
//
//            straLs = locale_schema_matter(papp, "en", "en");
//
//            for (index l = 0; l < straLs.get_count(); l++)
//            {
//
//               strLs = straLs[l];
//
//
//               for (j = 0; j < ca; j++)
//               {
//
//                  strPath = strLs / stra[j];
//
//                  if (bDir)
//                  {
//                     if (System.dir().is(strPath, get_app()))
//                        goto ret;
//                  }
//                  else
//                  {
//                     if (System.file().exists(strPath, get_app()))
//                        goto ret;
//                  }
//
//               }
//
//            }
//
//
//            if (papp->m_psession != NULL && papp->m_psession != papp &&
//                  (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp)
//            {
//               strPath = matter(papp->m_psession, stra, bDir);
//               if (bDir)
//               {
//                  if (System.dir().is(strPath, get_app()))
//                     goto ret;
//               }
//               else
//               {
//                  if (System.file().exists(strPath, get_app()))
//                     goto ret;
//               }
//            }
//
//            if (papp->m_psystem != NULL && papp->m_psystem != papp &&
//                  (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp->m_psession)
//            {
//               strPath = matter(papp->m_psystem, stra, bDir);
//               if (bDir)
//               {
//                  if (System.dir().is(strPath, get_app()))
//                     goto ret;
//               }
//               else
//               {
//                  if (System.file().exists(strPath, get_app()))
//                     goto ret;
//               }
//            }
//
//            strPath = strLs / stra[0];
//
//ret:
//
//            if (Session.m_bMatterFromHttpCache)
//            {
//               Application.file().put_contents(strFile, strPath);
//            }
//
//            return strPath;
//
//         }

         //string system::matter(::aura::application * papp, const char * psz, const char * psz2, bool bDir, const char * pszRoot, const char * pszApp)
         //{
         //   return matter(papp, string(psz), string(psz2), bDir, pszRoot, pszApp);
         //}

         //string system::matter(::aura::application * papp, const string & str, const char * psz, bool bDir, const char * pszRoot, const char * pszApp)
         //{
         //   return matter(papp, str, string(psz), bDir, pszRoot, pszApp);
         //}

         //string system::matter(::aura::application * papp, const char * psz, const string & str, bool bDir, const char * pszRoot, const char * pszApp)
         //{
         //   return matter(papp, string(psz), str, bDir, pszRoot, pszApp);
         //}


//         ::file::path system::matter(::aura::application * papp, const ::file::path & str, bool bDir, const ::file::path & root, const ::file::path & domain)
//         {
//
//
//            return ::file::dir::system::matter(papp, str, bDir, root, domain);
//
////            ::aura::str_context * pcontext = Sess(papp).str_context();
////
////            string strLocale;
////
////            string strSchema;
////
////            ::file::path strLs;
////
////            ::file::path strPath;
////
////            ::file::path strFile;
////
////            ::file::patha straLs;
////
////            string strExistsQuestion;
////
////            if (root.has_char() && domain.has_char())
////            {
////
////               if (Session.m_bMatterFromHttpCache)
////               {
////
////                  ::file::patha patha;
////
////                  strLocale = pcontext->m_plocaleschema->m_idLocale;
////                  strSchema = pcontext->m_plocaleschema->m_idSchema;
////                  straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
////
////                  strFile = System.dir().cache() / papp->m_psession->get_locale_schema_dir(strLocale, strSchema) / str + ".map_question";
////
////                  strsize iFind = strFile.find(DIR_SEPARATOR);
////
////                  if (iFind > 0)
////                  {
////
////                     strFile.replace(":", "_", iFind + 1);
////
////                  }
////
////                  strPath = Application.file().as_string(strFile);
////
////                  if (strPath.has_char())
////                  {
////                     // todo: keep cache timeout information;
////                     return strPath;
////                  }
////
////
////                  for (int k = 0; k < straLs.get_count(); k++)
////                  {
////
////                     strLs = straLs[k];
////
////                     patha.add(strLs / str);
////                  }
////
////                  for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
////                  {
////                     strLocale = pcontext->localeschema().m_idaLocale[i];
////                     strSchema = pcontext->localeschema().m_idaSchema[i];
////                     straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
////                     for (int k = 0; k < straLs.get_count(); k++)
////                     {
////
////                        strLs = straLs[k];
////
////                        patha.add(strLs / str);
////                     }
////                  }
////
////                  straLs = locale_schema_matter(papp, "en", "en");
////                  for (int k = 0; k < straLs.get_count(); k++)
////                  {
////
////                     strLs = straLs[k];
////
////                     patha.add(strLs / str);
////                  }
////
////                  property_set set(papp);
////
////                  set["raw_http"] = true;
////
////
////                  string strCandidate = patha.implode("|");
////
////                  string strParam = System.url().url_encode(strCandidate);
////
////                  if (bDir)
////                  {
////                     strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_dir?candidate=" + strCandidate, set);
////                  }
////                  else
////                  {
////                     strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_file?candidate=" + strCandidate, set);
////                  }
////
////                  strPath.trim();
////
////                  if (strPath.has_char())
////                     goto ret;
////
////               }
////
////               else
////
////               {
////
////                  strLocale = pcontext->m_plocaleschema->m_idLocale;
////                  strSchema = pcontext->m_plocaleschema->m_idSchema;
////                  straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
////
////                  for (index l = 0; l < straLs.get_count(); l++)
////                  {
////                     strLs = straLs[l];
////
////                     strPath = strLs / str;
////                     if (bDir)
////                     {
////                        if (System.dir().is(strPath, papp))
////                           goto ret;
////                     }
////                     else
////                     {
////                        if (System.file().exists(strPath, papp))
////                           goto ret;
////                     }
////
////                  }
////
////
////                  for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
////                  {
////
////                     strLocale = pcontext->localeschema().m_idaLocale[i];
////                     strSchema = pcontext->localeschema().m_idaSchema[i];
////                     straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);
////
////                     for (index l = 0; l < straLs.get_count(); l++)
////                     {
////                        strLs = straLs[l];
////
////                        strPath = strLs / str;
////                        if (bDir)
////                        {
////                           if (System.dir().is(strPath, papp))
////                              goto ret;
////                        }
////                        else
////                        {
////                           if (System.file().exists(strPath, papp))
////                              goto ret;
////                        }
////                     }
////
////                  }
////
////
////                  straLs = locale_schema_matter(papp, "en", "en", root, domain);
////
////                  for (index l = 0; l < straLs.get_count(); l++)
////                  {
////                     strLs = straLs[l];
////
////                     strPath = strLs / str;
////                     if (bDir)
////                     {
////                        if (System.dir().is(strPath, papp))
////                           goto ret;
////                     }
////                     else
////                     {
////                        if (System.file().exists(strPath, papp))
////                           goto ret;
////                     }
////                  }
////
////               }
////
////
////
////
////            }
////
////
////            if (Session.m_bMatterFromHttpCache)
////            {
////
////
////               ::file::patha patha;
////
////               strLocale = pcontext->m_plocaleschema->m_idLocale;
////               strSchema = pcontext->m_plocaleschema->m_idSchema;
////               straLs = locale_schema_matter(papp, strLocale, strSchema);
////
////               strFile = System.dir().commonappdata() / "cache" / papp->m_psession->get_locale_schema_dir(strLocale, strSchema) / str + ".map_question";
////
////               strsize iFind = strFile.find(DIR_SEPARATOR);
////
////               if (iFind > 0)
////               {
////
////                  strFile.replace(":", "_", iFind + 1);
////
////               }
////
////               strPath = Application.file().as_string(strFile);
////
////               if (strPath.has_char())
////               {
////                  // todo: keep cache timeout information;
////                  return strPath;
////               }
////
////
////               for (int k = 0; k < straLs.get_count(); k++)
////               {
////
////                  strLs = straLs[k];
////
////                  patha.add(strLs / str);
////
////               }
////
////               for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
////               {
////                  strLocale = pcontext->localeschema().m_idaLocale[i];
////                  strSchema = pcontext->localeschema().m_idaSchema[i];
////                  straLs = locale_schema_matter(papp, strLocale, strSchema);
////                  for (int k = 0; k < straLs.get_count(); k++)
////                  {
////
////                     strLs = straLs[k];
////
////                     patha.add(strLs / str);
////                  }
////               }
////
////               straLs = locale_schema_matter(papp, "en", "en");
////               for (int k = 0; k < straLs.get_count(); k++)
////               {
////
////                  strLs = straLs[k];
////
////                  patha.add(strLs / str);
////               }
////
////               string strUrl;
////
////               string strCandidate = patha.implode("|");
////
////               string strParam = System.url().url_encode(strCandidate);
////
////               if (bDir)
////               {
////                  strUrl = "https://ca2.cc/api/matter/query_dir?candidate=" + strParam;
////               }
////               else
////               {
////                  strUrl = "https://ca2.cc/api/matter/query_file?candidate=" + strParam;
////               }
////
////               property_set set(papp);
////
////               set["raw_http"] = true;
////
////               output_debug_string("\n");
////               output_debug_string(strUrl);
////               output_debug_string("\n");
////
////               auto & http = App(papp).http();
////
////               strPath = http.get(strUrl, set);
////
////               strPath.trim();
////
////               if (strPath.has_char())
////                  goto ret;
////
////            }
////
////            else
////
////            {
////
////               strLocale = pcontext->m_plocaleschema->m_idLocale;
////               strSchema = pcontext->m_plocaleschema->m_idSchema;
////               straLs = locale_schema_matter(papp, strLocale, strSchema);
////
////               for (index l = 0; l < straLs.get_count(); l++)
////               {
////                  strLs = straLs[l];
////
////                  strPath = strLs / str;
////                  if (bDir)
////                  {
////                     if (System.dir().is(strPath, papp))
////                        goto ret;
////                  }
////                  else
////                  {
////                     if (System.file().exists(strPath, papp))
////                        goto ret;
////                  }
////
////               }
////
////
////               for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
////               {
////
////                  strLocale = pcontext->localeschema().m_idaLocale[i];
////                  strSchema = pcontext->localeschema().m_idaSchema[i];
////                  straLs = locale_schema_matter(papp, strLocale, strSchema);
////
////                  for (index l = 0; l < straLs.get_count(); l++)
////                  {
////                     strLs = straLs[l];
////
////                     strPath = strLs / str;
////                     if (bDir)
////                     {
////                        if (System.dir().is(strPath, papp))
////                           goto ret;
////                     }
////                     else
////                     {
////                        if (System.file().exists(strPath, papp))
////                           goto ret;
////                     }
////
////                  }
////
////               }
////
////
////               straLs = locale_schema_matter(papp, "en", "en");
////
////               for (index l = 0; l < straLs.get_count(); l++)
////               {
////                  strLs = straLs[l];
////
////                  strPath = strLs / str;
////                  if (bDir)
////                  {
////                     if (System.dir().is(strPath, papp))
////                        goto ret;
////                  }
////                  else
////                  {
////                     if (System.file().exists(strPath, papp))
////                        goto ret;
////                  }
////
////               }
////
////
////               if (papp->m_psession != NULL && papp->m_psession != papp &&
////                  (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp
////                  && papp->m_psession->m_bAxisInitialize1)
////               {
////                  strPath = matter(papp->m_psession, str, bDir);
////                  if (bDir)
////                  {
////                     if (System.dir().is(strPath, papp))
////                        goto ret;
////                  }
////                  else
////                  {
////                     if (System.file().exists(strPath, papp))
////                        goto ret;
////                  }
////               }
////
////            }
////
////
////
////            if (papp->m_psystem != NULL && papp->m_psystem != papp &&
////               dynamic_cast < ::aura::application * >(papp->m_psystem) != dynamic_cast < ::aura::application * >(papp->m_psession)
////               && papp->m_psystem->m_bAxisInitialize1)
////            {
////               strPath = matter(papp->m_psystem, str, bDir);
////               if (bDir)
////               {
////                  if (System.dir().is(strPath, get_app()))
////                     goto ret;
////               }
////               else
////               {
////                  if (System.file().exists(strPath, get_app()))
////                     goto ret;
////               }
////
////            }
////
////            strPath = strLs / str;
////
////         ret:
////
////            if (Session.m_bMatterFromHttpCache)
////
////            {
////               Application.file().put_contents(strFile, strPath);
////
////               strFile = strPath;
////
////               strFile.replace(":", "_");
////               strFile.replace("//", "/");
////               strFile.replace("?", "%19");
////               strFile = System.dir().appdata() / "cache" / strFile + ".exists_question";
////
////               Application.file().put_contents(strFile, "yes");
////
////
////            }
////
////            return strPath;
////
////
////            /*static const string strEn("en");
////            //static const string strStd("_std");
////            //static const string strEmpty("");
////            string strPath;
////            string strLs = locale_schema_matter(papp, strEmpty, strEmpty);
////            strPath = strLs / str;
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strLs = locale_schema_matter(papp, strEn, strEmpty);
////            strPath = strLs / str;
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strPath = path(locale_schema_matter(papp, strStd, strEmpty), str);
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strPath = path(locale_schema_matter(papp, strEmpty, App(papp).get_locale()), str);
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strPath = path(locale_schema_matter(papp, strEmpty, strEn), str);
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strPath = path(locale_schema_matter(papp, strEmpty, strStd), str);
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strLs = locale_schema_matter(papp, strEn, strEn);
////            strPath = strLs / str;
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strPath = path(locale_schema_matter(papp, strStd, strStd), str);
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            strPath = path(locale_schema_matter(papp, "se", "se"), str);
////            if(System.file().exists(strPath, papp))
////               return strPath;
////            if(papp->m_psession != NULL && papp->m_psession != papp &&
////               (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp)
////            {
////               strPath = matter(papp->m_psession, str);
////               if(System.file().exists(strPath, papp))
////                  return strPath;
////            }
////            if(papp->m_psystem != NULL && papp->m_psystem != papp &&
////               (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp->m_psession)
////            {
////               strPath = matter(papp->m_psystem, str);
////               if(System.file().exists(strPath, papp))
////                  return strPath;
////            }
////            return path(locale_schema_matter(papp, strEmpty, strEmpty), str);*/
//         }

         //string system::matter(::aura::application * papp, const char * psz, bool bDir, const char * pszRoot, const char * pszApp)
         //{
         //   string str(psz);
         //   string str2;
         //   return matter(papp, str, bDir, pszRoot, pszApp);
         //}

         //string system::matter(::aura::application * papp,const ::file::path & str,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   string str2;
         //   return matter(papp, str, bDir, pszRoot, pszApp);
         //}

         //string system::matter(::aura::application * papp)
         //{
         //   string str;
         //   string str2;
         //   return matter(papp, str);
         //}

         //::file::path system::matter_from_locator(::aura::str_context * pcontext,const string & strLocator)
         //{
         //   string str;


         //   string str2;

         //   return matter_from_locator(pcontext, strLocator, str);

         //}

         //::file::path system::matter_from_locator(::aura::str_context * pcontext,const string & strLocator,const ::file::path & str)
         //{

         //   return matter_from_locator(pcontext, strLocator, str);

         //}

         /* static const string strEn("en");
            //static const string strStd("_std");
            //static const string strEmpty("");

            string strPath;
            string strLs = locale_schema_matter(strLocator, App(papp).get_locale(), App(papp).get_style());
            strPath = strLs / str;
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strLs = locale_schema_matter(strLocator, strEn, App(papp).get_style());
            strPath = strLs / str;
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strPath = path(locale_schema_matter(strLocator, strStd, App(papp).get_style()), str);
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strPath = path(locale_schema_matter(strLocator, App(papp).get_locale(), App(papp).get_locale()), str);
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strPath = path(locale_schema_matter(strLocator, App(papp).get_locale(), strEn), str);
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strPath = path(locale_schema_matter(strLocator, App(papp).get_locale(), strStd), str);
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strLs = locale_schema_matter(strLocator, strEn, strEn);
            strPath = strLs / str;
            if(System.file().exists(strPath, get_app()))
               return strPath;
            strPath = path(locale_schema_matter(strLocator, strStd, strStd), str);
            if(System.file().exists(strPath, get_app()))
               return strPath;
         */

         //::file::path system::matter_from_locator(::aura::str_context * pcontext, const string & strLocator, const ::file::path & str)
         //{

         //   ::file::path strPath;

         //   ::file::patha straLs;

         //   ::file::path strLs;

         //   string strLocale = pcontext->m_plocaleschema->m_idLocale;
         //   string strSchema = pcontext->m_plocaleschema->m_idSchema;
         //   straLs = locale_schema_matter(strLocator, strLocale, strSchema);
         //   for (index l = 0; l < straLs.get_count(); l++)
         //   {
         //      strLs = straLs[l];

         //      strPath = strLs / str;
         //      if (System.file().exists(strPath, get_app()))
         //         return strPath;

         //   }


         //   for (int32_t i = 0; i < pcontext->m_plocaleschema->m_idaLocale.get_count(); i++)
         //   {

         //      strLocale = pcontext->m_plocaleschema->m_idaLocale[i];
         //      strSchema = pcontext->m_plocaleschema->m_idaSchema[i];
         //      straLs = locale_schema_matter(strLocator, strLocale, strSchema);

         //      for (index l = 0; l < straLs.get_count(); l++)
         //      {
         //         strLs = straLs[l];

         //         strPath = strLs / str;
         //         if (System.file().exists(strPath, get_app()))
         //            return strPath;

         //      }

         //   }


         //   straLs = locale_schema_matter(strLocator, "en", "en");

         //   for (index l = 0; l < straLs.get_count(); l++)
         //   {

         //      strLs = straLs[l];

         //      strPath = strLs / str;

         //      if (System.file().exists(strPath, get_app()))
         //         return strPath;

         //   }


         //   straLs = locale_schema_matter(strLocator, "se", "se");

         //   for (index l = 0; l < straLs.get_count(); l++)
         //   {

         //      strLs = straLs[l];

         //      strPath = strLs / str;

         //      if (System.file().exists(strPath, get_app()))
         //         return strPath;

         //   }

         //   return strLs / str;


         //}

         //void system::appmatter_locators(::file::path & strRoot, ::file::path & strDomain, ::aura::application * papp)
         //{

         //   if (papp->is_system())
         //   {
         //      strRoot = "app";
         //      strDomain = "main";
         //   }
         //   else
         //   {

         //      appmatter_locators(strRoot, strDomain, papp->m_strLibraryName, papp->m_strAppId);


         //      if (strRoot.is_empty() || strDomain.is_empty())
         //      {

         //         strRoot = "app";
         //         strDomain = "main";

         //      }


         //   }

         //}

         //void system::appmatter_locators(::file::path & strRoot, ::file::path & strDomain, const string & strAppName)
         //{

         //   appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

         //}

         //void system::appmatter_locators(::file::path & strRoot, ::file::path & strDomain, const string & strLibraryNameParam, const string & strAppNameParam)
         //{

         //   strsize iFind = strAppNameParam.find('/');

         //   if (iFind < 0)
         //      return;

         //   strRoot = strAppNameParam.Left(iFind);

         //   strsize iFind2 = strAppNameParam.find('/', iFind + 1);

         //   if (iFind2 > iFind + 1)
         //   {
         //      strDomain = "_" + strAppNameParam.Mid(iFind + 1);
         //      return;
         //   }
         //   else
         //   {
         //      strDomain = strAppNameParam.Mid(iFind + 1);
         //   }


         //   /*string strLibraryRoot;
         //   string strLibraryName;
         //   if(strLibraryNameParam.has_char() && strLibraryNameParam != "app_" + strAppNameParam
         //      && ::str::begins_ci(strLibraryNameParam, "app_") && strLibraryNameParam.find("_", strlen("app_")) > 4)
         //   {
         //      stringa stra2;
         //      stra2.add_tokens(strLibraryNameParam, "_", FALSE);
         //      strLibraryRoot = stra2[1];
         //      strLibraryName = stra2.implode("_", 2);
         //   }
         //   else
         //   {
         //      strLibraryName = strLibraryNameParam;
         //   }

         //   int32_t iFind = strAppNameParam.find("/");

         //   if(iFind > 0)
         //   {

         //      strRoot = strAppNameParam.Left(iFind);

         //   }
         //   else if(strLibraryRoot.has_char())
         //   {

         //      strRoot = "app-" + strLibraryRoot;

         //   }
         //   else
         //   {

         //      strRoot = "app";

         //   }

         //   if(iFind > 0)
         //   {

         //      int32_t iFind2 = strAppNameParam.find('/', iFind + 1);

         //      if(iFind2 < 0)
         //      {

         //         strDomain = strAppNameParam.Mid(iFind + 1);

         //      }
         //      else
         //      {

         //         strDomain = "_" + strAppNameParam.Mid(iFind + 1);

         //      }


         //   }
         //   else
         //   {

         //      strDomain += strAppNameParam;

         //   }

         //   */
         //}

         //::file::path system::appmatter_locator(::aura::application * papp)
         //{

         //   ::file::path strRoot;
         //   ::file::path strDomain;

         //   appmatter_locators(strRoot, strDomain, papp);

         //   if (Session.m_bMatterFromHttpCache)
         //   {

         //      output_debug_string("MATTER m_bMatterFromHttpCache");

         //      return ::file::path(strRoot) / "appmatter" / strDomain;

         //   }
         //   else
         //   {

         //      output_debug_string("MATTER from file system");

         //      return install() / strRoot / "appmatter" / strDomain;

         //   }

         //}


         //::file::path system::appmatter_locator(const string & strLibraryName, const string & strAppName)
         //{

         //   ::file::path strRoot;
         //   ::file::path strDomain;

         //   appmatter_locators(strRoot, strDomain, strLibraryName, strAppName);

         //   return install() / strRoot / "appmatter" / strDomain;

         //}

         //::file::path system::appmatter_locator(const string & strAppName)
         //{

         //   ::file::path pathRoot;

         //   ::file::path pathDomain;

         //   string strLibrary = System.m_mapAppLibrary[strAppName];

         //   appmatter_locators(pathRoot, pathDomain, strLibrary, strAppName);

         //   if(Session.m_bMatterFromHttpCache)
         //   {

         //      return pathRoot / "appmatter" / pathDomain;

         //   }
         //   else
         //   {

         //      return install() / pathRoot / "appmatter" / pathDomain;

         //   }

         //}


         //::file::path system::base_appmatter_locator(const ::file::path & strBase, const string & strLibraryName, const string & strAppName)
         //{

         //   ::file::path strRoot;
         //   ::file::path strDomain;

         //   appmatter_locators(strRoot, strDomain, strLibraryName, strAppName);

         //   return strBase / strRoot / "appmatter" / strDomain;

         //}

         //::file::path system::base_appmatter_locator(const ::file::path & strBase, const string & strAppName)
         //{

         //   ::file::path strRoot;
         //   ::file::path strDomain;

         //   appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

         //   return strBase / strRoot / "appmatter" / strDomain;

         //}


         //class ::file::path & system::path()
         //{
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //bool system::initialize()
         //{
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::trash_that_is_not_trash(const char * lpcsz)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::appdata(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}


         //string system::commonappdata()
         //{

         //      return "/var/lib";

         //}

         //string system::commonappdata(const char * lpcsz,const char * lpcsz2)
         //{

         //   return element_commonappdata(install(),lpcsz,lpcsz2);

         //}

         //string system::element_commonappdata(const string & strElement,const char * lpcsz,const char * lpcsz2)
         //{

         //string strRelative;

         //strRelative = strElement;

         //index iFind = strRelative.find(':');

         //if(iFind >= 0)
         //{

         //   strsize iFind1 = strRelative.reverse_find("\\",iFind);

         //   strsize iFind2 = strRelative.reverse_find("/",iFind);

         //   strsize iStart = MAX(iFind1 + 1,iFind2 + 1);

         //   strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart,iFind - iStart) + strRelative.Mid(iFind + 1);

         //}

         //return path(path(commonappdata(),"ca2",strRelative),lpcsz,lpcsz2);

         //}


         //string system::usersystemappdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcszPrefix);
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::userappdata(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::userdata(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::userfolder(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::default_os_user_path_prefix(::aura::application * papp)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::default_userappdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcszPrefix);
         //   UNREFERENCED_PARAMETER(lpcszLogin);
         //   UNREFERENCED_PARAMETER(pszRelativePath);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::default_userdata(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcszPrefix);
         //   UNREFERENCED_PARAMETER(lpcszLogin);
         //   UNREFERENCED_PARAMETER(pszRelativePath);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::default_userfolder(::aura::application * papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcszPrefix);
         //   UNREFERENCED_PARAMETER(lpcszLogin);
         //   UNREFERENCED_PARAMETER(pszRelativePath);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::userquicklaunch(::aura::application * papp, const char * lpcszRelativePath, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcszRelativePath);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::userprograms(::aura::application * papp, const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(papp);
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::commonprograms(const char * lpcsz, const char * lpcsz2)
         //{
         //   UNREFERENCED_PARAMETER(lpcsz);
         //   UNREFERENCED_PARAMETER(lpcsz2);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //bool system::is_inside_time(const char * pszPath, ::aura::application * papp)
         //{
         //   UNREFERENCED_PARAMETER(pszPath);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //bool system::is_inside(const char * pszDir, const char * pszPath, ::aura::application * papp)
         //{
         //   UNREFERENCED_PARAMETER(pszDir);
         //   UNREFERENCED_PARAMETER(pszPath);
         //   _throw(interface_only_exception(get_app(), "this is an interface"));
         //}

         //string system::pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode, ::aura::application * papp)
         //{

         //   stringa stra;

         //   stra.add_tokens(pszEnv, ":", false);

         //   string strCandidate;

         //   for(int32_t i = 0; i < stra.get_count(); i++)
         //   {

         //      strCandidate = path(stra[i], pszTopic);

         //      if(Sess(papp).file().exists(strCandidate))
         //      {
         //         return strCandidate;
         //      }

         //   }

         //   return "";

         //}

         ::file::path system::get_api_cc()
         {

            synch_lock sl(&m_mutex);

            if (m_strApiCc.has_char())
               return m_strApiCc;

            if (&Session == NULL || Session.fontopus().is_null() || Session.fontopus().m_p->m_pthreadCreatingUser != NULL)
               return "api.ca2.cc";

            string strApiServer = "ca2.cc";

            if (Session.fontopus()->m_strFirstFontopusServer.has_char())
            {

               strApiServer = ::str::replace("account", "", Session.fontopus()->m_strFirstFontopusServer);

            }

            string strGetFontopus("https://ca2.cc/get_fontopus_login"); //("https://api.ca2.cc/get_fontopus_login");

            string strFontopusServer = Session.fontopus()->get_server(strGetFontopus, 8);

            strApiServer = strFontopusServer;

            strApiServer.replace("account", "api");

            if (strApiServer.is_empty())
            {

               strApiServer = "api.ca2.cc";

            }

            m_strApiCc = strApiServer;

            return m_strApiCc;

         }

      } // namespace axis


   }  // namespace dir


} // namespace file




