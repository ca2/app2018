#include "framework.h"


// gone to ansios_file.cpp/.h
/*

//#include <sys/stat.h>


void fd_ensure_file_size(int32_t fd, int64_t iSize)
{

   if(ftruncate(fd, iSize) == -1)
      throw "fd_ensure_file_size exception";

}


int64_t fd_get_file_size(int32_t fd)
{

   struct stat st;

   if(fstat(fd, &st) == -1)
   {

      ::close(fd);

      return -1;

   }

   return st.st_size;

}

*/




CLASS_DECL_AURA void dll_processes(uint_array & dwa, stringa & straProcesses, const char * pszDll)
{



}



::file::path dir::sys_temp()
{

   return ::file::path(getenv("HOME")) /  ".core" / "time";

}



::file::path dir::home()
{

   //return getenv("HOME");
   return "/data/home";

}



::file::path dir::favorites()
{

   return ::dir::local() / "localconfig/favorites";

}


