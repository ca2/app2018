#include "framework.h" // previously aura/user/user.h


namespace aura
{


   printer::printer(::aura::application * papp) :
      object(papp)
   {
   }

   printer::~printer()
   {
      close();
   }

   bool printer::open(const char * pszDeviceName)
   {
      UNREFERENCED_PARAMETER(pszDeviceName);
      return false;
   }

   ::draw2d::graphics * printer::create_graphics()
   {
      return NULL;
   }

   bool printer::is_opened()
   {
      return false;
   }

   bool printer::close()
   {
      return true;
   }


} // namespace user













































