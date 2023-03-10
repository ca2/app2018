#include "framework.h"
#include "aura/aura/os/windows_common/windows_common_cotaskptr.h"


namespace windows
{


   dir::dir(::aura::application * papp) :
      ::object(papp),
      ::file::dir::system(papp),
      ::file_watcher::file_watcher(papp)
   {


   }


   dir::~dir()
   {

   }


   inline bool myspace(char ch)
   {
      return ch == ' ' ||
             ch == '\t' ||
             ch == '\r' ||
             ch == '\n';
   }



   //string dir::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl)
   //{

   //   bool bEmptyRelative = iLenRelative == 0 || pszRelative == NULL || *pszRelative == '\0';
   //   bool bEmpty2 = iLen2 == 0 || psz2 == NULL || *psz2 == '\0';

   //   if(bEmptyRelative && bEmpty2)
   //      return pszFolder;

   //   string strPath;
   //   LPSTR lpsz;

   //   if(bEmptyRelative)
   //   {
   //      pszRelative = psz2;
   //      iLenRelative = iLen2;
   //   }

   //   while((pszFolder[iLenFolder - 1] == '\\' || pszFolder[iLenFolder - 1] == '/') && iLenFolder > 0)
   //   {
   //      if(bUrl)
   //      {
   //         if((iLenFolder - 2) >= 0 && (pszFolder[iLenFolder - 2] == '\\' || pszFolder[iLenFolder - 2] == '/' || pszFolder[iLenFolder - 2] == ':'))
   //         {
   //            if(pszFolder[iLenFolder - 2] == ':')
   //            {
   //               break;
   //            }
   //            else
   //            {
   //               if((iLenFolder - 3) >= 0 && (pszFolder[iLenFolder - 3] == ':'))
   //               {
   //                  iLenFolder--;
   //                  break;
   //               }
   //            }
   //         }
   //         else
   //         {
   //            iLenFolder--;
   //         }
   //      }
   //      else
   //      {
   //         iLenFolder--;
   //      }
   //   }

   //   while(*pszRelative != '\0' && (*pszRelative == '\\' || *pszRelative == '/') && iLenRelative > 0)
   //   {
   //      pszRelative++;
   //      iLenRelative--;
   //   }

   //   if(bEmptyRelative || bEmpty2)
   //   {
   //      lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative);
   //      strncpy(lpsz, pszFolder, iLenFolder);
   //      if(strnicmp(&lpsz[iLenFolder - 5], ".zip:", 5) == 0)
   //      {
   //         iLenFolder--;
   //      }
   //      else
   //      {
   //         lpsz[iLenFolder] = '/';
   //      }
   //      strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
   //      lpsz[iLenFolder + 1 + iLenRelative] = '\0';
   //      {
   //         if(bUrl)
   //         {
   //            while(*lpsz++ != '\0')
   //               if(*lpsz == '\\') *lpsz = '/';
   //         }
   //         else
   //         {
   //            while(*lpsz++ != '\0')
   //               if(*lpsz == '/') *lpsz = '\\';
   //         }
   //      }
   //      strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative);
   //      return strPath;
   //   }

   //   while((pszRelative[iLenRelative - 1] == '\\' || pszRelative[iLenRelative - 1] == '/') && iLenRelative > 0)
   //   {
   //      iLenRelative--;
   //   }

   //   while(*psz2 != '\0' && (*psz2 == '\\' || *psz2 == '/') && iLen2 > 0)
   //   {
   //      psz2++;
   //      iLen2--;
   //   }

   //   lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative + 1 + iLen2);
   //   strncpy(lpsz, pszFolder, iLenFolder);
   //   lpsz[iLenFolder] = '/';
   //   strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
   //   lpsz[iLenFolder + 1 + iLenRelative] = '/';
   //   strncpy(&lpsz[iLenFolder + 1 + iLenRelative + 1], psz2, iLen2);
   //   lpsz[iLenFolder + 1 + iLenRelative + 1 + iLen2] = '\0';
   //   {
   //      if(bUrl)
   //      {
   //         while(*lpsz++ != '\0')
   //            if(*lpsz == '\\') *lpsz = '/';
   //      }
   //      else
   //      {
   //         while(*lpsz++ != '\0')
   //            if(*lpsz == '/') *lpsz = '\\';
   //      }
   //   }
   //   strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative + 1 + iLen2);
   //   return strPath;
   //}

