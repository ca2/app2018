#include "framework.h"


bool command_target_interface::_001SendCommand(id id)
{
   ::aura::cmd_msg msg(id);
   return _001OnCmdMsg(&msg);
}

bool command_target_interface::_001SendUpdateCmdUi(cmd_ui * pcmdui)
{
   ::aura::cmd_msg msg(pcmdui);
   return _001OnCmdMsg(&msg);
}

bool command_target_interface::_001OnCmdMsg(::aura::cmd_msg * pcmdmsg)
{

   if(pcmdmsg->m_etype == ::aura::cmd_msg::type_command)
   {

      CTestCmdUI cmdui(get_app());

      cmdui.m_id = pcmdmsg->m_id;

      if(on_simple_update(&cmdui))
      {
         if(!cmdui.m_bEnabled)
            return false;
      }

      if(on_simple_action(pcmdmsg->m_id))
         return true;

   }
   else
   {

      if(on_simple_update(pcmdmsg->m_pcmdui))
         return true;

      if(_001HasCommandHandler(pcmdmsg->m_pcmdui->m_id))
      {
         pcmdmsg->m_pcmdui->Enable();
         return true;
      }

   }

   return false;

}

command_target_interface::command_signalid::~command_signalid()
{
}

command_target_interface::command_signalrange::~command_signalrange()
{
}

command_target_interface::command_target_interface()
{
}

command_target_interface::command_target_interface(::aura::application * papp)
   : object(papp)
{
}

bool command_target_interface::on_simple_action(id id)
{
   ::dispatch::signal_item_ptr_array signalptra;
   get_command_signal_array(::aura::cmd_msg::type_command,signalptra,id);
   bool bOk = false;
   for(int32_t i = 0; i < signalptra.get_size(); i++)
   {
      ::aura::command command(signalptra[i]->m_psignal);
      command.m_id = id;
      signalptra[i]->m_psignal->emit(&command);
      if(command.m_bRet)
         bOk = true;
   }
   return bOk;
}

bool command_target_interface::_001HasCommandHandler(id id)
{

   ::dispatch::signal_item_ptr_array signalptra;

   get_command_signal_array(::aura::cmd_msg::type_command,signalptra,id);

   return signalptra.get_size() > 0;

}


bool command_target_interface::on_simple_update(cmd_ui * pcmdui)
{
   ::dispatch::signal_item_ptr_array signalptra;
   get_command_signal_array(::aura::cmd_msg::type_cmdui,signalptra,pcmdui->m_id);
   bool bOk = false;
   for(int32_t i = 0; i < signalptra.get_size(); i++)
   {
      ::aura::cmd_ui cmdui(signalptra[i]->m_psignal);
      cmdui.m_pcmdui = pcmdui;
      signalptra[i]->m_psignal->emit(&cmdui);
      if(cmdui.m_bRet)
         bOk = true;
   }
   return bOk;
}

void command_target_interface::get_command_signal_array(::aura::cmd_msg::e_type etype,::dispatch::signal_item_ptr_array & signalptra,id id)
{
   command_signalid signalid;
   signalid.m_id = id;
   // collect all signals with the specified command id (pszId)
   for(int32_t i = 0; i < m_signalidaCommand.get_size(); i++)
   {
      class signalid * pid = m_signalidaCommand[i];
      if(pid->matches(&signalid))
      {
         if(etype == ::aura::cmd_msg::type_command)
         {
            m_dispatchCommand.m_signala.GetSignalsById(signalptra,&signalid);
         }
         else if(etype == ::aura::cmd_msg::type_cmdui)
         {
            m_dispatchUpdateCmdUi.m_signala.GetSignalsById(signalptra,&signalid);
         }
         else
         {
            ASSERT(FALSE);
         }
      }
   }
}

void command_target_interface::install_message_handling(::message::dispatch * pdispatch)
{

   UNREFERENCED_PARAMETER(pdispatch);

}








































