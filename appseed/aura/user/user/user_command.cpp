#include "framework.h"


namespace aura
{


   cmd_msg::cmd_msg()
   {

      m_pcommandtargetSource        = NULL;
      m_etype                       = type_command;

   }


   cmd_msg::cmd_msg(id id)
   {

      m_pcommandtargetSource        = NULL;
      m_etype                       = type_command;
      m_id                          = id;

   }


   cmd_msg::cmd_msg(::command_ui * pcommandui)
   {

      m_pcommandtargetSource        = NULL;
      m_etype                       = type_cmdui;
      m_pcmdui                      = pcommandui;

   }


   bool cmd_msg::handle(::command_target * pcommandtarget)
   {
      return m_commandtargetptraHandle.add_unique(pcommandtarget);
   }

   bool cmd_msg::is_handled(::command_target * pcommandtarget)
   {
      return m_commandtargetptraHandle.contains(pcommandtarget);
   }


} // namespace aura








