   //string dir::relpath(const string & lpcszSource,const string & lpcszRelative)
   //{
   //   const char * pszRequest;
   //   if(::url::is_url(lpcszSource,&pszRequest))
   //   {
   //      if(::str::begins(lpcszRelative,astr.strSlash))
   //      {
   //         return path((const char *)string(lpcszSource,pszRequest - lpcszSource),lpcszRelative);
   //      }
   //      else if(*pszRequest == '\0' || ::str::ends(lpcszSource,"/"))
   //      {
   //         return path(lpcszSource,lpcszRelative);
   //      }
   //      else
   //      {
   //         return path((const char *)name(lpcszSource),lpcszRelative);
   //      }
   //   }
   //   else
   //   {
   //      if(::str::ends(lpcszSource,"\\") || ::str::ends(lpcszSource,"/"))
   //      {
   //         return path(lpcszSource,lpcszRelative);
   //      }
   //      else
   //      {
   //         return path((const char *)name(lpcszSource),lpcszRelative);
   //      }
   //   }
   //}


   //string dir::relpath(const string & lpcszSource, const string & lpcszRelative, const string & psz2)
   //{
   //   const char * pszRequest;
   //   if(::url::is_url(lpcszSource, &pszRequest))
   //   {
   //      if(::str::begins(lpcszRelative,astr.strSlash))
   //      {
   //         return path((const char *) string(lpcszSource, pszRequest - lpcszSource), lpcszRelative, psz2);
   //      }
   //      else if(*pszRequest == '\0' || ::str::ends(lpcszSource, "/"))
   //      {
   //         return path(lpcszSource, lpcszRelative, psz2);
   //      }
   //      else
   //      {
   //         return path((const char *) name(lpcszSource), lpcszRelative, psz2);
   //      }
   //   }
   //   else
   //   {
   //      if(::str::ends(lpcszSource, "\\") || ::str::ends(lpcszSource, "/"))
   //      {
   //         return path(lpcszSource, lpcszRelative, psz2);
   //      }
   //      else
   //      {
   //         return path((const char *) name(lpcszSource), lpcszRelative, psz2);
   //      }
   //   }
   //}

   ::file::listing & dir::root_ones(::file::listing & listing,::aura::application * papp)
   {
      DWORD dwSize = ::GetLogicalDriveStrings(0, NULL);
      LPTSTR lpszAlloc = (LPTSTR) malloc(dwSize + 1);
      LPTSTR lpsz = lpszAlloc;
      dwSize = ::GetLogicalDriveStrings(dwSize + 1, lpsz);

      string str;
      while(*lpsz)
      {
         str.Empty();
         while(*lpsz)
         {
            str += *lpsz;
            lpsz++;
         }
         listing.add(::file::path(str));
         ::file::path & path = listing.last();
         path.m_iDir = 1;
         str.trim(":/\\");
         listing.m_straTitle.add("Drive " + str);
         lpsz++;
      }

      free(lpszAlloc);

      return listing;

   }





