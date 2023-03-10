#include "framework.h"


namespace plugin
{


   system::system(::aura::application * papp, ::app_core * pappcore) :
      object(papp == NULL ? this : papp),
      ::aura::system(papp == NULL ? this : papp, pappcore),
      ::axis::system(papp == NULL ? this : papp, pappcore),
      ::base::system(papp == NULL ? this : papp, pappcore),
      ::core::system(papp == NULL ? this : papp, pappcore)
   {

   }


   bool system::os_native_bergedge_start()
   {

      //return m_pplugin->os_native_bergedge_start();

      return true;

   }


   bool system::open_link(const string & pszLink, const string & pszTarget)
   {

      UNREFERENCED_PARAMETER(pszTarget);

      m_pplugin->m_puiHost->post_message(host_interaction::message_check, 2, (LPARAM) (void *) (new string(pszLink)));

      return true;

   }


   bool system::verb()
   {

      return ::core::application::verb();

   }


   string system::get_host_location_url()
   {

      return (const string &) m_pplugin->get_host_location_url();

   }


} // namespace plugin


