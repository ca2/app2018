#include "framework.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
#include <link.h>


struct PROCESS_INFO_t
{
    string csProcess;
    uint32_t dwImageListIndex;
};


namespace linux
{


   file_system::file_system(::aura::application *  papp) :
      ::object(papp),
      ::file::system(papp)
   {
   }


   file_system::~file_system()
   {

   }


   bool file_system::update_module_path()
   {

      m_pathModule = ::file::app_module();

      {

         void * handle = dlopen("libaura.so", RTLD_NOW);

         if(handle == NULL)
         {

            m_pathCa2Module = m_pathModule;

         }
         else
         {

            link_map * plm;

            dlinfo(handle, RTLD_DI_LINKMAP, &plm);

            m_pathCa2Module = plm->l_name;

            if(m_pathCa2Module.is_empty() || m_pathCa2Module[0] != '/')
            {

                m_pathCa2Module = m_pathModule;

            }

            dlclose(handle);

         }

      }

		return true;

   }


   ::file::file_sp file_system::get_file(var varFile,UINT nOpenFlags,cres * pfesp,::aura::application * papp)
   {

      ::file::file_sp spfile;

      spfile = ::file::system::get_file(varFile,nOpenFlags,pfesp,papp);

      if(spfile.is_set())
      {

         return spfile;

      }

      return NULL;

   }


   bool file_system::is_link(string strPath)
   {

      struct stat stat;

      ZERO(stat);

      if(lstat(strPath, &stat) != 0)
      {

         return false;

      }

      if(S_ISLNK(stat.st_mode))
      {

         return true;

      }

      return false;

   }



} // namespace linux






