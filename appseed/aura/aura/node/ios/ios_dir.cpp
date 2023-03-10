
bool _ui_library_dir(char * psz, unsigned int * puiSize);


namespace ios
{
   
   
   dir::dir(::aura::application *   papp) :
   ::object(papp),
   ::file::dir::system(papp),
   ::file_watcher::file_watcher(papp)
   {
      
      
   }
   
   
   ::file::listing & dir::root_ones(::file::listing & listing,::aura::application * papp)
   {
      
      listing.add("/");
      
      listing.m_straTitle.add("File System");
      
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
            
            if(::file::dir::system::ls(papp,listing))
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
         
         ::file::patha  straPath;
         
         ::dir::ls(straPath, listing.m_path);
         
         //            file_find file_find;
         
         //          bool bWorking = file_find.FindFile(listing.m_path / listing.os_pattern()) != FALSE;
         
         for(auto & strPath : straPath)
         {
            
            bool bDir = is(strPath, papp);
            
            if((listing.m_bDir && bDir) || (listing.m_bFile && !bDir))
            {
               
               if(!bDir && !matches_wildcard_criteria(listing.m_straPattern, strPath.name()))
                  continue;
               
               listing.add(strPath);
               
               listing.last().m_iDir = bDir ? 1 : 0;
               
            }
            
         }
         
         for(index i = iStart; i < listing.get_size(); i++)
         {
            
            listing[i].m_iRelative = listing.m_path.get_length() + 1;
            
         }
         
      }
      else
      {
         
         if(::file::dir::system::ls(papp,listing))
         {
            
            return listing;
            
         }
         
         ::file::patha  straPath;
         
         ::dir::ls(straPath, listing.m_path);
         
         //            file_find file_find;
         
         //          bool bWorking = file_find.FindFile(listing.m_path / listing.os_pattern()) != FALSE;
         
         for(auto & strPath : straPath)
         {
            
            bool bDir = is(strPath, papp);
            
            if((listing.m_bDir && bDir) || (listing.m_bFile && !bDir))
            {
               
               if(!bDir && !matches_wildcard_criteria(listing.m_straPattern, strPath.name()))
                  continue;
               
               listing.add(strPath);
               
               listing.last().m_iDir = bDir ? 1 : 0;
               
            }
            
         }
         
         
      }
      
