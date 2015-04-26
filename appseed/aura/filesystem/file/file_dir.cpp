#if defined(METROWIN)
#pragma push_macro("System")
#undef System
using namespace ::Windows::System;

#pragma pop_macro("System")
#endif



string dir::get_ca2_module_folder()
{
#if defined(METROWIN)

   wchar_t lpszModuleFolder[MAX_PATH * 8];


   return "";

/*   char lpszModuleFilePath[MAX_PATH * 8];

   HMODULE hmodule = ::LoadPackagedLibrary(L"ca.dll", 0);

   if(hmodule == NULL)
      hmodule = ::LoadPackagedLibrary(L"spalib.dll", 0);

   if(hmodule == NULL)
   {

      string buf;

      throw metrowin_todo();
      //HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_NO_ALIAS, NULL, wtostring(buf, 4096));
      //if(FAILED(hr))
      // throw "dir::get_ca2_module_folder_dup : SHGetKnownFolderPath failed";

      /*strcpy(lpszModuleFilePath, buf.c_str());

      if(lpszModuleFilePath[strlen_dup(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[strlen_dup(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[strlen_dup(lpszModuleFilePath) - 1] = '\0';
      }
      strcat_dup(lpszModuleFilePath, "\\core\\");
#ifdef X86
      strcat_dup(lpszModuleFilePath, "stage\\x86\\");
#else
      strcat_dup(lpszModuleFilePath, "stage\\x64\\");
#endif

      wcscpy_dup(lpszModuleFolder, lpszModuleFilePath);*/

  /*    return true;

   }

   throw metrowin_todo();
   //GetModuleFileName(hmodule, lpszModuleFilePath, sizeof(lpszModuleFilePath));

   // xxx   LPTSTR lpszModuleFileName;

   throw metrowin_todo();
   //GetFullPathName(lpszModuleFilePath, sizeof(lpszModuleFilePath), lpszModuleFolder, &lpszModuleFileName);

   throw metrowin_todo();
   //lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   throw metrowin_todo();
   /*
   if(strlen_dup(lpszModuleFolder) > 0)
   {

   if(lpszModuleFolder[strlen_dup(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[strlen_dup(lpszModuleFolder) - 1] == '/')
   {

   lpszModuleFolder[strlen_dup(lpszModuleFolder) - 1] = '\0';

   }

   }
   */

   return "";

#elif defined(WINDOWS)


   hwstring lpszModuleFolder(MAX_PATH * 8);

   hwstring lpszModuleFilePath(MAX_PATH * 8);

   HMODULE hmodule = ::GetModuleHandleA("core.dll");

   if(hmodule == NULL)
      hmodule = ::GetModuleHandleA("spalib.dll");

   if(hmodule == NULL)
   {

      SHGetSpecialFolderPathW(
         NULL,
         lpszModuleFilePath,
         CSIDL_PROGRAM_FILES,
         FALSE);
      if(lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] = '\0';
      }
      wcscat(lpszModuleFilePath, L"\\ca2\\");
#ifdef X86
      wcscat(lpszModuleFilePath,L"stage\\x86\\");
#else
      wcscat_dup(lpszModuleFilePath, L"stage\\x64\\");
#endif

      wcscpy(lpszModuleFolder, lpszModuleFilePath);

      return lpszModuleFolder;

   }

   if(!GetModuleFileNameW(hmodule,lpszModuleFilePath, lpszModuleFilePath.count()))
      return "";

   LPWSTR lpszModuleFileName;

   if (!GetFullPathNameW(lpszModuleFilePath, lpszModuleFilePath.count(), lpszModuleFolder, &lpszModuleFileName))
      return "";

   lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   if(wcslen(lpszModuleFolder) > 0)
   {

      if(lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '/')
      {

         lpszModuleFolder[wcslen(lpszModuleFolder) - 1] = '\0';

      }

   }


#else

   wchar_t lpszModuleFolder[MAX_PATH * 8];

   wcscpy_dup(lpszModuleFolder, L"/ca2/");

#endif

   return lpszModuleFolder;

}



