   ::file::listing & dir::ls(::aura::application * papp,::file::listing & listing)
   {


      if(listing.m_bRecursive)
      {


         index iStart = listing.get_size();


         {

            RESTORE(listing.m_path);

            RESTORE(listing.m_eextract);

            if(::file::dir::system::ls(papp,listing).succeeded())
            {

               listing.m_cres = cres(failure);

               return listing;

            }


            ::file::listing dira(papp);

            dira.ls_dir(listing.m_path);

            for(int32_t i = 0; i < dira.get_count(); i++)
            {

               ::file::path dir = dira[i];

               if(dir == listing.m_path)
                  continue;

               listing.m_path = dir;

               if(listing.m_eextract != extract_all)
               {

                  listing.m_eextract = extract_none;

               }

               listing.ls();

            }

         }

         file_find file_find;

         bool bWorking = file_find.FindFile(listing.m_path / "*") != FALSE;

         if(bWorking)
         {

            while(bWorking)
            {

               bWorking = file_find.FindNextFileA() != FALSE;

               if(!file_find.IsDots() && file_find.GetFilePath() != listing.m_path)
               {

                  if((listing.m_bDir && file_find.IsDirectory()) || (listing.m_bFile && !file_find.IsDirectory()))
                  {

                     if (matches_wildcard_criteria_ci(listing.m_straPattern, file_find.GetFileName()))
                     {

                        listing.add(file_find.GetFilePath());

                        listing.last().m_iSize = file_find.get_length();

                        listing.last().m_iDir = file_find.IsDirectory() != FALSE;

                     }

                  }

               }

            }

         }
         else
         {

            listing.m_cres = cres(failure);

         }

         for(index i = iStart; i < listing.get_size(); i++)
         {

            listing[i].m_iRelative = listing.m_path.get_length() + 1;

         }

      }
      else
      {

         if(::file::dir::system::ls(papp,listing).succeeded())
         {

            return listing;

         }

         file_find file_find;

         bool bWorking;

         bWorking = file_find.FindFile(listing.m_path / "*");

         if(!bWorking)
         {

            return listing;

         }

         while(bWorking)
         {

            bWorking = file_find.FindNextFileA();

            if(!file_find.IsDots())
            {

               if((listing.m_bDir && file_find.IsDirectory()) || (listing.m_bFile && !file_find.IsDirectory()))
               {

                  string strFile = file_find.GetFileName();

                  if (strFile.begins_ci("resident_"))
                  {

                     TRACE("resident_*");
                  }

                  if (matches_wildcard_criteria_ci(listing.m_straPattern, strFile))
                  {

                     listing.add(file_find.GetFilePath());

                     listing.last().m_iSize = file_find.get_length();

                     listing.last().m_iDir = file_find.IsDirectory() != FALSE;

                  }

               }

            }

         }

      }

      return listing;

   }


   ::file::listing & dir::ls_relative_name(::aura::application * papp, ::file::listing & listing)
   {


      if (listing.m_bRecursive)
      {

         // to finish;

         index iStart = listing.get_size();

         {

            RESTORE(listing.m_path);

            RESTORE(listing.m_eextract);

            if (::file::dir::system::ls(papp, listing).succeeded())
            {

               listing.m_cres = cres(failure);

               return listing;

            }


            ::file::listing dira(papp);

            dira.ls_dir(listing.m_path);

            for (int32_t i = 0; i < dira.get_count(); i++)
            {

               ::file::path dir = dira[i];

               if (dir == listing.m_path)
                  continue;

               listing.m_path = dir;

               if (listing.m_eextract != extract_all)
               {

                  listing.m_eextract = extract_none;

               }

               listing.ls();

            }

         }

         file_find file_find;

         bool bWorking = file_find.FindFile(listing.m_path / "*") != FALSE;

         if (bWorking)
         {

            while (bWorking)
            {

               bWorking = file_find.FindNextFileA() != FALSE;

               if (!file_find.IsDots() && file_find.GetFilePath() != listing.m_path)
               {

                  if ((listing.m_bDir && file_find.IsDirectory()) || (listing.m_bFile && !file_find.IsDirectory()))
                  {

                     if (matches_wildcard_criteria_ci(listing.m_straPattern, file_find.GetFileName()))
                     {

                        listing.add(file_find.GetFilePath());

                        listing.last().m_iSize = file_find.get_length();

                        listing.last().m_iDir = file_find.IsDirectory() != FALSE;

                     }

                  }

               }

            }

         }
         else
         {

            listing.m_cres = cres(failure);

         }

         for (index i = iStart; i < listing.get_size(); i++)
         {

            listing[i].m_iRelative = listing.m_path.get_length() + 1;

         }

      }
      else
      {

         if (::file::dir::system::ls(papp, listing).succeeded())
         {

            return listing;

         }

         file_find file_find;

         bool bWorking;

         bWorking = file_find.FindFile(listing.m_path / "*");

         if (!bWorking)
         {

            return listing;

         }

         while (bWorking)
         {

            bWorking = file_find.FindNextFileA();

            if (!file_find.IsDots())
            {

               if ((listing.m_bDir && file_find.IsDirectory()) || (listing.m_bFile && !file_find.IsDirectory()))
               {

                  ::file::path pathName = file_find.GetFileName();

                  //if (strFile.begins_ci("resident_"))
                  //{

                  //   TRACE("resident_*");
                  //}

                  if (matches_wildcard_criteria_ci(listing.m_straPattern, pathName))
                  {

                     listing.add(pathName);

                     //listing.last().m_iSize = file_find.get_length();

                     //listing.last().m_iDir = file_find.IsDirectory() != FALSE;

                  }

               }

            }

         }

      }

      return listing;

   }


