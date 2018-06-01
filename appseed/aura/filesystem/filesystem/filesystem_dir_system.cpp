﻿#include "framework.h"


#include "aura/aura/compress/zip/zip.h"



#ifdef WINDOWS

#define DIR_SEPARATOR "\\"

#else

#define DIR_SEPARATOR "/"

#endif


namespace file
{


   namespace dir
   {


      system * system::g_pthis = NULL;


      system::system(::aura::application * papp) :
         ::object(papp),
         m_mutex(papp),
         ::file_watcher::file_watcher(papp) //,
         //::file_watcher::listener_thread(papp)
      {

         m_isdirmap.m_dwTimeOut = 180000;

         m_pathInstall = ::dir::install();

      }

      system::~system()
      {
//         if(m_pziputil != NULL)
//         {
////            delete m_pziputil;
//         }
      }



      //::file::path system::path(const string & pszFolder, strsize iLenFolder, const string & pszRelative, strsize iLenRelative, const string & psz2, strsize iLen2, bool bUrl)
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

      //::file::path system::path(const string & pszFolder, strsize iLenFolder, const string & pszRelative, strsize iLenRelative, bool bUrl)
      //{

      //   return path(pszFolder, iLenFolder, pszRelative, iLenRelative, NULL, 0, bUrl);

      //}

      //::file::path system::path(const string & pszFolder, strsize iLenFolder, const string & pszRelative, strsize iLenRelative, const string & psz2, strsize iLen2)
      //{

      //   return path(pszFolder, iLenFolder, pszRelative, iLenRelative, psz2, iLen2, ::url::is_url(pszFolder));

      //}

      //::file::path system::path(const string & strFolder, const string & strRelative)
      //{

      //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), NULL, 0, ::url::is_url(strFolder));

      //}


      //::file::path system::path(const string & strFolder, const string & strRelative, const string & str2)
      //{

      //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), str2, str2.get_length(), ::url::is_url(strFolder));

      //}


      //::file::path system::sibling(const string & strPath,const string & strRelative,bool bUrl)
      //{

      //   return path(name(strPath),strRelative,bUrl);

      //}


      //::file::path system::sibling(const string & strPath,const string & strRelative,const string & str2,bool bUrl)
      //{

      //   return path(name(strPath),strRelative,str2,bUrl);

      //}


      //::file::path system::sibling(const string & strPath,const string & strRelative)
      //{

      //   return sibling(strPath,strRelative,::url::is_url(strPath));

      //}


      //::file::path system::sibling(const string & strPath,const string & strRelative,const string & str2)
      //{

      //   return sibling(strPath,strRelative, str2,::url::is_url(strPath));

      //}


      //::file::path system::path(const string & strFolder, const string & strRelative, bool bUrl)
      //{

      //   return path(strFolder, strFolder.get_length(), strRelative, strRelative.get_length(), NULL, 0, bUrl);

      //}

      //::file::path system::path(const string & strFolder, const string & strRelative, const string & str2, bool bUrl)
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
      string & psz = strCat.GetBufferSetLength(end1 - beg1 + 1 + end2 - beg2 + 1 + 1); \
      strncpy(psz, &((const string &)str1)[beg1], end1 - beg1 + 1); \
      psz[end1 - beg1 + 1] = ch; \
      strncpy(&psz[end1 - beg1 + 2], &((const string &)str2)[beg2], end2 - beg2 + 1); \
      strPath.ReleaseBuffer(end1 - beg1 + 1 + end2 - beg2 + 1 + 1); \
   }

      //::file::path system::simple_path(const string & strFolder, const string & strRelative)
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
      //      string & psz = strPath.GetBufferSetLength(iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1);
      //      strncpy(psz, &((const string &)strFolder)[iFolderBeg], iFolderEnd - iFolderBeg + 1);
      //      #if defined(LINUX) || defined(APPLEOS)
      //      psz[iFolderEnd - iFolderBeg + 1] = '/';
      //      #else
      //      psz[iFolderEnd - iFolderBeg + 1] = '\\';
      //      #endif
      //      strncpy(&psz[iFolderEnd - iFolderBeg + 2], &((const string &)strRelative)[iRelativeBeg], iRelativeEnd - iRelativeBeg + 1);
      //      strPath.ReleaseBuffer(iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1);
      //   }


      //   return strPath;
      //}

      //::file::path system::simple_path(const string & strFolder, const string & strRelative, const string & str2)
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

      //   string & psz = strPath.GetBufferSetLength(iEnd2 - iBeg2 + 1 + iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1 + 1);
      //   strncpy(psz, &((const string &)strFolder)[iFolderBeg], iFolderEnd - iFolderBeg + 1);
      //   #if defined(LINUX) || defined(APPLEOS)
      //   psz[iFolderEnd - iFolderBeg + 1] = '/';
      //   #else
      //   psz[iFolderEnd - iFolderBeg + 1] = '\\';
      //   #endif
      //   strncpy(&psz[iFolderEnd - iFolderBeg + 2], &((const string &)strRelative)[iRelativeBeg], iRelativeEnd - iRelativeBeg + 1);
      //   #if defined(LINUX) || defined(APPLEOS)
      //   psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1] = '/';
      //   #else
      //   psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1] = '\\';
      //   #endif
      //   strncpy(&psz[iFolderEnd - iFolderBeg + 1 + 1 + iRelativeEnd - iRelativeBeg + 1 + 1], &((const string &)str2)[iBeg2], iEnd2 - iBeg2 + 1);
      //   strPath.ReleaseBuffer(iEnd2 - iBeg2 + 1 + iRelativeEnd - iRelativeBeg + 1 + iFolderEnd - iFolderBeg + 1 + 1 + 1);

      //   return strPath;

      //}


      //::file::path system::relpath(const string & lpcszSource,const string & lpcszRelative)
      //{
      //   UNREFERENCED_PARAMETER(lpcszSource);
      //   UNREFERENCED_PARAMETER(lpcszRelative);
      //   _throw(interface_only_exception(get_app(),"this is an interface"));
      //}