string dir::get_base_module_folder()
{
#if defined(METROWIN)

   return "";
/*   wchar_t lpszModuleFolder[MAX_PATH * 8];

   char lpszModuleFilePath[MAX_PATH * 8];

   HMODULE hmodule = ::LoadPackagedLibrary(L"ca.dll", 0);

   if (hmodule == NULL)
      hmodule = ::LoadPackagedLibrary(L"spalib.dll", 0);

   if (hmodule == NULL)
   {

      string buf;

      throw metrowin_todo();
      //HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_NO_ALIAS, NULL, wtostring(buf, 4096));
      //if(FAILED(hr))
      // throw "dir::get_ca2_module_folder_dup : SHGetKnownFolderPath failed";

      strcpy(lpszModuleFilePath, buf.c_str());

      if (lpszModuleFilePath[strlen_dup(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[strlen_dup(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[strlen_dup(lpszModuleFilePath) - 1] = '\0';
      }
      strcat_dup(lpszModuleFilePath, "\\ca2\\");
#ifdef X86
      strcat_dup(lpszModuleFilePath, "stage\\x86\\");
#else
      strcat_dup(lpszModuleFilePath, "stage\\x64\\");
#endif

      strcpy_dup(lpszModuleFolder, lpszModuleFilePath);

      return lpszModuleFolder;

   }

   throw metrowin_todo();
   //GetModuleFileName(hmodule, lpszModuleFilePath, sizeof(lpszModuleFilePath));

   // xxx   LPTSTR lpszModuleFileName;

   throw metrowin_todo();
   //GetFullPathName(lpszModuleFilePath, sizeof(lpszModuleFilePath), lpszModuleFolder, &lpszModuleFileName);

   throw metrowin_todo();
   //lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   throw metrowin_todo();
   /*
   if(strlen_dup(lpszModuleFolder) > 0)
   {

   if(lpszModuleFolder[strlen_dup(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[strlen_dup(lpszModuleFolder) - 1] == '/')
   {

   lpszModuleFolder[strlen_dup(lpszModuleFolder) - 1] = '\0';

   }

   }
   */

//   return true;

#elif defined(WINDOWS)

   hwstring lpszModuleFolder(MAX_PATH * 8);

   hwstring lpszModuleFilePath(MAX_PATH * 8);

   HMODULE hmodule = ::GetModuleHandleA("aura.dll");

   if (hmodule == NULL)
      hmodule = ::GetModuleHandleA("spalib.dll");

   if (hmodule == NULL)
   {

      SHGetSpecialFolderPathW(
         NULL,
         lpszModuleFilePath,
         CSIDL_PROGRAM_FILES,
         FALSE);
      if(lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] = '\0';
      }
      wcscat(lpszModuleFilePath, L"\\ca2\\");
#ifdef X86
      wcscat(lpszModuleFilePath,L"stage\\x86\\");
#else
      wcscat_dup(lpszModuleFilePath,L"stage\\x64\\");
#endif

      wcscpy(lpszModuleFolder, lpszModuleFilePath);

      return lpszModuleFolder;

   }

   if(!GetModuleFileNameW(hmodule,lpszModuleFilePath, lpszModuleFilePath.count()))
      return "";

   LPWSTR lpszModuleFileName;

   if (!GetFullPathNameW(lpszModuleFilePath, lpszModuleFilePath.count(), lpszModuleFolder, &lpszModuleFileName))
      return "";

   lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   if (wcslen_dup(lpszModuleFolder) > 0)
   {

      if(lpszModuleFolder[wcslen_dup(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[wcslen_dup(lpszModuleFolder) - 1] == '/')
      {

         lpszModuleFolder[wcslen_dup(lpszModuleFolder) - 1] = '\0';

      }

   }


   return lpszModuleFolder;


#else

   wchar_t lpszModuleFolder[MAX_PATH * 8];
wcscpy_dup(lpszModuleFolder, L"/core/");


   return lpszModuleFolder;

#endif


}


























bool eat_end_level_dup(string & str, int32_t iLevelCount, const char * lpSeparator)
{

   strsize iLast = str.length() - 1;

   if(iLast < 0)
      return iLevelCount <= 0;

   while(str[iLast] == '/' || str[iLast] == '\\')
      iLast--;

   for(int32_t i = 0; i < iLevelCount; i++)
   {

      strsize iFind1 = str.reverse_find('/', iLast);

      strsize iFind2 = str.reverse_find('\\', iLast);

      strsize iFind = MAX(iFind1, iFind2);

      if(iFind >= iLast)
         return false;

      if(iFind < 0)
         return false;

      iLast = iFind;

      while(str[iLast] == '/' || str[iLast] == '\\')
         iLast--;

   }

   str = str.substr(0, iLast + 1);

   return true;

}



string ca2_module_folder_dup()
{

   static string * s_pstrCalc = NULL;

   if(s_pstrCalc != NULL)
   {

      return *s_pstrCalc;

   }

   string str;

#ifdef WINDOWSEX

   wchar_t lpszModuleFilePath[MAX_PATH + 1];
   GetModuleFileNameW(::GetModuleHandleA("ca.dll"), lpszModuleFilePath, MAX_PATH + 1);
   wchar_t lpszModuleFolder[MAX_PATH + 1];
   LPWSTR lpszModuleFileName;
   GetFullPathNameW(lpszModuleFilePath, MAX_PATH + 1, lpszModuleFolder, &lpszModuleFileName);
   str = string(lpszModuleFolder, lpszModuleFileName - lpszModuleFolder);

#elif defined(LINUX)

   void * handle = dlopen("libcore.so", RTLD_NOW);

   if(handle == NULL)
      return "";

   link_map * plm;

   dlinfo(handle, RTLD_DI_LINKMAP, &plm);

   string strCa2ModuleFolder = ::dir::name(plm->l_name);

   dlclose(handle);

   str = strCa2ModuleFolder;

#elif defined(METROWIN)

   str = "";

#elif defined(APPLEOS)

   {

      char * pszCurDir = getcwd(NULL, 0);

      string strCurDir = pszCurDir;

      free(pszCurDir);

/*      if(file_exists_dup(::dir::path(strCurDir, "libbase.dylib")))
      {
         return strCurDir;
      }
*/

      str = ::dir::name(::dir::pathfind(getenv("DYLD_LIBRARY_PATH"), "libbase.dylib", "rfs")); // readable - normal file - non zero sized
      
      if(str.has_char())
      {
         
         goto found;
         
      }

      str = ::dir::name(::dir::pathfind(getenv("DYLD_FALLBACK_LIBRARY_PATH"), "libbase.dylib", "rfs")); // readable - normal file - non zero sized

   found:;
   }

#endif

   s_pstrCalc = new string(str);

   return *s_pstrCalc;


}


string dir::element(const char * path1, const char * path2, const char * path3, const char * path4)
{ 
   
   return path(element(),path1,path2,path3,path4); 

}


string dir::element()
{

#ifdef WINDOWS

   string str;


   str = get_ca2_module_folder();
   stringa stra;

   str.replace("/", "\\");
   stra.add_tokens(str, "\\");
   if(stra.get_count() <= 0)
      return "";
   if(stra[stra.get_count() - 1].is_empty())
   {
      stra.remove_at(stra.get_count() - 1);
   }
   if(stra.get_count() <= 0)
      return "";
   stra.remove_at(stra.get_count() - 1);
   if(stra.get_count() <= 0)
      return "";
   stra.remove_at(stra.get_count() - 1);
   if(stra.get_count() <= 0)
      return "";
   str = "";
   for(int32_t i = 0; i < stra.get_count(); i++)
   {
      str += stra[i];
      str += "\\";
   }

   return str;

#else

   string strRelative = ca2_module_folder_dup();

   eat_end_level_dup(strRelative, 2, "/");

   string str = path(getenv("HOME"), ".core/appdata");

   return str;

#endif

}

bool dir::mk(const char * lpcsz)
{

#ifdef WINDOWS

   if(is(lpcsz))
      return true;

   string url(lpcsz);
   string tmp;
   string dir;
   index oldpos = -1;
   index pos = url.find("\\");
   string unc("\\\\?\\");
   while (pos >= 0)
   {
      tmp = url.substr(oldpos + 1, pos - oldpos -1 );
      dir += tmp + "\\";
      wstring wstr(unc + dir);
      uint32_t dw = ::GetFileAttributesW(wstr);
      if(dw == INVALID_FILE_ATTRIBUTES)
      {
         ::CreateDirectoryW(wstr, NULL);
      }
      oldpos = pos;
      pos = url.find("\\", oldpos + 1);

   }
   tmp = url.substr(oldpos + 1);
   dir += tmp + "\\";
   wstring wstr(unc + dir);
   if(::GetFileAttributesW(wstr) == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectoryW(wstr, NULL);
   }
   return true;

#else

   // stat -> Sir And Arthur - Serenato
   struct stat st;

   string url(lpcsz);
   string tmp;
   string dir;
   ::index oldpos = -1;
   ::index pos = url.find("/");
   while (pos >= 0)
   {
      tmp = url.substr(oldpos + 1, pos - oldpos -1 );
      dir += tmp + "/";
      if(stat(dir, &st))
      {
         mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO);
      }
      oldpos = pos;
      pos = url.find("/", oldpos + 1);

   }
   tmp = url.substr(oldpos + 1);
   dir += tmp + "/";
   if(stat(dir, &st))
   {
      mkdir(dir, S_IRWXU | S_IRWXG | S_IRWXO);
   }
   return true;

#endif

}