      return listing;
      
   }
   
   
   //    void dir::ls_pattern(::aura::application *   papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, bool_array * pbaIsDir, int64_array * piaSize)
   //   {
   //
   //      if(::file::dir::system::is(lpcsz, papp)) // if base class "already" "says" it is a dir, let it handle it: may be not a operational system dir, e.g., zip or compressed directory...
   //      {
   //
   //         return ::file::dir::system::ls_pattern(papp, lpcsz, pszPattern, pstraPath, pstraTitle, pbaIsDir, piaSize);
   //
   //      }
   //
   //      string strDir(lpcsz);
   //
   //      if(!::str::ends(strDir, "/"))
   //      {
   //
   //         strDir += "/";
   //
   //      }
   //
   //      stringa stra;
   //
   //      ::dir::ls(stra, lpcsz);
   //
   //      for(int32_t i = 0; i < stra.get_count(); i++)
   //      {
   //
   //         string strPath = stra[i];
   //
   //         string strName = strPath;
   //
   //         if(!::str::begins_eat(strName, strDir))
   //            continue;
   //
   //         if(!matches_wildcard_criteria(pszPattern, strName))
   //            continue;
   //
   //         if(pstraPath != NULL)
   //         {
   //
   //            pstraPath->add(strPath);
   //
   //         }
   //
   //         if(pstraTitle != NULL)
   //         {
   //
   //            pstraTitle->add(strName);
   //
   //         }
   //
   //         bool bIsDir = false;
   //
   //         if(pbaIsDir != NULL || piaSize != NULL)
   //         {
   //
   //            bIsDir = ::dir::is(strPath);
   //
   //         }
   //
   //         if(pbaIsDir != NULL)
   //         {
   //
   //            pbaIsDir->add(bIsDir);
   //
   //         }
   //
   //         if(piaSize != NULL)
   //         {
   //
   //            if(bIsDir)
   //            {
   //
   //               piaSize->add(0);
   //
   //            }
   //            else
   //            {
   //
   //               piaSize->add(file_length_dup(strPath));
   //
   //            }
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //   void dir::rls(::aura::application *   papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, e_extract eextract)
   //   {
   //      rls_pattern(papp, lpcsz, "*.*", pstraPath, pstraTitle, pstraRelative, NULL, NULL, eextract);
   //   }
   //
   //   void dir::rls_pattern(::aura::application *   papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, bool_array * pbaIsDir, int64_array * piaSize, e_extract eextract)
   //   {
   //
   //      stringa straDir;
   //
   //      ::dir::ls_dir(straDir, lpcsz);
   //
   //      for(int32_t i = 0; i < straDir.get_count(); i++)
   //      {
   //
   //         string strDir = straDir[i];
   //
   //         if(strDir == lpcsz)
   //            continue;
   //
   //         index iStart = 0;
   //
   //         if(pstraRelative != NULL)
   //         {
   //            iStart = pstraRelative->get_size();
   //         }
   //
   //         rls_pattern(papp, strDir, pszPattern, pstraPath, pstraTitle, pstraRelative, pbaIsDir, piaSize, eextract == extract_all ? extract_all : extract_none);
   //
   //         if(pstraRelative != NULL)
   //         {
   //
   //            for(index i = iStart; i < pstraRelative->get_size(); i++)
   //            {
   //
   //               pstraRelative->element_at(i) = System.dir().path(System.file().name_(strDir), pstraRelative->element_at(i));
   //
   //            }
   //
   //         }
   //
   //      }
   //
   //      string strDir(lpcsz);
   //
   //      if(!::str::ends(strDir, "/"))
   //      {
   //
   //         strDir += "/";
   //
   //      }
   //
   //      stringa stra;
   //
   //      ::dir::ls(stra, lpcsz);
   //
   //      for(int32_t i = 0; i < stra.get_count(); i++)
   //      {
   //
   //         string strPath = stra[i];
   //
   //         string strName = strPath;
   //
   //         if(!::str::begins_eat(strName, strDir))
   //            continue;
   //
   //         if(!matches_wildcard_criteria(pszPattern, strName))
   //            continue;
   //
   //         if(pstraPath != NULL)
   //         {
   //
   //            pstraPath->add(strPath);
   //
   //         }
   //
   //         if(pstraTitle != NULL)
   //         {
   //
   //            pstraTitle->add(strName);
   //
   //         }
   //
   //         bool bIsDir = false;
   //
   //         if(pbaIsDir != NULL || piaSize != NULL)
   //         {
   //
   //            bIsDir = ::dir::is(strPath);
   //
   //         }
   //
   //         if(pbaIsDir != NULL)
   //         {
   //
   //            pbaIsDir->add(bIsDir);
   //
   //         }
   //
   //         if(piaSize != NULL)
   //         {
   //
   //            if(bIsDir)
   //            {
   //
   //               piaSize->add(0);
   //
   //            }
   //            else
   //            {
   //
   //               piaSize->add(file_length_dup(strPath));
   //
   //            }
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //   void dir::rls_dir(::aura::application *   papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative)
   //   {
   //
   //      stringa stra;
   //
   //      ::dir::ls(stra, lpcsz);
   //
   //      string strDir(lpcsz);
   //
   //      if(!::str::ends(strDir, "/"))
   //      {
   //
   //         strDir += "/";
   //
   //      }
   //
   //
   //      for(int32_t i = 0; i < stra.get_count(); i++)
   //      {
   //
   //         string strPath = stra[i];
   //
   //         string strName = strPath;
   //
   //         if(!::str::begins_eat(strName, strDir))
   //            continue;
   //
   //         if(!System.dir().is(strPath, papp))
   //            continue;
   //
   //         if(pstraPath != NULL)
   //         {
   //
   //            pstraPath->add((const char *) stra[i]);
   //
   //         }
   //
   //         if(pstraTitle != NULL)
   //         {
   //
   //            pstraTitle->add(strName);
   //
   //         }
   //
   //         if(pstraRelative != NULL)
   //         {
   //
   //            pstraRelative->add(strName);
   //
   //         }
   //
   //         index iStart = 0;
   //
   //         if(pstraRelative != NULL)
   //         {
   //
   //            iStart = pstraRelative->get_size();
   //
   //         }
   //
   //         rls_dir(papp, strPath, pstraPath, pstraTitle, pstraRelative);
   //
   //         if(pstraRelative != NULL)
   //         {
   //
   //            for(index i = iStart; i < pstraRelative->get_size(); i++)
   //            {
   //
   //               pstraRelative->element_at(i) = System.dir().path(strPath, pstraRelative->element_at(i));
   //
   //            }
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //
   //   void dir::ls_dir(::aura::application *   papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
   //   {
   //
   //      stringa stra;
   //
   //      ::dir::ls(stra, lpcsz);
   //
   //      string strDir(lpcsz);
   //
   //      if(!::str::ends(strDir, "/"))
   //      {
   //
   //         strDir += "/";
   //
   //      }
   //
   //
   //      for(int32_t i = 0; i < stra.get_count(); i++)
   //      {
   //
   //         string strPath = stra[i];
   //
   //         string strName = strPath;
   //
   //         if(!::str::begins_eat(strName, strDir))
   //            continue;
   //
   //         if(!System.dir().is(strPath, papp))
   //            continue;
   //
   //         if(pstraPath != NULL)
   //         {
   //
   //            pstraPath->add((const char *) stra[i]);
   //
   //         }
   //
   //         if(pstraTitle != NULL)
   //         {
   //
   //            pstraTitle->add(strName);
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //   void dir::ls_file(::aura::application *   papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
   //   {
   //
   //      stringa stra;
   //
   //      ::dir::ls(stra, lpcsz);
   //
   //      string strDir(lpcsz);
   //
   //      if(!::str::ends(strDir, "/"))
   //      {
   //
   //         strDir += "/";
   //
   //      }
   //
   //
   //      for(int32_t i = 0; i < stra.get_count(); i++)
   //      {
   //
   //         string strPath = stra[i];
   //
   //         string strName = strPath;
   //
   //         if(!::str::begins_eat(strName, strDir))
   //            continue;
   //
   //         if(System.dir().is(strPath, papp))
   //            continue;
   //
   //
   //         if(pstraPath != NULL)
   //         {
   //
   //            pstraPath->add((const char *) stra[i]);
   //
   //         }
   //
   //         if(pstraTitle != NULL)
   //         {
   //
   //            pstraTitle->add(strName);
   //
   //         }
   //
   //      }
   //
   //   }
   //
   //   void dir::ls(::aura::application *   papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, bool_array * pbaIsDir, int64_array * piaSize)
   //   {
   //
   //      stringa stra;
   //
   //      ::dir::ls(stra, lpcsz);
   //
   //      string strDir(lpcsz);
   //
   //      if(!::str::ends(strDir, "/"))
   //      {
   //
   //         strDir += "/";
   //
   //      }
   //
   //
   //      for(int32_t i = 0; i < stra.get_count(); i++)
   //      {
   //
   //         string strPath = stra[i];
   //
   //         string strName = strPath;
   //
   //         if(!::str::begins_eat(strName, strDir))
   //            continue;
   //
   //         if(pstraPath != NULL)
   //         {
   //
   //            pstraPath->add((const char *) stra[i]);
   //
   //         }
   //
   //         if(pstraTitle != NULL)
   //         {
   //
   //            pstraTitle->add(strName);
   //
   //         }
   //
   //
   //         bool bIsDir = false;
   //
   //         if(pbaIsDir != NULL || piaSize != NULL)
   //         {
   //
   //            bIsDir = System.dir().is(strPath, papp);
   //
   //         }
   //
   //         if(pbaIsDir != NULL)
   //         {
   //
   //            pbaIsDir->add(bIsDir);
   //
   //         }
   //
   //         if(piaSize != NULL)
   //         {
   //
   //            if(bIsDir)
   //            {
   //
   //               piaSize->add(0);
   //
   //            }
   //            else
   //            {
   //
   //               piaSize->add(file_length_dup(strPath));
   //
   //            }
   //
   //         }
   //
   //
   //      }
   //
   //   }
   
   bool dir::is(const ::file::path & lpcszPath, ::aura::application * papp)
   {
      
      if(::file::dir::system::is(lpcszPath, papp))
      {
         
         return true;
      
      }
      
      string strPath(lpcszPath);
      
#ifdef WINDOWS
      
      if(strPath.get_length() >= MAX_PATH)
      {
         
         if(::str::begins(strPath, "\\\\"))
         {
            
            strPath = "\\\\?\\UNC" + strPath.Mid(1);
            
         }
         else
         {
            
            strPath = "\\\\?\\" + strPath;
       
         }
         
      }
      
#endif
      
      bool bIsDir = ::dir::_is(strPath);
      
      return bIsDir;
      
   }
   
   
   //   bool dir::is(const string & strPath, ::aura::application *   papp)
   //   {
   //
   //      if(::file::dir::system::is(strPath, papp))
   //         return true;
   //
   //      bool bIsDir;
   //
   //      DWORD dwLastError;
   //
   //      if(m_isdirmap.lookup(strPath, bIsDir, dwLastError))
   //      {
   //
   //         if(!bIsDir)
   //         {
   //
   //            set_last_error(dwLastError);
   //
   //         }
   //
   //         return bIsDir;
   //
   //      }
   //
   //
   //      wstring wstrPath;
   //
   //      //strsize iLen = ::str::international::utf8_to_unicode_count(strPath);
   //      //wstrPath.alloc(iLen + 32);
   //      wstrPath = ::str::international::utf8_to_unicode(strPath);
   //      if(wstrPath.get_length() >= MAX_PATH)
   //      {
   //         if(::str::begins(wstrPath, L"\\\\"))
   //         {
   //            ::str::begin(wstrPath, L"\\\\?\\UNC");
   //         }
   //         else
   //         {
   //            ::str::begin(wstrPath, L"\\\\?\\");
   //         }
   //      }
   //
   //      bIsDir = ::dir::is(::str::international::unicode_to_utf8(wstrPath));
   //
   //      m_isdirmap.set(strPath, bIsDir, bIsDir ? 0 : ::get_last_error());
   //
   //      return bIsDir;
   //   }
   
   bool dir::name_is(const ::file::path & str, ::aura::application *   papp)
   {
      //output_debug_string(str);
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
      
      
      if(thread_zip_is_dir() && iLast >= 3  && !strnicmp_dup(&((const char *) str)[iLast - 3], ".zip", 4))
      {
         
         //m_isdirmap.set(str.Left(iLast + 1), true, 0);
         
         return true;
         
      }
      
      wstring wstrPath;
      
      //strsize iLen = ::str::international::utf8_to_unicode_count(str, iLast + 1);
      
      //wstrPath.alloc(iLen + 32);
      
      wstrPath = ::str::international::utf8_to_unicode(str, iLast + 1);
      
      //output_debug_string(wstrPath);
      
//      if(wstrPath.get_length() >= MAX_PATH)
//      {
//         if(::str::begins(wstrPath, L"\\\\"))
//         {
//            ::str::begin(wstrPath, L"\\\\?\\UNC");
//         }
//         else
//         {
//            ::str::begin(wstrPath, L"\\\\?\\");
//         }
//      }
      
      bool bIsDir = ::dir::is(::str::international::unicode_to_utf8(wstrPath));
      
      return bIsDir;
      
   }
   
   
   bool dir::mk(const ::file::path & lpcsz, ::aura::application *   papp)
   {
      
      if(is(lpcsz, papp))
         return true;
      
      ::file::patha stra;
      
      lpcsz.ascendants_path(stra);
      
      index i = stra.get_upper_bound();
      
      for(; i >= 0; i--)
      {
         
         if(is(stra[i], papp))
         {
            
            i++;
            
            break;
            
         }
         
      }
      
      if(i < 0)
      {
       
         return true;
         
      }

      for(; i < stra.get_size(); i++)
      {
         
         if(!is(stra[i], papp))
         {
            
            if(::dir::mkdir(stra[i]))
            {
               
            }
            
            if(!::dir::is(stra[i]))
            {
               
               return false;
               
            }
            
         }
         
      }
      
      return true;
      
   }
   
   
   //   bool dir::rm(::aura::application *   papp, const ::file::path & psz, bool bRecursive)
   //   {
   //      if(bRecursive)
   //      {
   //         stringa straPath;
   //         stringa straTitle;
   //         ls(papp, psz, &straPath, &straTitle);
   //         for(int32_t i = 0; i < straPath.get_count(); i++)
   //         {
   //            if(is(straPath[i], papp))
   //            {
   //               rm(papp, path(psz, straTitle[i]), true);
   //            }
   //            else
   //            {
   //               ::unlink(straPath[i]);
   //            }
   //         }
   //      }
   //      return ::rmdir(psz) != FALSE;
   //   }
   //
   //
   //   string dir::name(const char * path1)
   //   {
   //      const char * psz = path1 + strlen(path1) - 1;
   //      while(psz >= path1)
   //      {
   //         if(*psz != '\\' && *psz != '/' && *psz != ':')
   //            break;
   //         psz--;
   //      }
   //      while(psz >= path1)
   //      {
   //         if(*psz == '\\' || *psz == '/' || *psz == ':')
   //            break;
   //         psz--;
   //      }
   //      if(psz >= path1) // strChar == "\\" || strChar == "/"
   //      {
   //         const char * pszEnd = psz;
   //         /*while(psz >= path1)
   //          {
   //          if(*psz != '\\' && *psz != '/' && *psz != ':')
   //          break;
   //          psz--;
   //          }*/
   //         return string(path1, pszEnd - path1 + 1);
   //      }
   //      else
   //      {
   //         return "";
   //      }
   //   }
   //
   //   string dir::name(const string & str)
   //   {
   //
   //      strsize iLast = str.get_length() - 1;
   //
   //      while(iLast >= 0)
   //      {
   //         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
   //            break;
   //         iLast--;
   //      }
   //      while(iLast >= 0)
   //      {
   //         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
   //            break;
   //         iLast--;
   //      }
   //      if(iLast >= 0)
   //      {
   //         while(iLast >= 0)
   //         {
   //            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
   //               break;
   //            iLast--;
   //         }
   //         return str.Left(iLast + 1);
   //      }
   //      else
   //      {
   //         return "";
   //      }
   //   }
   //
   //
   //   class ::file::path & dir::path()
   //   {
   //      return m_path;
   //   }
   
   
   
   ::file::path dir::trash_that_is_not_trash(const ::file::path & psz)
   {
      if(psz == NULL)
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
   
   
   
   
   //   string dir::usersystemappdata(::aura::application *   papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
   //   {
   //      UNREFERENCED_PARAMETER(papp);
   //      return path(appdata(lpcszPrefix), lpcsz, lpcsz2);
   //   }
   //
   //   string dir::appdata(::aura::application *   papp, const char * lpcsz, const char * lpcsz2)
   //   {
   //      return path(userfolder(papp, "appdata"), lpcsz, lpcsz2);
   //   }
   //
   //   string dir::userdata(::aura::application *   papp, const char * lpcsz, const char * lpcsz2)
   //   {
   //      return path(userfolder(papp, "data"), lpcsz, lpcsz2);
   //   }
   
   //   ::file::path dir::userfolder(::aura::application * papp)
   //   {
   //      
   //      return App(papp).dir().userfolder();
   //
   //   }
   
   
//   ::file::path dir::default_os_user_path_prefix(::aura::application *   papp)
//   {
//
//      return ::getlogin();
//
//   }
   
   
   //   string dir::default_userappdata(::aura::application *   papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
   //   {
   //      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "appdata"), pszRelativePath);
   //   }
   //
   //   string dir::default_userdata(::aura::application *   papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
   //   {
   //      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "data"), pszRelativePath);
   //   }
   //
   //   string dir::default_userfolder(::aura::application *   papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
   //   {
   //
   //      return userfolder(papp, pszRelativePath);
   //
   //      /*      UNREFERENCED_PARAMETER(papp);
   //       string str;
   //       SHGetSpecialFolderPath(
   //       NULL,
   //       str,
   //       CSIDL_APPDATA,
   //       FALSE);
   //       return path(path(str, "ca2\\user", lpcszPrefix), lpcszLogin, pszRelativePath);*/
   //   }
   //
   //   string dir::userquicklaunch(::aura::application *   papp, const char * lpcszRelativePath, const char * lpcsz2)
   //   {
   //      UNREFERENCED_PARAMETER(papp);
   //      string str;
   //      /*SHGetSpecialFolderPath(
   //       NULL,
   //       str,
   //       CSIDL_APPDATA,
   //       FALSE);*/
   //      str = path(getenv("HOME"), "Microsoft\\Internet Explorer\\Quick Launch");
   //      return path(str, lpcszRelativePath, lpcsz2);
   //   }
   
   //   string dir::userprograms(::aura::application *   papp, const char * lpcszRelativePath, const char * lpcsz2)
   //   {
   //      UNREFERENCED_PARAMETER(papp);
   //      string str;
   //      /*      SHGetSpecialFolderPath(
   //       NULL,
   //       str,
   //       CSIDL_PROGRAMS,
   //       FALSE);*/
   //
   //      str = "/usr/bin";
   //      return path(str, lpcszRelativePath, lpcsz2);
   //   }
   //
   //   string dir::commonprograms(const char * lpcszRelativePath, const char * lpcsz2)
   //   {
   //      string str;
   //      /*      SHGetSpecialFolderPath(
   //       NULL,
   //       str,
   //       CSIDL_COMMON_PROGRAMS,
   //       FALSE);*/
   //      str = "/usr/share/";
   //      return path(str, lpcszRelativePath, lpcsz2);
   //   }
   
   //   bool dir::is_inside_time(const char * pszPath, ::aura::application *   papp)
   //   {
   //      return is_inside(time(), pszPath, papp);
   //   }
   //
   //   bool dir::is_inside(const char * pszDir, const char * pszPath, ::aura::application *   papp)
   //   {
   //      return ::str::begins_ci(pszDir, pszPath);
   //   }
   //
   bool dir::has_subdir(::aura::application *   papp, const ::file::path & pszDir)
   {
      ::file::listing stra(papp);
      stra.ls_dir(pszDir);
      return stra.get_size() > 0;
      
   }
   
   
   
   
   
   //    bool dir::is(const ::file::path & lpcszPath, ::aura::application * papp)
   //    {
   //        
   //        if(::file::dir::system::is(lpcszPath, papp))
   //            return true;
   //        
   //        string strPath(lpcszPath);
   //        
   //        if(strPath.get_length() >= MAX_PATH)
   //        {
   //            
   //            if(::str::begins(strPath,astr.strDoubleBackSlash))
   //            {
   //                
   //                strPath = "\\\\?\\UNC" + strPath.Mid(1);
   //                
   //            }
   //            else
   //            {
   //                
   //                strPath = "\\\\?\\" + strPath;
   //                
   //            }
   //            
   //        }
   //        
   //        DWORD dwAttrib;
   //        
   //        dwAttrib = GetFileAttributesW(::str::international::utf8_to_unicode(strPath));
   //        
   //        bool bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
   //        
   //        m_isdirmap.set(lpcszPath, bIsDir, bIsDir ? 0 : ::get_last_error());
   //        
   //        return bIsDir;
   //        
   //    }
   
   
   //bool dir::is(const ::file::path & strPath, ::aura::application * papp)
   //{
   //
   //   if(::file::dir::system::is(strPath, papp))
   //      return true;
   
   //   bool bIsDir;
   
   //   uint32_t uiLastError;
   
   //   if(m_isdirmap.lookup(strPath, bIsDir, uiLastError))
   //   {
   //      if(!bIsDir)
   //      {
   //         ::set_last_error(uiLastError);
   //      }
   //      return bIsDir;
   //   }
   
   //   wstring wstrPath;
   //
   //   //strsize iLen = ::str::international::utf8_to_unicode_count(strPath);
   //   //wstrPath.alloc(iLen + 32);
   //   wstrPath = ::str::international::utf8_to_unicode(strPath);
   //   if(wstrPath.get_length() >= MAX_PATH)
   //   {
   //      if(::str::begins(wstrPath, L"\\\\"))
   //      {
   //         ::str::begin(wstrPath, L"\\\\?\\UNC");
   //      }
   //      else
   //      {
   //         ::str::begin(wstrPath, L"\\\\?\\");
   //      }
   //   }
   //   DWORD dwAttrib;
   //   dwAttrib = GetFileAttributesW(wstrPath);
   //   /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
   //   {
   //      dwAttrib = GetFileAttributes(strPath);
   //   }*/
   //
   //   bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
   //
   //   m_isdirmap.set(strPath, bIsDir, bIsDir ? 0 : ::get_last_error());
   
   //   return bIsDir;
   //}
   
   //    bool dir::name_is(const ::file::path & str, ::aura::application * papp)
   //    {
   //        //output_debug_string(str);
   //        strsize iLast = str.get_length() - 1;
   //        while(iLast >= 0)
   //        {
   //            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
   //                break;
   //            iLast--;
   //        }
   //        while(iLast >= 0)
   //        {
   //            if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
   //                break;
   //            iLast--;
   //        }
   //        if(iLast >= 0)
   //        {
   //            while(iLast >= 0)
   //            {
   //                if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
   //                {
   //                    iLast++;
   //                    break;
   //                }
   //                iLast--;
   //            }
   //        }
   //        else
   //        {
   //            return true; // assume empty string is root_ones directory
   //        }
   //        
   //        
   //        bool bIsDir;
   //        
   //        
   //        uint32_t uiLastError;
   //        
   //        if(m_isdirmap.lookup(str, bIsDir, uiLastError, (int32_t) iLast))
   //        {
   //            if(!bIsDir)
   //            {
   //                ::set_last_error(uiLastError);
   //            }
   //            return bIsDir;
   //        }
   //        
   //        
   //        if(::get_thread() != NULL && ::get_thread()->m_bZipIsDir && iLast >= 3 && !strnicmp_dup(&((const char *)str)[iLast - 3],".zip",4))
   //        {
   //            m_isdirmap.set(str.Left(iLast + 1), true, 0);
   //            return true;
   //        }
   //        
   //        
   //        
   //        
   //        wstring wstrPath;
   //        
   //        //strsize iLen = ::str::international::utf8_to_unicode_count(str, iLast + 1);
   //        
   //        //wstrPath.alloc(iLen + 32);
   //        
   //        wstrPath = ::str::international::utf8_to_unicode(str, iLast + 1);
   //        
   //        //output_debug_string(wstrPath);
   //        
   //        if(wstrPath.get_length() >= MAX_PATH)
   //        {
   //            if(::str::begins(wstrPath, L"\\\\"))
   //            {
   //                ::str::begin(wstrPath, L"\\\\?\\UNC");
   //            }
   //            else
   //            {
   //                ::str::begin(wstrPath, L"\\\\?\\");
   //            }
   //        }
   //        DWORD dwAttrib;
   //        dwAttrib = GetFileAttributesW(wstrPath);
   //        /*if(dwAttrib == INVALID_FILE_ATTRIBUTES)
   //         {
   //         dwAttrib = GetFileAttributes(strPath);
   //         }*/
   //        
   //        bIsDir = (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
   //        
   //        m_isdirmap.set(str.Left(iLast + 1), bIsDir, bIsDir ? 0 : ::get_last_error());
   //        
   //        return bIsDir;
   //    }
   
   
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
      return stage() / "basis";
   }
   
   ::file::path dir::netseed()
   {
      return m_strNetSeedFolder;
   }
   
   // stage in ca2os spalib
