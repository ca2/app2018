#include "framework.h"
//#include <sched.h>
//#include <time.h>
//#include <pthread.h>



bool defer_process_x_message(HTHREAD hthread, LPMESSAGE lpMsg, oswindow interaction_impl, bool bPeek)
{
#ifndef ANDROID


   if(hthread == NULL || hthread->m_pthread == NULL || hthread->get_x_window_count() <= 0)
      return false;

   synch_lock sl(&user_mutex());

   bool bRet = false;

   bool bContinue = true;

   while(bContinue && !bRet)
   {

      bContinue = false;

      for(int i = 0; i < ::oswindow_data::s_pdataptra->get_count() && !bRet; i++)
      {

         ::oswindow_data * pdata = ::oswindow_data::s_pdataptra->element_at(i);

         if(pdata == NULL || pdata->m_bMessageOnlyWindow)
            continue;

         if(pdata->m_hthread != hthread && g_oswindowDesktop != pdata)
            continue;

         Display * display = pdata->display();

         if(display == NULL)
            continue;


         xdisplay d(display);

         XEvent e;

         if(XPending(display))
         {

            bContinue = false;

            XNextEvent(display, &e);

            if(e.type == Expose)
            {

               lpMsg->message       = WM_PAINT;
               lpMsg->hwnd          = oswindow_get(display, e.xbutton.interaction_impl);
               lpMsg->lParam        = 0;
               lpMsg->wParam        = 0;

               bRet                 = true;

            }
            else if(e.type == ConfigureNotify)
            {
               if(e.xconfigure.interaction_impl == g_oswindowDesktop->interaction_impl())
               {
                  for(int j = 0; j < ::oswindow_data::s_pdataptra->get_count(); j++)
                  {
                     if(j == i)
                        continue;
                     PostMessage(::oswindow_data::s_pdataptra->element_at(j), WM_DISPLAYCHANGE, 0, 0);
                  }
                  continue;
               }
               //               XClearWindow(w.display(), w.interaction_impl());
            }
            else if(e.type == ButtonPress || e.type == ButtonRelease)
            {

               bRet                 = true;

               if(e.xbutton.type == ButtonPress)
               {
                  if(e.xbutton.button == Button1)
                  {
                     lpMsg->message = WM_LBUTTONDOWN;
                  }
                  else if(e.xbutton.button == Button2)
                  {
                     lpMsg->message = WM_MBUTTONDOWN;
                  }
                  else if(e.xbutton.button == Button3)
                  {
                     lpMsg->message = WM_RBUTTONDOWN;
                  }
                  else
                  {
                     bRet = false;
                  }

               }
               else if(e.xbutton.type == ButtonRelease)
               {
                  if(e.xbutton.button == Button1)
                  {
                     lpMsg->message = WM_LBUTTONUP;
                  }
                  else if(e.xbutton.button == Button2)
                  {
                     lpMsg->message = WM_MBUTTONUP;
                  }
                  else if(e.xbutton.button == Button3)
                  {
                     lpMsg->message = WM_RBUTTONUP;
                  }
                  else
                  {
                     bRet = false;
                  }

               }
               else
               {

                  bRet = false;

               }


               if(bRet)
               {

                  lpMsg->hwnd          = oswindow_get(display, e.xbutton.interaction_impl);
                  lpMsg->wParam        = 0;
                  lpMsg->lParam        = MAKELONG(e.xbutton.x_root, e.xbutton.y_root);

               }

            }
            else if(e.type == KeyPress || e.type == KeyRelease)
            {

               oswindow w = oswindow_get(display, e.xexpose.interaction_impl);

               bRet                 = true;

               if(e.xkey.type == KeyPress)
               {

                  lpMsg->message = WM_KEYDOWN;

               }
               else if(e.xkey.type == KeyRelease)
               {

                  lpMsg->message = WM_KEYUP;

               }
               else
               {

                  bRet = false;

               }

               lpMsg->hwnd          = oswindow_get(display, e.xbutton.interaction_impl);
               lpMsg->wParam        = e.xkey.keycode;
               lpMsg->lParam        = MAKELONG(0, e.xkey.keycode);



            }
            else if(e.type == MotionNotify)
            {

               lpMsg->hwnd          = oswindow_get(display, e.xbutton.interaction_impl);
               lpMsg->message       = WM_MOUSEMOVE;
               lpMsg->wParam        = 0;
               lpMsg->lParam        = MAKELONG(e.xmotion.x_root, e.xmotion.y_root);

               bRet                 = true;

            }
            else if(e.type == DestroyNotify)
            {

               lpMsg->hwnd          = oswindow_get(display, e.xdestroywindow.interaction_impl);
               lpMsg->message       = WM_DESTROY;

               bRet                 = true;

            }

            if(bPeek && bRet)
               XPutBackEvent(display, &e);



         }

         if(bRet && lpMsg->hwnd->interaction_impl() != None)
         {

            if(lpMsg->hwnd->m_hthread != hthread)
            {

               bRet = false;

               bContinue = true;

               PostMessage(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);

            }

         }

      }

   }

   return bRet;

#endif

   return false;

}

