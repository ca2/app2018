#pragma once

namespace npca2
{

   class run_start_installer :
      virtual public bergedge::bergedge::run_start_installer
   {
   public:

      
      plugin * m_pplugin;


      run_start_installer(::ca::application * papp, plugin * pplugin);
         
   
      virtual void run_start_install(const char * pszStart);

   };


} // namespace npca2