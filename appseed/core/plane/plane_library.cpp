#include "framework.h"


#ifndef _WIN32
extern "C"
#endif
::core::library * get_new_library(sp(base_application) papp);


namespace plane
{


   library::library(sp(base_application) papp) :
      element(papp),
      ::base_library(papp),
      ::core::library(papp, "app"),
      ::core::single_application_library < application >(papp, "app"),
      ::filehandler::library(papp),
      ::filehandler::menu_library(papp)
   {

   }


   void library::get_app_list(stringa & stra)
   {


      stra.add("fontopus");
      stra.add("netnode");
      stra.add("default_file_handler");

   }


   void library::get_extension_list(stringa & stra)
   {

      stra.add("*");

   }

   void library::get_extension_app(stringa & straApp, const char * pszExtension)
   {

      string strExtension(pszExtension);

      strExtension.make_lower();

      if(strExtension == "*")
      {
         straApp.add_unique("default_file_handler");
      }
      if(strExtension == "mid" || strExtension == "karaoke" || strExtension == "st3")
      {
         straApp.add_unique("mplite");
         straApp.add_unique("vmsp");
      }

   }



} // namespace cube





#ifndef _WIN32
extern "C"
#endif
::core::library * get_new_library(sp(base_application) papp)
{
   return canew(::plane::library(papp));
}
