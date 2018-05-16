#include "framework.h"


namespace userex
{


   property_page::property_page(::aura::application * papp) :
      object(papp),
      ::user::place_holder_container(papp),
      ::user::tab_view(papp),
      ::userex::pane_tab_view(papp)
   {

   }


   property_page::~property_page()
   {

   }


} // namespace userex