      //::file::path system::relpath(const string & lpcszSource, const string & lpcszRelative, const string & psz2)
      //{
      //   UNREFERENCED_PARAMETER(lpcszSource);
      //   UNREFERENCED_PARAMETER(lpcszRelative);
      //   UNREFERENCED_PARAMETER(psz2);
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}


      ::file::listing & system::root_ones(::file::listing & listing,::aura::application * papp)
      {

         UNREFERENCED_PARAMETER(listing);

         _throw(interface_only_exception(get_app(), "this is an interface"));

      }


      ::file::listing & system::ls(::aura::application * papp, listing & l)
      {

         if (l.m_path.begins_ci("matter://"))
         {

            ::file::path path = l.m_path;

            ::str::begins_eat_ci(path, "matter://");

            matter_ls(papp, path, l);

            return l;

         }

         if (l.m_bRecursive)
         {

            if (l.m_eextract != extract_none && ::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (icmp(l.m_path.ext(), ".zip") == 0 || l.m_path.find_ci("zip:") >= 0))
            {

               //_throw(simple_exception(get_app(), "should implement recursive zip"));

               //m_pziputil->ls(papp,l);

               l.m_cres = failure_no_log;

            }
            else
            {

               l.m_cres = failure_no_log;


            }


         }
         else
         {

            if (::str::begins_ci(l.m_path, "http://") || ::str::begins_ci(l.m_path, "https://"))
            {

               property_set set(get_app());

               string str = Sess(papp).http().get(l.m_path, set);

               l.add_tokens(str, "\n", false);

            }
            else if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::ends_ci(l.m_path, ".zip") || ::str::find_file_extension("zip:", l.m_path) >= 0))
            {

               m_pziputil->ls(papp, l);

            }
            else
            {

               l.m_cres = failure_no_log;


            }

         }

         return l;

      }

      ::file::listing & system::ls_relative_name(::aura::application * papp, listing & l)
      {

         if (l.m_bRecursive)
         {

            if (l.m_eextract != extract_none && ::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (icmp(l.m_path.ext(), ".zip") == 0 || l.m_path.find_ci("zip:") >= 0))
            {

               //_throw(simple_exception(get_app(), "should implement recursive zip"));

               //m_pziputil->ls(papp,l);

               l.m_cres = failure_no_log;

            }
            else
            {

               l.m_cres = failure_no_log;


            }


         }
         else
         {

            if (::str::begins_ci(l.m_path, "http://") || ::str::begins_ci(l.m_path, "https://"))
            {

               property_set set(get_app());

               string str = Sess(papp).http().get(l.m_path, set);

               l.add_tokens(str, "\n", false);

            }
            else if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::ends_ci(l.m_path, ".zip") || ::str::find_file_extension("zip:", l.m_path) >= 0))
            {

               m_pziputil->ls(papp, l);

            }
            else
            {

               l.m_cres = failure_no_log;


            }

         }

         return l;

      }

      bool system::has_subdir(::aura::application * papp,const ::file::path & psz)
      {

         ::file::listing ls(papp);

         ls.ls_dir(psz);

         return ls.get_count() > 0;

      }


      bool system::is_cached(bool & bIs, const ::file::path & lpcszPath, ::aura::application * papp)
      {

         bIs = false;



#ifdef WINDOWSEX
#ifdef WINDOWSEX
         if(::str::ends_ci(lpcszPath,".lnk"))
         {
#endif

            string strTarget;

            string strFolder;

            string strParams;

            if(System.file().resolve_link(strTarget,strFolder, strParams, lpcszPath))
            {

               bIs = is(strTarget,papp);

               return true;

            }

#ifdef WINDOWSEX
         }
#endif
#endif

         //bIs = false;

         //if (::str::begins_ci(lpcszPath, "http://") || ::str::begins_ci(lpcszPath, "https://"))
         //{

         //   property_set set(get_app());

         //   bIs = Sess(papp).http().exists(lpcszPath, set);

         //   return true;

         //}

         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::ends_ci(lpcszPath, ".zip")))
         {

            bIs = true;

            return true;
         }

         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::find_file_extension("zip:", lpcszPath) >= 0))
         {

            bool bHasSubFolder;

            uint32_t dwLastError;

            if (m_isdirmap.lookup(lpcszPath, bHasSubFolder, dwLastError))
            {

               bIs = bHasSubFolder;

               return true;

            }

            bHasSubFolder = m_pziputil->has_sub_folder(papp, lpcszPath);

            m_isdirmap.set(lpcszPath, bHasSubFolder, ::get_last_error());

            bIs = bHasSubFolder;

            return true;

         }

