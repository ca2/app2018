#include "framework.h"



void * __node_library_touch(const char * pszPath, string & strMessage)
{

   return __node_library_open(pszPath, strMessage);

}


void * __node_library_open(const char * pszPath, string & strMessage)
{

   void * plibrary = NULL;

   string strPath(pszPath);

   if(str_ends_ci_dup(strPath,".ilk"))
      return false;

   if(str_ends_ci_dup(strPath,".pdb"))
      return false;

   if(str_ends_ci_dup(strPath,".lib"))
      return false;

   if(str_ends_ci_dup(strPath,".exp"))
      return false;

   if(strstr_dup(file_name_dup(strPath),".") == NULL)
      strPath += ".dll";

   try
   {

      plibrary = ::LoadPackagedLibrary(gen_utf8_to_16(strPath), 0);

   }
   catch(...)
   {

   }

   if(plibrary == NULL)
   {

      try
      {

         plibrary = ::LoadPackagedLibrary(gen_utf8_to_16("\\\\?\\" + strPath),0);

      }
      catch(...)
      {

      }

   }

   if(plibrary == NULL)
   {

      try
      {

         plibrary = ::LoadPackagedLibrary(gen_utf8_to_16(::dir::ca2_module() / strPath),0);

      }
      catch(...)
      {

      }

   }

   if(plibrary == NULL)
   {

      try
      {

         plibrary = ::LoadPackagedLibrary(gen_utf8_to_16("\\\\?\\" + ::dir::ca2_module() / strPath),0);

      }
      catch(...)
      {

      }

   }

   if(plibrary == NULL)
   {

      try
      {

         plibrary = ::LoadPackagedLibrary(gen_utf8_to_16(::dir::base_module() / strPath),0);

      }
      catch(...)
      {

      }

   }

   if(plibrary == NULL)
   {

      try
      {

         plibrary = ::LoadPackagedLibrary(gen_utf8_to_16("\\\\?\\" + ::dir::base_module() /strPath),0);

      }
      catch(...)
      {

      }

   }

   return plibrary;

}




void * __node_library_open_ca2(const char * psz, string & strMessage)
{
   /*      string str(psz);
   if(str.find("..") >= 0)
   return FALSE;
   if(str.find(":") >= 0)
   return FALSE;
   if(str.find("\\\\") >= 0)
   return FALSE;
   if(str[0] == '\\')
   return FALSE;
   if(str[0] == '/')
   return FALSE;
   #ifdef _M_X64
   //::SetDllDirectory(dir::element("stage\\x64") + "\\");
   #else
   //::SetDllDirectory(dir::element("stage\\x86") + "\\");
   #endif*/

   return LoadPackagedLibrary(gen_utf8_to_16(psz),0);

}

void * __node_library_raw_get(void * plibrary,const char * pszEntryName)
{

   return ::GetProcAddress((HINSTANCE)plibrary,pszEntryName);

}



bool __node_library_close(void * plibrary)
{

   if (plibrary == NULL)
      return false;

   bool bOk = false;

   try
   {

      bOk = ::FreeLibrary((HINSTANCE)plibrary) != FALSE;

   }
   catch (...)
   {

   }

   return bOk;

}
