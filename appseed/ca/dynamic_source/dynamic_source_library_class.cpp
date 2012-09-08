#include "framework.h"


namespace dynamic_source
{


   typedef struct tagLOADPARMS32 {
      char * lpEnvAddress;  // address of environment strings
      char * lpCmdLine;     // address of command line
      char * lpCmdShow;     // how to show new program
      DWORD dwReserved;    // must be zero
   } LOADPARMS32;

   library_class::library_class(::ca::application * papp)
      : m_memfileError(papp), ca(papp)
   {
   }

   bool library_class::DoesMatchVersion()
   {
      for(int i = 0; i < m_straSourcePath.get_size(); i++)
      {
         struct stat st;
         memset(&st, 0, sizeof(st));
//         memset(&ftAccess, 0, sizeof(__time_t));
//         memset(&ftModified, 0, sizeof(__time_t));
//         HANDLE h = ::CreateFile(m_straSourcePath[i], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
         stat(m_straSourcePath[i], &st);
//         ::CloseHandle(h);
         if(memcmp(&st.st_ctime, &m_ftaCreation[i], sizeof(__time_t)) != 0
            || memcmp(&m_ftaModified[i], &st.st_mtime, sizeof(__time_t)) != 0)
         {
            return false;
         }
      }
      return true;
   }
   void library_class::Load()
   {
      if(!m_library.open(m_strLibraryPath))
         return;
      m_ftaCreation.set_size(m_straSourcePath.get_size());
      m_ftaAccess.set_size(m_straSourcePath.get_size());
      m_ftaModified.set_size(m_straSourcePath.get_size());
      for(int i = 0; i < m_straSourcePath.get_size(); i++)
      {
         //HANDLE h = ::CreateFile(m_straSourcePath[i], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
         struct stat st;
         stat(m_straSourcePath[i], &st);
/*         memset(&m_ftaCreation[i], 0, sizeof(FILETIME));
         memset(&m_ftaAccess[i], 0, sizeof(FILETIME));
         memset(&m_ftaModified[i], 0, sizeof(FILETIME));
         GetFileTime(h , &m_ftaCreation[i], &m_ftaAccess[i], &m_ftaModified[i]);*/
         m_ftaCreation[i] = st.st_ctime;
         m_ftaAccess[i] = st.st_atime;
         m_ftaModified[i] = st.st_mtime;
         //::CloseHandle(h);
      }
   }
   void library_class::Unload()
   {
   }



   library_class::~library_class()
   {
   }

   ::ca::application * library_class::get_app() const
   {
      return m_pmanager->get_app();
   }


} // namespace dynamic_source


