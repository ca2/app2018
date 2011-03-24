#include "StdAfx.h"

namespace video
{

   player_command::player_command()
   {
      m_pbResult = NULL;
   }

   player_command::player_command(const player_command & command)
   {
      operator =(command);
   }

   player_command & player_command::operator =(const player_command & command)
   {
      SetCommand(command.GetCommand());
      SetOpenFile(command.GetOpenFile());
      m_bResult = command.m_bResult;
      m_pbResult = command.m_pbResult;
      m_evptra = command.m_evptra;
      return *this;

   }

   e_command player_command::GetCommand() const
   {
      return m_ecommand;
   }

   void player_command::SetCommand(e_command ecommand)
   {
      m_ecommand = ecommand;
   }

   ex1::file * player_command::GetOpenFile() const
   {
      return m_pfileOpen;
   }

   void player_command::SetOpenFile(ex1::file *pfile)
   {
      m_pfileOpen = pfile;
   }


   void player_command::OpenFile(ex1::file * pfile)
   {
      SetCommand(command_open_file);
      SetOpenFile(pfile);
   }

   void player_command::Play()
   {
      SetCommand(command_execute_play);
   }

   void player_command::AttachEvent(CEvent *pevent)
   {
      m_evptra.add(pevent);
   }
   void player_command::SetEvents()
   {
      for(int i = 0;  i < m_evptra.get_size(); i++)
      {
         m_evptra[i]->SetEvent();
      }
   }
   void player_command::Stop()
   {
      SetCommand(command_execute_stop);

   }
   //void player_command::CloseDevice()
   //{
   //   SetCommand(audCommandCloseDevice);

   //}

   //void player_command::OpenDevice()
   //{
   //   SetCommand(audCommandOpenDevice);

   //}


} // namespace video