string dir::module_folder(const char * path1)
{

#ifdef WINDOWSEX

   wchar_t path[MAX_PATH * 4];
   if(!GetModuleFileNameW(NULL,path,sizeof(path) / sizeof(wchar_t)))
   {
      return path1;
   }
   else
   {
      return dir::path(name(string(path)), path1);
   }

#elif defined(METROWIN)

   throw metrowin_todo();
   return path1;

#else

   return dir::path("/core/stage", path1);

#endif

}


CLASS_DECL_AURA bool dir::eat_end_level(string & str, int iLevelCount, const char * lpSeparator)
{

strsize iLast = str.length() - 1;

if(iLast < 0)
   return iLevelCount <= 0;

while(str[iLast] == '/' || str[iLast] == '\\')
iLast--;

for(int32_t i = 0; i < iLevelCount; i++)
{
   
   strsize iFind1 = str.reverse_find('/', iLast);
   
   strsize iFind2 = str.reverse_find('\\', iLast);
   
   strsize iFind = MAX(iFind1, iFind2);
   
   if(iFind >= iLast)
      return false;
   
   if(iFind < 0)
      return false;
   
   iLast = iFind;
   
   while(str[iLast] == '/' || str[iLast] == '\\')
      iLast--;
   
}

str.Truncate(iLast + 1);

return true;

}

