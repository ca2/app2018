#pragma once

#include "userbase_menu_list_window.h"

namespace userbase
{

   class CLASS_DECL_ca menu_list_view : 
      virtual public menu_list_window
   {
   public:

      ::user::interaction   * m_pguieNotify;


      menu_list_view(::ca::application * papp);
      virtual ~menu_list_view();

      void _001InstallMessageHandling(::user::win::message::dispatch * pinterface);

      virtual void GuieProc(gen::signal_object * pobj);

      virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);

      bool LoadMenu(xml::node * pnode, ::user::interaction* pguieNotify, UINT uiCallbackMessage);
         
   #ifdef _DEBUG
      virtual void assert_valid() const;
   #ifndef _WIN32_WCE
      virtual void dump(dump_context & dumpcontext) const;
   #endif
   #endif

   };

} // namespace userbase