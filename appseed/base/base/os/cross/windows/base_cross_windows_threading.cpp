﻿// thank you white heads (Rus, Slovk, Polsk... etc... ) Thank you CCamilo Sasuke Tsumanuma!
// Livecoding.tv Rheghal, szulak, eprykak..
// Somewhere Camilo Sasuke Tsumanuma
// My Mummi
// 2015-08-11 16:00:00

#include "framework.h"
#include "base/user/user/user_interaction.h"


#if defined(LINUX) || defined(VSNORD) || defined(METROWIN)

DWORD dwDebugPostMessageTime;

CLASS_DECL_BASE::user::interaction * oswindow_interaction(oswindow);

CLASS_DECL_BASE int_bool PostMessageW(oswindow oswindow,UINT Msg,WPARAM wParam,LPARAM lParam)
{

   ::user::interaction * pui = oswindow_interaction(oswindow);

   if(pui == NULL)
      return FALSE;

   if(pui->m_bDestroying)
      return FALSE;

   IDTHREAD h = pui->m_pauraapp->get_os_int();

   if(h == 0)
      return FALSE;

   mq * pmq = __get_mq(h, oswindow != NULL || Msg != WM_QUIT);

   if(pmq == NULL)
      return FALSE;

   synch_lock ml(&pmq->m_mutex);

   MESSAGE msg;

   DWORD dwNow = get_tick_count();

   if(dwNow - dwDebugPostMessageTime < 10)
   {

      writeln("PostMessage flooded?");

   }
   else
   {

      dwDebugPostMessageTime = dwNow;

   }

   //zero(&msg, sizeof(msg));

   msg.hwnd = oswindow;
   msg.message = Msg;
   msg.wParam = wParam;
   msg.lParam = lParam;
   msg.pt.x = 0x80000000;
   msg.pt.y = 0x80000000;

   pmq->ma.add(msg);

   pmq->m_eventNewMessage.set_event();

   return true;

}


CLASS_DECL_BASE int_bool mq_remove_window_from_all_queues(oswindow oswindow)
{
   
   ::user::interaction * pui = oswindow_interaction(oswindow);

   if(pui == NULL)
      return FALSE;

   if(pui->m_pauraapp == NULL)
      return false;

   IDTHREAD h = pui->m_pauraapp->get_os_int();

   if(h == 0)
      return FALSE;


   mq * pmq = __get_mq(h, false);

   if(pmq == NULL)
      return FALSE;

   synch_lock ml(&pmq->m_mutex);

   pmq->ma.pred_remove([=](MESSAGE & item) {
      return item.hwnd == oswindow;
   });

   return TRUE;

}


#endif
