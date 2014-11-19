#include "framework.h" // from "axis/user/user.h"


namespace user
{


   print_job::print_job(sp(::aura::application) papp) :
      element(papp),
      ::print_job(papp),
      ::job(papp),
      ::user::job(papp),
      thread(papp)
   {

   }


   print_job::~print_job()
   {

   }


   int32_t print_job::run()
   {

      ::exception::throw_interface_only(get_app());

      return -1;

   }

} // namespace user




