#include "framework.h"


   command::command(sp(base_application) papp) :
      element(papp)
   {
   }

   command::~command()
   {
   }


   var command::run()
   {
      return true;
   }

   void command::on_request(sp(::create_context) pcreatecontext)
   {
      UNREFERENCED_PARAMETER(pcreatecontext);
   }

   string command::get_description()
   {
      return "this is a command that was not described";
   }