//   ::file::path dir::install()
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return m_strCa2;
//
//   }
   
   
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
   
   
   //    bool dir::mk(const ::file::path & lpcsz,::aura::application * papp)
   //    {
   //        
   //        if(is(lpcsz, papp))
   //            return true;
   //        
   //        ::file::patha stra;
   //        lpcsz.ascendants_path(stra);
   //        for(int32_t i = 0; i < stra.get_size(); i++)
   //        {
   //            
   //            string strDir = stra[i];
   //            
   //            if(!is(strDir, papp))
   //            {
   //                
   //                if(::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + strDir), NULL))
   //                {
   //                    
   //                    m_isdirmap.set(strDir, true, 0);
   //                    
   //                }
   //                else
   //                {
   //                    
   //                    DWORD dwError = ::get_last_error();
   //                    
   //                    if (dwError == ERROR_ALREADY_EXISTS)
   //                    {
   //                        
   //                        if (::dir::is(strDir))
   //                        {
   //                            m_isdirmap.set(strDir, true, 0);
   //                        }
   //                        else
   //                        {
   //                            string str;
   //                            str = "\\\\?\\" + strDir;
   //                            str.trim_right("\\/");
   //                            try
   //                            {
   //                                Application.file().del(str);
   //                            }
   //                            catch (...)
   //                            {
   //                            }
   //                            str = stra[i];
   //                            str.trim_right("\\/");
   //                            try
   //                            {
   //                                Application.file().del(str);
   //                            }
   //                            catch (...)
   //                            {
   //                            }
   //                            if (::CreateDirectoryW(::str::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
   //                            {
   //                                m_isdirmap.set(stra[i], true, 0);
   //                                goto try1;
   //                            }
   //                            else
   //                            {
   //                                dwError = ::get_last_error();
   //                            }
   //                        }
   //                        char * pszError;
   //                        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPTSTR)&pszError, 8, NULL);
   //                        
   //                        //TRACE("dir::mk CreateDirectoryW last error(%d)=%s", dwError, pszError);
   //                        ::LocalFree(pszError);
   //                        //m_isdirmap.set(stra[i], false);
   //                    }
   //                }
   //            try1:
   //                
   //                if(!is(stra[i], papp))
   //                {
   //                    return false;
   //                }
   //                
   //            }
   //        }
   //        return true;
   //    }
   
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
               ::unlink(path);
            }
         }
      }
      return ::rmdir(psz) == 0;
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
   
   
   //class ::file::file_path & dir::path()
   //{
   //   return m_path;
   //}
   
   /*!
    * Print a known folder.
    */
   //    ::file::path get_known_folder(REFKNOWNFOLDERID kfid)
   //    {
   //        ::file::path str;
   //        PWSTR pszPath = NULL;
   //        HANDLE hToken = NULL;
   //        ::OpenProcessToken(::GetCurrentProcess(),TOKEN_QUERY | TOKEN_IMPERSONATE | TOKEN_DUPLICATE,&hToken);
   //        HRESULT hr = SHGetKnownFolderPath(kfid,0,hToken,&pszPath);
   //        if(SUCCEEDED(hr))
   //        {
   //            str = pszPath;
   //            // The calling application is responsible for calling CoTaskMemFree
   //            // to free this resource after use.
   //            CoTaskMemFree(pszPath);
   //        }
   //        else
   //        {
   //        }
   //        return str;
   //    }
   
   
   bool dir::initialize()
   {
      
      if(!update_module_path())
      {
       
         return false;
         
      }
      
//      m_strCa2 = ::file::path(getenv("HOME")) / "Library/Application Support";
      {
         
         string str;
         
         unsigned int uiSize = 4096;
    
         char * psz = str.GetBufferSetLength(uiSize);
      
         _ui_library_dir(psz, &uiSize);
         
         str.ReleaseBuffer(uiSize);
         
         m_strCa2 = str;
                      
      }
      
      
      ::file::path pathHome;
      
      pathHome = m_strCa2/"Documents";
      
      System.m_strIosHome = pathHome;
      
      m_pathHome = m_strCa2/"Documents";
      
      nodeos_set_home(System.m_strIosHome);
      
      ::file::path str = m_strCa2 / ".ca2/appdata";
      
//      m_pathUser = m_strCa2 / ".ca2/user";
      
      string strRelative;
      
      strRelative = install();
      
      m_strCa2AppData = str / "ca2" / strRelative;
      
      str = m_pathModule.folder();
      
      m_strCommonAppData = str / "commonappdata";
      
      xml::document doc(get_app());
      
      string strPath = appdata() / "configuration/directory.xml";
      
      string strDocument = Application.file().as_string(strPath);
      
      if(doc.load(strDocument))
      {
         
         if(doc.get_root()->get_name() == "directory_configuration")
         {
            
            m_strTimeFolder = doc.get_root()->get_child_value("time");
            
            m_strNetSeedFolder = doc.get_root()->get_child_value("netseed");
            
         }
         
      }
      
      if(m_strTimeFolder.is_empty())
      {
         
         m_strTimeFolder = m_strCa2 / "Library/Cache/time";
         
      }
      
      if(m_strNetSeedFolder.is_empty())
      {
         
         m_strNetSeedFolder = install() / "net";
         
      }
      
      mk(m_strTimeFolder, get_app());
      
      if(!is(m_strTimeFolder, get_app()))
      {
         
         return false;
         
      }
      
      string strTime = m_strTimeFolder / "time";
      
      mk(strTime, get_app());
      
      if(!is(strTime, get_app()))
      {
         
         return false;
         
      }
      
      System.m_strIosTemp = strTime;
      
      nodeos_set_temp(System.m_strIosTemp);
      
      str = "/usr/bin";
      
      m_strPrograms = str;
      
      str = "/usr/share/";
      
      m_strCommonPrograms = str;
      
      return true;
      
   }
   
   
   //    ::file::path dir::trash_that_is_not_trash(const ::file::path & psz)
   //    {
   //        if(psz.is_empty())
   //            return "";
   //        
   //        if(psz[1] == ':')
   //        {
   //            string strDir = name(psz);
   //            string str;
   //            str = strDir.Left(2);
   //            str += "\\trash_that_is_not_trash\\";
   //            string strFormat;
   //            ::datetime::time time;
   //            time = ::datetime::time::get_current_time();
   //            strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
   //            str += strFormat;
   //            if(strDir.m_pszData[2] == '\\')
   //            {
   //                str += strDir.Mid(3);
   //            }
   //            else
   //            {
   //                str += strDir.Mid(2);
   //            }
   //            return str;
   //        }
   //        
   //        return "";
   //    }
   
   ::file::path dir::appdata()
   {
      
      return m_strCa2AppData;
      
   }
   
   
   ::file::path dir::commonappdata_root()
   {
      
      return m_strCommonAppData;
      
   }
   
   
