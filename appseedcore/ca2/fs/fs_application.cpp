#include "StdAfx.h"

namespace fs
{

   application::application()
   {
   }

   application::~application(void)
   {
   }

   bool application::initialize()
   {


      System.factory().creatable_small < main_view > ();
      System.factory().creatable_small < tree > ();
      System.factory().creatable_small < list > ();

      return true;
   }
 


} // namespace fs