//#ifdef WINDOWSEX
//#ifdef WINDOWSEX
//         if (::str::ends_ci(lpcszPath, ".lnk"))
//         {
//#endif
//
//            string strTarget;
//
//            string strFolder;
//
//            string strParams;
//
//            if (System.file().resolve_link(strTarget, strFolder, strParams, lpcszPath))
//            {
//
//               bIs = is(strTarget, papp);
//
//               return true;
//
//            }
//
//#ifdef WINDOWSEX
//         }
//#endif
//#endif

         bool bHasSubFolder;

         u32 dwLastError;

         if (!m_isdirmap.lookup(lpcszPath, bHasSubFolder, dwLastError))
         {
            return false;
         }
         bIs = bHasSubFolder;
         return true;

      }


      bool system::is(const ::file::path & path,::aura::application * papp)
      {

         bool bIs;

         if (is_cached(bIs, path, papp))
         {

            return bIs;

         }

         return is_impl(path, papp);

      }


      bool system::is_impl(const ::file::path & path, ::aura::application * papp)
      {

         if (path.ends_ci("://") || path.ends_ci(":/") || path.ends_ci(":"))
         {

            return true;

         }

         if (::str::begins_ci(path, "http://") || ::str::begins_ci(path, "https://"))
         {

            property_set set(get_app());

            return Sess(papp).http().exists(path, set);

         }

         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::ends_ci(path, ".zip")))
         {

            m_isdirmap.set(path, true, 0);

            return true;

         }

         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::find_file_extension("zip:", path) >= 0))
         {

            bool bHasSubFolder;

            bHasSubFolder = m_pziputil->has_sub_folder(papp, path);

            m_isdirmap.set(path, bHasSubFolder, get_last_error());

            return bHasSubFolder;

         }

         return false;

      }


      bool system::name_is(const ::file::path & strPath,::aura::application * papp)
      {

         //output_debug_string(strPath);
         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::ends_ci(strPath, ".zip")))
         {
            m_isdirmap.set(strPath, true, 0);
            return true;
         }
         if (::get_thread() != NULL && ::get_thread()->m_bZipIsDir && (::str::find_file_extension("zip:", strPath) >= 0))
         {
            bool bHasSubFolder;
            uint32_t dwLastError;
            if (m_isdirmap.lookup(strPath, bHasSubFolder, dwLastError))
               return bHasSubFolder;
            bHasSubFolder = m_pziputil->has_sub_folder(papp, strPath);
            m_isdirmap.set(strPath, bHasSubFolder, get_last_error());
            return bHasSubFolder;
         }
         return false;


      }


      system::is_dir_map::is_dir_map()
      {

      }


      bool system::is_dir_map::lookup(const ::file::path & path, bool &bIsDir, u32 & dwLastError)
      {

         return lookup(path, bIsDir, dwLastError, path.length());

      }


      bool system::is_dir_map::lookup(const ::file::path & path, bool &bIsDir, u32 &dwLastError, i32 iLastChar)
      {

         if (path.get_length() <= 0)
         {

            bIsDir = false;

            return true;

         }

         if (iLastChar < 0)
         {

            bIsDir = true; // root_ones dir

            return true;

         }

         cslock sl(&m_cs);

         if (path.length() + 1 < 2048)
         {

            return lookup_small(path, bIsDir, dwLastError, iLastChar);

         }
         else
         {

            return lookup_dynamic(path, bIsDir, dwLastError, iLastChar);

         }

      }


      bool system::is_dir_map::lookup_dynamic(const ::file::path & path, bool &bIsDir, u32 & dwLastError, i32 iLastChar)
      {

         is_dir * pdir = this;

         is_dir find;

         index iFind0 = 0;

         index iFind3 = 0;

         while (iFind3 >= 0 && iFind3 < iLastChar)
         {

            index iFind1 = path.find('/', iFind0);

            index iFind2 = path.find('\\', iFind0);

            iFind3 = min_non_neg(iFind1, iFind2);

            if (iFind3 < 0)
            {

               find.m_str = path.Mid(iFind0);

            }
            else
            {

               find.m_str = path.Mid(iFind0, iFind3 - iFind0);

            }

            index iFind = pdir->pred_binary_search(&find, [&](auto & t1, auto & t2)
            {

               return t1->m_str.compare_ci(t2->m_str) < 0;

            });

            if (iFind < 0)
            {

               return false;

            }

            pdir = pdir->element_at(iFind);

            iFind0 = iFind3 + 1;

         }

         if (::get_fast_tick_count() > pdir->m_dwLastCheck + m_dwTimeOut)
         {

            return false;

         }

         if (pdir->m_iIsDir < 0)
         {

            return false;

         }

         bIsDir = pdir->m_iIsDir == 1;

         dwLastError = pdir->m_dwError;

         return true;

      }


      bool system::is_dir_map::lookup_small(const ::file::path & path,bool &bIsDir,u32 &dwLastError, i32 iLastChar)
      {

         const char * pszEnd = path.c_str() + iLastChar;

         char sz[2048];

         is_dir_work find;

         find.m_psz = sz;

         strcpy(sz, path);

         is_dir * pdir = this;

         char * psz3 = find.m_psz;

         while (psz3 != NULL && psz3 < pszEnd)
         {

            char * psz1 = strchr(find.m_psz, '/');

            char * psz2 = strchr(find.m_psz, '\\');

            psz3 = min_non_null(psz1, psz2);

            if (psz3 != NULL)
            {

               *psz3 = '\0';

            }

            index iFind = pdir->pred_binary_search(&find, [&](auto & t1, auto & t2)
            {

               return stricmp(t1->c_str(), t2->c_str()) < 0;

            });

            if (iFind < 0)
            {

               return false;

            }

            pdir = pdir->element_at(iFind);

            find.m_psz = psz3 + 1;

            if (psz1 == NULL)
            {

               goto lookup_backslash;

            }
            else if (psz2 == NULL)
            {

               goto lookup_slash;

            }

         }

         goto end;

lookup_slash:

         while (psz3 != NULL && psz3 < pszEnd)
         {

            psz3 = strchr(find.m_psz, '/');

            if (psz3 != NULL)
            {

               *psz3 = '\0';

            }

            index iFind = pdir->pred_binary_search(&find, [&](auto & t1, auto & t2)
            {

               return stricmp(t1->c_str(), t2->c_str()) < 0;

            });

            if (iFind < 0)
            {

               return false;

            }

            pdir = pdir->element_at(iFind);

            find.m_psz = psz3 + 1;

         }

         goto end;

lookup_backslash:
         while (psz3 != NULL && psz3 < pszEnd)
         {

            psz3 = strchr(find.m_psz, '\\');

            if (psz3 != NULL)
            {

               *psz3 = '\0';

            }

            index iFind = pdir->pred_binary_search(&find, [&](auto & t1, auto & t2)
            {

               return stricmp(t1->c_str(), t2->c_str()) < 0;

            });

            if (iFind < 0)
            {

               return false;

            }

            pdir = pdir->element_at(iFind);

            find.m_psz = psz3 + 1;

         }

end:
         if (::get_fast_tick_count() > pdir->m_dwLastCheck + m_dwTimeOut)
         {

            return false;

         }

         if (pdir->m_iIsDir < 0)
         {

            return false;

         }

         bIsDir = pdir->m_iIsDir == 1;

         dwLastError = pdir->m_dwError;

         return true;

      }


      void system::is_dir_map::set(const ::file::path & path,bool bIsDir,uint32_t dwLastError)
      {

         cslock sl(&m_cs);

         string strPath(path);

         is_dir * pdir = this;

         sp(is_dir) pfind(canew(is_dir));

         index iFind0 = 0;
         index iFind3 = 0;
         while (iFind3 >= 0)
         {

            index iFind1 = strPath.find('/', iFind0);
            index iFind2 = strPath.find('\\', iFind0);
            iFind3 = min_non_neg(iFind1, iFind2);
            if (iFind3 < 0)
            {
               pfind->m_str = strPath.Mid(iFind0);
            }
            else
            {
               pfind->m_str = strPath.Mid(iFind0, iFind3 - iFind0);
            }

            index iFind = pdir->pred_binary_search(pfind, [&](auto & t1, auto & t2)
            {

               return t1->m_str.compare_ci(t2->m_str) < 0;

            });

            if (iFind < 0)
            {
               while (true)
               {


                  index iFind1 = strPath.find('/', iFind0);
                  index iFind2 = strPath.find('\\', iFind0);
                  iFind3 = min_non_neg(iFind1, iFind2);
                  if (iFind3 < 0)
                  {
                     pfind->m_str = strPath.Mid(iFind0);
                  }
                  else
                  {
                     pfind->m_str = strPath.Mid(iFind0, iFind3 - iFind0);
                  }
                  pfind->m_iIsDir = bIsDir ? 1 : (iFind3 >= 0 ? -1 : 0);

                  pfind->m_dwError = dwLastError;

                  pfind->m_dwLastCheck = ::get_fast_tick_count();

                  pdir->add(pfind);

                  pdir->pred_sort([&](auto & t1, auto & t2)
                  {

                     return t1->m_str.compare_ci(t2->m_str) < 0;

                  });

                  if (iFind3 < 0)
                  {

                     return;

                  }

                  pdir = pfind;

                  pfind = canew(is_dir);
                  iFind0 = iFind3 + 1;
               }
            }

            pdir = pdir->element_at(iFind);

            if (bIsDir)
            {

               pdir->m_iIsDir = 1;

            }

            pdir->m_dwLastCheck = ::get_fast_tick_count();
            iFind0 = iFind3 + 1;
         }


         pdir->m_iIsDir = bIsDir ? 1 : 0;

         pfind->m_dwError = dwLastError;

      }


      ::file::path system::time()
      {
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::stage()
      {
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::stageapp()
      {
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::netseed()
      {
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::install()
      {

         synch_lock sl(m_pmutex);

         return m_pathInstall;

      }

      ::file::path system::config()
      {

         synch_lock sl(m_pmutex);

         return m_pathCa2Config;

      }


      ::file::path system::home()
      {

         synch_lock sl(m_pmutex);

         return m_pathHome;

      }

      //::file::path system::userfolder(::aura::application * papp)
      //{

      //   UNREFERENCED_PARAMETER(papp);

      //   synch_lock sl(m_pmutex);

      //   return m_pathUser;

      //}


      ::file::path system::module()
      {

         synch_lock sl(m_pmutex);

         return m_pathModule;

      }


      ::file::path system::ca2module()
      {

         synch_lock sl(m_pmutex);

         return m_pathCa2Module;

      }


      ::file::path system::time_square(::aura::application * papp)
      {

         UNREFERENCED_PARAMETER(papp);

         return time() / "time";

      }


      ::file::path system::time_log(const string & pszId)
      {

         UNREFERENCED_PARAMETER(pszId);
         _throw(interface_only_exception(get_app(), "this is an interface"));

      }


      bool system::mk(const ::file::path & path,::aura::application * papp)
      {

         UNREFERENCED_PARAMETER(path);
         _throw(interface_only_exception(get_app(), "this is an interface"));

      }


      bool system::rm(::aura::application * papp,const ::file::path & path,bool bRecursive)
      {

         UNREFERENCED_PARAMETER(path);
         UNREFERENCED_PARAMETER(bRecursive);
         _throw(interface_only_exception(get_app(), "this is an interface"));

      }


      //::file::path system::name(const ::file::path & path1)
      //{
      //   UNREFERENCED_PARAMETER(path1);
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}

      ::file::path system::locale_schema(::aura::application * papp, const string & strLocale, const string & strSchema)
      {

         return papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema);

      }



      ::file::patha system::locale_schema_matter(::aura::application * papp,const string & strLocale,const string & strSchema,const ::file::path & strRoot,const ::file::path & strDomain)
      {

         //single_lock sl(&papp->m_mutexMatterLocator, true);
         if(strRoot.has_char() && strDomain.has_char())
         {

            ::file::patha patha;

            if(Session.m_bMatterFromHttpCache)
            {

               return   {strRoot / "appmatter" / strDomain / papp->m_paurasession->get_locale_schema_dir(strLocale,strSchema)};
            }
            else
            {
               return {install() / strRoot / "appmatter" / strDomain / papp->m_paurasession->get_locale_schema_dir(strLocale,strSchema) };

            }

         }
         else
         {

            ::file::patha stra;

            for (index i = 0; i < papp->m_straMatterLocator.get_count(); i++)
            {

               stra.add_unique(papp->m_straMatterLocator[i] / papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema));

            }

            return stra;

         }


      }

      ::file::patha system::locale_schema_matter(const string & strLocator, const string & strLocale, const string & strSchema)
      {

         return{::file::path(strLocator) / strLocale / strSchema};

      }


      void system::matter_ls(::aura::application * papp, const ::file::path & path, ::file::patha & stra)
      {

         string strDir = matter(papp, path, true);

         if (Sess(papp).m_bMatterFromHttpCache)
         {

            string strMatter = strDir;

            strsize iFind1 = strMatter.find_ci("/matter/");

            strsize iFind2 = strMatter.find_ci("\\matter\\");

            strsize iFind = min_non_neg(iFind1, iFind2);

            if(iFind > 0)
            {

               strMatter = strMatter.Mid(iFind);

            }

            property_set set(get_app());

            set["raw_http"] = true;

            ::file::path strFile = System.dir().commonappdata() / "cache" / strMatter / "list_dir.list_dir";

            iFind = strFile.find(DIR_SEPARATOR);

            if (iFind > 0)
            {

               strFile.replace(":", "_", iFind + 1);

            }

            strFile.replace("////", "//");
            strFile.replace("\\\\", "\\", 1);

            ::file::path strLs;

            if (Application.file().exists(strFile))
            {

               strLs = Application.file().as_string(strFile);

            }
            else
            {

               // todo: keep cache timeout information;

               string strUrl = "https://" + get_api_cc() + "/api/matter/list_dir?dir=" + System.url().url_encode(strMatter);

               strLs = Sess(papp).http().get(strUrl, set);

               Application.file().put_contents(strFile, strLs);

            }



            stringa straLs;

            stringa straSep;

            straSep.add("\r");
            straSep.add("\n");
            straSep.add("\r\n");

            straLs.add_smallest_tokens(strLs, straSep, false);

            for (index i = 0; i < straLs.get_count(); i++)
            {

               ::file::path strPath = strDir / straLs[i];

               if (::str::ends(straLs[i], "/"))
               {

                  strPath.m_iDir = 1;

               }

               stra.add(strPath);

            }

         }
         else
         {

            stra = listing(papp).ls(strDir);

         }

      }


      void system::matter_ls_file(::aura::application * papp,const ::file::path & str,::file::patha & stra)
      {

         ::file::path strDir = matter(papp, str, true);

         if(Sess(papp).m_bMatterFromHttpCache)
         {

            property_set set(get_app());

            set["raw_http"] = true;

            string strFile = System.dir().commonappdata() / "cache"/ strDir/ "list_dir.list_dir";

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


            ::file::patha straLs;

            stringa straSep;

            straSep.add("\r");
            straSep.add("\n");
            straSep.add("\r\n");

            straLs.add_smallest_tokens(strLs,straSep,false);

            for(index i = 0; i < straLs.get_count(); i++)
            {

               if(!::str::ends(straLs[i],"/"))
               {
                  ::file::path strPath  = strDir / straLs[i];
                  stra.add(strPath);
               }

            }

         }
         else
         {

            stra = listing(papp).ls(strDir);

         }


      }

      ::file::path system::matter(::aura::application * papp,const ::file::patha & stra,bool bDir,const ::file::path & root,const ::file::path & domain)
      {

         ::aura::str_context * pcontext = Sess(papp).str_context();

         ::index j;

         ::count ca = stra.get_count();

         ::file::path strPath;

         string strLocale;

         string strSchema;

         ::file::patha straLs;

         ::file::path strLs;

         ::file::path strFile;

         if (root.has_char() && domain.has_char())
         {

            ::index j;

            ::count ca = stra.get_count();

            if (ca <= 0)
               return "";

            ::aura::str_context * pcontext = Sess(papp).str_context();

            ::file::path strFile;

            if (Session.m_bMatterFromHttpCache)
            {


               ::file::patha patha;

               strLocale = pcontext->m_plocaleschema->m_idLocale;

               strSchema = pcontext->m_plocaleschema->m_idSchema;

               straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

               for (index l = 0; l < straLs.get_count(); l++)
               {

                  strLs = straLs[l];

                  strFile = System.dir().commonappdata() / "cache" / papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema) / stra.implode(",") + ".map_question";

                  strsize iFind = strFile.find(DIR_SEPARATOR);

                  if (iFind > 0)
                  {

                     strFile.replace(":", "_", iFind + 1);

                  }

                  strPath = Application.file().as_string(strFile);

                  if (strPath.has_char())
                  {
                     // todo: keep cache timeout information;
                     return strPath;
                  }

                  for (j = 0; j < ca; j++)
                  {

                     patha.add(strLs / stra[j]);

                  }

                  for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
                  {

                     strLocale = pcontext->localeschema().m_idaLocale[i];
                     strSchema = pcontext->localeschema().m_idaSchema[i];
                     straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

                     for (int k = 0; k < straLs.get_count(); k++)
                     {

                        ::file::path strLd = straLs[k];

                        for (j = 0; j < ca; j++)
                        {

                           patha.add(strLs / stra[j]);

                        }

                     }

                  }

                  straLs = locale_schema_matter(papp, "en", "en");
                  for (int k = 0; k < straLs.get_count(); k++)
                  {

                     ::file::path strLd = straLs[k];

                     patha.add(strLs / stra[0]);

                  }

                  property_set set(papp);

                  string strCandidate = patha.implode("|");

                  string strParam = System.url().url_encode(strCandidate);

                  if (bDir)
                  {
                     strPath = Sess(papp).http().get("https://" + get_api_cc() + "/api/matter/query_dir?candidate=" + strParam, set);
                  }
                  else
                  {
                     strPath = Sess(papp).http().get("https://" + get_api_cc() + "/api/matter/query_file?candidate=" + strParam, set);
                  }

                  if (strPath.has_char())
                     goto ret;

               }

            }

            else
            {

               strLocale = pcontext->m_plocaleschema->m_idLocale;
               strSchema = pcontext->m_plocaleschema->m_idSchema;
               straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

               for (index l = 0; l < straLs.get_count(); l++)
               {
                  strLs = straLs[l];

                  for (j = 0; j < ca; j++)
                  {

                     strPath = strLs / stra[j];

                     if (bDir)
                     {
                        if (System.dir().is(strPath, get_app()))
                           return strPath;
                     }
                     else
                     {
                        if (System.file().exists(strPath, get_app()))
                           return strPath;
                     }

                  }


                  for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
                  {

                     strLocale = pcontext->localeschema().m_idaLocale[i];
                     strSchema = pcontext->localeschema().m_idaSchema[i];
                     straLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

                     for (index l = 0; l < straLs.get_count(); l++)
                     {
                        strLs = straLs[l];

                        for (j = 0; j < ca; j++)
                        {

                           strPath = strLs / stra[j];

                           if (bDir)
                           {
                              if (System.dir().is(strPath, get_app()))
                                 return strPath;
                           }
                           else
                           {
                              if (System.file().exists(strPath, get_app()))
                                 return strPath;
                           }

                        }

                     }

                  }

               }

            }




            straLs = locale_schema_matter(papp, "en", "en", root, domain);

            for (index l = 0; l < straLs.get_count(); l++)
            {

               strLs = straLs[l];

               for (j = 0; j < ca; j++)
               {

                  strPath = strLs / stra[j];

                  if (bDir)
                  {
                     if (System.dir().is(strPath, get_app()))
                        goto ret;
                  }
                  else
                  {
                     if (System.file().exists(strPath, get_app()))
                        goto ret;
                  }

               }


            }


            if (ca <= 0)
               return "";

         }


         if (Session.m_bMatterFromHttpCache)

         {


            ::file::patha patha;

            strLocale = pcontext->m_plocaleschema->m_idLocale;
            strSchema = pcontext->m_plocaleschema->m_idSchema;
            straLs = locale_schema_matter(papp, strLocale, strSchema);

            for (index l = 0; l < straLs.get_count(); l++)
            {
               strLs = straLs[l];

               strFile = System.dir().commonappdata() / "cache" / papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema) / stra.implode(",") + ".map_question";

               strsize iFind = strFile.find(DIR_SEPARATOR);

               if (iFind > 0)
               {

                  strFile.replace(":", "_", iFind + 1);

               }

               strPath = Application.file().as_string(strFile);

               if (strPath.has_char())
               {
                  // todo: keep cache timeout information;
                  return strPath;
               }

               for (j = 0; j < ca; j++)
               {

                  patha.add(strLs / stra[j]);

               }

               for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
               {

                  strLocale = pcontext->localeschema().m_idaLocale[i];
                  strSchema = pcontext->localeschema().m_idaSchema[i];
                  straLs = locale_schema_matter(papp, strLocale, strSchema);
                  for (int k = 0; k < straLs.get_count(); k++)
                  {

                     strLs = straLs[k];

                     for (j = 0; j < ca; j++)
                     {

                        patha.add(strLs / stra[j]);

                     }

                  }

               }

               straLs = locale_schema_matter(papp, "en", "en");
               for (int k = 0; k < straLs.get_count(); k++)
               {

                  strLs = straLs[k];
                  patha.add(strLs / stra[0]);
               }

               property_set set(papp);

               string strCandidate = patha.implode("|");

               string strParam = System.url().url_encode(strCandidate);

               if (bDir)
               {
                  strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_dir?candidate=" + strParam, set);
               }
               else
               {
                  strPath = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_file?candidate=" + strParam, set);
               }

               if (strPath.has_char())
                  goto ret;

            }

         }

         else

         {

            strLocale = pcontext->m_plocaleschema->m_idLocale;
            strSchema = pcontext->m_plocaleschema->m_idSchema;
            straLs = locale_schema_matter(papp, strLocale, strSchema);


            for (index l = 0; l < straLs.get_count(); l++)
            {

               strLs = straLs[l];

               for (j = 0; j < ca; j++)
               {

                  strPath = strLs / stra[j];

                  if (bDir)
                  {
                     if (System.dir().is(strPath, get_app()))
                        return strPath;
                  }
                  else
                  {
                     if (System.file().exists(strPath, get_app()))
                        return strPath;
                  }

               }

            }


            for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
            {

               strLocale = pcontext->localeschema().m_idaLocale[i];
               strSchema = pcontext->localeschema().m_idaSchema[i];
               straLs = locale_schema_matter(papp, strLocale, strSchema);

               for (index l = 0; l < straLs.get_count(); l++)
               {

                  strLs = straLs[l];

                  for (j = 0; j < ca; j++)
                  {

                     strPath = strLs / stra[j];

                     if (bDir)
                     {
                        if (System.dir().is(strPath, get_app()))
                           return strPath;
                     }
                     else
                     {
                        if (System.file().exists(strPath, get_app()))
                           return strPath;
                     }

                  }

               }

            }

         }


         straLs = locale_schema_matter(papp, "en", "en");

         for (index l = 0; l < straLs.get_count(); l++)
         {

            strLs = straLs[l];


            for (j = 0; j < ca; j++)
            {

               strPath = strLs / stra[j];

               if (bDir)
               {
                  if (System.dir().is(strPath, get_app()))
                     goto ret;
               }
               else
               {
                  if (System.file().exists(strPath, get_app()))
                     goto ret;
               }

            }

         }


         if (papp->m_paurasession != NULL && papp->m_paurasession != papp &&
               (sp(::aura::application)) papp->m_paurasystem != (sp(::aura::application)) papp)
         {
            strPath = matter(papp->m_paurasession, stra, bDir);
            if (bDir)
            {
               if (System.dir().is(strPath, get_app()))
                  goto ret;
            }
            else
            {
               if (System.file().exists(strPath, get_app()))
                  goto ret;
            }
         }

         if (papp->m_paurasystem != NULL && papp->m_paurasystem != papp &&
               (sp(::aura::application)) papp->m_paurasystem != (sp(::aura::application)) papp->m_paurasession)
         {
            strPath = matter(papp->m_paurasystem, stra, bDir);
            if (bDir)
            {
               if (System.dir().is(strPath, get_app()))
                  goto ret;
            }
            else
            {
               if (System.file().exists(strPath, get_app()))
                  goto ret;
            }
         }

         strPath = strLs / stra[0];

