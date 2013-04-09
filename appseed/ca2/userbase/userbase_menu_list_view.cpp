#include "framework.h"


namespace userbase
{


   menu_list_view::menu_list_view(sp(::ca::application) papp) :
      ca(papp),
      menu_base(papp),
      menu_list_window(papp)
   {
      m_etranslucency      = TranslucencyPresent;
      m_pguieNotify        = ::null();
      m_bAutoClose         = false;
   }

   void menu_list_view::install_message_handling(::ca::message::dispatch * pinterface)
   {
      menu_list_window::install_message_handling(pinterface);
   }

   menu_list_view::~menu_list_view()
   {
   }

#ifdef DEBUG
   void menu_list_view::assert_valid() const
   {
      view::assert_valid();
   }

   #ifndef _WIN32_WCE
   void menu_list_view::dump(dump_context & dumpcontext) const
   {
      view::dump(dumpcontext);
   }
   #endif

#endif //DEBUG

   void menu_list_view::GuieProc(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::base, pbase, pobj);
      if(pbase->m_uiMessage == m_uiMessage)
      {
         if(base < ::user::place_holder >::bases(get_parent()))
         {
            pbase->set_lresult(get_parent()->get_parent()->send_message(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         }
         else
         {
            pbase->set_lresult(get_parent()->send_message(pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         }
         pbase->m_bRet = true;
         return;
      }
      return view::GuieProc(pobj);
   }

   bool menu_list_view::_001OnCmdMsg(BaseCmdMsg * pcmdmsg)
   {
      if(m_pguieNotify != ::null() && m_pguieNotify != this)
      {
         if(m_pguieNotify->_001OnCmdMsg(pcmdmsg))
            return TRUE;
      }
      return view::_001OnCmdMsg(pcmdmsg);
   }

   bool menu_list_view::LoadMenu(sp(::xml::node) pnode, sp(::user::interaction) pguieNotify, UINT uiCallbackMessage)
   {
      
      m_uiMessage = uiCallbackMessage;
      m_bAutoClose = false;

      if(!menu_list_window::LoadMenu(pnode))
         return false;

      MenuFill(this, GetParentFrame());

      m_pguieNotify = pguieNotify;

      return true;

   }


} // namespace userbase


