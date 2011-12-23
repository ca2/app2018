#include "StdAfx.h"


namespace window_frame 
{


   SysMenuManager::SysMenuManager(WorkSet * pwf)
   {
      m_pworkset = pwf;
      m_enumState = state_initial;
   }

   SysMenuManager::~SysMenuManager()
   {

   }

   void SysMenuManager::relay_event(gen::signal_object * pobj)
   {
       
      SCAST_PTR(gen::message::base, pbase, pobj);

      if(m_pworkset->GetEventWindow() == NULL || pbase->m_pwnd != m_pworkset->GetEventWindow())
         return;
       if(pbase->m_uiMessage == WM_RBUTTONDOWN)
       {
           m_enumState = StateMBDown;
       }
       else if(pbase->m_uiMessage == WM_RBUTTONUP)
       {
           if(m_enumState == StateMBDown)
           {
//               ::user::interaction * pwnd = m_pworkset->GetEventWindow();

   /*            ::userbase::menu * pmenu = pwnd->GetSystemMenu(FALSE);
            if(pmenu != NULL)
            {
               pmenu->TrackPopupMenu(
                  TPM_LEFTALIGN |
                  TPM_RIGHTBUTTON,
                  lpMsg->pt.x, lpMsg->pt.y,
                  pwnd);
            }*/
               pbase->m_bRet = true;
               return;
           }
           m_enumState = state_initial;
       }
   }

   void SysMenuManager::message_handler(::user::interaction * pwnd, gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pwnd);
      SCAST_PTR(gen::message::base, pbase, pobj);
      switch(pbase->m_uiMessage)
      {
      case WM_LBUTTONDBLCLK:
      {
         point point((DWORD)pbase->m_lparam);
         if(OnLButtonDblClk(pbase->m_wparam, point))
         {
            pbase->set_lresult(0);
            pbase->m_bRet = true;
            return;
         }
         break;
      }
      }

       if(pbase->m_uiMessage == WM_RBUTTONDOWN)
       {
           m_enumState = StateMBDown;
       }
       if(pbase->m_uiMessage == WM_RBUTTONDOWN)
       {
           m_enumState = StateMBDown;
       }
       else if(pbase->m_uiMessage == WM_RBUTTONUP)
       {
           if(m_enumState == StateMBDown)
           {
//               ::user::interaction * pwnd = m_pworkset->GetEventWindow();

   /*            ::userbase::menu * pmenu = pwnd->GetSystemMenu(FALSE);
            if(pmenu != NULL)
            {
               point pt(LOWORD(lparam), HIWORD(lparam));
               pwnd->ClientToScreen(&pt);
               pmenu->TrackPopupMenu(
                  TPM_LEFTALIGN |
                  TPM_RIGHTBUTTON,
                  pt.x, pt.y,
                  pwnd);
            }*/
               pbase->m_bRet = true;
               return;
           }
           m_enumState = state_initial;
       }
   }

   bool SysMenuManager::OnLButtonDblClk(UINT nFlags, point point)
   {
      UNREFERENCED_PARAMETER(nFlags);
      UNREFERENCED_PARAMETER(point);
      if(m_pworkset == NULL
         || m_pworkset->m_pappearance == NULL)
         return false;
      /*xxx appearance::EElement eelement;
      if(m_pworkset->m_pappearance->hit_test(point, eelement))
      {
         if(eelement == appearance::ElementTopLeftIcon)
         {
            m_pworkset->WindowClose();
            return true;
         }
      }*/
      return false;

   }

   bool SysMenuManager::update(WorkSet * pset)
   {
      m_pworkset = pset;
      return true;
   }


} // namespace window_frame



