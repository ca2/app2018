#include "framework.h"
#include <dlfcn.h>


base_library::base_library(sp(base_application) papp) :
element(papp)
{

   m_plibrary = NULL;

   m_bAutoClose = false;

}


base_library::base_library(sp(base_application) papp, const char * pszOpen) :
element(papp)
{

   m_plibrary = NULL;

   m_bAutoClose = false;

   open(pszOpen);

}


base_library::~base_library()
{

   if(m_bAutoClose)
   {

      close();

   }

}


bool base_library::open(const char * pszPath, bool bAutoClose)
{

   if(m_bAutoClose)
   {

      close();

   }

   m_bAutoClose = bAutoClose;

   string strPath(pszPath);

   if(strPath == "os")
   {

      strPath = "ca2os";

   }
   else if(strPath == "app_sphere")
   {

      strPath = "basesphere";

   }

   if(strstr_dup(strPath, ".") == NULL)
      strPath += ".so";

   if(strstr((const char *) strPath, "/") == NULL && !str_begins_dup(strPath, "lib"))
      strPath = "lib" + strPath;

   m_plibrary = dlopen(strPath, RTLD_LOCAL | RTLD_NOW | RTLD_NODELETE);
   int iError = errno;

   const char * psz = strerror(iError);

   if(psz != NULL)
   {

      fprintf(stderr, "%s\n", psz);

   }

   const char * psz2 = dlerror();

   if(psz2 != NULL)
   {

      fprintf(stderr, "%s\n", psz2);

   }

   return m_plibrary != NULL;

}


bool base_library::close()
{
   if(m_plibrary != NULL)
   {
      dlclose(m_plibrary);
   }
}


void * base_library::raw_get(const char * pszElement)
{
   return dlsym(m_plibrary, pszElement);
}

ca2_library::ca2_library(sp(base_application) papp) :
element(papp),
base_library(papp)
{
}

ca2_library::ca2_library(sp(base_application) papp, const char * pszOpen) :
   element(papp),
   base_library(papp, pszOpen)
{

}

ca2_library::~ca2_library()
{

}


bool ca2_library::open(const char * pszPath, bool bAutoClose)
{

   return base_library::open(pszPath, bAutoClose);

}


bool base_library::is_opened()
{

   return m_plibrary != NULL;

}


bool base_library::is_closed()
{

   return !is_opened();

}



