#include "framework.h"


namespace user
{


   LRESULT message::send()
   {

      try
      {
         
         if(m_uiMessage == message_post_user && m_wparam == 1)
         {
            
            sp(::message::base) pbase(m_lparam);
            
            return m_pui->send(pbase);
            
         }

         return m_pui->send_message(m_uiMessage,m_wparam,m_lparam);

      }
      catch(...)
      {

      }

      return 0;

   }
   

   UINT message::ThreadProcSendMessage(LPVOID lp)
   {
      
      message * pmessage = (message *) lp;
      
      pmessage->send();
      
      delete pmessage;
      
      return 0;
      
   }

   void message::post(sp(::user::interaction) puie, UINT uiMessage, WPARAM wparam, LPARAM lparam, int32_t epriority)
   {
      message * pmessage = new message;
      pmessage->m_pui = puie;
      pmessage->m_uiMessage = uiMessage;
      pmessage->m_wparam = wparam;
      pmessage->m_lparam = lparam;
      __begin_thread(puie->get_app(), &ThreadProcSendMessage, pmessage, epriority);
   }

}
