#include "framework.h"


namespace user
{


   menu_list_view::menu_list_view(::aura::application * papp) :
      object(papp),
      menu(papp),
      menu_list_window(papp)
   {

      m_bAutoClose = false;

   }


   void menu_list_view::install_message_routing(::message::sender * pinterface)
   {

      BASE::install_message_routing(pinterface);

   }

   menu_list_view::~menu_list_view()
   {

   }


   void menu_list_view::assert_valid() const
   {
      impact::assert_valid();
   }


   void menu_list_view::dump(dump_context & dumpcontext) const
   {
      impact::dump(dumpcontext);
   }





   bool menu_list_view::pre_create_window(::user::create_struct & cs)
   {

      return ::user::impact::pre_create_window(cs);

   }


   void menu_list_view::route_command_message(::user::command * pcommand)
   {

      if (m_puiMenuNotify != NULL && m_puiMenuNotify != this)
      {

         m_puiMenuNotify->route_command_message(pcommand);

         //if(pcommand->m_bRet)
         {

            return;

         }

      }

      return impact::route_command_message(pcommand);

   }


   bool menu_list_view::load_menu(::xml::node * pnode, ::user::interaction * puiNotify, UINT uiCallbackMessage)
   {

      destroy_menu();

      m_bAutoClose = false;

      if (!menu_list_window::load_menu(pnode))
         return false;

      //menu_fill(GetParentFrame(), this);

      m_puiMenuNotify = puiNotify;

      return true;

   }


   void menu_list_view::PostNcDestroy()
   {

      ::user::menu_list_window::PostNcDestroy();

   }

} // namespace user


