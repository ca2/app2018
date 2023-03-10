#include "framework.h"


#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void file_memory_map::construct()
{

   m_strName.Empty();
   m_iFile = -1;
   m_pdata = (void *) MAP_FAILED;
   m_bRead = false;
   m_bWrite = false;
   m_bCreate = false;

}


bool file_memory_map::is_mapped()
{
   return m_pdata != (void *)MAP_FAILED;
}



bool file_memory_map::close()
{

   if(m_pdata != (void *)MAP_FAILED)
   {

      ::munmap(m_pdata,::get_file_size(m_iFile));

      m_pdata = NULL;

   }

   if(m_iFile != -1)
   {

      ::close(m_iFile);

      m_iFile = -1;

   }

   return true;

}



bool file_memory_map::open()
{

   close();

   int iOpen;

   if(m_bCreate)
   {

      iOpen = O_RDWR | O_CREAT;

   }
   else
   {

      iOpen = O_RDWR;

   }

   ::file::path path(get_path());

   ::dir::mk(path.folder());

   m_iFile = ::open(path,iOpen, S_IRUSR |  S_IWUSR );


   if(m_iFile == -1)
   {

      close();

      return false;

   }

   ensure_file_size_fd(m_iFile,m_size);

   m_pdata = (COLORREF *)mmap(NULL,m_size,(m_bRead ? PROT_READ : 0) | (m_bWrite ? PROT_WRITE : 0),MAP_SHARED,m_iFile,0);

   if(m_pdata == (void *)MAP_FAILED)
   {

      close();

      return false;

   }

   string strMutex;

   strMutex = m_strName + "-mutex";

   m_pmutex = new mutex(get_app(), false, strMutex, NULL);

   return true;

}