ret:

         if (Session.m_bMatterFromHttpCache)
         {
            Application.file().put_contents(strFile, strPath);
         }

         return strPath;

      }

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


      ::file::path system::matter(::aura::application * papp, const ::file::path & str, bool bDir, const ::file::path & root, const ::file::path & domain)
      {

         ::aura::str_context * pcontext = Sess(papp).str_context();

         string strLocale;

         string strSchema;

         ::file::path path;

         ::file::path pathLs;

         ::file::path pathFind;

         ::file::path pathMapFile;

         ::file::patha pathaLs;

         string strExistsQuestion;

         if (root.has_char() && domain.has_char())
         {

            if (Session.m_bMatterFromHttpCache)
            {

               ::file::patha patha;

               strLocale = pcontext->m_plocaleschema->m_idLocale;
               strSchema = pcontext->m_plocaleschema->m_idSchema;
               pathaLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

               pathMapFile = System.dir().commonappdata() / "cache" / papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema) / str + ".map_question";

               strsize iFind = pathMapFile.find(DIR_SEPARATOR);

               if (iFind > 0)
               {

                  pathMapFile.replace(":", "_", iFind + 1);

               }

               pathFind = Application.file().as_string(pathMapFile);

               if (pathFind.has_char())
               {

                  // todo: keep cache timeout information;

                  return pathFind;

               }

               for (int k = 0; k < pathaLs.get_count(); k++)
               {

                  pathLs = pathaLs[k];

                  path = pathLs / str;

                  patha.add(path);

               }

               for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
               {

                  strLocale = pcontext->localeschema().m_idaLocale[i];

                  strSchema = pcontext->localeschema().m_idaSchema[i];

                  pathaLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

                  for (int k = 0; k < pathaLs.get_count(); k++)
                  {

                     pathLs = pathaLs[k];

                     path = pathLs / path;

                     patha.add(path);

                  }

               }

               pathaLs = locale_schema_matter(papp, "en", "en");

               for (int k = 0; k < pathaLs.get_count(); k++)
               {

                  pathLs = pathaLs[k];

                  path = pathLs / str;

                  patha.add(path);

               }

               property_set set(papp);

               set["raw_http"] = true;

               string strCandidate = patha.implode("|");

               string strParam = System.url().url_encode(strCandidate);

               if (bDir)
               {

                  pathFind = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_dir?candidate=" + strCandidate, set);

               }
               else
               {

                  pathFind = Sess(papp).http().get("http://" + get_api_cc() + "/api/matter/query_file?candidate=" + strCandidate, set);

               }

               pathFind.trim();

               if (pathFind.has_char())
               {

                  goto ret;

               }

            }

            else
            {

               strLocale = pcontext->m_plocaleschema->m_idLocale;

               strSchema = pcontext->m_plocaleschema->m_idSchema;

               pathaLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

               for (index l = 0; l < pathaLs.get_count(); l++)
               {

                  pathLs = pathaLs[l];

                  pathFind = pathLs / str;

                  if (bDir)
                  {

                     if (System.dir().is(pathFind, papp))
                     {

                        goto ret;

                     }

                  }
                  else
                  {

                     if (System.file().exists(pathFind, papp))
                     {

                        goto ret;

                     }

                  }

               }


               for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
               {

                  strLocale = pcontext->localeschema().m_idaLocale[i];

                  strSchema = pcontext->localeschema().m_idaSchema[i];

                  pathaLs = locale_schema_matter(papp, strLocale, strSchema, root, domain);

                  for (index l = 0; l < pathaLs.get_count(); l++)
                  {

                     pathLs = pathaLs[l];

                     pathFind = pathLs / str;

                     if (bDir)
                     {

                        if (System.dir().is(pathFind, papp))
                        {

                           goto ret;

                        }

                     }
                     else
                     {

                        if (System.file().exists(pathFind, papp))
                        {

                           goto ret;

                        }

                     }
                  }

               }


               pathaLs = locale_schema_matter(papp, "en", "en", root, domain);

               for (index l = 0; l < pathaLs.get_count(); l++)
               {

                  pathLs = pathaLs[l];

                  pathFind = pathLs / str;

                  if (bDir)
                  {

                     if (System.dir().is(pathFind, papp))
                     {

                        goto ret;

                     }

                  }
                  else
                  {

                     if (System.file().exists(pathFind, papp))
                     {

                        goto ret;

                     }

                  }
               }

            }

         }

         if (Session.m_bMatterFromHttpCache)
         {


            ::file::patha patha;

            strLocale = pcontext->m_plocaleschema->m_idLocale;

            strSchema = pcontext->m_plocaleschema->m_idSchema;

            pathaLs = locale_schema_matter(papp, strLocale, strSchema);

            pathMapFile = System.dir().commonappdata() / "cache" / papp->m_paurasession->get_locale_schema_dir(strLocale, strSchema) / str + ".map_question";

            strsize iFind = pathMapFile.find(DIR_SEPARATOR);

            if (iFind > 0)
            {

               pathMapFile.replace(":", "_", iFind + 1);

            }

            pathFind = Application.file().as_string(pathMapFile);

            if (pathFind.has_char())
            {

               // todo: keep cache timeout information;

               return pathFind;

            }


            for (int k = 0; k < pathaLs.get_count(); k++)
            {

               pathLs = pathaLs[k];

               path = pathLs / str;

               patha.add(path);

            }

            for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
            {

               strLocale = pcontext->localeschema().m_idaLocale[i];

               strSchema = pcontext->localeschema().m_idaSchema[i];

               pathaLs = locale_schema_matter(papp, strLocale, strSchema);

               for (int k = 0; k < pathaLs.get_count(); k++)
               {

                  pathLs = pathaLs[k];

                  path = pathLs / str;

                  patha.add(path);

               }

            }

            pathaLs = locale_schema_matter(papp, "en", "en");

            for (int k = 0; k < pathaLs.get_count(); k++)
            {

               pathLs = pathaLs[k];

               path = pathLs / str;

               patha.add(path);

            }

            string strUrl;

            string strCandidate = patha.implode("|");

            strCandidate.replace("\\", "/");

            string strParam = System.url().url_encode(strCandidate);

            if (bDir)
            {

               strUrl = "https://ca2.cc/api/matter/query_dir?candidate=" + strParam;

            }
            else
            {

               strUrl = "https://ca2.cc/api/matter/query_file?candidate=" + strParam;

            }

            property_set set(papp);

            set["raw_http"] = true;

            output_debug_string("\n");
            output_debug_string(strUrl);
            output_debug_string("\n");

            auto & http = App(papp).http();

            pathFind = http.get(strUrl, set);

            pathFind.trim();

            if (pathFind.has_char())
            {

               goto ret;

            }

         }
         else
         {

            strLocale = pcontext->m_plocaleschema->m_idLocale;

            strSchema = pcontext->m_plocaleschema->m_idSchema;

            pathaLs = locale_schema_matter(papp, strLocale, strSchema);

            for (index l = 0; l < pathaLs.get_count(); l++)
            {

               pathLs = pathaLs[l];

               pathFind = pathLs / str;

               if (bDir)
               {

                  if (System.dir().is(pathFind, papp))
                  {

                     goto ret;

                  }

               }
               else
               {

                  if (System.file().exists(pathFind, papp))
                  {

                     goto ret;

                  }

               }

            }

            for (int32_t i = 0; i < pcontext->localeschema().m_idaLocale.get_count(); i++)
            {

               strLocale = pcontext->localeschema().m_idaLocale[i];

               strSchema = pcontext->localeschema().m_idaSchema[i];

               pathaLs = locale_schema_matter(papp, strLocale, strSchema);

               for (index l = 0; l < pathaLs.get_count(); l++)
               {

                  pathLs = pathaLs[l];

                  pathFind = pathLs / str;

                  if (bDir)
                  {

                     if (System.dir().is(pathFind, papp))
                     {

                        goto ret;

                     }

                  }
                  else
                  {

                     if (System.file().exists(pathFind, papp))
                     {

                        goto ret;

                     }

                  }

               }

            }

            pathaLs = locale_schema_matter(papp, "en", "en");

            for (index l = 0; l < pathaLs.get_count(); l++)
            {

               pathLs = pathaLs[l];

               pathFind = pathLs / str;

               if (bDir)
               {

                  if (System.dir().is(pathFind, papp))
                  {

                     goto ret;

                  }

               }
               else
               {

                  if (System.file().exists(pathFind, papp))
                  {

                     goto ret;

                  }

               }

            }

            if (papp->m_paurasession != NULL && papp->m_paurasession != papp &&
                  (sp(::aura::application)) papp->m_paurasystem != (sp(::aura::application)) papp
                  && papp->m_paurasession->m_bAuraInitialize1)
            {

               pathFind = matter(papp->m_paurasession, str, bDir);

               if (bDir)
               {

                  if (System.dir().is(pathFind, papp))
                  {

                     goto ret;

                  }

               }
               else
               {

                  if (System.file().exists(pathFind, papp))
                  {

                     goto ret;

                  }

               }

            }

         }

         if (papp->m_paurasystem != NULL && papp->m_paurasystem != papp &&
               dynamic_cast < ::aura::application * >(papp->m_paurasystem) != dynamic_cast < ::aura::application * >(papp->m_paurasession)
               && papp->m_paurasystem->m_bAuraInitialize1)
         {

            pathFind = matter(papp->m_paurasystem, str, bDir);

            if (bDir)
            {

               if (System.dir().is(pathFind, get_app()))
               {

                  goto ret;

               }

            }
            else
            {

               if (System.file().exists(pathFind, get_app()))
               {

                  goto ret;

               }

            }

         }

         pathFind = pathLs / str;

