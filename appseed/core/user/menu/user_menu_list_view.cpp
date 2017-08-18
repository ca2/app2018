#include "framework.h"


namespace user
{


   menu_list_view::menu_list_view(::aura::application * papp) :
      object(papp),
      menu(papp),
      menu_list_window(papp)
   {

      m_puiNotify = NULL;
      m_bAutoClose = false;

   }


   void menu_list_view::install_message_handling(::message::dispatch * pinterface)
   {

      BASE::install_message_handling(pinterface);

   }

   menu_list_view::~menu_list_view()
   {

   }

#ifdef DEBUG
   void menu_list_view::assert_valid() const
   {
      impact::assert_valid();
   }

#ifndef _WIN32_WCE
   void menu_list_view::dump(dump_context & dumpcontext) const
   {
      impact::dump(dumpcontext);
   }
#endif

#endif //DEBUG


   bool menu_list_view::pre_create_window(::user::create_struct & cs)
   {

      return ::user::impact::pre_create_window(cs);

   }

   void menu_list_view::GuieProc(signal_details * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if (pbase->m_uiMessage == m_uiMessage)
      {
         if (base_class < ::user::place_holder >::bases(GetParent()))
         {
            pbase->set_lresult(GetParent()->GetParent()->send_message(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         }
         else
         {
            pbase->set_lresult(GetParent()->send_message(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         }
         
         pbase->m_bRet = true;
         
         return;
         
      }
      
      return impact::GuieProc(pobj);
      
   }
   

   bool menu_list_view::_001OnCmdMsg(::user::command * pcmdmsg)
   {
      
      if (m_puiNotify != NULL && m_puiNotify != this)
      {
         
         if (m_puiNotify->_001OnCmdMsg(pcmdmsg))
         {
            
            return true;
            
         }
         
      }
      
      return impact::_001OnCmdMsg(pcmdmsg);
      
   }


   bool menu_list_view::load_menu(::xml::node * pnode, ::user::interaction * puiNotify, UINT uiCallbackMessage)
   {

      destroy_menu();

      m_uiMessage = uiCallbackMessage;
      m_bAutoClose = false;

      if (!menu_list_window::load_menu(pnode))
         return false;

      menu_fill(this, GetParentFrame());

      m_puiNotify = puiNotify;

      return true;

   }


   void menu_list_view::PostNcDestroy()
   {

      ::user::menu_list_window::PostNcDestroy();

   }

} // namespace user


