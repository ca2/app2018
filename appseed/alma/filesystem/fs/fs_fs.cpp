#include "framework.h"


namespace fs
{


   fs::fs(::base::application * papp) :
      element(papp),
      ::base::departament(papp)
   {

   }


   fs::~fs()
   {

   }


   bool fs::initialize()
   {

      if(m_pbaseapp->is_system())
      {

/*       
         System.factory().creatable_small < main_view > ();
         System.factory().creatable_small < tree > ();
         System.factory().creatable_small < list > ();
*/

      }

      return true;

   }
 


} // namespace fs



