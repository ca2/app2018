#pragma once


#include <sys/stat.h>


#if defined(MACOS) 


#if defined(_LP64)


#define stat64 stat


#endif


#endif



namespace dynamic_source
{


   class CLASS_DECL_ca library_class :
      virtual public ::ca::object
   {
   public:


      http::memory_file                      m_memfileError;
      string                                 m_strName;
      stringa                                m_straSourcePath;
      stringa                                m_straCppPath;
      string                                 m_strLibraryPath;
      ::c::library                           m_library;

      array < __time_t, __time_t & >    m_ftaCreation;
      array < __time_t, __time_t & >    m_ftaAccess;
      array < __time_t, __time_t & >    m_ftaModified;

      sp(script_manager)                     m_pmanager;



      library_class(::ca::application * papp);
      ~library_class();





      bool DoesMatchVersion();
      void Load();
      void Unload();




      ::ca::application * get_app() const;

   };


} // namespace dynamic_source


