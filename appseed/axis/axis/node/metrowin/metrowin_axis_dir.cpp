#include "framework.h"
#include "metrowin.h"


namespace metrowin
{


   namespace axis
   {


      dir::dir(::aura::application *  papp) :
         ::object(papp),
         ::file::dir::system(papp),
         file_watcher::file_watcher(papp),
         ::metrowin::dir(papp),
         ::file::dir::axis::system(papp)
      {

         string strCa2Module = ca2module();

         m_strCa2 = strCa2Module;

         //System.file().path().eat_end_level(m_strCa2, 2, "\\");

      }


      ::file::listing & dir::ls(::aura::application * papp, ::file::listing & listing)
      {

         if(::file::dir::axis::system::ls(papp,listing).succeeded())
         {

            return listing;

         }

         return ::metrowin::dir::ls(papp,listing);

      }


      //bool dir::rls(::aura::application * papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, e_extract eextract)
      //{

      //   if(::file::dir::axis::system::rls(papp,lpcsz,pstraPath,pstraTitle,pstraRelative,eextract))
      //   {

      //      return true;

      //   }

      //   return ::metrowin::dir::rls(papp,lpcsz,pstraPath,pstraTitle,pstraRelative,eextract);

      //}


      //bool dir::rls_pattern(::aura::application * papp,const char * lpcsz,const char * lpszPattern,stringa * pstraPath,stringa * pstraTitle,stringa * pstraRelative,bool_array * pbaIsDir,int64_array * piaSize,e_extract eextract)
      //{

      //   if(::file::dir::axis::system::rls_pattern(papp,lpcsz,lpszPattern,pstraPath,pstraTitle,pstraRelative,pbaIsDir,piaSize,eextract))
      //   {

      //      return true;

      //   }

      //   return ::metrowin::dir::rls_pattern(papp,lpcsz,lpszPattern,pstraPath,pstraTitle,pstraRelative,pbaIsDir,piaSize,eextract);

      //}


      //bool dir::rls_dir(::aura::application * papp,const char * lpcsz,stringa * pstraPath,stringa * pstraTitle,stringa * pstraRelative)
      //{

      //   if(::file::dir::axis::system::rls_dir(papp,lpcsz,pstraPath,pstraTitle,pstraRelative))
      //   {

      //      return true;

      //   }

      //   return ::metrowin::dir::rls_dir(papp,lpcsz,pstraPath,pstraTitle,pstraRelative);

      //}


      //bool dir::ls_dir(::aura::application * papp,const char * lpcsz,stringa * pstraPath,stringa * pstraTitle)
      //{

      //   if(::file::dir::axis::system::ls_dir(papp,lpcsz,pstraPath,pstraTitle))
      //   {

      //      return true;

      //   }

      //   return ::metrowin::dir::ls_dir(papp,lpcsz,pstraPath,pstraTitle);

      //}


      //bool dir::ls_file(::aura::application * papp,const char * lpcsz,stringa * pstraPath,stringa * pstraTitle)
      //{

      //   if(::file::dir::axis::system::ls_file(papp,lpcsz,pstraPath,pstraTitle))
      //   {

      //      return true;

      //   }

      //   return ::metrowin::dir::ls_file(papp,lpcsz,pstraPath,pstraTitle);

      //}


      //bool dir::ls(::aura::application * papp,const char * lpcsz,stringa * pstraPath,stringa * pstraTitle,bool_array * pbaIsDir,int64_array * piaSize)
      //{

      //   if(::file::dir::axis::system::ls(papp,lpcsz,pstraPath,pstraTitle,pbaIsDir,piaSize))
      //   {

      //      return true;

      //   }

      //   return ::metrowin::dir::ls(papp,lpcsz,pstraPath,pstraTitle,pbaIsDir,piaSize);

      //}


      bool dir::is_impl(const ::file::path & path,::aura::application * papp)
      {

         if (::file::dir::axis::system::is_impl(path, papp))
         {

            return true;

         }

         if (::metrowin::dir::is_impl(path, papp))
         {

            return true;

         }

         return false;

      }

      //bool dir::is(const string & strPath,::aura::application * papp)
      //{

