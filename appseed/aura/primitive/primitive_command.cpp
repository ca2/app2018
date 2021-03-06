#include "framework.h"


namespace command
{


   command::command()
   {

      m_ecommand = command_default;

   }

   command::command(const command & command)
   {

      operator = (command);

   }


   command::command(::aura::application * papp) :
      object(papp)
   {

      m_ecommand = command_default;

   }

   command::command(::aura::application * papp, e_command ecommand) :
      object(papp)
   {

      m_ecommand = ecommand;

   }

   command::~command()
   {

   }


   var command::command_run()
   {

      return true;

   }


   void command::on_request(::create * pcreate)
   {

      UNREFERENCED_PARAMETER(pcreate);

   }


   string command::get_description()
   {

      return "this is a command that was not described";

   }


   ::command::command & command::operator = (const ::command::command & command)
   {

      if (this == &command)
      {

         return *this;

      }

      ::object::operator = (command);

      m_ecommand = command.m_ecommand;
      m_strCommandLine = command.m_strCommandLine;
      m_varFile = command.m_varFile;

      return *this;

   }



} // namespace command