string dir::path(const char * path1, const char * path2, const char * path3, const char * path4, const char * path5)
{
   string str(path1);
   if(str.substr(str.length() - 1, 1) == PATH_SEPARATOR)
   {
      str = str.substr(0, str.length() - 1);
   }
   if(path2 != NULL)
   {
      string strAdd(path2);
      if(strAdd.substr(0, 1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
   if(path3 != NULL)
   {
      string strAdd(path3);
      if(strAdd.substr(0, 1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
   if(path4 != NULL)
   {
      string strAdd(path4);
      if(strAdd.substr(0, 1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
   if(path5 != NULL)
   {
      string strAdd(path5);
      if(strAdd.substr(0, 1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
#if defined(LINUX) || defined(APPLEOS)
   str.replace("\\", "/");
#else
   str.replace("/", "\\");
#endif
   return str;
}

bool dir::is(const char * path1)
{

#ifdef WINDOWS
   string str;

   str = "\\\\?\\";
   str += path1;

   while(str_ends_dup(str, "\\") || str_ends_dup(str, "/"))
   {
      str = str.substr(0, str.length() - 1);
   }

   uint32_t dwFileAttributes = ::GetFileAttributesW(wstring(str));
   if(dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return true;
   else
      return false;

#else

   // dedicaverse stat -> Sir And Arthur - Cesar Serenato

   struct stat st;

   if(stat(path1, &st))
      return false;

   if(!(st.st_mode & S_IFDIR))
      return false;

   return true;

#endif

}

string dir::name(const char * path1)
{

   string str;

   str = path1;
   index iPos1 = str.reverse_find('/');
   index iPos2 = str.reverse_find('\\');

   return str.substr(0, MAX(iPos1, iPos2) + 1);

}

string dir::folder(const char * path1)
{

   string str;

   str = path1;
   index iPos1 = str.reverse_find('/');
   index iPos2 = str.reverse_find('\\');

   return str.substr(0,MAX(MAX(iPos1,iPos2),0));

}


void dir::rls(stringa & stra, const char *psz)
{
   ::count start = stra.get_count();
   ls(stra, psz);
   ::count end = stra.get_count();
   for(::index i = start; i < end; i++)
   {
      if(is(stra[i]))
      {
         rls(stra, stra[i]);
      }
   }

}

void dir::rls_dir(stringa & stra, const char *psz)
{
   ::count start = stra.get_count();
   ls_dir(stra, psz);
   ::count end = stra.get_count();
   for(::index i = start; i < end; i++)
   {
      rls_dir(stra, stra[i]);
   }

}


void dir::ls(stringa & stra, const char *psz)
{

#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

   DIR * dirp = opendir(psz);

   if(dirp == NULL)
      return;

   dirent * dp;

   while ((dp = readdir(dirp)) != NULL)
   {

      if(strcmp(dp->d_name, "..") == 0)
         continue;
      else if(strcmp(dp->d_name, ".") == 0)
         continue;
      stra.add(path(psz, dp->d_name));

   }

   closedir(dirp);

#elif defined(METROWIN)

   ::Windows::Storage::StorageFolder ^ folder = nullptr;

   try
   {

      folder = wait(::Windows::Storage::StorageFolder::GetFolderFromPathAsync(string(psz)));

   }
   catch (...)
   {

      return;

   }

   if(folder == nullptr)
      return;

   ::Windows::Foundation::Collections::IVectorView < ::Windows::Storage::IStorageItem ^ > ^ a = wait(folder->GetItemsAsync());

   for(uint32_t ui = 0; ui < a->Size; ui++)
   {
      stra.add(begin(a->GetAt(ui)->Path));
   }


#else

   WIN32_FIND_DATA FindFileData;

   HANDLE hFind;

   hFind = FindFirstFile(psz, &FindFileData);

   if (hFind == INVALID_HANDLE_VALUE)
      return;

   while(true)
   {

      if(!__win_file_find_is_dots(FindFileData) && (FindFileData.dwFileAttributes != INVALID_FILE_ATTRIBUTES))
         stra.add(FindFileData.cFileName);

      if(!FindNextFile(hFind, &FindFileData))
         break;

   }

   FindClose(hFind);

#endif


}

void dir::ls_dir(stringa & stra, const char *psz)
{

#if defined(LINUX) || defined(APPLEOS) || defined(ANDROID)

   DIR * dirp = opendir(psz);

   if(dirp == NULL)
      return;

   dirent * dp;

   while ((dp = readdir(dirp)) != NULL)
   {
      if(dp->d_name[0] == '.')
      {
         if(dp->d_name[1] == '\0')
            continue;
         if(dp->d_name[1] == '.')
         {
            if(dp->d_name[2] == '\0')
               continue;
         }
      }
      string strPath = path(psz, dp->d_name);
      if(is(strPath))
      {
         stra.add(strPath);
      }

   }

   closedir(dirp);

#elif defined(METROWIN)

   ::Windows::Storage::StorageFolder ^ folder = wait(::Windows::Storage::StorageFolder::GetFolderFromPathAsync(string(psz)));

   ::Windows::Foundation::Collections::IVectorView < ::Windows::Storage::StorageFolder ^ > ^ a = wait(folder->GetFoldersAsync());

   for(uint32_t ui = 0; ui < a->Size; ui++)
   {
      stra.add(begin(a->GetAt(ui)->Path));
   }


#else

   WIN32_FIND_DATA FindFileData;

   HANDLE hFind;

   hFind = FindFirstFile(psz, &FindFileData);

   if (hFind == INVALID_HANDLE_VALUE)
      return;

   while(true)
   {

      if(!__win_file_find_is_dots(FindFileData) && (FindFileData.dwFileAttributes != INVALID_FILE_ATTRIBUTES) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
         stra.add(FindFileData.cFileName);


      stra.add(FindFileData.cFileName);

      if(!FindNextFile(hFind, &FindFileData))
         break;

   }

   FindClose(hFind);

#endif


}

string dir::default_os_user_path_prefix()
{
#if defined(WINDOWSEX)
   wchar_t buf[MAX_PATH];
   ULONG ulSize = sizeof(buf) / sizeof(wchar_t);
   if(!::GetUserNameExW(NameCanonical, buf, &ulSize))
   {
      if(!::GetUserNameW(buf, &ulSize))
      {
         memset(buf, 0, sizeof(buf));
      }
   }
   string str;
   //str.attach(::str::international::unicode_to_utf8(buf));
   str = str::international::unicode_to_utf8(buf);
   return str;

#elif defined(METROWIN)

//#pragma push_macro("System")
//#undef System

   string str(UserProfile::UserInformation::GetDomainNameAsync()->GetResults()->Data());
//#pragma pop_macro("System")

   return str;

#elif defined(ANDROID)

   string str("ca2user");

   return str;

#else

   ::primitive::memory mem;

   mem.allocate(512);

retry:

   if(getlogin_r((char *) mem.get_data(), mem.get_size()))
   {

      if(errno == ERANGE)
      {

         if(mem.get_size() < 65536)
         {

            mem.allocate(mem.get_size() + 512);

            goto retry;

         }

      }

      return "";

   }

   return mem.to_string();

#endif

}



string dir::usersystemappdata(const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
{
   return path(appdata(lpcszPrefix), lpcsz, lpcsz2);
}



string dir::default_userappdata(const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
{
   return path(default_userfolder(lpcszPrefix, lpcszLogin, "appdata"), pszRelativePath);
}

string dir::default_userdata(const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
{
   return path(default_userfolder(lpcszPrefix, lpcszLogin, "data"), pszRelativePath);
}

string dir::default_userfolder(const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
{

   return userfolder(pszRelativePath);

}



string dir::userfolder(const char * lpcsz, const char * lpcsz2)
{

   string str;

#ifdef WINDOWSEX

   SHGetSpecialFolderPath(NULL, (string &) str, CSIDL_PROFILE, false);

#elif defined(METROWIN)

   str = ::Windows::Storage::KnownFolders::DocumentsLibrary->Path;

#else

   str = getenv("HOME");

#endif

   string strRelative;
   strRelative = element();
   index iFind = strRelative.find(':');
   if(iFind >= 0)
   {
      ::index iFind1 = strRelative.reverse_find('\\', iFind);
      ::index iFind2 = strRelative.reverse_find('/', iFind);
      ::index iStart = MAX(iFind1 + 1, iFind2 + 1);
      strRelative = strRelative.substr(0, iFind - 1) + "_" + strRelative.substr(iStart, iFind - iStart) + strRelative.substr(iFind + 1);
   }

   string strUserFolderShift;

   /*if(App(papp).directrix()->m_varTopicQuery.has_property("user_folder_relative_path"))
   {
   strUserFolderShift = path(strRelative, App(papp).directrix()->m_varTopicQuery["user_folder_relative_path"].get_string());
   }
   else*/
   {
      strUserFolderShift = strRelative;
   }

   return path(path(str, "ca2", strUserFolderShift), lpcsz, lpcsz2);

   //      return path(path(str, "ca2"), lpcsz);
   /*      if(&AppUser(papp) == NULL)
   {
   string str;
   SHGetSpecialFolderPath(
   NULL,
   str,
   CSIDL_PROFILE,
   FALSE);
   return path(path(str, "core\\_____default"), lpcsz);
   }
   else
   {
   return path(AppUser(papp).m_strPath, lpcsz, lpcsz2);
   }*/
}


string dir::pathfind(const string & pszEnv, const string & pszTopic, const string & pszMode)
{

   stringa stra;

   stra.add_tokens(pszEnv, ":");

   string strCandidate;

   for(int32_t i = 0; i < stra.get_count(); i++)
   {

      if(stra[i].is_empty())
         continue;

      strCandidate = path(stra[i], pszTopic);

      if(file_exists_dup(strCandidate))
      {
         return strCandidate;
      }

   }

   return "";

}