//   ::file::path dir::usersystemappdata(::aura::application * papp,const char * lpcszPrefix)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      return appdata() / lpcszPrefix;
//   }
//
//   ::file::path dir::appdata(::aura::application * papp)
//   {
//      return userfolder(papp) / "appdata";
//   }
//
//   ::file::path dir::userdata(::aura::application * papp)
//   {
//      return userfolder(papp) / "data";
//   }
   
   //    ::file::path dir::userfolder(::aura::application * papp)
   //    {
   //        
   //
   //        
   //    }
   //    
   //    ::file::path dir::default_os_user_path_prefix(::aura::application * papp)
   //    {
   //        UNREFERENCED_PARAMETER(papp);
   //        unichar buf[MAX_PATH];
   //        ULONG ulSize = sizeof(buf) / sizeof(unichar);
   //        if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
   //        {
   //            if(!::GetUserNameW(buf, &ulSize))
   //            {
   //                memset(buf, 0, sizeof(buf));
   //            }
   //        }
   //        return ::str::international::unicode_to_utf8(buf);
   //    }
   
//   ::file::path dir::default_userappdata(::aura::application * papp,const string & lpcszPrefix,const string & lpcszLogin)
//   {
//      return default_userfolder(papp, lpcszPrefix, lpcszLogin) /  "appdata" ;
//   }
//   
//   ::file::path dir::default_userdata(::aura::application * papp,const string & lpcszPrefix,const string & lpcszLogin)
//   {
//      return default_userfolder(papp, lpcszPrefix, lpcszLogin) / "data";
//   }
//   
//   ::file::path dir::default_userfolder(::aura::application * papp,const string & lpcszPrefix,const string & lpcszLogin)
//   {
//      
//      return userfolder(papp);
//      
//   }
   
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
   
   
   //    bool dir::has_subdir(::aura::application * papp, const ::file::path & pszDir)
   //    {
   //        
   //        file_find file_find;
   //        
   //        bool bWorking;
   //        
   //        bWorking = file_find.FindFile(pszDir / "*.*");
   //        
   //        while(bWorking)
   //        {
   //            
   //            bWorking = file_find.FindNextFileA();
   //            
   //            if(file_find.IsDirectory() && !file_find.IsDots())
   //            {
   //                
   //                return true;
   //                
   //            }
   //            
   //        }
   //        
   //        return false;
   //        
   //    }
   
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
   
   
   
   
   
} // namespace ios