   bool dir::is_impl(const ::file::path & lpcszPath, ::aura::application * papp, bool bOptional, bool bNoCache)
   {

      if (::file::dir::system::is_impl(lpcszPath, papp, bOptional, bNoCache))
      {

         return true;

      }

      string strPath(lpcszPath);

      if(strPath.get_length() >= MAX_PATH)
      {

         if(::str::begins(strPath,astr.strDoubleBackSlash))
         {

            strPath = "\\\\?\\UNC" + strPath.Mid(1);

         }
         else
         {

            strPath = "\\\\?\\" + strPath;

         }

      }

      DWORD dwAttrib;

      dwAttrib = GetFileAttributesW(::str::international::utf8_to_unicode(strPath));

      bool bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

      return bIsDir;

   }


   bool dir::name_is(const ::file::path & str, ::aura::application * papp)
   {

      strsize iLast = str.get_length() - 1;

      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            break;
         iLast--;
      }
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
            break;
         iLast--;
      }
      if(iLast >= 0)
      {
         while(iLast >= 0)
         {
            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            {
               iLast++;
               break;
            }
            iLast--;
         }
      }
      else
      {
         return true; // assume empty string is root_ones directory
      }

      bool bIsDir;

      //uint32_t uiLastError;

      //if(m_isdirmap.lookup(str, bIsDir, uiLastError, (int32_t) iLast))
      //{

      //   if(!bIsDir)
      //   {

      //      ::set_last_error(uiLastError);

      //   }

      //   return bIsDir;

      //}

      if(::thread_zip_is_dir() && iLast >= 3 && !strnicmp_dup(&((const char *)str)[iLast - 3],".zip",4))
      {

         //m_isdirmap.set(str.Left(iLast + 1), true, 0);

         return true;

      }

      wstring wstrPath;

      wstrPath = ::str::international::utf8_to_unicode(str, iLast + 1);

      if(wstrPath.get_length() >= MAX_PATH)
      {

         if(::str::begins(wstrPath, L"\\\\"))
         {

            ::str::begin(wstrPath, L"\\\\?\\UNC");

         }
         else
         {

            ::str::begin(wstrPath, L"\\\\?\\");

         }

      }

      DWORD dwAttrib;

      dwAttrib = GetFileAttributesW(wstrPath);

      bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

