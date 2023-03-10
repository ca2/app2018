#include "framework.h"
#include "aura/aura/os/os.h"
#include "aura/aura/os/os_os.h"
#include <dlfcn.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>
#include <sys/types.h> // for jl_raise_debugger
#elif !defined(_OS_WINDOWS_)
#include <link.h>
#endif

void * __node_library_touch(const char * pszPath, string & strMessage)
{

   for (int32_t i = _dyld_image_count(); i >= 0 ; i--)
   {

      const char *image_name = _dyld_get_image_name(i);
      
      if(::file::path(image_name).title().compare_ci(::file::path(pszPath).title()) == 0)
      {
      
         goto found;
         
      }

      if(::file::path(image_name).title().compare_ci(("lib" + ::file::path(pszPath).title())) == 0)
      {
         
         goto found;
         
      }

   }
   
   return NULL;
found:
   return __node_library_open(pszPath, strMessage);
   
}


   void * __node_library_open(const char * pszPath, string & strMessage)
   {
      
      strMessage.Empty();
   
      string strPath(pszPath);
      
      string strError;
      
      ::file::path path;
      
      if(!str_ends_dup(strPath, ".dylib"))
      {
         
         strPath += ".dylib";
         
      }

      if(::str::find_ci(strPath, "/") < 0 && !str_begins_dup(strPath, "lib"))
      {
         
         strPath = "lib" + strPath;
         
      }
      
      ::output_debug_string("\n\nGoing to dlopen: \"" + strPath + "\"");
      
      path = ::file::path(::get_exe_path()).folder() / strPath;
      
      void * plibrary = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
      
      if(plibrary != NULL)
      {
         
         goto finished;
         
      }
      
      strError = dlerror();
         
      strMessage += "\n(1) dlopen: " + path + " with the error: \"" + strError + "\"";
         
      path = strPath;
      
      plibrary = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
      
      if(plibrary != NULL)
      {
         
         goto finished;
         
      }
      
      strError = dlerror();
      
      strMessage += "\n(2) dlopen: " + path + " with the error: \"" + strError + "\"";
      
      path = ::file::path(::ca2_module_folder_dup()) / strPath;
      
      plibrary = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
      
      if(plibrary != NULL)
      {
         
         goto finished;
         
      }

      strError = dlerror();
         
      strMessage += "\n(3) dlopen: " + path + " with the error: \"" + strError + "\"";
      
      if(strPath.find('/') >= 0)
      {

         path = ::file::path(strPath).name();
      
         plibrary = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
      
         if(plibrary != NULL)
         {
         
            goto finished;
         
         }
      
         strError = dlerror();
      
         strMessage += "\n(4) dlopen: " + path + " with the error: \"" + strError + "\"";
         
      }

   finished:
      
      if(plibrary != NULL)
      {
         
         strMessage = "__node_library_open (1) Succeeded " + path;

      }
      else
      {
         
         strMessage = "__node_library_open : Failed with : " + strMessage;
         
      }
      
      ::output_debug_string("\n" + strMessage + "\n\n");

      return plibrary;
      
   }
   

   bool __node_library_close(void * plibrary)
   {
      
      if(plibrary != NULL)
      {
   
         dlclose(plibrary);
         
      }
      
      return true;
      
   }


   void * __node_library_raw_get(void * plibrary, const char * pszElement)
   {
      
      return dlsym(plibrary, pszElement);
      
   }
   
   


   
   
void * __node_library_open_ca2(const char * pszPath, string & strMessage)
{
   
   string strPath(pszPath);
   
   void * plibrary = dlopen(strPath, RTLD_LOCAL | RTLD_LAZY);
   
   if(plibrary != NULL)
   {
      
      strMessage = "__node_library_open_ca2 Succeeded " + strPath;
      
   }
   else
   {
      
      string strError;
      
      strError = dlerror();
      
      strMessage = "__node_library_open_ca2 : " + strPath + " with the error: \"" + strError + "\"";
      
   }
   
   ::output_debug_string("\n\n" + strMessage + "\n\n");

   return plibrary;
   
}