      //   if(::metrowin::dir::is(strPath,papp))
      //      return true;

      //   if(::file::dir::axis::system::is(strPath,papp))
      //      return true;

      //   return false;

      //}

      bool dir::name_is(const ::file::path & str, ::aura::application *  papp)
      {

         if(::metrowin::dir::name_is(str,papp))
            return true;

         if(::file::dir::axis::system::name_is(str,papp))
            return true;

         return false;

      }

      //string dir::votagus(const char * lpcsz, const char * lpcsz2)
      //{
      //   string strVotagusFolder = System.get_ca2_module_folder();
      //   System.file().path().eat_end_level(strVotagusFolder, 2, "\\");
      //   return path(strVotagusFolder, lpcsz, lpcsz2);
      //}

      //string dir::time(const char * lpcsz, const char * lpcsz2)
      //{
      //   return path(m_strTimeFolder, lpcsz, lpcsz2);
      //}

      //string dir::stage(const char * lpcsz, const char * lpcsz2)
      //{
      //   return path(element("stage"), lpcsz, lpcsz2);
      //}

      //string dir::stageapp(const char * lpcsz, const char * lpcsz2)
      //{
      //   return path(stage("basis", lpcsz), lpcsz2);
      //}

      //string dir::netseed(const char * lpcsz, const char * lpcsz2)
      //{
      //   return path(m_strNetSeedFolder, lpcsz, lpcsz2);
      //}

      //// stage in ccvotagus spalib
      //string dir::install(const char * lpcsz, const char * lpcsz2)
      //{
      //
      //   single_lock sl(&m_mutex, true);

      //   return path(m_strCa2, lpcsz, lpcsz2);

      //}

      //string dir::install(const string & str, const char * lpcsz2)
      //{
      //
      //   single_lock sl(&m_mutex, true);

      //   return path(m_strCa2, str, lpcsz2);

      //}

      //string dir::install(const char * lpcsz, const string & str2)
      //{
      //
      //   single_lock sl(&m_mutex, true);

      //   return path(m_strCa2, lpcsz, str2);

      //}

      //string dir::install(const string & str, const string & str2)
      //{
      //
      //   single_lock sl(&m_mutex, true);

      //   return path(m_strCa2, str, str2);

      //}

      //string dir::install(const string & str)
      //{
      //
      //   single_lock sl(&m_mutex, true);

      //   return path(m_strCa2, str);

      //}

      //string dir::install()
      //{
      //
      //   single_lock sl(&m_mutex, true);

      //   return m_strCa2;

      //}


      //string dir::module(const char * lpcsz, const char * lpcsz2)
      //{
      //   string str = System.get_module_folder();
      //   return path(str, lpcsz, lpcsz2);
      //}

      //string dir::ca2module(const char * lpcsz, const char * lpcsz2)
      //{
      //   string str = System.get_ca2_module_folder();
      //   return path(str, lpcsz, lpcsz2);
      //}


      //void dir::time_square(string &str)
      //{
      //   str = time("time");
      //}

      //string dir::time_log(const char * pszId)
      //{
      //   string strLogBaseDir;
      //   strLogBaseDir = appdata("log");
      //   return path(strLogBaseDir, pszId);
      //}

      //bool dir::mk(const char * lpcsz, ::aura::application *  papp)
      //{

      //   if(is(lpcsz, papp))
      //      return true;