//namespace ios
//{
//
//
//   dir::dir(sp(::aura::application)  papp) :
//      ::object(papp),
//      ::file::dir::system(papp),
//   m_path(papp)
//   {
//
//      string strCa2Module = ca2module();
//
//      m_strCa2 = strCa2Module;
//
//      System.file().path().eat_end_level(m_strCa2, 2, "/");
//
//   }
//
//   path::path(sp(::aura::application)  papp) :
//   ::object(papp)
//   {
//   }
//
//
//   inline bool myspace(char ch)
//   {
//      return ch == ' ' ||
//      ch == '\t' ||
//      ch == '\r' ||
//      ch == '\n';
//   }
//
//   string dir::path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl)
//   {
//
//      bool bEmptyRelative = iLenRelative == 0 || pszRelative == NULL || *pszRelative == '\0';
//      bool bEmpty2 = iLen2 == 0 || psz2 == NULL || *psz2 == '\0';
//
//      if(bEmptyRelative && bEmpty2)
//         return pszFolder;
//
//      string strPath;
//      LPSTR lpsz;
//
//      if(bEmptyRelative)
//      {
//         pszRelative = psz2;
//         iLenRelative = iLen2;
//      }
//
//      while((pszFolder[iLenFolder - 1] == '\\' || pszFolder[iLenFolder - 1] == '/') && iLenFolder > 0)
//      {
//         if(bUrl)
//         {
//            if((iLenFolder - 2) >= 0 && (pszFolder[iLenFolder - 2] == '\\' || pszFolder[iLenFolder - 2] == '/' || pszFolder[iLenFolder - 2] == ':'))
//            {
//               if(pszFolder[iLenFolder - 2] == ':')
//               {
//                  break;
//               }
//               else
//               {
//                  if((iLenFolder - 3) >= 0 && (pszFolder[iLenFolder - 3] == ':'))
//                  {
//                     iLenFolder--;
//                     break;
//                  }
//               }
//            }
//            else
//            {
//               iLenFolder--;
//            }
//         }
//         else
//         {
//            iLenFolder--;
//         }
//      }
//
//      while(*pszRelative != '\0' && (*pszRelative == '\\' || *pszRelative == '/') && iLenRelative > 0)
//      {
//         pszRelative++;
//         iLenRelative--;
//      }
//
//      if(bEmptyRelative || bEmpty2)
//      {
//         lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative);
//         strncpy(lpsz, pszFolder, iLenFolder);
//         if(iLenFolder >= 5 && strnicmp_dup(&lpsz[iLenFolder - 5], ".zip:", 5) == 0)
//         {
//            iLenFolder--;
//         }
//         else
//         {
//            lpsz[iLenFolder] = '/';
//         }
//         strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
//         lpsz[iLenFolder + 1 + iLenRelative] = '\0';
//         {
//            if(bUrl)
//            {
//               while(*lpsz++ != '\0')
//                  if(*lpsz == '\\') *lpsz = '/';
//            }
//            else
//            {
//               while(*lpsz++ != '\0')
//                  if(*lpsz == '/') *lpsz = '/';
//            }
//         }
//         strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative);
//         return strPath;
//      }
//
//      while((pszRelative[iLenRelative - 1] == '\\' || pszRelative[iLenRelative - 1] == '/') && iLenRelative > 0)
//      {
//         iLenRelative--;
//      }
//
//      while(*psz2 != '\0' && (*psz2 == '\\' || *psz2 == '/') && iLen2 > 0)
//      {
//         psz2++;
//         iLen2--;
//      }
//
//      lpsz = strPath.GetBufferSetLength(iLenFolder + 1 + iLenRelative + 1 + iLen2);
//      strncpy(lpsz, pszFolder, iLenFolder);
//      lpsz[iLenFolder] = '/';
//      strncpy(&lpsz[iLenFolder + 1], pszRelative, iLenRelative);
//      lpsz[iLenFolder + 1 + iLenRelative] = '/';
//      strncpy(&lpsz[iLenFolder + 1 + iLenRelative + 1], psz2, iLen2);
//      lpsz[iLenFolder + 1 + iLenRelative + 1 + iLen2] = '\0';
//      {
//         if(bUrl)
//         {
//            while(*lpsz++ != '\0')
//               if(*lpsz == '\\') *lpsz = '/';
//         }
//         else
//         {
//            while(*lpsz++ != '\0')
//               if(*lpsz == '\\') *lpsz = '/';
//         }
//      }
//      strPath.ReleaseBuffer(iLenFolder + 1 + iLenRelative + 1 + iLen2);
//      return strPath;
//   }
//
//
//
//   string dir::relpath(const char * lpcszSource, const char * lpcszRelative, const char * psz2)
//   {
//      const char * pszRequest;
//      if(::url::is_url(lpcszSource, &pszRequest))
//      {
//         if(::str::begins(lpcszRelative, "/"))
//         {
//            return path((const char *) string(lpcszSource, pszRequest - lpcszSource), lpcszRelative);
//         }
//         else if(*pszRequest == '\0' || ::str::ends(lpcszSource, "/"))
//         {
//            return path(lpcszSource, lpcszRelative, psz2);
//         }
//         else
//         {
//            return path((const char *) name(lpcszSource), lpcszRelative, psz2);
//         }
//      }
//      else
//      {
//         if(::str::ends(lpcszSource, "\\") || ::str::ends(lpcszSource, "/"))
//         {
//            return path(lpcszSource, lpcszRelative, psz2);
//         }
//         else
//         {
//            return path((const char *) name(lpcszSource), lpcszRelative, psz2);
//         }
//      }
//   }
//
//   bool path::is_equal(const char * lpcsz1, const char * lpcsz2)
//   {
//      return strcmp(lpcsz1, lpcsz2) == 0;
//   }
//
//   void dir::root_ones(stringa & stra, sp(::aura::application)  papp)
//   {
//      stra.add("/");
//      /*      DWORD dwSize = ::GetLogicalDriveStrings(0, NULL);
//       LPTSTR lpszAlloc = (LPTSTR) malloc(dwSize + 1);
//       LPTSTR lpsz = lpszAlloc;
//       dwSize = ::GetLogicalDriveStrings(dwSize + 1, lpsz);
//
//       string str;
//       while(*lpsz)
//       {
//       str.Empty();
//       while(*lpsz)
//       {
//       str += *lpsz;
//       lpsz++;
//       }
//       stra.add(str);
//       lpsz++;
//       }
//
//       free(lpszAlloc);*/
//   }
//
//   void dir::ls_pattern(sp(::aura::application)  papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, bool_array * pbaIsDir, int64_array * piaSize)
//   {
//
//      if(::file::dir::system::is(lpcsz, papp)) // if aura class "already" "says" it is a dir, let it handle it: may be not a operational system dir, e.g., zip or compressed directory...
//      {
//
//         return ::file::dir::system::ls_pattern(papp, lpcsz, pszPattern, pstraPath, pstraTitle, pbaIsDir, piaSize);
//
//      }
//
//      string strDir(lpcsz);
//
//      if(!::str::ends(strDir, "/"))
//      {
//
//         strDir += "/";
//
//      }
//
//      stringa stra;
//
//      ::dir::ls(stra, lpcsz);
//
//      for(int32_t i = 0; i < stra.get_count(); i++)
//      {
//
//         string strPath = stra[i];
//
//         string strName = strPath;
//
//         if(!::str::begins_eat(strName, strDir))
//            continue;
//
//         if(!matches_wildcard_criteria(pszPattern, strName))
//            continue;
//
//         if(pstraPath != NULL)
//         {
//
//            pstraPath->add(strPath);
//
//         }
//
//         if(pstraTitle != NULL)
//         {
//
//            pstraTitle->add(strName);
//
//         }
//
//         bool bIsDir = false;
//
//         if(pbaIsDir != NULL || piaSize != NULL)
//         {
//
//            bIsDir = ::dir::is(strPath);
//
//         }
//
//         if(pbaIsDir != NULL)
//         {
//
//            pbaIsDir->add(bIsDir);
//
//         }
//
//         if(piaSize != NULL)
//         {
//
//            if(bIsDir)
//            {
//
//               piaSize->add(0);
//
//            }
//            else
//            {
//
//               piaSize->add(file_length_dup(strPath));
//
//            }
//
//         }
//
//      }
//
//   }
//
//   void dir::rls(sp(::aura::application)  papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, e_extract eextract)
//   {
//      rls_pattern(papp, lpcsz, "*.*", pstraPath, pstraTitle, pstraRelative, NULL, NULL, eextract);
//   }
//
//   void dir::rls_pattern(sp(::aura::application)  papp, const char * lpcsz, const char * pszPattern, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, bool_array * pbaIsDir, int64_array * piaSize, e_extract eextract)
//   {
//
//      stringa straDir;
//
//      ::dir::ls_dir(straDir, lpcsz);
//
//      for(int32_t i = 0; i < straDir.get_count(); i++)
//      {
//
//         string strDir = straDir[i];
//
//         if(strDir == lpcsz)
//            continue;
//
//         index iStart = 0;
//
//         if(pstraRelative != NULL)
//         {
//            iStart = pstraRelative->get_size();
//         }
//
//         rls_pattern(papp, strDir, pszPattern, pstraPath, pstraTitle, pstraRelative, pbaIsDir, piaSize, eextract == extract_all ? extract_all : extract_none);
//
//         if(pstraRelative != NULL)
//         {
//
//            for(index i = iStart; i < pstraRelative->get_size(); i++)
//            {
//
//               pstraRelative->element_at(i) = ::dir_path(System.file_name(strDir), pstraRelative->element_at(i));
//
//            }
//
//         }
//
//      }
//
//      string strDir(lpcsz);
//
//      if(!::str::ends(strDir, "/"))
//      {
//
//         strDir += "/";
//
//      }
//
//      stringa stra;
//
//      ::dir::ls(stra, lpcsz);
//
//      for(int32_t i = 0; i < stra.get_count(); i++)
//      {
//
//         string strPath = stra[i];
//
//         string strName = strPath;
//
//         if(!::str::begins_eat(strName, strDir))
//            continue;
//
//         if(!matches_wildcard_criteria(pszPattern, strName))
//            continue;
//
//         if(pstraPath != NULL)
//         {
//
//            pstraPath->add(strPath);
//
//         }
//
//         if(pstraTitle != NULL)
//         {
//
//            pstraTitle->add(strName);
//
//         }
//
//         bool bIsDir = false;
//
//         if(pbaIsDir != NULL || piaSize != NULL)
//         {
//
//            bIsDir = ::dir::is(strPath);
//
//         }
//
//         if(pbaIsDir != NULL)
//         {
//
//            pbaIsDir->add(bIsDir);
//
//         }
//
//         if(piaSize != NULL)
//         {
//
//            if(bIsDir)
//            {
//
//               piaSize->add(0);
//
//            }
//            else
//            {
//
//               piaSize->add(file_length_dup(strPath));
//
//            }
//
//         }
//
//      }
//
//   }
//
//   void dir::rls_dir(sp(::aura::application)  papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative)
//   {
//
//      stringa stra;
//
//      ::dir::ls(stra, lpcsz);
//
//      string strDir(lpcsz);
//
//      if(!::str::ends(strDir, "/"))
//      {
//
//         strDir += "/";
//
//      }
//
//
//      for(int32_t i = 0; i < stra.get_count(); i++)
//      {
//
//         string strPath = stra[i];
//
//         string strName = strPath;
//
//         if(!::str::begins_eat(strName, strDir))
//            continue;
//
//         if(!System.dir().is(strPath, papp))
//            continue;
//
//         if(pstraPath != NULL)
//         {
//
//            pstraPath->add((const char *) stra[i]);
//
//         }
//
//         if(pstraTitle != NULL)
//         {
//
//            pstraTitle->add(strName);
//
//         }
//
//         if(pstraRelative != NULL)
//         {
//
//            pstraRelative->add(strName);
//
//         }
//
//         index iStart = 0;
//
//         if(pstraRelative != NULL)
//         {
//
//            iStart = pstraRelative->get_size();
//
//         }
//
//         rls_dir(papp, strPath, pstraPath, pstraTitle, pstraRelative);
//
//         if(pstraRelative != NULL)
//         {
//
//            for(index i = iStart; i < pstraRelative->get_size(); i++)
//            {
//
//               pstraRelative->element_at(i) = ::dir_path(strPath, pstraRelative->element_at(i));
//
//            }
//
//         }
//
//      }
//
//   }
//
//
//   void dir::ls_dir(sp(::aura::application)  papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
//   {
//
//      stringa stra;
//
//      ::dir::ls(stra, lpcsz);
//
//      string strDir(lpcsz);
//
//      if(!::str::ends(strDir, "/"))
//      {
//
//         strDir += "/";
//
//      }
//
//
//      for(int32_t i = 0; i < stra.get_count(); i++)
//      {
//
//         string strPath = stra[i];
//
//         string strName = strPath;
//
//         if(!::str::begins_eat(strName, strDir))
//            continue;
//
//         if(!System.dir().is(strPath, papp))
//            continue;
//
//         if(pstraPath != NULL)
//         {
//
//            pstraPath->add((const char *) stra[i]);
//
//         }
//
//         if(pstraTitle != NULL)
//         {
//
//            pstraTitle->add(strName);
//
//         }
//
//      }
//
//   }
//
//   void dir::ls_file(sp(::aura::application)  papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
//   {
//
//      stringa stra;
//
//      ::dir::ls(stra, lpcsz);
//
//      string strDir(lpcsz);
//
//      if(!::str::ends(strDir, "/"))
//      {
//
//         strDir += "/";
//
//      }
//
//
//      for(int32_t i = 0; i < stra.get_count(); i++)
//      {
//
//         string strPath = stra[i];
//
//         string strName = strPath;
//
//         if(!::str::begins_eat(strName, strDir))
//            continue;
//
//         if(System.dir().is(strPath, papp))
//            continue;
//
//
//         if(pstraPath != NULL)
//         {
//
//            pstraPath->add((const char *) stra[i]);
//
//         }
//
//         if(pstraTitle != NULL)
//         {
//
//            pstraTitle->add(strName);
//
//         }
//
//      }
//
//   }
//
//   void dir::ls(sp(::aura::application)  papp, const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, bool_array * pbaIsDir, int64_array * piaSize)
//   {
//
//      stringa stra;
//
//      ::dir::ls(stra, lpcsz);
//
//      string strDir(lpcsz);
//
//      if(!::str::ends(strDir, "/"))
//      {
//
//         strDir += "/";
//
//      }
//
//
//      for(int32_t i = 0; i < stra.get_count(); i++)
//      {
//
//         string strPath = stra[i];
//
//         string strName = strPath;
//
//         if(!::str::begins_eat(strName, strDir))
//            continue;
//
//         if(pstraPath != NULL)
//         {
//
//            pstraPath->add((const char *) stra[i]);
//
//         }
//
//         if(pstraTitle != NULL)
//         {
//
//            pstraTitle->add(strName);
//
//         }
//
//
//         bool bIsDir = false;
//
//         if(pbaIsDir != NULL || piaSize != NULL)
//         {
//
//            bIsDir = System.dir().is(strPath, papp);
//
//         }
//
//         if(pbaIsDir != NULL)
//         {
//
//            pbaIsDir->add(bIsDir);
//
//         }
//
//         if(piaSize != NULL)
//         {
//
//            if(bIsDir)
//            {
//
//               piaSize->add(0);
//
//            }
//            else
//            {
//
//               piaSize->add(file_length_dup(strPath));
//
//            }
//
//         }
//
//
//      }
//
//   }
//
//   bool dir::is(const char * lpcszPath, sp(::aura::application)  papp)
//   {
//
//      bool bIsDir;
//
//      DWORD dwLastError;
//
//      if(m_isdirmap.lookup(lpcszPath, bIsDir, dwLastError))
//      {
//
//         if(!bIsDir)
//         {
//
//            set_last_error(dwLastError);
//
//         }
//
//         return bIsDir;
//
//      }
//
//
//      if(::file::dir::system::is(lpcszPath, papp))
//         return true;
//
//
//      string strPath(lpcszPath);
//      if(strPath.get_length() >= MAX_PATH)
//      {
//         if(::str::begins(strPath, "\\\\"))
//         {
//            strPath = "\\\\?\\UNC" + strPath.Mid(1);
//         }
//         else
//         {
//            strPath = "\\\\?\\" + strPath;
//         }
//      }
//
//      bIsDir = ::dir::is(strPath);
//
//      m_isdirmap.set(lpcszPath, bIsDir, bIsDir ? 0 : ::get_last_error());
//
//      return bIsDir;
//   }
//
//   bool dir::is(const string & strPath, sp(::aura::application)  papp)
//   {
//
//      if(::file::dir::system::is(strPath, papp))
//         return true;
//
//      bool bIsDir;
//
//      DWORD dwLastError;
//
//      if(m_isdirmap.lookup(strPath, bIsDir, dwLastError))
//      {
//
//         if(!bIsDir)
//         {
//
//            set_last_error(dwLastError);
//
//         }
//
//         return bIsDir;
//
//      }
//
//
//      wstring wstrPath;
//
//      //strsize iLen = ::str::international::utf8_to_unicode_count(strPath);
//      //wstrPath.alloc(iLen + 32);
//      wstrPath = ::str::international::utf8_to_unicode(strPath);
//      if(wstrPath.get_length() >= MAX_PATH)
//      {
//         if(::str::begins(wstrPath, L"\\\\"))
//         {
//            ::str::begin(wstrPath, L"\\\\?\\UNC");
//         }
//         else
//         {
//            ::str::begin(wstrPath, L"\\\\?\\");
//         }
//      }
//
//      bIsDir = ::dir::is(::str::international::unicode_to_utf8(wstrPath));
//
//      m_isdirmap.set(strPath, bIsDir, bIsDir ? 0 : ::get_last_error());
//
//      return bIsDir;
//   }
//
//   bool dir::name_is(const string & str, sp(::aura::application)  papp)
//   {
//      //output_debug_string(str);
//      strsize iLast = str.get_length() - 1;
//      while(iLast >= 0)
//      {
//         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
//            break;
//         iLast--;
//      }
//      while(iLast >= 0)
//      {
//         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
//            break;
//         iLast--;
//      }
//      if(iLast >= 0)
//      {
//         while(iLast >= 0)
//         {
//            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
//            {
//               iLast++;
//               break;
//            }
//            iLast--;
//         }
//      }
//      else
//      {
//         return true; // assume empty string is root_ones directory
//      }
//
//
//      bool bIsDir;
//
//      DWORD dwLastError;
//
//      if(m_isdirmap.lookup(str, bIsDir, dwLastError, (int32_t) iLast))
//         return bIsDir;
//
//
//      if(papp->m_bZipIsDir && iLast >= 3  && !strnicmp_dup(&((const char *) str)[iLast - 3], ".zip", 4))
//      {
//         m_isdirmap.set(str.Left(iLast + 1), true, 0);
//         return true;
//      }
//
//      strsize iFind = ::str::find_ci(".zip:", str);
//
//      if(papp->m_bZipIsDir && iFind >= 0 && iFind < iLast)
//      {
//         bool bHasSubFolder;
//         if(m_isdirmap.lookup(str, bHasSubFolder, dwLastError))
//            return bHasSubFolder;
//         bHasSubFolder = m_pziputil->HasSubFolder(papp, str);
//         m_isdirmap.set(str.Left(iLast + 1), bHasSubFolder, bHasSubFolder ? 0 : ::get_last_error());
//         return bHasSubFolder;
//      }
//
//
//      wstring wstrPath;
//
//      //strsize iLen = ::str::international::utf8_to_unicode_count(str, iLast + 1);
//
//      //wstrPath.alloc(iLen + 32);
//
//      wstrPath = ::str::international::utf8_to_unicode(str, iLast + 1);
//
//      //output_debug_string(wstrPath);
//
//      if(wstrPath.get_length() >= MAX_PATH)
//      {
//         if(::str::begins(wstrPath, L"\\\\"))
//         {
//            ::str::begin(wstrPath, L"\\\\?\\UNC");
//         }
//         else
//         {
//            ::str::begin(wstrPath, L"\\\\?\\");
//         }
//      }
//
//      bIsDir = ::dir::is(::str::international::unicode_to_utf8(wstrPath));
//
//      m_isdirmap.set(str.Left(iLast + 1), bIsDir, bIsDir ? 0 : ::get_last_error());
//
//      return bIsDir;
//   }
//
//   string dir::votagus(const char * lpcsz, const char * lpcsz2)
//   {
//      string strVotagusFolder = System.get_ca2_module_folder();
//      System.file().path().eat_end_level(strVotagusFolder, 2, "\\");
//      return dir::path(strVotagusFolder, lpcsz, lpcsz2);
//   }
//
//   string dir::time(const char * lpcsz, const char * lpcsz2)
//   {
//      return dir::path(m_strTimeFolder, lpcsz, lpcsz2);
//   }
//
//   string dir::stage(const char * lpcsz, const char * lpcsz2)
//   {
//      return dir::path(element("stage"), lpcsz, lpcsz2);
//   }
//
//   string dir::stageapp(const char * lpcsz, const char * lpcsz2)
//   {
//      return dir::path(stage("basis", lpcsz), lpcsz2);
//   }
//
//   string dir::netseed(const char * lpcsz, const char * lpcsz2)
//   {
//      return dir::path(m_strNetSeedFolder, lpcsz, lpcsz2);
//   }
//
//   // stage in ccvotagus spalib
//   string dir::install(const char * lpcsz, const char * lpcsz2)
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return dir::path(m_strCa2, lpcsz, lpcsz2);
//
//   }
//
//   string dir::install(const string & str, const char * lpcsz2)
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return dir::path(m_strCa2, str, lpcsz2);
//
//   }
//
//   string dir::install(const char * lpcsz, const string & str2)
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return dir::path(m_strCa2, lpcsz, str2);
//
//   }
//
//   string dir::install(const string & str, const string & str2)
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return dir::path(m_strCa2, str, str2);
//
//   }
//
//   string dir::install(const string & str)
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return dir::path(m_strCa2, str);
//
//   }
//
//   string dir::install()
//   {
//
//      single_lock sl(&m_mutex, true);
//
//      return m_strCa2;
//
//   }
//
//
//   string dir::module(const char * lpcsz, const char * lpcsz2)
//   {
//      string str = System.get_module_folder();
//      return dir::path(str, lpcsz, lpcsz2);
//   }
//
//   string dir::ca2module(const char * lpcsz, const char * lpcsz2)
//   {
//      string str = System.get_ca2_module_folder();
//      return dir::path(str, lpcsz, lpcsz2);
//   }
//
//
//   void dir::time_square(string &str)
//   {
//      str = time("time");
//   }
//
//   string dir::time_log(const char * pszId)
//   {
//      string strLogBaseDir;
//      strLogBaseDir = appdata("log");
//      return path(strLogBaseDir, pszId);
//   }
//
//   bool dir::mk(const char * lpcsz, sp(::aura::application)  papp)
//   {
//
//      if(is(lpcsz, papp))
//         return true;
//
//      stringa stra;
//      System.file().get_ascendants_path(lpcsz, stra);
//      for(int32_t i = 0; i < stra.get_size(); i++)
//      {
//         if(!is(stra[i], papp))
//         {
//
//            if(!::dir::mk(stra[i]))
//            {
//               DWORD dwError = ::get_last_error();
//               if(dwError == ERROR_ALREADY_EXISTS)
//               {
//                  string str;
//                  str = "\\\\?\\" + stra[i];
//                  str.trim_right("\\/");
//                  try
//                  {
//                     System.file_del(str);
//                  }
//                  catch(...)
//                  {
//                  }
//                  str = stra[i];
//                  str.trim_right("\\/");
//                  try
//                  {
//                     System.file_del(str);
//                  }
//                  catch(...)
//                  {
//                  }
//                  //if(::CreateDirectory(::str::international::utf8_to_unicode("\\\\?\\" + stra[i]), NULL))
//                  if(::dir::mk("\\\\?\\" + stra[i]))
//                  {
//                     m_isdirmap.set(stra[i], true, 0);
//                     goto try1;
//                  }
//                  else
//                  {
//                     dwError = ::get_last_error();
//                  }
//               }
//               char * pszError;
//               FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPTSTR) &pszError, 8, NULL);
//
//               //TRACE("dir::mk CreateDirectoryW last error(%d)=%s", dwError, pszError);
//               // xxx               ::LocalFree(pszError);
//               //m_isdirmap.set(stra[i], false);
//            }
//            else
//            {
//               m_isdirmap.set(stra[i], true, 0);
//            }
//         try1:
//
//            if(!is(stra[i], papp))
//            {
//               return false;
//            }
//
//         }
//      }
//      return true;
//   }
//
//   bool dir::rm(sp(::aura::application)  papp, const char * psz, bool bRecursive)
//   {
//      if(bRecursive)
//      {
//         stringa straPath;
//         stringa straTitle;
//         ls(papp, psz, &straPath, &straTitle);
//         for(int32_t i = 0; i < straPath.get_count(); i++)
//         {
//            if(is(straPath[i], papp))
//            {
//               rm(papp, path(psz, straTitle[i]), true);
//            }
//            else
//            {
//               ::unlink(straPath[i]);
//            }
//         }
//      }
//      return ::rmdir(psz) != FALSE;
//   }
//
//
//   string dir::name(const char * path1)
//   {
//      const char * psz = path1 + strlen(path1) - 1;
//      while(psz >= path1)
//      {
//         if(*psz != '\\' && *psz != '/' && *psz != ':')
//            break;
//         psz--;
//      }
//      while(psz >= path1)
//      {
//         if(*psz == '\\' || *psz == '/' || *psz == ':')
//            break;
//         psz--;
//      }
//      if(psz >= path1) // strChar == "\\" || strChar == "/"
//      {
//         const char * pszEnd = psz;
//         /*while(psz >= path1)
//          {
//          if(*psz != '\\' && *psz != '/' && *psz != ':')
//          break;
//          psz--;
//          }*/
//         return string(path1, pszEnd - path1 + 1);
//      }
//      else
//      {
//         return "";
//      }
//   }
//
//   string dir::name(const string & str)
//   {
//
//      strsize iLast = str.get_length() - 1;
//
//      while(iLast >= 0)
//      {
//         if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
//            break;
//         iLast--;
//      }
//      while(iLast >= 0)
//      {
//         if(str.m_pszData[iLast] == '\\' || str.m_pszData[iLast] == '/' || str.m_pszData[iLast] == ':')
//            break;
//         iLast--;
//      }
//      if(iLast >= 0)
//      {
//         while(iLast >= 0)
//         {
//            if(str.m_pszData[iLast] != '\\' && str.m_pszData[iLast] != '/' && str.m_pszData[iLast] != ':')
//               break;
//            iLast--;
//         }
//         return str.Left(iLast + 1);
//      }
//      else
//      {
//         return "";
//      }
//   }
//
//
//   class ::file::path & dir::path()
//   {
//      return m_path;
//   }
//
//
//   bool dir::initialize()
//   {
//
//      xml::document doc(get_app());
//
//      string strPath = appdata("configuration\\directory.xml");
//
//      string strDocument = Application.file_as_string(strPath);
//
//      if(doc.load(strDocument))
//      {
//
//         if(doc.get_root()->get_name() == "directory_configuration")
//         {
//
//            m_strTimeFolder = doc.get_root()->get_child_value("time");
//
//            m_strNetSeedFolder = doc.get_root()->get_child_value("netseed");
//
//         }
//
//      }
//
//#if defined(LINUX)
//
//      m_strTimeFolder = "/var/tmp/ca2/time";
//
//#else
//
//      if(m_strTimeFolder.is_empty())
//         m_strTimeFolder = appdata("time");
//
//#endif
//
//      if(m_strNetSeedFolder.is_empty())
//         m_strNetSeedFolder = element("net");
//
//      mk(m_strTimeFolder, get_app());
//
//      if(!is(m_strTimeFolder, get_app()))
//         return false;
//
//      string strTime = path(m_strTimeFolder, "time");
//
//      mk(strTime, get_app());
//
//      if(!is(strTime, get_app()))
//         return false;
//
//      return true;
//
//   }
//
//   string dir::trash_that_is_not_trash(const char * psz)
//   {
//      if(psz == NULL)
//         return "";
//
//      if(psz[1] == ':')
//      {
//         string strDir = name(psz);
//         string str;
//         str = strDir.Left(2);
//         str += "\\trash_that_is_not_trash\\";
//         string strFormat;
//         ::datetime::time time;
//         time = ::datetime::time::get_current_time();
//         strFormat.Format("%04d-%02d-%02d %02d-%02d-%02d\\", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
//         str += strFormat;
//         if(strDir.m_pszData[2] == '\\')
//         {
//            str += strDir.Mid(3);
//         }
//         else
//         {
//            str += strDir.Mid(2);
//         }
//         return str;
//      }
//
//      return "";
//   }
//
//   string dir::appdata(const char * lpcsz, const char * lpcsz2)
//   {
//      string str;
//      /*SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_COMMON_APPDATA,
//       FALSE);*/
//
//      str = path(getenv("HOME"), ".ca2/appdata");
//      string strRelative;
//      strRelative = install();
//      //index iFind = strRelative.find(':');
//      //if(iFind >= 0)
//      {
//         // strsize iFind1 = strRelative.reverse_find("\\", iFind);
//         //strsize iFind2 = strRelative.reverse_find("/", iFind);
//         //strsize iStart = max(iFind1 + 1, iFind2 + 1);
//
//         //strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
//      }
//      return path(path(str, "ca2", strRelative), lpcsz, lpcsz2);
//   }
//
//   string dir::usersystemappdata(sp(::aura::application)  papp, const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      return path(appdata(lpcszPrefix), lpcsz, lpcsz2);
//   }
//
//   string dir::appdata(sp(::aura::application)  papp, const char * lpcsz, const char * lpcsz2)
//   {
//      return path(userfolder(papp, "appdata"), lpcsz, lpcsz2);
//   }
//
//   string dir::userdata(sp(::aura::application)  papp, const char * lpcsz, const char * lpcsz2)
//   {
//      return path(userfolder(papp, "data"), lpcsz, lpcsz2);
//   }
//
//   string dir::userfolder(sp(::aura::application)  papp, const char * lpcsz, const char * lpcsz2)
//   {
//
//      string str;
//      /*SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_PROFILE,
//       FALSE);*/
//
//      str = getenv("HOME");
//
//      string strRelative;
//      strRelative = install();
//      index iFind = strRelative.find(':');
//      if(iFind >= 0)
//      {
//         strsize iFind1 = strRelative.reverse_find("\\", iFind);
//         strsize iFind2 = strRelative.reverse_find("/", iFind);
//         strsize iStart = max(iFind1 + 1, iFind2 + 1);
//         strRelative = strRelative.Left(iFind - 1) + "_" + strRelative.Mid(iStart, iFind - iStart) + strRelative.Mid(iFind + 1);
//      }
//
//      string strUserFolderShift;
//
//      if(App(papp).handler()->m_varTopicQuery.has_property("user_folder_relative_path"))
//      {
//         strUserFolderShift = path(strRelative, App(papp).handler()->m_varTopicQuery["user_folder_relative_path"].get_string());
//      }
//      else
//      {
//         strUserFolderShift = strRelative;
//      }
//
//      return path(path(str, "ca2", strUserFolderShift), lpcsz, lpcsz2);
//
//      //      return path(path(str, "ca2"), lpcsz);
//      /*      if(&AppUser(papp) == NULL)
//       {
//       string str;
//       SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_PROFILE,
//       FALSE);
//       return path(path(str, "ca2\\_____default"), lpcsz);
//       }
//       else
//       {
//       return path(AppUser(papp).m_strPath, lpcsz, lpcsz2);
//       }*/
//   }
//
//   string dir::default_os_user_path_prefix(sp(::aura::application)  papp)
//   {
//      /*UNREFERENCED_PARAMETER(papp);
//       unichar buf[MAX_PATH];
//       ULONG ulSize = sizeof(buf) / sizeof(unichar);
//       if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
//       {
//       if(!::GetUserNameW(buf, &ulSize))
//       {
//       memset(buf, 0, sizeof(buf));
//       }
//       }*/
//      /*return ::str::international::unicode_to_utf8(buf);*/
//      return ::getlogin();
//   }
//
//   string dir::default_userappdata(sp(::aura::application)  papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
//   {
//      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "appdata"), pszRelativePath);
//   }
//
//   string dir::default_userdata(sp(::aura::application)  papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
//   {
//      return path(default_userfolder(papp, lpcszPrefix, lpcszLogin, "data"), pszRelativePath);
//   }
//
//   string dir::default_userfolder(sp(::aura::application)  papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
//   {
//
//      return userfolder(papp, pszRelativePath);
//
//      /*      UNREFERENCED_PARAMETER(papp);
//       string str;
//       SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_APPDATA,
//       FALSE);
//       return path(path(str, "ca2\\user", lpcszPrefix), lpcszLogin, pszRelativePath);*/
//   }
//
//   string dir::userquicklaunch(sp(::aura::application)  papp, const char * lpcszRelativePath, const char * lpcsz2)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      string str;
//      /*SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_APPDATA,
//       FALSE);*/
//      str = path(getenv("HOME"), "Microsoft\\Internet Explorer\\Quick Launch");
//      return path(str, lpcszRelativePath, lpcsz2);
//   }
//
//   string dir::userprograms(sp(::aura::application)  papp, const char * lpcszRelativePath, const char * lpcsz2)
//   {
//      UNREFERENCED_PARAMETER(papp);
//      string str;
//      /*      SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_PROGRAMS,
//       FALSE);*/
//
//      str = "/usr/bin";
//      return path(str, lpcszRelativePath, lpcsz2);
//   }
//
//   string dir::commonprograms(const char * lpcszRelativePath, const char * lpcsz2)
//   {
//      string str;
//      /*      SHGetSpecialFolderPath(
//       NULL,
//       str,
//       CSIDL_COMMON_PROGRAMS,
//       FALSE);*/
//      str = "/usr/share/";
//      return path(str, lpcszRelativePath, lpcsz2);
//   }
//
//   bool dir::is_inside_time(const char * pszPath, sp(::aura::application)  papp)
//   {
//      return is_inside(time(), pszPath, papp);
//   }
//
//   bool dir::is_inside(const char * pszDir, const char * pszPath, sp(::aura::application)  papp)
//   {
//      return ::str::begins_ci(pszDir, pszPath);
//   }
//
//   bool dir::has_subdir(sp(::aura::application)  papp, const char * pszDir)
//   {
//      stringa stra;
//      ls_dir(papp, pszDir, &stra);
//      return stra.get_size() > 0;
//
//   }
//
//} // namespace ios
