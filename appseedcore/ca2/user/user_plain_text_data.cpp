#include "StdAfx.h"


namespace user
{

   plain_text_data::plain_text_data(::ca::application * papp) :
      ca(papp),
      ::ca::data(papp),
      ex1::tree_data(papp),
      m_editfile(papp)
   {
      m_pgroupcommand      = NULL;
      m_ptreeitem          = NULL;
      m_pfile              = NULL;
      m_iBranch            = 0;
   }
         
   plain_text_data::~plain_text_data()
   {
      if(m_pfile != NULL)
      {
         delete m_pfile;
      }
   }


   void plain_text_data::SetFile(ex1::file * pfile)
   {
      m_editfile.SetFile(pfile);
      on_update_data(update_hint_set_file);
   }




   plain_text_data::ECommand plain_text_data::Command::get_command()
   {
      return CommandUndefined;
   }
   
   void plain_text_data::Command::Undo(plain_text_data * pedit)
   {
   }
   
   void plain_text_data::Command::Redo(plain_text_data * pedit)
   {
   }



   void plain_text_data::SetSelCommand::Undo(plain_text_data * pdoc)
   {
      m_iSelStart = m_iPreviousSelStart;
      m_iSelEnd = m_iPreviousSelEnd;
   }
   void plain_text_data::SetSelCommand::Redo(plain_text_data * pdoc)
   {
      m_iSelStart = m_iSelStart;
      m_iSelEnd = m_iSelEnd;
   }

   void plain_text_data::FileCommand::Undo(plain_text_data * pdoc)
   {
      pdoc->m_editfile.Undo();
   }

   void plain_text_data::FileCommand::Redo(plain_text_data * pdoc)
   {
      pdoc->m_editfile.Redo();
   }




   plain_text_data::GroupCommand::GroupCommand()
   {
      m_pparent = NULL;
   }

   void plain_text_data::GroupCommand::Undo(plain_text_data * pdoc)
   {
      for(int i = get_upper_bound(); i >= 0; i--)
      {
         element_at(i)->Undo(pdoc);
      }
   }
   void plain_text_data::GroupCommand::Redo(plain_text_data * pdoc)
   {
      for(int i = 0; i < get_size(); i++)
      {
         element_at(i)->Redo(pdoc);
      }
   }


} // namespace user