      //   stringa stra;
      //   System.file().get_ascendants_path(lpcsz, stra);
      //   for(int i = 0; i < stra.get_size(); i++)
      //   {
      //      if(!is(stra[i], papp) && ::get_last_error() != ERROR_ACCESS_DENIED)
      //      {
      //
      //         if(!::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
      //         {
      //            uint32_t dwError = ::get_last_error();
      //            if(dwError == ERROR_ALREADY_EXISTS)
      //            {
      //               string str;
      //               str = "\\\\?\\" + stra[i];
      //               str.trim_right("\\/");
      //               try
      //               {
      //                  System.file().del(str);
      //               }
      //               catch(...)
      //               {
      //               }
      //               str = stra[i];
      //               str.trim_right("\\/");
      //               try
      //               {
      //                  System.file().del(str);
      //               }
      //               catch(...)
      //               {
      //               }
      //               if(::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
      //               {
      //                  m_isdirmap.set(stra[i], true, 0);
      //                  goto try1;
      //               }
      //               else
      //               {
      //                  dwError = ::get_last_error();
      //               }
      //            }
      //            string strError = get_system_error_message(dwError);
      //            APPTRACE(papp)("dir::mk CreateDirectoryW last error(%d)=%s", dwError, strError);
      //            //m_isdirmap.set(stra[i], false);
      //         }
      //         else
      //         {
      //            m_isdirmap.set(stra[i], true, 0);
      //         }
      //         try1:
      //
      //         if(!is(stra[i], papp))
      //         {
      //            return false;
      //         }
      //
      //      }
      //   }
      //   return true;
      //}

      //bool dir::rm(::aura::application *  papp, const char * psz, bool bRecursive)
      //{
      //   if(bRecursive)
      //   {
      //      stringa straPath;
      //      stringa straTitle;
      //      ls(papp, psz, &straPath, &straTitle);
      //      for(int i = 0; i < straPath.get_count(); i++)
      //      {
      //         if(is(straPath[i], papp))
      //         {
      //            rm(papp, path(psz, straTitle[i]), true);
      //         }
      //         else
      //         {
      //            ::DeleteFileW(::str::international::utf8_to_unicode(straPath[i]));
      //         }
      //      }
      //   }
      //   return RemoveDirectoryW(::str::international::utf8_to_unicode(psz)) != FALSE;
      //}


      //string dir::name(const char * path1)
      //{
      //   const char * psz = path1 + strlen(path1) - 1;
      //   while(psz >= path1)
      //   {
      //      if(*psz != '\\' && *psz != '/' && *psz != ':')
      //         break;
      //      psz--;
      //   }
      //   while(psz >= path1)
      //   {
      //      if(*psz == '\\' || *psz == '/' || *psz == ':')
      //         break;
      //      psz--;
      //   }
      //   if(psz >= path1) // strChar == "\\" || strChar == "/"
      //   {
      //      const char * pszEnd = psz;
      //      /*while(psz >= path1)
      //      {
      //         if(*psz != '\\' && *psz != '/' && *psz != ':')
      //            break;
      //         psz--;
      //      }*/
      //      return string(path1, pszEnd - path1 + 1);
      //   }
      //   else
      //   {
      //      return "";
      //   }
      //}

      //string dir::name(const string & str)
      //{
      //
      //   strsize iLast = str.get_length() - 1;

      //   while(iLast >= 0)
      //   {
      //      if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
      //         break;
      //      iLast--;
      //   }
      //   while(iLast >= 0)
      //   {
      //      if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
      //         break;
      //      iLast--;
      //   }
      //   if(iLast >= 0)
      //   {
      //      while(iLast >= 0)
      //      {
      //         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
      //            break;
      //         iLast--;
      //      }
      //      return str.Left(iLast + 1);
      //   }
      //   else
      //   {
      //      return "";
      //   }
      //}


      //class ::file::path & dir::path()
      //{
      //   return m_path;
      //}


      bool dir::initialize()
      {


         if(!::metrowin::dir::initialize())
            return false;

         xml::document doc(get_app());

         doc.load(Application.file().as_string(appdata() / "configuration\\directory.xml"));

         if(doc.get_root()->get_name() == "directory_configuration")
         {

            m_strTimeFolder = doc.get_root()->get_child_value("time");

            m_strNetSeedFolder = doc.get_root()->get_child_value("netseed");

         }
         if(m_strTimeFolder.is_empty())
            m_strTimeFolder = appdata() / "time";

         if(m_strNetSeedFolder.is_empty())
            m_strNetSeedFolder = install() / "net";

         mk(m_strTimeFolder, get_app());

         if(!is(m_strTimeFolder,get_app()))
         {

            TRACE("Failed to create time folder");

            return false;

         }

         mk(m_strTimeFolder/  "time",get_app());

         return true;

      }

