#include "framework.h"
#include "base/user/user.h"
#include  "windows_system_interaction_impl.h"

namespace base
{

   system_interaction_impl::system_interaction_impl(::aura::application * papp):
      ::object(papp),
      ::user::interaction(papp)
   {

   }

   system_interaction_impl::~system_interaction_impl()
   {


   }

   void system_interaction_impl::install_message_routing(::message::sender * psender)
   {

      ::user::interaction::install_message_routing(pdispatch);

      IGUI_WIN_MSG_LINK(WM_SETTINGCHANGE,pdispatch,this,&system_interaction_impl::_001MessageHub);
      IGUI_WIN_MSG_LINK(WM_DISPLAYCHANGE,pdispatch,this,&system_interaction_impl::_001MessageHub);

   }

   void system_interaction_impl::_001MessageHub(::message::message * pobj)
   {

      SCAST_PTR(::message::base,pbase,pobj);

      if(pbase != NULL)
      {

         if(pbase->m_uiMessage == WM_DISPLAYCHANGE ||
            (pbase->m_uiMessage == WM_SETTINGCHANGE &&
            (pbase->m_wparam == SPI_SETWORKAREA)))
         {

            System.enum_display_monitors();

            sp(::user::interaction) pui;

            while(System.get_frame(pui))
            {

               try
               {

                  pui->post_message(message_display_change);

               }
               catch(...)
               {
               }

            }


         }

      }

   }



} // namespace base