ret:

         if (Session.m_bMatterFromHttpCache)
         {

            Application.file().put_contents(pathMapFile, pathFind);

            path = pathFind;

            path.replace(":", "_");
            path.replace("//", "/");
            path.replace("?", "%19");

            path = System.dir().appdata() / "cache" / path + ".exists_question";

            Application.file().put_contents(path, "yes");

         }

         return pathFind;


         /*static const string strEn("en");
         //static const string strStd("_std");
         //static const string strEmpty("");
         string strPath;
         string strLs = locale_schema_matter(papp, strEmpty, strEmpty);
         strPath = strLs / str;
         if(System.file().exists(strPath, papp))
         return strPath;
         strLs = locale_schema_matter(papp, strEn, strEmpty);
         strPath = strLs / str;
         if(System.file().exists(strPath, papp))
         return strPath;
         strPath = path(locale_schema_matter(papp, strStd, strEmpty), str);
         if(System.file().exists(strPath, papp))
         return strPath;
         strPath = path(locale_schema_matter(papp, strEmpty, App(papp).get_locale()), str);
         if(System.file().exists(strPath, papp))
         return strPath;
         strPath = path(locale_schema_matter(papp, strEmpty, strEn), str);
         if(System.file().exists(strPath, papp))
         return strPath;
         strPath = path(locale_schema_matter(papp, strEmpty, strStd), str);
         if(System.file().exists(strPath, papp))
         return strPath;
         strLs = locale_schema_matter(papp, strEn, strEn);
         strPath = strLs / str;
         if(System.file().exists(strPath, papp))
         return strPath;
         strPath = path(locale_schema_matter(papp, strStd, strStd), str);
         if(System.file().exists(strPath, papp))
         return strPath;
         strPath = path(locale_schema_matter(papp, "se", "se"), str);
         if(System.file().exists(strPath, papp))
         return strPath;
         if(papp->m_psession != NULL && papp->m_psession != papp &&
         (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp)
         {
         strPath = matter(papp->m_psession, str);
         if(System.file().exists(strPath, papp))
         return strPath;
         }
         if(papp->m_psystem != NULL && papp->m_psystem != papp &&
         (sp(::aura::application)) papp->m_psystem != (sp(::aura::application)) papp->m_psession)
         {
         strPath = matter(papp->m_psystem, str);
         if(System.file().exists(strPath, papp))
         return strPath;
         }
         return path(locale_schema_matter(papp, strEmpty, strEmpty), str);*/
      }



      ::file::path system::appmatter(string strApp, ::file::path pathRel)
      {

         ::file::path e = install();

         strsize iFind = strApp.find('/');

         string strRepo;

         if (iFind > 0)
         {

            strRepo = strApp.Left(iFind);

            strApp = strApp.Mid(iFind + 1);

         }
         else
         {

            strApp.replace("-", "_");

            strApp.replace("\\", "_");

            if (::str::begins_eat_ci(strApp, "app_veriwell_"))
            {

               strRepo = "app-veriwell";

            }
            else if (::str::begins_eat_ci(strApp, "app_core_"))
            {

               strRepo = "app-core";

            }
            else if (::str::begins_eat_ci(strApp, "app_cidadedecuritiba_"))
            {

               strRepo = "app-cidadedecuritiba";

            }
            else if (::str::begins_eat_ci(strApp, "app_"))
            {

               strRepo = "app";

            }
            else
            {

               return "";

            }

         }

         ::file::path p = e / strRepo / "appmatter" / strApp / "_std" / "_std" / pathRel;

         return p;

      }


      /* static const string strEn("en");
         static const string strStd("_std");
         static const string strEmpty("");

         string strPath;
         string strLs = locale_schema_matter(strLocator, App(papp).get_locale(), App(papp).get_style());
         strPath = strLs + str + str2;
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strLs = locale_schema_matter(strLocator, strEn, App(papp).get_style());
         strPath = strLs + str + str2;
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
         strPath = strLs + str + str2;
         if(System.file().exists(strPath, get_app()))
            return strPath;
         strPath = path(locale_schema_matter(strLocator, strStd, strStd), str, str2);
         if(System.file().exists(strPath, get_app()))
            return strPath;
      */

      ::file::path system::matter_from_locator(::aura::str_context * pcontext, const string & strLocator,  const ::file::path & str)
      {

         ::file::path strPath;

         ::file::patha straLs;

         string strLs;

         string strLocale  = pcontext->m_plocaleschema->m_idLocale;
         string strSchema   = pcontext->m_plocaleschema->m_idSchema;
         straLs      = locale_schema_matter(strLocator, strLocale, strSchema);
         for (index l = 0; l < straLs.get_count(); l++)
         {
            strLs = straLs[l];

            strPath = strLs / str;
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

               strPath = strLs / str;
               if (System.file().exists(strPath, get_app()))
                  return strPath;

            }

         }


         straLs             = locale_schema_matter(strLocator, "en", "en");

         for (index l = 0; l < straLs.get_count(); l++)
         {

            strLs = straLs[l];

            strPath = strLs / str;

            if (System.file().exists(strPath, get_app()))
               return strPath;

         }


         straLs = locale_schema_matter(strLocator, "se", "se");

         for (index l = 0; l < straLs.get_count(); l++)
         {

            strLs = straLs[l];

            strPath = strLs / str;

            if (System.file().exists(strPath, get_app()))
               return strPath;

         }

         return strLs + str;


      }

      void system::appmatter_locators(::file::path & strRoot,::file::path & strDomain,::aura::application * papp)
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

      void system::appmatter_locators(::file::path & strRoot,::file::path & strDomain,const string & strAppName)
      {

         appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

      }

      void system::appmatter_locators(::file::path & strRoot,::file::path & strDomain,const string & strLibraryNameParam,const string & strAppNameParam)
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
            ::file::patha stra2;
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

      ::file::path system::appmatter_locator(::aura::application * papp)
      {

         ::file::path strRoot;
         ::file::path strDomain;

         appmatter_locators(strRoot,strDomain,papp);

         if(Session.m_bMatterFromHttpCache)
         {
            return strRoot/ "appmatter"/ strDomain;

         }
         else
         {
            return install()/ strRoot/"appmatter"/strDomain;

         }

      }

      ::file::path system::appmatter_locator(const string & strLibraryName, const string & strAppName)
      {

         ::file::path strRoot;
         ::file::path strDomain;

         appmatter_locators(strRoot, strDomain, strLibraryName, strAppName);

         return install() / strRoot / "appmatter"/ strDomain;

      }

      ::file::path system::appmatter_locator(const string & strAppName)
      {

         ::file::path strRoot;
         ::file::path strDomain;

         appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

#if defined(CUBE) || defined(VSNORD) || defined(METROWIN)

         return strRoot /  "appmatter" /  strDomain;

#else

         return install() / strRoot / "appmatter"/ strDomain;

#endif

      }

      ::file::path system::base_appmatter_locator(const ::file::path & strBase,const string & strLibraryName,const string & strAppName)
      {

         ::file::path strRoot;
         ::file::path strDomain;

         appmatter_locators(strRoot, strDomain, strLibraryName, strAppName);

         return strBase / strRoot / "appmatter" / strDomain;

      }

      ::file::path system::base_appmatter_locator(const ::file::path & strBase,const string & strAppName)
      {

         ::file::path strRoot;
         ::file::path strDomain;

         appmatter_locators(strRoot, strDomain, System.m_mapAppLibrary[strAppName], strAppName);

         return strBase /strRoot / "appmatter" / strDomain;

      }


      //class ::file::file_path & system::path()
      //{
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}

      bool system::initialize()
      {

         if(!update_module_path())
         {

            return false;

         }

         return true;

      }


      ::file::path system::commonappdata(const char * pszAppId, const char * pszBuild, const char * pszPlatform, const char * pszConfiguration)
      {

         synch_lock sl(m_pmutex);

         string strAppId(pszAppId);

         string strPlatform(pszPlatform);

         if (strPlatform.is_empty())
         {

            strPlatform = System.get_system_platform();

         }

         string strConfiguration(pszConfiguration);

         if (strConfiguration.is_empty())
         {

            strConfiguration = System.get_system_configuration();

         }

         string strBuild(pszBuild);

         return commonappdata() / strBuild / strPlatform / strConfiguration / strAppId;

      }


      ::file::path system::commonappdata_locale_schema(const char * pszAppId, const char * pszBuild, const char * pszPlatform, const char * pszConfiguration, const char * pszLocale, const char * pszSchema)
      {

         synch_lock sl(m_pmutex);

         string strLocale(pszLocale);

         if (strLocale.is_empty())
         {

            strLocale = Session.m_strLocale;

         }

         string strSchema(pszSchema);

         if (strSchema.is_empty())
         {

            strSchema = Session.m_strSchema;

         }

         return commonappdata(pszAppId, pszBuild, pszPlatform, pszConfiguration) / strLocale / strSchema;

      }


      ::file::path system::trash_that_is_not_trash(const ::file::path & psz)
      {

         UNREFERENCED_PARAMETER(psz);

         _throw(interface_only_exception(get_app(), "this is an interface"));

      }


      ::file::path system::appdata()
      {

         _throw(interface_only_exception(get_app(), "this is an interface"));

      }


      ::file::path system::commonappdata_root()
      {

         return "/var/lib";

      }

      ::file::path system::commonappdata()
      {

         return element_commonappdata(install());

      }

      ::file::path system::element_commonappdata(const string & strElement)
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

         return commonappdata_root()/ strRelative;

      }


      //::file::path system::usersystemappdata(::aura::application * papp, const string & lpcszPrefix)
      //{
      //   UNREFERENCED_PARAMETER(papp);
      //   UNREFERENCED_PARAMETER(lpcszPrefix);
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}


      //::file::path system::userappdata(::aura::application * papp)
      //{

      //   UNREFERENCED_PARAMETER(papp);

      //   _throw(interface_only_exception(get_app(), "this is an interface"));

      //}


      //::file::path system::userdata(::aura::application * papp)
      //{

      //   UNREFERENCED_PARAMETER(papp);

      //   _throw(interface_only_exception(get_app(), "this is an interface"));

      //}


      //::file::path system::default_os_user_path_prefix(::aura::application * papp)
      //{

      //   UNREFERENCED_PARAMETER(papp);

      //   _throw(interface_only_exception(get_app(), "this is an interface"));

      //}


      //::file::path system::default_userappdata(::aura::application * papp, const string & lpcszPrefix, const string & lpcszLogin)
      //{
      //   UNREFERENCED_PARAMETER(papp);
      //   UNREFERENCED_PARAMETER(lpcszPrefix);
      //   UNREFERENCED_PARAMETER(lpcszLogin);
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}

      //::file::path system::default_userdata(::aura::application * papp, const string & lpcszPrefix, const string & lpcszLogin)
      //{
      //   UNREFERENCED_PARAMETER(papp);
      //   UNREFERENCED_PARAMETER(lpcszPrefix);
      //   UNREFERENCED_PARAMETER(lpcszLogin);
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}

      //::file::path system::default_userfolder(::aura::application * papp, const string & lpcszPrefix, const string & lpcszLogin)
      //{
      //   UNREFERENCED_PARAMETER(papp);
      //   UNREFERENCED_PARAMETER(lpcszPrefix);
      //   UNREFERENCED_PARAMETER(lpcszLogin);
      //   _throw(interface_only_exception(get_app(), "this is an interface"));
      //}

      ::file::path system::userquicklaunch(::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(papp);
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::userprograms(::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(papp);
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::commonprograms()
      {
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      bool system::is_inside_time(const ::file::path & pszPath,::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(pszPath);
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      bool system::is_inside(const ::file::path & pszDir,const ::file::path & pszPath,::aura::application * papp)
      {
         UNREFERENCED_PARAMETER(pszDir);
         UNREFERENCED_PARAMETER(pszPath);
         _throw(interface_only_exception(get_app(), "this is an interface"));
      }

      ::file::path system::pathfind(const string & pszEnv, const string & pszTopic, const string & pszMode, ::aura::application * papp)
      {

         ::file::patha stra;

         stra.add_tokens(pszEnv, ":", false);

         string strCandidate;

         for(int32_t i = 0; i < stra.get_count(); i++)
         {

            strCandidate = stra[i] / pszTopic;

            if(Sess(papp).file().exists(strCandidate))
            {
               return strCandidate;
            }

         }

         return "";

      }

      ::file::path system::get_api_cc()
      {

         if(m_strApiCc.has_char())
            return m_strApiCc;

         synch_lock sl(&m_mutex);



         return m_strApiCc;

      }


      bool system::update_module_path()
      {

         if(!System.file().update_module_path())
         {

            return false;

         }

         m_pathModule = System.file().module().folder();

         m_pathCa2Module = System.file().ca2module().folder();

         return true;

      }


      ::file::path system::favorites()
      {

         return ::dir::favorites();

      }


      //::file::path system::home()
      //{

      //   return ::dir::home();

      //}

      ::file::path system::desktop()
      {

         return home() / "Desktop";

      }


      ::file::path system::document()
      {

         return home() / "Document";

      }



      ::file::path system::download()
      {

         return home() / "Downloads";

      }


      ::file::path system::music()
      {

         return home() / "Music";

      }


      ::file::path system::video()
      {

         return home() / "Video";

      }


      ::file::path system::image()
      {

         return home() / "Image";

      }


   }  // namespace dir


} // namespace file