      //string dir::trash_that_is_not_trash(const char * psz)
      //{
      //   if(psz == NULL)
      //      return "";

      //   if(psz[1] == ':')
      //   {
      //      string strDir = name(psz);
      //      string str;
      //      str = strDir.Left(2);
      //      str += "\\trash_that_is_not_trash\\";
      //      string strFormat;
      //      ::datetime::time time;
      //      time = ::datetime::time::get_current_time();
      //      strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
      //      str += strFormat;
      //      if(strDir.m_pszData[2] == '\\')
      //      {
      //         str += strDir.Mid(3);
      //      }
      //      else
      //      {
      //         str += strDir.Mid(2);
      //      }
      //      return str;
      //   }

      //   return "";
      //}

      //string dir::appdata(const char * lpcsz, const char * lpcsz2)
      //{

      //   return path(::Windows::Storage::ApplicationData::Current->LocalFolder->Path, lpcsz, lpcsz2);

      //}

      //   string dir::usersystemappdata(::aura::application *  papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
      //   {
      //      UNREFERENCED_PARAMETER(papp);
      //      return path(appdata(lpcszPrefix), lpcsz, lpcsz2);
      //   }
      //
      //   string dir::appdata(::aura::application *  papp, const char * lpcsz, const char * lpcsz2)
      //   {
      //      return path(userfolder(papp, "appdata"), lpcsz, lpcsz2);
      //   }
      //
      //   string dir::userdata(::aura::application *  papp, const char * lpcsz, const char * lpcsz2)
      //   {
      //      return path(userfolder(papp, "data"), lpcsz, lpcsz2);
      //   }
      //
      //   string dir::userfolder(::aura::application *  papp, const char * lpcsz, const char * lpcsz2)
      //   {
      //#ifdef WINDOWSEX
      //      string str;
      //      SHGetSpecialFolderPath(
      //         NULL,
      //         str,
      //         CSIDL_PROFILE,
      //         FALSE);
      //
      //
      //      string strRelative;
      //      strRelative = ca2();
      //      index iFind = strRelative.find(':');
      //      if(iFind >= 0)
      //      {
      //         strsize iFind1 = strRelative.reverse_find("\\", iFind);
      //         strsize iFind2 = strRelative.reverse_find("/", iFind);
      //         strsize iStart = MAX(iFind1 + 1, iFind2 + 1);
      //         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
      //      }
      //
      //      string strUserFolderShift;
      //
      //      if(App(papp).handler().m_varTopicQuery.has_property("user_folder_relative_path"))
      //      {
      //         strUserFolderShift = path(strRelative, App(papp).handler().m_varTopicQuery["user_folder_relative_path"].get_string());
      //      }
      //      else
      //      {
      //         strUserFolderShift = strRelative;
      //      }
      //
      //      return path(path(str, "ca2", strUserFolderShift), lpcsz, lpcsz2);
      //
      ////      return path(path(str, "ca2"), lpcsz);
      ///*      if(&AppUser(papp) == NULL)
      //      {
      //         string str;
      //         SHGetSpecialFolderPath(
      //            NULL,
      //            str,
      //            CSIDL_PROFILE,
      //            FALSE);
      //         return path(path(str, "ca2\\_____default"), lpcsz);
      //      }
      //      else
      //      {
      //         return path(AppUser(papp).m_strPath, lpcsz, lpcsz2);
      //      }*/
      //#else
      //
      //      string str = appdata();
      //
      ///*      string strRelative;
      //      strRelative = ca2();
      //      index iFind = strRelative.find(':');
      //      if(iFind >= 0)
      //      {
      //         strsize iFind1 = strRelative.reverse_find("\\", iFind);
      //         strsize iFind2 = strRelative.reverse_find("/", iFind);
      //         strsize iStart = MAX(iFind1 + 1, iFind2 + 1);
      //         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
      //      }*/
      //
      //      string strUserFolderShift;
      //
      //      if(App(papp).handler()->m_varTopicQuery.has_property("user_folder_relative_path"))
      //      {
      //         strUserFolderShift = App(papp).handler()->m_varTopicQuery["user_folder_relative_path"].get_string();
      //      }
      //
      //      return path(path(str, "ca2", strUserFolderShift), lpcsz, lpcsz2);
      //
      //
      //#endif
      //
      //   }