//
//
//
//void get_os_priority(int32_t * piPolicy, sched_param * pparam, int32_t nCa2Priority)
//{
//
//   int iOsPolicy;
//
//   int iCa2Min;
//
//   int iCa2Max;
//
///*   if(nCa2Priority == ::multithreading::priority_normal)
//   {
//
//      iOsPolicy = SCHED_OTHER;
//
//      iCa2Min = (int) ::multithreading::priority_normal;
//
//      iCa2Max = (int) ::multithreading::priority_normal;
//
//   }*/
//   if(nCa2Priority > ::multithreading::priority_normal)
//   {
//
//      iOsPolicy = SCHED_RR;
//
//      iCa2Min = (int) ::multithreading::priority_normal;
//
//      iCa2Max = 99;
//
//   }
//   else
//   {
//
//      //iOsPolicy = SCHED_IDLE;
//      iOsPolicy = SCHED_OTHER;
//
//      iCa2Min = 0;
//
//      iCa2Max = (int) ::multithreading::priority_normal;
//
//   }
//
//   int iOsMax = sched_get_priority_max(iOsPolicy);
//
//   int iOsMin = sched_get_priority_min(iOsPolicy);
//
//   int iOsPriority;
//
//   if(iCa2Min == iCa2Max)
//   {
//      iOsPriority = 0;
//   }
//   else
//   {
//      iOsPriority = (((nCa2Priority - iCa2Min)  * (iOsMax - iOsMin)) / (iCa2Max - iCa2Min)) + iOsMin;
//   }
//
//   iOsPriority = MAX(iOsMin, MIN(iOsMax, iOsPriority));
//
//   *piPolicy = iOsPolicy;
//
//   pparam->sched_priority = iOsPriority;
//
//}
//
//int32_t get_scheduling_priority(int32_t iOsPolicy, const sched_param * pparam)
//{
//
//   int iCa2Min;
//
//   int iCa2Max;
//
//
//   if(iOsPolicy == SCHED_RR)
//   {
//
//      iCa2Min = (int) ::multithreading::priority_normal;
//
//      iCa2Max = 99;
//
//   }
///*   else if(iOsPolicy == SCHED_IDLE)
//   {
//
//      iCa2Min = 0;
//
//      iCa2Max = (int) ::multithreading::priority_normal;
//
//   }*/
//   else
//   {
//
//      //iCa2Min = (int) ::multithreading::priority_normal;
//      iCa2Min = (int) 0;
//
//      iCa2Max = (int) ::multithreading::priority_normal;
//
//   }
//
//   int iOsMax = sched_get_priority_max(iOsPolicy);
//
//   int iOsMin = sched_get_priority_min(iOsPolicy);
//
//   int iCa2Priority;
//
//   if(iOsMax == iOsMin)
//   {
//      iCa2Priority = (int32_t) ::multithreading::priority_normal;
//   }
//   else
//   {
//      iCa2Priority = (((pparam->sched_priority - iOsMin)  * (iCa2Max - iCa2Min)) / (iOsMax - iOsMin)) + iCa2Min;
//   }
//
//   iCa2Priority = MAX(iCa2Min, MIN(iCa2Max, iCa2Priority));
//
//   return iCa2Priority;
//
//}
//
//
//void thread_get_os_priority(int32_t * piPolicy, sched_param * pparam, int32_t nCa2Priority)
//{
//
//   get_os_priority(piPolicy, pparam, nCa2Priority);
//
//}
//
//
//int32_t thread_get_scheduling_priority(int32_t iOsPolicy, const sched_param * pparam)
//{
//
//   return get_scheduling_priority(iOsPolicy, pparam);
//
//}
//
//
//void process_get_os_priority(int32_t * piPolicy, sched_param * pparam, int32_t nCa2Priority)
//{
//
//   get_os_priority(piPolicy, pparam, nCa2Priority);
//
//}
//
//
//int32_t process_get_scheduling_priority(int32_t iOsPolicy, const sched_param * pparam)
//{
//
//   return get_scheduling_priority(iOsPolicy, pparam);
//
//}
//
//
//
//
//
//
//namespace core
//{
//
//   CLASS_DECL_AURA bool set_priority_class(int32_t priority)
//   {
//
//      int32_t iPolicy = SCHED_OTHER;
//
//      sched_param schedparam;
//
//      schedparam.sched_priority = 0;
//
//      get_os_priority(&iPolicy, &schedparam, priority);
//
//      sched_setscheduler(0, iPolicy, &schedparam);
//
//      return true;
//
//   }
//
//
//
//
//} // namespace core


#if !defined(VSNORD) || defined(__arm__)

::thread_object < _TEB > t_pteb;

struct _TEB * WINAPI NtCurrentTeb(void)
{

   return t_pteb;

}

#endif






/*
bool on_init_thread()
{

   if(!__os_init_thread())
   {

      return false;

   }

   return true;

}


bool on_term_thread()
{

   bool bOk1 = __os_term_thread();

   return bOk1;

}





void __node_init_multithreading()
{

}



void __node_term_multithreading()
{

}

*/


thread_int_ptr < HRESULT > t_hresultCoInitialize;


bool __os_init_thread()
{

   __clear_mq(false);

   return true;

}



bool __os_term_thread()
{

   __clear_mq(true);

   thread_shutdown();

   return true;

}


int get_current_process_affinity_order()
{

   return get_processor_count();

}

