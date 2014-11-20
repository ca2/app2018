#include "framework.h"


namespace userex
{


   property_sheet::property_sheet(::aura::application * papp) :
      element(papp),
      ::user::scroll_view(papp),
      ::user::form_interface(papp),
      ::user::form(papp),
      html_form(papp),
      html_form_view(papp),
      form_view(papp)
   {

   }


   property_sheet::~property_sheet()
   {

   }


} // namespace userex




