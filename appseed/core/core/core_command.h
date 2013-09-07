#pragma once


class command_target;


class CLASS_DECL_ca2 base_cmd_ui :
   public signal_details
{
public:


   cmd_ui * m_pcmdui;


   base_cmd_ui(::signal * psignal);


};


class CLASS_DECL_ca2 BaseCommand :
   public signal_details
{
public:


   id   m_id;


   BaseCommand(::signal * psignal);


};


class CLASS_DECL_ca2 BaseCmdMsg :
   public object
{
public:


   enum e_type
   {
      type_command,
      type_cmdui
   };


   cmd_ui  *                                    m_pcmdui;
   id                                           m_id;
   e_type                                       m_etype;
   comparable_array < ::command_target * >      m_commandtargetptraHandle;


   BaseCmdMsg();
   BaseCmdMsg(id id);
   BaseCmdMsg(cmd_ui * pcmdui);


   bool handle(::command_target * pcommandtarget);
   bool is_handled(::command_target * pcommandtarget);


};