      //   string dir::default_os_user_path_prefix(::aura::application *  papp)
      //   {
      //      UNREFERENCED_PARAMETER(papp);
      ////      unichar buf[MAX_PATH];
      //#ifdef WINDOWSEX
      //      ULONG ulSize = sizeof(buf) / sizeof(unichar);
      //      if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
      //      {
      //         if(!::GetUserNameW(buf, &ulSize))
      //         {
      //            memset(buf, 0, sizeof(buf));
      //         }
      //      }
      //#else
      //      return "CurrentUser";
      //#endif
      //      //return ::str::international::unicode_to_utf8(buf);
      //   }
      //
      //   string dir::default_userappdata(::aura::application *  papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      //   {
      //      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "appdata"), pszRelativePath);
      //   }

      //   string dir::default_userdata(::aura::application *  papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      //   {
      //      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "data"), pszRelativePath);
      //   }
      //
      //   string dir::default_userfolder(::aura::application *  papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      //   {
      //
      //      return userfolder(papp, pszRelativePath);
      //
      ///*      UNREFERENCED_PARAMETER(papp);
      //      string str;
      //      SHGetSpecialFolderPath(
      //         NULL,
      //         str,
      //         CSIDL_APPDATA,
      //         FALSE);
      //      return path(path(str, "ca2\\user", lpcszPrefix), lpcszLogin, pszRelativePath);*/
      //   }
      //
      //   string dir::userquicklaunch(::aura::application *  papp, const char * lpcszRelativePath, const char * lpcsz2)
      //   {
      //_throw(todo(get_app()));
      //UNREFERENCED_PARAMETER(papp);
      //string str;
      //SHGetSpecialFolderPath(
      //   NULL,
      //   str,
      //   CSIDL_APPDATA,
      //   FALSE);
      //str = path(str, "Microsoft\\Internet Explorer\\Quick Launch");
      //return path(str, lpcszRelativePath, lpcsz2);
      //   }
      //
      //   string dir::userprograms(::aura::application *  papp, const char * lpcszRelativePath, const char * lpcsz2)
      //   {
      //_throw(todo(get_app()));
      //      //UNREFERENCED_PARAMETER(papp);
      //      //string str;
      //      //SHGetSpecialFolderPath(
      //      //   NULL,
      //      //   str,
      //      //   CSIDL_PROGRAMS,
      //      //   FALSE);
      //      //return path(str, lpcszRelativePath, lpcsz2);
      //   }
      //
      //   string dir::commonprograms(const char * lpcszRelativePath, const char * lpcsz2)
      //   {
      //_throw(todo(get_app()));
      //      //string str;
      //      //SHGetSpecialFolderPath(
      //      //   NULL,
      //      //   str,
      //      //   CSIDL_COMMON_PROGRAMS,
      //      //   FALSE);
      //      //return path(str, lpcszRelativePath, lpcsz2);
      //   }

      //bool dir::is_inside_time(const char * pszPath, ::aura::application *  papp)
      //{
      //   return is_inside(time(), pszPath, papp);
      //}

      //bool dir::is_inside(const char * pszDir, const char * pszPath, ::aura::application *  papp)
      //{
      //   return ::str::begins_ci(pszDir, pszPath);
      //}

      //   bool dir::has_subdir(::aura::application *  papp, const char * pszDir)
      //   {
      //      _throw(todo(get_app()));
      //
      ///*      file_find filefind;
      //      bool bWorking;
      //      bWorking = filefind.FindFile(path(pszDir, "*.*"));
      //      while(bWorking)
      //      {
      //         bWorking = filefind.FindNextFileA();
      //         if(filefind.IsDirectory() && !filefind.IsDots())
      //         {
      //            return true;
      //         }
      //      }
      //      return false;*/
      //   }


      /*string dir::commonappdata()
      {

         return path(::Windows::Storage::ApplicationData::Current->LocalFolder->Path,"commonappdata");

      }*/


   } // namespace axis


} // namespace metrowin
