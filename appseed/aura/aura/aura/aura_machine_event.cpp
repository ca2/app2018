#include "framework.h"
#include <stdio.h>

//#ifndef CA2_SPA
//#define PATH_SEPARATOR "\\"
/*void get_ca2_module_folder_dup(char * lpszModuleFolder)
{
   char lpszModuleFilePath[MAX_PATH * 8];
   HMODULE hmodule = ::GetModuleHandleA("ca.dll");
   if(hmodule == NULL)
      hmodule = ::GetModuleHandleA("spalib.dll");
   GetModuleFileName(hmodule, lpszModuleFilePath, sizeof(lpszModuleFilePath));
   LPTSTR lpszModuleFileName;
   GetFullPathName(lpszModuleFilePath, sizeof(lpszModuleFilePath), lpszModuleFolder, &lpszModuleFileName);
   lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';
   if(strlen(lpszModuleFolder) > 0)
   {
      if(lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '\\'
      || lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '/')
      {
         lpszModuleFolder[strlen(lpszModuleFolder) - 1] = '\0';
      }
   }
}


string dir_beforeca2()
{
   char lpszModuleFolder[MAX_PATH * 8];
   get_ca2_module_folder_dup(lpszModuleFolder);
   char * psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip x86 or x64...
   psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip stage
   psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip ca
   // now, lpszModuleFolder should be beforeca2
   return lpszModuleFolder;
}
string dir_path(const char * path1, const char * path2)
{
   string str1(path1);
   if(str1.substr(str1.length() - 2,1) != PATH_SEPARATOR)
   {
      str1 = str1 + PATH_SEPARATOR;
   }
   if(path2 == NULL)
   {
      return str1;
   }
   else
   {
      return str1 + path2;
   }
}

string dir_ca2(const char * path = NULL)
{
   if(path == NULL)
      return dir_path(dir_beforeca2(), "ca2");
   else
      return dir_path(dir_ca2(), path);
}

string dir_appdata(const char * path = NULL)
{
   char sz[MAX_PATH * 8];
   ::SHGetSpecialFolderPathA(NULL, sz, CSIDL_COMMON_APPDATA, TRUE);
   string str = dir_path(sz, "ca2");
   return dir_path(str, path);
}

#endif*/


machine_event::machine_event()
{

   m_bInitialized = false;

   initialize();

}


machine_event::~machine_event()
{

}


bool machine_event::initialize()
{

   if(m_bInitialized)
   {

      return true;

   }

   m_bInitialized = true;

   return true;

}


::file::path machine_event_file_path()
{

   return ::dir::appdata() / "machine/event/machine_event.bin";

}


bool machine_event::read(machine_event_data * pdata)
{

   FILE * pfile = NULL;

   try
   {

      pfile = fopen_dup(machine_event_file_path(), "r", _SH_DENYNO);

      if (pfile == NULL)
      {

         ZEROP(pdata);

         return false;

      }

      pdata->read(pfile);

   }
   catch (...)
   {

   }

   fclose_dup(pfile);

   return true;

}


bool machine_event::write(machine_event_data * pdata)
{

   FILE * pfile = NULL;

   try
   {

      dir::mk(dir::name(machine_event_file_path()));

      pfile = fopen_dup(machine_event_file_path(), "w", _SH_DENYWR);

      if (pfile == NULL)
      {

         return false;

      }

      pdata->write(pfile);

   }
   catch (...)
   {

   }

   fclose_dup(pfile);

   return true;

}