//      m_isdirmap.set(str.Left(iLast + 1), bIsDir, bIsDir ? 0 : ::get_last_error());

      return bIsDir;

   }


   ::file::path dir::time()
   {
      return m_strTimeFolder;
   }

   ::file::path dir::stage()
   {
      return install() / "stage";
   }

   ::file::path dir::stageapp()
   {
      return install() / "basis";
   }

   ::file::path dir::netseed()
   {
      return m_strNetSeedFolder;
   }

   //::file::path dir::install()
   //{

   //   single_lock sl(&m_mutex, true);

   //   return m_pathElement;

   //}


   ::file::path dir::module()
   {

      return m_pathModule;

   }


   ::file::path dir::ca2module()
   {

      return m_pathCa2Module;

   }


   ::file::path dir::time_square(::aura::application * papp,const string & strPrefix,const string & strSuffix)
   {

      UNREFERENCED_PARAMETER(papp);
      UNREFERENCED_PARAMETER(strPrefix);
      UNREFERENCED_PARAMETER(strSuffix);
      return time() / "time";

   }


   ::file::path dir::time_log()
   {

      return appdata() / "log";

   }

   bool dir::mk(const ::file::path & path,::aura::application * papp)
   {

      if (is(path, papp))
      {

         return true;

      }

      ::file::patha stra;

      path.ascendants_path(stra);

      index i = stra.get_upper_bound();

      for (; i >= 0; i--)
      {

         string strDir = stra[i];

         if (is(strDir, papp))
         {

            break;

         }

      }

      if (i < 0)
      {

         return true;

      }

      for (; i < stra.get_count(); i++)
      {

         string strDir = stra[i];

         if(::dir::mkdir(strDir))
         {

//            m_isdirmap.set(strDir, true, 0);

         }
         else
         {

            DWORD dwError = ::get_last_error();

            if (dwError == ERROR_ALREADY_EXISTS)
            {

               string str;

               str = "\\\\?\\" + strDir;

               str.trim_right("\\/");

               try
               {

                  Application.file().del(str);

               }
               catch (...)
               {

               }

               str = stra[i];

               str.trim_right("\\/");

               try
               {

                  Application.file().del(str);

               }
               catch (...)
               {

               }

               if (::dir::mkdir(strDir))
               {

//                  m_isdirmap.set(strDir, true, 0);

                  continue;

               }

               //   m_isdirmap.set(strDir, false, 0);

               dwError = ::get_last_error();

               char * pszError;

               FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPTSTR)&pszError, 8, NULL);

               //TRACE("dir::mk CreateDirectoryW last error(%d)=%s", dwError, pszError);

               ::LocalFree(pszError);

               //m_isdirmap.set(stra[i], false);

               return false;

            }

         }

      }

      return true;

   }


   bool dir::rm(::aura::application * papp, const ::file::path & psz, bool bRecursive)
   {
      if(bRecursive)
      {
         ::file::listing patha(papp);
         patha.ls(psz);
         for(auto & path : patha)
         {
            if(is(path, papp))
            {
               rm(papp, psz / path.name(), true);
            }
            else
            {
               ::DeleteFile(path);
            }
         }
      }
      return RemoveDirectory(psz) != FALSE;
   }


   //::file::path dir::name(const ::file::path & path1)
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

   ::file::path dir::name(const ::file::path & str)
   {

      strsize iLast = str.get_length() - 1;

      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
            break;
         iLast--;
      }
      while(iLast >= 0)
      {
         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
            break;
         iLast--;
      }
      if(iLast >= 0)
      {
         while(iLast >= 0)
         {
            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
               break;
            iLast--;
         }
         return str.Left(iLast + 1);
      }
      else
      {
         return "";
      }
   }


   CLASS_DECL_AURA ::file::path get_known_folder(REFKNOWNFOLDERID kfid)
   {

      ::file::path str;

      ::cotaskptr < PWSTR > pwszPath;

      HANDLE hToken = NULL;

      ::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_IMPERSONATE | TOKEN_DUPLICATE,&hToken);

      HRESULT hr = SHGetKnownFolderPath(kfid, 0, hToken, &pwszPath);

      return utf8(pwszPath);

   }


   bool dir::initialize()
   {

      update_module_path();

      m_pathInstall = ::dir::install();

      shell_get_special_folder_path(
      NULL,
      m_strCommonAppData,
      CSIDL_COMMON_APPDATA,
      FALSE);
      //shell_get_special_folder_path(
      // NULL,
      //m_pathProfile,
      //CSIDL_PROFILE,
      //FALSE);

      m_pathHome = get_known_folder(FOLDERID_Profile);

      m_pathCa2Config = ::dir::ca2config();

      m_strCommonAppData /= "ca2";

      m_strAppData = get_known_folder(FOLDERID_RoamingAppData);

      shell_get_special_folder_path(
      NULL,
      m_strPrograms,
      CSIDL_PROGRAMS,
      FALSE);
      shell_get_special_folder_path(
      NULL,
      m_strCommonPrograms,
      CSIDL_COMMON_PROGRAMS,
      FALSE);

      {

         //string strRelative;
         //strRelative = install();
         //index iFind = strRelative.find(':');
         //if (iFind >= 0)
         //{
         //   strsize iFind1 = strRelative.reverse_find("\\", iFind);
         //   strsize iFind2 = strRelative.reverse_find("/", iFind);
         //   strsize iStart = MAX(iFind1 + 1, iFind2 + 1);
         //   strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
         //}

         string strRelative = ::dir::app_relative();

         m_strCa2AppData = ::dir::ca2config() / "appdata" / strRelative;

         m_strCa2AppData /= System.get_system_platform();

         m_strCa2AppData /= System.get_system_configuration();

      }



      xml::document doc(get_app());

      doc.load(System.file().as_string(appdata() / "configuration/directory.xml",get_app()));

      xxdebug_box("win_dir::initialize (configuration)","win_dir::initialize",0);
      if(doc.get_root()->get_name() == "directory_configuration")
      {

         m_strTimeFolder = doc.get_root()->get_child_value("time");

         m_strNetSeedFolder = doc.get_root()->get_child_value("netseed");

      }
      if(m_strTimeFolder.is_empty())
         m_strTimeFolder = appdata() / "time";

      if(m_strNetSeedFolder.is_empty())
         m_strNetSeedFolder = install() / "net";

      mk(m_strTimeFolder,get_app());
      xxdebug_box("win_dir::initialize (m_strTimeFolder)","win_dir::initialize",0);

      if(!is(m_strTimeFolder,get_app()))
         return false;

      mk(m_strTimeFolder / "time",get_app());
      xxdebug_box("win_dir::initialize","win_dir::initialize",0);


      //string str;
      //str = System.dir().profile();


      //::file::path strRelative;
      //strRelative = System.dir().install();
      //index iFind = strRelative.find(':');
      //if(iFind >= 0)
      //{
      //   strsize iFind1 = strRelative.reverse_find("\\",iFind);
      //   strsize iFind2 = strRelative.reverse_find("/",iFind);
      //   strsize iStart = MAX(iFind1 + 1,iFind2 + 1);
      //   strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart,iFind - iStart) + strRelative.Mid(iFind + 1);
      //}

//      ::file::path strUserFolderShift;

//      if(App(papp).handler()->m_varTopicQuery.has_property("user_folder_relative_path"))
//      {
//         strUserFolderShift = strRelative / App(papp).handler()->m_varTopicQuery["user_folder_relative_path"].get_string();
//      }
//      else
//      {
      //       strUserFolderShift = strRelative;
//      }

      //m_pathUser = ::file::path(str) /"app"/ strRelative;



      return true;

   }

   ::file::path dir::trash_that_is_not_trash(const ::file::path & psz)
   {
      if(psz.is_empty())
         return "";

      if(psz[1] == ':')
      {
         string strDir = name(psz);
         string str;
         str = strDir.Left(2);
         str += "\\trash_that_is_not_trash\\";
         string strFormat;
         ::datetime::time time;
         time = ::datetime::time::get_current_time();
         strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
         str += strFormat;
         if(strDir.m_pszData[2] == '\\')
         {
            str += strDir.Mid(3);
         }
         else
         {
            str += strDir.Mid(2);
         }
         return str;
      }

      return "";
   }

   ::file::path dir::appdata()
   {

      return m_strCa2AppData;

   }


   ::file::path dir::commonappdata_root()
   {

      return m_strCommonAppData;

   }


   //::file::path dir::usersystemappdata(::aura::application * papp,const char * lpcszPrefix)
   //{
   //   UNREFERENCED_PARAMETER(papp);
   //   return appdata() / lpcszPrefix;
   //}

   //::file::path dir::appdata(::aura::application * papp)
   //{
   //   return userfolder(papp) / "appdata";
   //}

   //::file::path dir::userdata(::aura::application * papp)
   //{
   //   return userfolder(papp) / "data";
   //}


   //::file::path dir::default_os_user_path_prefix(::aura::application * papp)
   //{
   //   UNREFERENCED_PARAMETER(papp);
   //   unichar buf[MAX_PATH];
   //   ULONG ulSize = sizeof(buf) / sizeof(unichar);
   //   if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
   //   {
   //      if(!::GetUserNameW(buf, &ulSize))
   //      {
   //         memset(buf, 0, sizeof(buf));
   //      }
   //   }
   //   return ::str::international::unicode_to_utf8(buf);
   //}

   //::file::path dir::default_userappdata(::aura::application * papp,const string & lpcszPrefix,const string & lpcszLogin)
   //{
   //   return default_userfolder(papp, lpcszPrefix, lpcszLogin) /  "appdata" ;
   //}

   //::file::path dir::default_userdata(::aura::application * papp,const string & lpcszPrefix,const string & lpcszLogin)
   //{
   //   return default_userfolder(papp, lpcszPrefix, lpcszLogin) / "data";
   //}

   //::file::path dir::default_userfolder(::aura::application * papp,const string & lpcszPrefix,const string & lpcszLogin)
   //{

   //   return userfolder(papp);

   //}

   ::file::path dir::userquicklaunch(::aura::application * papp)
   {

      UNREFERENCED_PARAMETER(papp);

      return m_strAppData / "Microsoft\\Internet Explorer\\Quick Launch";

   }


   ::file::path dir::userprograms(::aura::application * papp)
   {

      UNREFERENCED_PARAMETER(papp);

      return m_strPrograms;

   }


   ::file::path dir::commonprograms()
   {

      return m_strCommonPrograms;

   }


   bool dir::is_inside_time(const ::file::path & pszPath, ::aura::application * papp)
   {

      return is_inside(time(), pszPath, papp);

   }


   bool dir::is_inside(const ::file::path & pszDir, const ::file::path & pszPath, ::aura::application * papp)
   {

      return ::str::begins_ci(pszDir, pszPath);

   }


   bool dir::has_subdir(::aura::application * papp, const ::file::path & pszDir)
   {

      file_find file_find;

      bool bWorking;

      bWorking = file_find.FindFile(pszDir / "*.*");

      while(bWorking)
      {

         bWorking = file_find.FindNextFileA();

         if(file_find.IsDirectory() && !file_find.IsDots())
         {

            return true;

         }

      }

      return false;

   }

   //bool file::GetStatus(const char * lpszFileName,::file::file_status& rStatus)
   //{
   //   // attempt to fully qualify path first
   //   wstring wstrFullName;
   //   wstring wstrFileName;
   //   wstrFileName = ::str::international::utf8_to_unicode(lpszFileName);
   //   if(!vfxFullPath(wstrFullName,wstrFileName))
   //   {
   //      rStatus.m_strFullName.Empty();
   //      return FALSE;
   //   }
   //   ::str::international::unicode_to_utf8(rStatus.m_strFullName,wstrFullName);

   //   WIN32_FIND_DATA findFileData;
   //   HANDLE hFind = FindFirstFile((LPTSTR)lpszFileName,&findFileData);
   //   if(hFind == INVALID_HANDLE_VALUE)
   //      return FALSE;
   //   VERIFY(FindClose(hFind));

   //   // strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
   //   rStatus.m_attribute = (BYTE)(findFileData.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL);

   //   // get just the low DWORD of the file size
   //   ASSERT(findFileData.nFileSizeHigh == 0);
   //   rStatus.m_size = (LONG)findFileData.nFileSizeLow;

   //   // convert times as appropriate
   //   rStatus.m_ctime = ::datetime::time(findFileData.ftCreationTime);
   //   rStatus.m_atime = ::datetime::time(findFileData.ftLastAccessTime);
   //   rStatus.m_mtime = ::datetime::time(findFileData.ftLastWriteTime);

   //   if(rStatus.m_ctime.get_time() == 0)
   //      rStatus.m_ctime = rStatus.m_mtime;

   //   if(rStatus.m_atime.get_time() == 0)
   //      rStatus.m_atime = rStatus.m_mtime;

   //   return TRUE;
   //}

   ::file::path dir::document()
   {

      ::file::path path;

      shell_get_special_folder_path(
      NULL,
      path,
      CSIDL_MYDOCUMENTS,
      FALSE);

      return path;

   }

   ::file::path dir::desktop()
   {

      ::file::path path;

      shell_get_special_folder_path(
      NULL,
      path,
      CSIDL_DESKTOP,
      FALSE);

      return path;

   }

   ::file::path dir::download()
   {

      ::file::path path;

      path = ::windows::get_known_folder(FOLDERID_Downloads);

      return path;

   }

   ::file::path dir::music()
   {

      ::file::path path;

      shell_get_special_folder_path(
      NULL,
      path,
      CSIDL_MYMUSIC,
      FALSE);

      return path;

   }


   ::file::path dir::video()
   {

      ::file::path path;

      shell_get_special_folder_path(
      NULL,
      path,
      CSIDL_MYVIDEO,
      FALSE);

      return path;

   }


   ::file::path dir::image()
   {

      ::file::path path;

      shell_get_special_folder_path(
      NULL,
      path,
      CSIDL_MYPICTURES,
      FALSE);

      return path;

   }




} // namespace windows


::file::path dir::favorites()
{

   return ::dir::local() / "localconfig/favorites";

}


::file::path dir::home()
{

   return ::windows::get_known_folder(FOLDERID_Profile);

}





