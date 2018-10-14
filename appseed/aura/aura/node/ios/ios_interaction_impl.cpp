#include "framework.h"
#include "ios.h"
#include "aura/user/user/user_interaction.h"
#include "aura/user/user/user.h"


void ns_main_async(dispatch_block_t block);


strsize str_begins_common(const string & str1, const string & str2)
{

   strsize i = 0;

   for(i = 0; i < str1.get_length() && i < str2.get_length(); i++)
   {

      if(str1[i] != str2[i])
      {

         break;

      }

   }

   return i;

}

strsize str_begins_eat_common(string & str1, string & str2, string & str)
{

   strsize i = 0;

   for(i = 0; i < str1.get_length() && i < str2.get_length(); i++)
   {

      if(str1[i] != str2[i])
      {

         break;

      }

   }

   str = str1.Left(i);

   str1 = str1.Mid(i);

   str2 = str2.Mid(i);

   return i;

}


strsize str_ends_common(const string & str1, const string & str2)
{

   strsize i = 0;

   strsize i1 = str1.get_length() - 1;

   strsize i2 = str2.get_length() - 1;

   for(; i1 >= 0 && i2 >= 0; i1--, i2--)
   {

      if(str1[i1] != str2[i2])
      {

         break;

      }

      i++;

   }

   return i;

}


strsize str_ends_eat_common(string & str1, string & str2, string & str)
{

   strsize i = 0;

   strsize i1 = str1.get_length() - 1;

   strsize i2 = str2.get_length() - 1;

   for(; i1 >= 0 && i2 >= 0; i1--, i2--)
   {

      if(str1[i1] != str2[i2])
      {

         break;

      }

      i++;

   }

   if(i1 >= 0)
   {

      str = str1.Mid(i1 + 1);

      str1 = str1.Left(i1 + 1);

      str2 = str2.Mid(i2 + 1);

   }

   return i;

}


void process_process_new_text(
string & strInsert,
strsize & iInsBeg,
strsize & iInsEnd,
strsize & iSelBeg,
strsize & iSelEnd,
string strPrevious,
string strNow)
{

   string str;

   strsize iBeg = str_begins_common(strPrevious, strNow);

   strsize iEnd = str_ends_common(strPrevious, strNow);

   iSelBeg = iBeg;

   iInsBeg = iBeg;

   iInsEnd = strPrevious.get_length() - iEnd;

   strInsert = strNow.Mid(iBeg, strNow.get_length() - iEnd - iBeg);

   iSelEnd = iBeg + strInsert.get_length();

   const char * psz = ::str::utf8_inc(strInsert);

   if(psz == NULL || *psz == '\0')
   {

      iSelBeg = iSelEnd;

   }

}

WINBOOL SetWindowRect(oswindow hwnd, LPRECT lprect);

WINBOOL PeekMessage(
LPMESSAGE lpMsg,
oswindow hWnd,
UINT wMsgFilterMin,
UINT wMsgFilterMax,
UINT wRemoveMsg);


WINBOOL GetMessage(
LPMESSAGE lpMsg,
oswindow hWnd,
UINT wMsgFilterMin,
UINT wMsgFilterMax);

namespace ios
{




   interaction_impl::interaction_impl() :
      ::aura::timer_array(get_app())
   {

      //set_handle(NULL);
      //      m_pui->m_nFlags    = 0;
      //m_pfnSuper         = NULL;
      //m_bMouseHover        = false;
      m_oswindow           = NULL;
      m_proundwindow = NULL;

   }


   void interaction_impl::construct(oswindow hWnd)
   {

      set_handle(hWnd);
      //m_bMouseHover        = false;
      m_oswindow           = NULL;
      m_proundwindow = NULL;
      m_bCanBecomeFirstResponder = false;

   }


   interaction_impl::interaction_impl(::aura::application * papp) :
      ::object(papp),
      ::aura::timer_array(papp)
   {

      set_handle(NULL);
      m_proundwindow = NULL;
      m_bCanBecomeFirstResponder = false;

   }


   interaction_impl::~interaction_impl()
   {


   }




   ::user::interaction_impl *  interaction_impl::from_os_data(void * pdata)
   {

      return from_handle((oswindow) pdata);

   }


   void * interaction_impl::get_os_data() const
   {

      return ((oswindow &) m_oswindow);

   }



   // Change a window's style

   /*__STATIC bool CLASS_DECL_BASE __modify_style(oswindow hWnd, int32_t nStyleOffset,
    DWORD dwRemove, DWORD dwAdd, UINT nFlags)
    {
    ASSERT(hWnd != NULL);
    DWORD dwStyle = ::GetWindowLong(hWnd, nStyleOffset);
    DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
    if (dwStyle == dwNewStyle)
    return FALSE;

    ::SetWindowLong(hWnd, nStyleOffset, dwNewStyle);
    if (nFlags != 0)
    {
    ::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
    SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
    }
    return TRUE;
    }*/

   /*
      bool  interaction_impl::ModifyStyle(oswindow hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
      {

         //      _throw(todo(get_app()));

         if(!::is_window(hWnd))
            return false;

         DWORD dw = hWnd->get_window_long(GWL_STYLE);

         dw &= ~dwRemove;

         dw |= dwAdd;

         hWnd->set_window_long(GWL_STYLE, dw);

         //return __modify_style(hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);

         return true;

      }


      bool interaction_impl::ModifyStyleEx(oswindow hWnd, DWORD dwRemove, DWORD dwAdd, UINT nFlags)
      {

         //      _throw(todo(get_app()));

         if(!::is_window(hWnd))
            return false;

         DWORD dw = hWnd->get_window_long(GWL_EXSTYLE);

         dw &= ~dwRemove;

         dw |= dwAdd;

         hWnd->set_window_long(GWL_EXSTYLE, dw);

         return true;

         //      return __modify_style(hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);

      }

   */

   const MESSAGE* PASCAL interaction_impl::GetCurrentMessage()
   {

      return NULL;

   }

   LRESULT interaction_impl::Default()
   {

      return 0;

   }


   ::user::interaction_impl * interaction_impl::from_handle(oswindow oswindow)
   {

      if(oswindow == NULL)
         return NULL;

      return oswindow->m_pimpl;

   }


   ::user::interaction_impl * PASCAL interaction_impl::FromHandlePermanent(oswindow oswindow)
   {

      if(oswindow == NULL)
         return NULL;

      return oswindow->m_pimpl;

   }


   bool interaction_impl::Attach(oswindow hWndNew)
   {

      ASSERT(get_handle() == NULL);     // only attach once, detach on destroy
      //  ASSERT(FromHandlePermanent(hWndNew) == NULL);
      // must not already be in permanent ::collection::map

      if (hWndNew == NULL)
         return FALSE;
      //single_lock sl(afxMutexHwnd(), TRUE);
      //hwnd_map * pMap = afxMapHWND(TRUE); // create ::collection::map if not exist
      //ASSERT(pMap != NULL);

      //pMap->set_permanent(set_handle(hWndNew), this);
      //if(m_pui == NULL)
      {
         //m_pui = this;
      }

      m_oswindow = hWndNew;

      return TRUE;

   }

   oswindow interaction_impl::Detach()
   {
      oswindow hWnd = (oswindow) get_handle();
      if (hWnd != NULL)
      {
         //         single_lock sl(afxMutexHwnd(), TRUE);
         //  ;;       hwnd_map * pMap = afxMapHWND(); // don't create if not exist
         //     if (pMap != NULL)
         //      pMap->remove_handle(get_handle());
         //         set_handle(NULL);
         m_oswindow = NULL;
      }

      return hWnd;
   }

   void interaction_impl::pre_subclass_window()
   {
      // no default processing
   }


   /////////////////////////////////////////////////////////////////////////////
   // user::interaction creation

   bool interaction_impl::create_window_ex(
                                           ::user::interaction * pui,
                                           ::user::create_struct & cs,
                                           ::user::interaction * pParentWnd,
                                           id id)
   {

      if(!native_create_window_ex(pui, cs,
                                  pParentWnd == NULL ? NULL : pParentWnd->get_safe_handle(), id))
      {
         
         return false;
         
      }


      return true;

   }


   bool interaction_impl::native_create_window_ex(
                                                  ::user::interaction * pui,
                                                  ::user::create_struct & cs,
                                                  oswindow oswindowParent,
                                                  id id)
   {

      if(::is_window(get_handle()))
      {

         DestroyWindow();

      }

      m_pui = pui;

      ENSURE_ARG(cs.lpszName == NULL || __is_valid_string(cs.lpszName));

      if(m_pui != NULL)
      {

         if(!m_pui->pre_create_window(cs))
         {

            PostNcDestroy();

            return FALSE;

         }

      }
      else
      {

         if (!pre_create_window(cs))
         {

            PostNcDestroy();

            return FALSE;

         }

      }

      if(cs.hwndParent == NULL)
      {

         cs.style &= ~WS_CHILD;

      }
      
      ::rect r;
      
      cs.get_rect(r);

      CGRect rect;

      copy(rect, r);

      if(oswindowParent == HWND_MESSAGE)
      {

         return true;

      }
      else
      {

         m_oswindow = oswindow_get(new_round_window(this, rect));

         ::copy(&m_rectParentClient, r);

         m_spgraphics.alloc(allocer());

         m_spgraphics->on_create_window(this);

         m_oswindow->set_user_interaction_impl(this);

      }

      send_message(WM_CREATE, 0, (LPARAM) &cs);

      return TRUE;

   }

   // for child windows
   bool interaction_impl::pre_create_window(::user::create_struct& cs)
   {
      /*      if (cs.lpszClass == NULL)
       {
       // make sure the default user::interaction class is registered
       VERIFY(__end_defer_register_class(__WND_REG, &cs.lpszClass));

       // no WNDCLASS provided - use child user::interaction default
       ASSERT(cs.style & WS_CHILD);
       }*/
      return true;
   }

   bool interaction_impl::create_window(::user::interaction * pui,const char * lpszClassName,
                                        const char * lpszWindowName, DWORD dwStyle,
                                        const RECT& rect,
                                        ::user::interaction *  pParentWnd, id id,
                                        ::create * pContext)
   {
      // can't use for desktop or pop-up windows (use CreateEx instead)
      ASSERT(pParentWnd != NULL);
      ASSERT((dwStyle & WS_POPUP) == 0);
      
      ::user::create_struct cs(0, lpszClassName, lpszWindowName, dwStyle | WS_CHILD,
                               rect, (LPVOID) pContext);

      return create_window_ex(pui, cs, pParentWnd, id);
   }


   bool interaction_impl::initialize_native_window(::user::native_window_initialize * pinitialize)
   {

      CGRect rect;


      rect.origin.x = pinitialize->m_rect.left;
      rect.origin.y = pinitialize->m_rect.top;
      rect.size.width = width(pinitialize->m_rect);
      rect.size.height = height(pinitialize->m_rect);

      m_rectParentClientRequest = pinitialize->m_rect;

      m_oswindow = oswindow_get(new_round_window(this, rect));

      install_message_routing(this);

      m_spgraphics.alloc(allocer());

      m_spgraphics->on_create_window(this);

      m_oswindow->set_user_interaction_impl(this);

      m_pui->ModifyStyle(0, WS_VISIBLE);

      m_pui->send_message(WM_CREATE);

      m_pui->send_message(WM_SIZE);

      m_pui->send_message(WM_MOVE);

      m_pui->send_message(WM_SHOWWINDOW, 1);

      return true;

   }


   bool interaction_impl::create_message_queue(::user::interaction * pui,const char * pszName)
   {
      
      if(IsWindow())
      {

         set_window_text(pszName);

      }
      else
      {
         
         ::user::create_struct cs(0, NULL, pszName, WS_CHILD);
         
         if(!native_create_window_ex(pui, cs, HWND_MESSAGE, pszName))
         {
            
            return false;
            
         }
         
      }
      
      return true;
      
   }


   void interaction_impl::install_message_routing(::message::sender * pinterface)
   {

      last_install_message_routing(pinterface);

      ::user::interaction_impl::install_message_routing(pinterface);
      //m_pbuffer->InstallMessageHandling(pinterface);
      IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &interaction_impl::_001OnDestroy);
      IGUI_MSG_LINK(WM_NCDESTROY, pinterface, this, &interaction_impl::_001OnNcDestroy);
      IGUI_MSG_LINK(WM_PAINT, pinterface, this, &interaction_impl::_001OnPaint);
      IGUI_MSG_LINK(WM_PRINT, pinterface, this, &interaction_impl::_001OnPrint);

      if(m_pui != NULL)
      {

         m_pui->install_message_routing(pinterface);

      }

      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &interaction_impl::_001OnCreate);
      IGUI_MSG_LINK(WM_SETCURSOR, pinterface, this, &interaction_impl::_001OnSetCursor);
      IGUI_MSG_LINK(WM_ERASEBKGND, pinterface, this, &interaction_impl::_001OnEraseBkgnd);
      IGUI_MSG_LINK(WM_MOVE, pinterface, this, &interaction_impl::_001OnMove);
      IGUI_MSG_LINK(WM_SIZE, pinterface, this, &interaction_impl::_001OnSize);
      IGUI_MSG_LINK(WM_SETFOCUS, pinterface, this, &interaction_impl::_001OnSetFocus);
      IGUI_MSG_LINK(WM_KILLFOCUS, pinterface, this, &interaction_impl::_001OnKillFocus);
//      IGUI_MSG_LINK(WM_SHOWWINDOW        , pinterface, this, &interaction_impl::_001OnShowWindow);
//      IGUI_MSG_LINK(ca2m_PRODEVIAN_SYNCH , pinterface, this, &interaction_impl::_001OnProdevianSynch);
      //      //IGUI_MSG_LINK(WM_TIMER             , pinterface, this, &interaction_impl::_001OnTimer);

      prio_install_message_routing(pinterface);

   }


   void interaction_impl::_001OnMove(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);

   }

   void interaction_impl::_001OnSize(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);

      size sizeRequest = m_rectParentClientRequest.get_size();

      for(auto & pui : m_pui->m_uiptraChild)
      {

         pui->ResizeWindow(sizeRequest);

      }



   }



   void interaction_impl::_001OnDestroy(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      Default();

   }


   // WM_NCDESTROY is the absolute LAST message sent.
   void interaction_impl::_001OnNcDestroy(::message::message * pobj)
   {

      single_lock sl(m_papp == NULL ? NULL : m_papp->m_pmutex, TRUE);

      pobj->m_bRet = true;

      ::thread* pThread = ::get_thread();

      if (pThread != NULL)
      {

         if (pThread->get_active_ui() == m_pui)
         {

            pThread->set_active_ui(NULL);

         }

      }

      round_window_destroy();

      Detach();

      ASSERT(get_handle() == NULL);

      PostNcDestroy();

      if(m_pui != NULL)
      {

         m_pui->PostNcDestroy();

      }

   }


   void interaction_impl::PostNcDestroy()
   {
      //set_handle(NULL);
      //      m_oswindow.post_nc_destroy();
      // default to nothing
   }

   void interaction_impl::on_final_release()
   {
      if (get_handle() != NULL)
         DestroyWindow();    // will call PostNcDestroy
      else
         PostNcDestroy();
   }

   void interaction_impl::assert_valid() const
   {
      if (get_handle() == NULL)
         return;     // null (unattached) windows are valid

      // check for special wnd??? values
      //      ASSERT(oswindow_TOP == NULL);       // same as desktop
      /*      if (get_handle() == oswindow_BOTTOM)
       {
       }
       else if (get_handle() == oswindow_TOPMOST)
       {
       }
       else if (get_handle() == oswindow_NOTOPMOST)
       {
       }
       else
       {
       // should be a normal user::interaction
       ASSERT(::is_window(get_handle()));

       // should also be in the permanent or temporary handle ::collection::map
       single_lock sl(afxMutexHwnd(), TRUE);
       hwnd_map * pMap = afxMapHWND();
       if(pMap == NULL) // inside thread not having windows
       return; // let go
       ASSERT(pMap != NULL);

       //         ::ca2::object* p=NULL;
       if(pMap)
       {
       ASSERT( (p = pMap->lookup_permanent(get_handle())) != NULL ||
       (p = pMap->lookup_temporary(get_handle())) != NULL);
       }*/

      //ASSERT(dynamic_cast < ::user::interaction *  > (p) == this);   // must be us

      // Note: if either of the above asserts fire and you are
      // writing a multithreaded application, it is likely that
      // you have passed a C++ object from one thread to another
      // and have used that object in a way that was not intended.
      // (only simple inline wrapper functions should be used)
      //
      // In general, user::interaction objects should be passed by oswindow from
      // one thread to another.  The receiving thread can wrap
      // the oswindow with a user::interaction object by using ::macos::interaction_impl::from_handle.
      //
      // It is dangerous to pass C++ objects from one thread to
      // another, unless the objects are designed to be used in
      // such a manner.
      //      }*/
   }


   void interaction_impl::dump(dump_context & dumpcontext) const
   {
      ::object::dump(dumpcontext);

      dumpcontext << "\nm_hWnd = " << (void *)get_handle();

      /*      if (get_handle() == NULL || get_handle() == oswindow_BOTTOM ||
       get_handle() == oswindow_TOPMOST || get_handle() == oswindow_NOTOPMOST)
       {
       // not a normal user::interaction - nothing more to dump
       return;
       }*/

      /*      if (!::is_window(get_handle()))
       {
       // not a valid user::interaction
       dumpcontext << " (illegal oswindow)";
       return; // don't do anything more
       }*/

      ::user::interaction_impl * pWnd = (::user::interaction_impl *) this;
      if (pWnd != this)
         dumpcontext << " (Detached or temporary user::interaction)";
      else
         dumpcontext << " (permanent user::interaction)";

      // dump out user::interaction specific statistics
      char szBuf [64];
      //      if (!const_cast < user::interaction * > (this)->send_message(WM_QUERYAFXWNDPROC, 0, 0) && pWnd == this)
      //         ((::user::interaction *) this)->get_window_text(szBuf, _countof(szBuf));
      //    else
      //         ::DefWindowProc(get_handle(), WM_GETTEXT, _countof(szBuf), (LPARAM)&szBuf[0]);
      dumpcontext << "\ncaption = \"" << szBuf << "\"";

      //      ::GetClassName(get_handle(), szBuf, _countof(szBuf));
      //    dumpcontext << "\nclass name = \"" << szBuf << "\"";

      rect rect;
      ((::user::interaction_impl *) this)->GetWindowRect(&rect);
      dumpcontext << "\nrect = " << rect;
      dumpcontext << "\nparent ::user::interaction * = " << (void *)((::user::interaction_impl *) this)->GetParent();

      //      dumpcontext << "\nstyle = " << (void *)(dword_ptr)::GetWindowLong(get_handle(), GWL_STYLE);
      //    if (::GetWindowLong(get_handle(), GWL_STYLE) & WS_CHILD)
      //     dumpcontext << "\nid = " << __get_dialog_control_id(get_handle());

      dumpcontext << "\n";
   }


   bool interaction_impl::DestroyWindow()
   {

      single_lock sl(m_papp == NULL ? NULL : m_papp->m_pmutex, TRUE);

      if(get_handle() == NULL)
         return false;

      bool bResult = ::user::interaction_impl::DestroyWindow();

      return bResult;

   }


   LRESULT interaction_impl::DefWindowProc(UINT nMsg, WPARAM wparam, ::lparam lparam)
   {
      /*  if (m_pfnSuper != NULL)
       return ::CallWindowProc(m_pfnSuper, get_handle(), nMsg, wparam, lparam);

       WNDPROC pfnWndProc;
       if ((pfnWndProc = *GetSuperWndProcAddr()) == NULL)
       return ::DefWindowProc(get_handle(), nMsg, wparam, lparam);
       else
       return ::CallWindowProc(pfnWndProc, get_handle(), nMsg, wparam, lparam);*/

      return 0;
   }

   /*
    WNDPROC* interaction_impl::GetSuperWndProcAddr()
    {
    // Note: it is no longer necessary to override GetSuperWndProcAddr
    //  for each control class with a different WNDCLASS.
    //  This implementation now uses instance data, such that the previous
    //  WNDPROC can be anything.

    return &m_pfnSuper;
    }
    */
   void interaction_impl::pre_translate_message(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // no default processing
   }


   void interaction_impl::get_window_text(string & rString)
   {

      rString = m_strWindowText;

   }


   bool interaction_impl::GetWindowPlacement(WINDOWPLACEMENT* lpwndpl)
   {

      return false;

   }


   bool interaction_impl::SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl)
   {

      return false;

   }


   /////////////////////////////////////////////////////////////////////////////
   // user::interaction will delegate owner draw messages to self drawing controls

   // Drawing: for all 4 control types
   // /*   void interaction_impl::OnDrawItem(int32_t /*nIDCtl*/, LPDRAWITEMSTRUCT lpDrawItemStruct)
   // {

   // reflect notification to child user::interaction control
   //  if (ReflectLastMsg(lpDrawItemStruct->hwndItem))
   //   return;     // eat it

   // not handled - do default
   //      Default();
   // }

   // Drawing: for all 4 control types
   //   int32_t interaction_impl::OnCompareItem(int32_t /*nIDCtl*/, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
   // {
   //  // reflect notification to child user::interaction control
   //LRESULT lResult;
   //      if (ReflectLastMsg(lpCompareItemStruct->hwndItem, &lResult))
   //       return (int32_t)lResult;        // eat it

   // not handled - do default
   //  return (int32_t)Default();
   //   }

   // void interaction_impl::OnDeleteItem(int32_t /*nIDCtl*/, LPDELETEITEMSTRUCT lpDeleteItemStruct)
   //{
   // reflect notification to child user::interaction control
   // if (ReflectLastMsg(lpDeleteItemStruct->hwndItem))
   //  return;     // eat it
   // not handled - do default
   //      Default();
   // }

   bool interaction_impl::_EnableToolTips(bool bEnable, UINT nFlag)
   {
      UNREFERENCED_PARAMETER(bEnable);
      UNREFERENCED_PARAMETER(nFlag);
      return FALSE;
   }


   // Measure item implementation relies on unique control/menu IDs
   //   void interaction_impl::OnMeasureItem(int32_t /*nIDCtl*/, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
   /* {
    if (lpMeasureItemStruct->CtlType == ODT_MENU)
    {
    ASSERT(lpMeasureItemStruct->CtlID == 0);
    }
    else
    {
    ::user::interaction * pChild = GetDescendantWindow(lpMeasureItemStruct->CtlID);
    if (pChild != NULL && pChild->SendChildNotifyLastMsg())
    return;     // eaten by child
    }
    // not handled - do default
    Default();
    }*/

   /*   bool interaction_impl::GetWindowInfo(PWINDOWINFO pwi) const
    {
    ASSERT(::is_window((oswindow)get_handle()));
    return ::GetWindowInfo((oswindow)get_handle(), pwi) != FALSE;
    }*/

   /*   ::user::interaction * interaction_impl::GetAncestor(UINT gaFlags) const
    { ASSERT(::is_window((oswindow)get_handle())); return  ::macos::interaction_impl::from_handle(::GetAncestor((oswindow)get_handle(), gaFlags)); }

    */

   /*   bool interaction_impl::GetScrollBarInfo(LONG idObject, PSCROLLBARINFO psbi) const
    {
    ASSERT(::is_window((oswindow)get_handle()));
    ASSERT(psbi != NULL);
    return ::GetScrollBarInfo((oswindow)get_handle(), idObject, psbi) != FALSE;
    }
    */
   /*   bool interaction_impl::GetTitleBarInfo(PTITLEBARINFO pti) const
    {
    ASSERT(::is_window((oswindow)get_handle()));
    ASSERT(pti != NULL);
    return ::GetTitleBarInfo((oswindow)get_handle(), pti) != FALSE;
    }
    */
   /*   bool interaction_impl::AnimateWindow(DWORD dwTime, DWORD dwFlags)
    {
    ASSERT(::is_window((oswindow)get_handle()));
    return ::AnimateWindow((oswindow)get_handle(), dwTime, dwFlags) != FALSE;
    }

    bool interaction_impl::FlashWindowEx(DWORD dwFlags, UINT  uCount, DWORD dwTimeout)
    {
    ASSERT(::is_window((oswindow)get_handle()));
    FLASHWINFO fwi;
    fwi.cbSize = sizeof(fwi);
    fwi.hwnd = (oswindow)get_handle();
    fwi.dwFlags = dwFlags;
    fwi.uCount = uCount;
    fwi.dwTimeout = dwTimeout;

    return ::FlashWindowEx(&fwi) != FALSE;
    }
    */

   /*
    bool interaction_impl::SetLayeredWindowAttributes(COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
    {
    ASSERT(::is_window((oswindow)get_handle()));
    return ::SetLayeredWindowAttributes((oswindow)get_handle(), crKey, bAlpha, dwFlags) != FALSE;
    }

    bool interaction_impl::UpdateLayeredWindow(::draw2d::graphics * pDCDst, POINT *pptDst, SIZE *psize,
    ::draw2d::graphics * pDCSrc, POINT *pptSrc, COLORREF crKey, BLENDFUNCTION *pblend, DWORD dwFlags)
    {
    ASSERT(::is_window((oswindow)get_handle()));
    return ::UpdateLayeredWindow((oswindow)get_handle(), WIN_HDC(pDCDst), pptDst, psize,
    WIN_HDC(pDCSrc), pptSrc, crKey, pblend, dwFlags) != FALSE;
    }

    */
   /*
    bool interaction_impl::GetLayeredWindowAttributes(COLORREF *pcrKey, BYTE *pbAlpha, DWORD *pdwFlags) const
    {
    ASSERT(::is_window((oswindow)get_handle()));
    return ::GetLayeredWindowAttributes((oswindow)get_handle(), pcrKey, pbAlpha, pdwFlags) != FALSE;
    }

    bool interaction_impl::PrintWindow(::draw2d::graphics * pgraphics, UINT nFlags) const
    {
    ASSERT(::is_window((oswindow)get_handle()));
    return ::PrintWindow((oswindow)get_handle(), (HDC)(dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->get_handle(), nFlags) != FALSE;
    }

    */
   LRESULT interaction_impl::OnNTCtlColor(WPARAM wparam, LPARAM lparam)
   {
      return 0;
   }


   void interaction_impl::PrepareForHelp()
   {
      /*if (IsFrameWnd())
       {
       // frame_window windows should be allowed to exit help mode first
       frame_window* pFrameWnd = dynamic_cast < frame_window * >(this);
       pFrameWnd->ExitHelpMode();
       }

       // cancel any tracking modes
       send_message(WM_CANCELMODE);
       send_message_to_descendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

       // need to use top level parent (for the case where get_handle() is in DLL)
       ::user::interaction * pWnd = EnsureTopLevelParent();
       IOS_WINDOW(pWnd)->send_message(WM_CANCELMODE);
       IOS_WINDOW(pWnd)->send_message_to_descendants(WM_CANCELMODE, 0, 0, TRUE, TRUE);

       // attempt to cancel capture
       oswindow hWndCapture = ::GetCapture();
       if (hWndCapture != NULL)
       ::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);*/
   }


   /*void interaction_impl::WinHelpInternal(dword_ptr dwData, UINT nCmd)
    {
    UNREFERENCED_PARAMETER(dwData);
    UNREFERENCED_PARAMETER(nCmd);
    _throw(not_implemented(get_app()));

    application* pApp = &System;
    ASSERT_VALID(pApp);
    if (pApp->m_eHelpType == afxHTMLHelp)
    {
    // translate from WinHelp commands and data to to HtmlHelp
    ASSERT((nCmd == HELP_CONTEXT) || (nCmd == HELP_CONTENTS) || (nCmd == HELP_FINDER));
    if (nCmd == HELP_CONTEXT)
    nCmd = HH_HELP_CONTEXT;
    else if (nCmd == HELP_CONTENTS)
    nCmd = HH_DISPLAY_TOC;
    else if (nCmd == HELP_FINDER)
    nCmd = HH_HELP_FINDER;
    HtmlHelp(dwData, nCmd);
    }
    else
    WinHelp(dwData, nCmd);*/
   //}


   void interaction_impl::route_command_message(::user::command * pcommand)
   {

      command_target::route_command_message(pcommand);

   }


   void interaction_impl::on_control_event(::user::control_event * pevent)
   {

      UNREFERENCED_PARAMETER(pevent);

      return false;

   }

   void interaction_impl::_002OnDraw(::draw2d::graphics * pgraphics)
   {

      //      ::CallWindowProc(*GetSuperWndProcAddr(), get_handle(), WM_PRINT, (WPARAM)((dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->get_handle()), (LPARAM)(PRF_CHILDREN | PRF_CLIENT));

   }


   void interaction_impl::message_handler(::message::base * pbase)
   {


      if(pbase->m_id == WM_SIZE || pbase->m_id == WM_MOVE)
      {

         //         win_update_graphics();

      }

      if(pbase->m_id == WM_KEYDOWN || pbase->m_id == WM_KEYUP || pbase->m_id == WM_CHAR)
      {

         SCAST_PTR(::message::key, pkey, pbase);

         Session.translate_os_key_message(pkey);

         if(pbase->m_id == WM_KEYDOWN)
         {

            try
            {

               Session.set_key_pressed(pkey->m_ekey, true);

            }
            catch(...)
            {

            }

         }
         else if(pbase->m_id == WM_KEYUP)
         {

            try
            {

               Session.set_key_pressed(pkey->m_ekey, false);

            }
            catch(...)
            {

            }

         }

      }

      if(m_pui != NULL)
      {

         m_pui->pre_translate_message(pbase);

         if(pbase->m_bRet)
            return;

      }

      if(pbase->m_id == WM_TIMER)
      {
         //         m_papp->m_papp->step_timer();
      }
      else if(pbase->m_id == WM_LBUTTONDOWN)
      {
         //  g_pwndLastLButtonDown = this;
      }
      else if(pbase->m_id == WM_SIZE)
      {
         m_bUpdateGraphics = true;
      }
      /*      else if(pbase->m_id == CA2M_BERGEDGE)
       {
       if(pbase->m_wparam == BERGEDGE_GETAPP)
       {
       ::application ** ppapp= (::application **) pbase->m_lparam;
       *ppapp = get_app();
       pbase->m_bRet = true;
       return;
       }
       }*/
      pbase->set_lresult(0);

      if(pbase->m_id == WM_MOUSELEAVE)
      {

         _000OnMouseLeave(pbase);

         return;

      }

      if(pbase->m_id == WM_LBUTTONDOWN ||
            pbase->m_id == WM_LBUTTONUP ||
            pbase->m_id == WM_MBUTTONDOWN ||
            pbase->m_id == WM_MBUTTONUP ||
            pbase->m_id == WM_RBUTTONDOWN ||
            pbase->m_id == WM_RBUTTONUP ||
            pbase->m_id == WM_MOUSEMOVE ||
            pbase->m_id == WM_MOUSEMOVE)
         //         pbase->m_id == WM_MOUSEWHEEL)
      {
         if(pbase->m_id == WM_LBUTTONDOWN)
         {

            //            TRACE("WM_LBUTTONDOWN");

         }

         ::message::mouse * pmouse = (::message::mouse *) pbase;


         // user presence status activity reporting

         Session.on_ui_mouse_message(pmouse);


         if(m_papp->m_psession != NULL)
         {

            Session.m_ptCursor = pmouse->m_pt;

         }

         /*         if(m_pui != NULL && m_pui != this && m_pui->m_papp->m_psession != NULL && m_pui->m_papp->m_psession != m_papp->m_psession)
          {

          BaseSess(m_pui->m_papp->m_psession).m_ptCursor = pmouse->m_pt;

          }
          */

         if(m_bTranslateMouseMessageCursor && !pmouse->m_bTranslated)
         {

            pmouse->m_bTranslated = true;

            rect64 rectWindow;

            if(m_bScreenRelativeMouseMessagePosition)
            {
               class rect rectWindow32;
               //               ::GetWindowRect(get_handle(), &rectWindow32);
               ::copy(rectWindow, rectWindow32);
            }
            else
            {
               m_pui->GetWindowRect(rectWindow);
            }
            if(System.get_monitor_count() > 0)
            {
               rect rcMonitor;
               System.get_monitor_rect(0, &rcMonitor);
               if(rectWindow.left >= rcMonitor.left)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= rcMonitor.top)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
            else
            {
               if(rectWindow.left >= 0)
                  pmouse->m_pt.x += (LONG) rectWindow.left;
               if(rectWindow.top >= 0)
                  pmouse->m_pt.y += (LONG) rectWindow.top;
            }
         }

         if(pbase->m_id == WM_MOUSEMOVE)
         {
            // We are at the message handler procedure.
            // mouse messages originated from message handler and that are mouse move events should end up with the correct cursor.
            // So the procedure starts by setting to the default cursor,
            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
            // handler has set it to another one.
            pmouse->m_ecursor = visual::cursor_default;

         }

         _008OnMouse(pmouse);

         return;

      }
      else if(pbase->m_id == WM_KEYDOWN ||
              pbase->m_id == WM_KEYUP ||
              pbase->m_id == WM_CHAR)
      {

         ::message::key * pkey = (::message::key *) pbase;

         //         Application.keyboard().translate_os_key_message(pkey);
         /*
          if(pbase->m_id == WM_KEYDOWN)
          {
          try
          {
          Application.set_key_pressed(pkey->m_ekey, true);
          }
          catch(...)
          {
          }
          }
          else if(pbase->m_id == WM_KEYUP)
          {
          try
          {
          Application.set_key_pressed(pkey->m_ekey, false);
          }
          catch(...)
          {
          }
          }
          */

         ::user::interaction * puiFocus = dynamic_cast < ::user::interaction * > (Session.get_keyboard_focus());
         if(puiFocus != NULL
               && puiFocus->IsWindow()
               && puiFocus->GetTopLevel() != NULL)
         {
            puiFocus->send(pkey);
            if(pbase->m_bRet)
               return;
         }
         else if(!pkey->m_bRet)
         {
            if(m_pui != NULL)
            {
               m_pui->_000OnKey(pkey);
               if(pbase->m_bRet)
                  return;
            }
         }

         pbase->set_lresult(DefWindowProc((UINT) pbase->m_id, pbase->m_wparam, pbase->m_lparam));

         return;

      }

      if(pbase->m_id == ::message::message_event)
      {
         
         if(m_pui != NULL)
         {
            
            m_pui->on_control_event(pbase->m_lparam.cast < ::user::control_event >());
            
         }
         else
         {

            on_control_event(pbase->m_lparam.cast < ::user::control_event > ());
            
         }
         
      }

      route_message(pbase);

      if(pbase->m_bRet)
         return;

      pbase->set_lresult(DefWindowProc((UINT) pbase->m_id, pbase->m_wparam, pbase->m_lparam));

   }


   bool interaction_impl::OnCommand(WPARAM wparam, LPARAM lparam)
   // return TRUE if command invocation was attempted
   {
      UNREFERENCED_PARAMETER(wparam);
      UNREFERENCED_PARAMETER(lparam);
      /*   UINT nID = LOWORD(wparam);
       oswindow hWndCtrl = (oswindow)lparam;
       int32_t nCode = HIWORD(wparam);

       // default routing for command messages (through closure table)

       if (hWndCtrl == NULL)
       {
       // zero IDs for normal commands are not allowed
       if (nID == 0)
       return FALSE;

       // make sure command has not become disabled before routing
       probe_::user::command state;
       state.m_id = nID;
       _001OnCommand(nID, CN_UPDATE_::user::command, &state, NULL);
       if (!state.m_bEnabled)
       {
       TRACE(::ca2::trace::category_AppMsg, 0, "Warning: not executing disabled command %d\n", nID);
       return TRUE;
       }

       // menu or accelerator
       nCode = CN_COMMAND;
       }
       else
       {
       // control notification
       ASSERT(nID == 0 || ::is_window(hWndCtrl));

       if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
       return TRUE;        // locked out - ignore control notification

       // reflect notification to child user::interaction control
       if (ReflectLastMsg(hWndCtrl))
       return TRUE;    // eaten by child

       // zero IDs for normal commands are not allowed
       if (nID == 0)
       return FALSE;
       }

       #ifdef DEBUG
       if (nCode < 0 && nCode != (int32_t)0x8000)
       TRACE(::ca2::trace::category_AppMsg, 0, "Implementation Warning: control notification = $%X.\n",
       nCode);
       #endif

       return _001OnCommand(nID, nCode, NULL, NULL);*/
      return FALSE;
   }

   bool interaction_impl::OnNotify(WPARAM, LPARAM lparam, LRESULT* pResult)
   {
      /*    ASSERT(pResult != NULL);
       NMHDR* pNMHDR = (NMHDR*)lparam;
       oswindow hWndCtrl = pNMHDR->hwndFrom;

       // get the child ID from the user::interaction itself
       //      uint_ptr nID = __get_dialog_control_id(hWndCtrl);
       //      int32_t nCode = pNMHDR->code;

       ASSERT(hWndCtrl != NULL);
       ASSERT(::is_window(hWndCtrl));

       if (gen_ThreadState->m_hLockoutNotifyWindow == get_handle())
       return true;        // locked out - ignore control notification

       // reflect notification to child user::interaction control
       if (ReflectLastMsg(hWndCtrl, pResult))
       return true;        // eaten by child

       //      __NOTIFY notify;
       //    notify.pResult = pResult;
       //  notify.pNMHDR = pNMHDR;
       //xxx   return _001OnCommand((UINT)nID, MAKELONG(nCode, WM_NOTIFY), &notify, NULL);*/
      return false;
   }

   /////////////////////////////////////////////////////////////////////////////
   // user::interaction extensions

   /*

    sp(::user::frame_window) interaction_impl::GetParentFrame()
    {
    if (get_handle() == NULL) // no oswindow attached
    {
    return NULL;
    }

    ASSERT_VALID(this);

    ::user::interaction * pParentWnd = get_parent();  // start with one parent up
    while (pParentWnd != NULL)
    {
    if (pParentWnd->is_frame_window())
    {
    return dynamic_cast < ::user::frame_window * > (pParentWnd);
    }
    pParentWnd = pParentWnd->get_parent();
    }
    return NULL;
    }

    */

   /* trans oswindow CLASS_DECL_BASE __get_parent_owner(::user::interaction * hWnd)
    {
    // check for permanent-owned user::interaction first
    ::user::interaction * pWnd = ::macos::interaction_impl::FromHandlePermanent(hWnd);
    if (pWnd != NULL)
    return IOS_WINDOW(pWnd)->GetOwner();

    // otherwise, return parent in the oswindows sense
    return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ?
    ::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
    }*/


   /*

    ::user::interaction *  interaction_impl::GetTopLevel() const
    {
    if (get_handle() == NULL) // no oswindow attached
    return NULL;

    ASSERT_VALID(this);
    //
    //      ::user::interaction * hWndParent = this;
    //      ::user::interaction * hWndT;
    //while ((hWndT = __get_parent_owner(hWndParent)) != NULL)
    // hWndParent = hWndT;

    //      return hWndParent;
    return m_pui;

    }


    ::user::interaction *  interaction_impl::GetTopLevelOwner()
    {
    if (get_handle() == NULL) // no oswindow attached
    return NULL;

    ASSERT_VALID(this);

    //      oswindow hWndOwner = get_handle();
    //    oswindow hWndT;
    //  while ((hWndT = ::GetWindow(hWndOwner, GW_OWNER)) != NULL)
    //   hWndOwner = hWndT;

    //      return ::macos::interaction_impl::from_handle(hWndOwner);
    return NULL;
    }

    ::user::interaction *  interaction_impl::GetParentOwner()
    {
    if (get_handle() == NULL) // no oswindow attached
    return NULL;

    ASSERT_VALID(this);

         oswindow hWndParent = get_handle();
    oswindow hWndT;
    while ((::GetWindowLong(hWndParent, GWL_STYLE) & WS_CHILD) &&
    (hWndT = ::GetParent(hWndParent)) != NULL)
    {
    hWndParent = hWndT;
    }

    return ::macos::interaction_impl::from_handle(hWndParent);*/

   //      return NULL;
   // }

   bool interaction_impl::IsTopParentActive()
   {
      ASSERT(get_handle() != NULL);
      ASSERT_VALID(this);

      ::user::interaction *pWndTopLevel=EnsureTopLevel();

      return interaction_impl::GetForegroundWindow() == pWndTopLevel->GetLastActivePopup();
   }

   void interaction_impl::ActivateTopParent()
   {
      // special activate logic for floating toolbars and palettes
      //      ::user::interaction * pActiveWnd = GetForegroundWindow();
      //      if (pActiveWnd == NULL || !(IOS_WINDOW(pActiveWnd)->get_handle() == get_handle() || ::IsChild(IOS_WINDOW(pActiveWnd)->get_handle(), get_handle())))
      {
         // clicking on floating frame when it does not have
         // focus itself -- activate the toplevel frame instead.
         EnsureTopLevel()->SetForegroundWindow();
      }
   }


   /*

    sp(::user::frame_window) interaction_impl::GetTopLevelFrame()
    {
    if (get_handle() == NULL) // no oswindow attached
    return NULL;

    ASSERT_VALID(this);

    ::user::frame_window* pFrameWnd = NULL;
    if(m_pui != NULL)
    pFrameWnd = dynamic_cast < ::user::frame_window * > (m_pui);
    if (pFrameWnd == NULL || !pFrameWnd->is_frame_window())
    pFrameWnd = GetParentFrame();

    if (pFrameWnd != NULL)
    {
    ::user::frame_window* pTemp;
    while ((pTemp = pFrameWnd->GetParentFrame()) != NULL)
    pFrameWnd = pTemp;
    }
    return pFrameWnd;
    }


    */




   int32_t interaction_impl::message_box(const char * lpszText, const char * lpszCaption, UINT nType)
   {

      string strCaption;

      if (lpszCaption == NULL)
         lpszCaption = Application.m_strAppName;
      else
         lpszCaption = strCaption;

      int32_t nResult = ::MessageBox((oswindow)get_handle(), lpszText, lpszCaption, nType);

      return nResult;

   }


   ::user::interaction *  PASCAL interaction_impl::GetDescendantWindow(::user::interaction *  hWnd, id id)
   {
      single_lock sl(hWnd->m_papp->m_pmutex, TRUE);
      // GetDlgItem recursive (return first found)
      // breadth-first for 1 level, then depth-first for next level

      // use GetDlgItem since it is a fast USER function
      //      ::user::interaction * pWndChild;
      /*      if ((pWndChild = hWnd->GetDlgItem(id)) != NULL)
       {
       if (pWndChild->GetTopWindow() != NULL)
       {
       // children with the same ID as their parent have priority
       pWndChild = GetDescendantWindow(pWndChild, id);
       if (pWndChild != NULL)
       return pWndChild;
       }
       if (pWndChild != NULL)
       return pWndChild;
       }*/

      for(int32_t i = 0; i < hWnd->m_uiptraChild.get_count(); i++)
      {
         if(hWnd->m_uiptraChild[i]->GetDlgCtrlId() == id)
         {
            if(hWnd->m_uiptraChild[i]->GetDescendantWindow(id))
               return hWnd->m_uiptraChild[i]->GetDescendantWindow(id);
            else
               return hWnd->m_uiptraChild[i];
         }
      }
      //
      //      if(pWndChild == NULL)
      //         return NULL;
      //
      //      // walk each child
      //      for (pWndChild = pWndChild->GetTopWindow(); pWndChild != NULL;
      //           pWndChild = pWndChild->GetNextWindow(GW_HWNDNEXT))
      //      {
      //         pWndChild = GetDescendantWindow(pWndChild, id);
      //         if (pWndChild != NULL)
      //            return pWndChild;
      //      }
      return NULL;    // not found
   }

   void PASCAL interaction_impl::send_message_to_descendants(void * hWnd, UINT message,
         WPARAM wparam, lparam lparam, bool bDeep, bool bOnlyPerm)
   {
      // walk through oswindows to avoid creating temporary user::interaction objects
      // unless we need to call this function recursively
      /*for (oswindow hWndChild = ::GetTopWindow(hWnd); hWndChild != NULL;
       hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))*/
      {
         // if bOnlyPerm is TRUE, don't send to non-permanent windows
         /*if (bOnlyPerm)
          {
          ::user::interaction * pWnd = ::macos::interaction_impl::FromHandlePermanent(hWndChild);
          if (pWnd != NULL)
          {
          // call user::interaction proc directly since it is a C++ user::interaction
          __call_window_procedure(dynamic_cast < ::user::interaction * > (pWnd), IOS_WINDOW(pWnd)->get_handle(), message, wparam, lparam);
          }
          }
          else
          {
          try
          {
          // send message with oswindows SendMessage API
          ::SendMessage(hWndChild, message, wparam, lparam);
          }
          catch(...)
          {
          }
          }
          if (bDeep && ::GetTopWindow(hWndChild) != NULL)
          {
          // send to child windows after parent
          try
          {
          send_message_to_descendants(hWndChild, message, wparam, lparam,
          bDeep, bOnlyPerm);
          }
          catch(...)
          {
          }
          } */
      }

   }

   /////////////////////////////////////////////////////////////////////////////
   // Scroll bar helpers
   //  hook for user::interaction functions
   //    only works for derived class (eg: ::view) that override 'GetScrollBarCtrl'
   // if the user::interaction doesn't have a _visible_ windows scrollbar - then
   //   look for a sibling with the appropriate ID

   //   CScrollBar* interaction_impl::GetScrollBarCtrl(int32_t) const
   // {
   //  return NULL;        // no special scrollers supported
   // }

   int32_t interaction_impl::SetScrollPos(int32_t nBar, int32_t nPos, bool bRedraw)
   {
      //      return ::SetScrollPos(get_handle(), nBar, nPos, bRedraw);
      return 0;
   }

   int32_t interaction_impl::GetScrollPos(int32_t nBar) const
   {
      //return ::GetScrollPos(get_handle(), nBar);
      return 0;
   }

   void interaction_impl::SetScrollRange(int32_t nBar, int32_t nMinPos, int32_t nMaxPos, bool bRedraw)
   {
      //::SetScrollRange(get_handle(), nBar, nMinPos, nMaxPos, bRedraw);
   }

   void interaction_impl::GetScrollRange(int32_t nBar, LPINT lpMinPos, LPINT lpMaxPos) const
   {
      //::GetScrollRange(get_handle(), nBar, lpMinPos, lpMaxPos);
   }

   // Turn on/off non-control scrollbars
   //   for WS_?SCROLL scrollbars - show/hide them
   //   for control scrollbar - enable/disable them
   void interaction_impl::EnableScrollBarCtrl(int32_t nBar, bool bEnable)
   {
      // WS_?SCROLL scrollbar - show or hide
      ShowScrollBar(nBar, bEnable);
   }

   /*
    bool interaction_impl::SetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, bool bRedraw)
    {
    ASSERT(lpScrollInfo != NULL);

    oswindow hWnd = get_handle();
    lpScrollInfo->cbSize = sizeof(*lpScrollInfo);
    ::SetScrollInfo(hWnd, nBar, lpScrollInfo, bRedraw);
    return true;
    }

    bool interaction_impl::GetScrollInfo(int32_t nBar, LPSCROLLINFO lpScrollInfo, UINT nMask)
    {
    UNREFERENCED_PARAMETER(nMask);
    ASSERT(lpScrollInfo != NULL);

    oswindow hWnd = get_handle();
    return ::GetScrollInfo(hWnd, nBar, lpScrollInfo) != FALSE;
    }
    */
   int32_t interaction_impl::GetScrollLimit(int32_t nBar)
   {
      int32_t nMin, nMax;
      GetScrollRange(nBar, &nMin, &nMax);
      /*      SCROLLINFO info;
       if (GetScrollInfo(nBar, &info, SIF_PAGE))
       {
       nMax -= __max(info.nPage-1,0);
       }*/
      return nMax;
   }

   void interaction_impl::ScrollWindow(int32_t xAmount, int32_t yAmount,
                                       LPCRECT lpRect, LPCRECT lpClipRect)
   {
      /*      ASSERT(::is_window(get_handle()));

       if (IsWindowVisible() || lpRect != NULL || lpClipRect != NULL)
       {
       // When visible, let oswindows do the scrolling
       ::ScrollWindow(get_handle(), xAmount, yAmount, lpRect, lpClipRect);
       }
       else
       {
       // oswindows does not perform any scrolling if the user::interaction is
       // not visible.  This leaves child windows unscrolled.
       // To account for this oversight, the child windows are moved
       // directly instead.
       oswindow hWndChild = ::GetWindow(get_handle(), GW_CHILD);
       if (hWndChild != NULL)
       {
       for (; hWndChild != NULL;
       hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
       {
       rect rect;
       ::GetWindowRect(hWndChild, &rect);
       ScreenToClient(&rect);
       ::SetWindowPos(hWndChild, NULL,
       rect.left+xAmount, rect.top+yAmount, 0, 0,
       SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
       }
       }
       }
       */
   }

   /////////////////////////////////////////////////////////////////////////////
   // minimal on_layout support

   /*
    void interaction_impl::RepositionBars(const char * pszPrefix, const char * pszIdLeftOver,
    UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch)
    {
    ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery ||
    (nFlags & ~reposNoPosLeftOver) == reposExtra);

    // walk kids in order, control bars get the resize notification
    //   which allow them to shrink the client area
    // remaining size goes to the 'nIDLeftOver' pane
    // NOTE: nIDFirst->nIDLast are usually 0->0xffff

    __SIZEPARENTPARAMS on_layout;
    ::user::interaction * hWndLeftOver = NULL;

    on_layout.bStretch = bStretch;
    on_layout.sizeTotal.cx = on_layout.sizeTotal.cy = 0;
    if (lpRectClient != NULL)
    on_layout.rect = *lpRectClient;    // starting rect comes from parameter
    else
    {
    if(m_pui != this)
    m_pui->GetClientRect(&on_layout.rect);    // starting rect comes from client rect
    else
    GetClientRect(&on_layout.rect);    // starting rect comes from client rect
    }

    if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
    on_layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
    else
    on_layout.hDWP = NULL; // not actually doing on_layout

    if(m_pui != this && m_pui != NULL)
    {
    for (::user::interaction * hWndChild = m_pui->GetTopWindow(); hWndChild != NULL;
    hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
    {
    string strIdc = hWndChild->GetDlgCtrlId();
    ::user::interaction * pWnd = hWndChild;
    if (strIdc == pszIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
    hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
    }
    for (int32_t i = 0; i < m_pui->m_uiptra.get_count();   i++)
    {
    ::user::interaction * hWndChild = m_pui->m_uiptra[i];
    string strIdc = hWndChild->GetDlgCtrlId();
    ::user::interaction * pWnd = hWndChild;
    if (strIdc == pszIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
    hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
    }
    }
    else
    {
    for (::user::interaction * hWndChild = GetTopWindow(); hWndChild != NULL;
    hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
    {
    string strIdc = hWndChild->GetDlgCtrlId();
    ::user::interaction * pWnd = hWndChild;
    if (strIdc == pszIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
    hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
    }
    for (int32_t i = 0; i < m_uiptra.get_count();   i++)
    {
    ::user::interaction * hWndChild = m_uiptra[i];
    string strIdc = hWndChild->GetDlgCtrlId();
    ::user::interaction * pWnd = hWndChild;
    if (strIdc == pszIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (::str::begins(strIdc, pszPrefix) && pWnd != NULL)
    hWndChild->SendMessage(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
    }
    }

    // if just getting the available rectangle, return it now...
    if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
    {
    ASSERT(lpRectParam != NULL);
    if (bStretch)
    ::copy(lpRectParam, &on_layout.rect);
    else
    {
    lpRectParam->left = lpRectParam->top = 0;
    lpRectParam->right = on_layout.sizeTotal.cx;
    lpRectParam->bottom = on_layout.sizeTotal.cy;
    }
    return;
    }

    // the rest is the client size of the left-over pane
    if (pszIdLeftOver != NULL && hWndLeftOver != NULL)
    {
    ::user::interaction * pLeftOver = hWndLeftOver;
    // allow extra space as specified by lpRectBorder
    if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
    {
    ASSERT(lpRectParam != NULL);
    on_layout.rect.left += lpRectParam->left;
    on_layout.rect.top += lpRectParam->top;
    on_layout.rect.right -= lpRectParam->right;
    on_layout.rect.bottom -= lpRectParam->bottom;
    }
    // reposition the user::interaction
    if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
    {
    pLeftOver->CalcWindowRect(&on_layout.rect);
    __reposition_window(&on_layout, pLeftOver, &on_layout.rect);
    }
    }

    // move and resize all the windows at once!
    if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
    TRACE(::ca2::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");
    }

    */
   /*
    void interaction_impl::RepositionBars(UINT nIDFirst, UINT nIDLast, id nIdLeftOver,
    UINT nFlags, LPRECT lpRectParam, LPCRECT lpRectClient, bool bStretch)
    {
    UNREFERENCED_PARAMETER(nIDFirst);
    UNREFERENCED_PARAMETER(nIDLast);

    ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery ||
    (nFlags & ~reposNoPosLeftOver) == reposExtra);

    // walk kids in order, control bars get the resize notification
    //   which allow them to shrink the client area
    // remaining size goes to the 'nIDLeftOver' pane
    // NOTE: nIDFirst->nIDLast are usually 0->0xffff

    __SIZEPARENTPARAMS on_layout;
    ::user::interaction * hWndLeftOver = NULL;

    on_layout.bStretch = bStretch;
    on_layout.sizeTotal.cx = on_layout.sizeTotal.cy = 0;
    if (lpRectClient != NULL)
    on_layout.rect = *lpRectClient;    // starting rect comes from parameter
    else
    {
    if(m_pui != NULL)
    m_pui->GetClientRect(&on_layout.rect);    // starting rect comes from client rect
    else
    GetClientRect(&on_layout.rect);    // starting rect comes from client rect
    }

    //      if ((nFlags & ~reposNoPosLeftOver) != reposQuery)
    //         on_layout.hDWP = ::BeginDeferWindowPos(8); // reasonable guess
    //      else
    on_layout.hDWP = NULL; // not actually doing on_layout

    if(m_pui != NULL)
    {

    for(::user::interaction * hWndChild = m_pui->GetTopWindow(); hWndChild != NULL; hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
    {

    id id = hWndChild->GetDlgCtrlId();

    ::user::interaction * pWnd = hWndChild;

    if (id == nIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (pWnd != NULL)
    hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
    }

    for(::user::interaction * hWndChild = m_pui->get_top_child(); hWndChild != NULL; hWndChild = hWndChild->under_sibling())
    {

    id id = hWndChild->GetDlgCtrlId();

    ::user::interaction * pWnd = hWndChild;

    if (id == nIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (pWnd != NULL)
    hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);
    }

    }
    else
    {

    for(::user::interaction * hWndChild = GetTopWindow(); hWndChild != NULL; hWndChild = hWndChild->GetNextWindow(GW_HWNDNEXT))
    {

    id id = hWndChild->GetDlgCtrlId();

    ::user::interaction * pWnd = hWndChild;

    if (id == nIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (pWnd != NULL)
    hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);

    }

    for(::user::interaction * hWndChild = m_pui->get_top_child(); hWndChild != NULL; hWndChild = hWndChild->under_sibling())
    {

    id id = hWndChild->GetDlgCtrlId();

    ::user::interaction * pWnd = hWndChild;

    if(id == nIdLeftOver)
    hWndLeftOver = hWndChild;
    else if (pWnd != NULL)
    hWndChild->send_message(WM_SIZEPARENT, 0, (LPARAM)&on_layout);

    }

    }

    // if just getting the available rectangle, return it now...
    if ((nFlags & ~reposNoPosLeftOver) == reposQuery)
    {

    ASSERT(lpRectParam != NULL);

    if(bStretch)
    ::CopyRect(lpRectParam, &on_layout.rect);
    else
    {
    lpRectParam->left = lpRectParam->top = 0;
    lpRectParam->right = on_layout.sizeTotal.cx;
    lpRectParam->bottom = on_layout.sizeTotal.cy;
    }
    return;
    }

    // the rest is the client size of the left-over pane
    if(!nIdLeftOver.is_null() && hWndLeftOver != NULL)
    {

    ::user::interaction * pLeftOver = hWndLeftOver;

    // allow extra space as specified by lpRectBorder

    if ((nFlags & ~reposNoPosLeftOver) == reposExtra)
    {
    ASSERT(lpRectParam != NULL);
    on_layout.rect.left += lpRectParam->left;
    on_layout.rect.top += lpRectParam->top;
    on_layout.rect.right -= lpRectParam->right;
    on_layout.rect.bottom -= lpRectParam->bottom;
    }

    // reposition the user::interaction
    if ((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
    {

    pLeftOver->CalcWindowRect(&on_layout.rect);
    __reposition_window(&on_layout, pLeftOver, &on_layout.rect);
    }
    }

    // move and resize all the windows at once!
    //       if (on_layout.hDWP == NULL || !::EndDeferWindowPos(on_layout.hDWP))
    //     TRACE(::ca2::trace::category_AppMsg, 0, "Warning: DeferWindowPos failed - low system resources.\n");*/
   //   }



   void interaction_impl::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
   {
      /*DWORD dwExStyle = GetExStyle();
       if (nAdjustType == 0)
       dwExStyle &= ~WS_EX_CLIENTEDGE;
       ::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE, dwExStyle);*/
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special keyboard/system command processing

   bool interaction_impl::HandleFloatingSysCommand(UINT nID, LPARAM lparam)
   {
      /*      ::user::interaction* pParent = GetTopLevelParent();
       switch (nID & 0xfff0)
       {
       case SC_PREVWINDOW:
       case SC_NEXTWINDOW:
       if (LOWORD(lparam) == VK_F6 && pParent != NULL)
       {
       pParent->SetFocus();
       return true;
       }
       break;

       case SC_CLOSE:
       case SC_KEYMENU:
       // Check lparam.  If it is 0L, then the ::fontopus::user may have done
       // an Alt+Tab, so just ignore it.  This breaks the ability to
       // just press the Alt-key and have the first menu selected,
       // but this is minor compared to what happens in the Alt+Tab
       // case.
       if ((nID & 0xfff0) == SC_CLOSE || lparam != 0L)
       {
       if (pParent != NULL)
       {
       // Sending the above WM_SYSCOMMAND may destroy the cast,
       // so we have to be careful about restoring activation
       // and focus after sending it.
       oswindow hWndSave = get_handle();
       oswindow hWndFocus = ::GetFocus();
       pParent->SetActiveWindow();
       pParent->send_message(WM_SYSCOMMAND, nID, lparam);

       // be very careful here...
       if (::is_window(hWndSave))
       ::set_active_window(hWndSave);
       if (::is_window(hWndFocus))
       ::SetFocus(hWndFocus);
       }
       }
       return true;
       }
       return false;*/
      return false;
   }

   void interaction_impl::WalkPreTranslateTree(::user::interaction *  puiStop, ::message::message * pobj)
   {
      ASSERT(puiStop == NULL || puiStop->IsWindow());
      ASSERT(pobj != NULL);

      SCAST_PTR(::message::base, pbase, pobj);
      // walk from the target user::interaction up to the hWndStop user::interaction checking
      //  if any user::interaction wants to translate this message

      for (sp(::user::interaction) pui = pbase->m_pwnd; pui != NULL; pui->GetParent())
      {

         pui->pre_translate_message(pobj);

         if(pobj->m_bRet)
            return; // trapped by target user::interaction (eg: accelerators)

         // got to hWndStop user::interaction without interest
         if(pui == puiStop)
            break;

      }
      // no special processing
   }


   bool interaction_impl::SendChildNotifyLastMsg(LRESULT* pResult)
   {

      return false;

   }

   bool PASCAL interaction_impl::ReflectLastMsg(oswindow hWndChild, LRESULT* pResult)
   {
      // get the ::collection::map, and if no ::collection::map, then this message does not need reflection
      /*      single_lock sl(afxMutexHwnd(), TRUE);
       hwnd_map * pMap = afxMapHWND();
       if (pMap == NULL)
       return FALSE;

       // check if in permanent ::collection::map, if it is reflect it (could be OLE control)
       ::user::interaction * pWnd = dynamic_cast < ::user::interaction * > (pMap->lookup_permanent(hWndChild)); */
      ::user::interaction * pWnd = dynamic_cast < ::user::interaction * > (FromHandlePermanent(hWndChild));
      ASSERT(pWnd == NULL || pWnd->get_handle() == hWndChild);
      if (pWnd == NULL)
      {
         return FALSE;
      }

      // only OLE controls and permanent windows will get reflected msgs
      ASSERT(pWnd != NULL);
      //return IOS_WINDOW(pWnd)->SendChildNotifyLastMsg(pResult);
      return false;
   }

   bool interaction_impl::OnChildNotify(UINT uMsg, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
   {

      return ReflectChildNotify(uMsg, wparam, lparam, pResult);
   }

   bool interaction_impl::ReflectChildNotify(UINT uMsg, WPARAM wparam, LPARAM lparam, LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(wparam);
      // Note: reflected messages are send directly to interaction_impl::OnWndMsg
      //  and interaction_impl::_001OnCommand for speed and because these messages are not
      //  routed by normal _001OnCommand routing (they are only dispatched)

      switch (uMsg)
      {
      // normal messages (just wparam, lparam through OnWndMsg)
      case WM_HSCROLL:
      case WM_VSCROLL:
      case WM_PARENTNOTIFY:
      case WM_DRAWITEM:
      case WM_MEASUREITEM:
      case WM_DELETEITEM:
      case WM_VKEYTOITEM:
      case WM_CHARTOITEM:
      case WM_COMPAREITEM:
         // reflect the message through the message ::collection::map as WM_REFLECT_BASE+uMsg
         //return interaction_impl::OnWndMsg(WM_REFLECT_BASE+uMsg, wparam, lparam, pResult);
         return FALSE;

      // special case for WM_COMMAND
      case WM_COMMAND:
      {
         // reflect the message through the message ::collection::map as OCM_COMMAND
         /* xxx         int32_t nCode = HIWORD(wparam);
          if (interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_COMMAND), NULL, NULL))
          {
          if (pResult != NULL)
          *pResult = 1;
          return TRUE;
          } */
      }
      break;

      // special case for WM_NOTIFY
      /*      case WM_NOTIFY:
       {
       // reflect the message through the message ::collection::map as OCM_NOTIFY
       NMHDR* pNMHDR = (NMHDR*)lparam;
       //            int32_t nCode = pNMHDR->code;
       //            __NOTIFY notify;
       //          notify.pResult = pResult;
       //        notify.pNMHDR = pNMHDR;
       // xxxx         return interaction_impl::_001OnCommand(0, MAKELONG(nCode, WM_REFLECT_BASE+WM_NOTIFY), &notify, NULL);
       }

       // other special cases (WM_CTLCOLOR family)*/
      default:
         if (uMsg >= WM_CTLCOLORMSGBOX && uMsg <= WM_CTLCOLORSTATIC)
         {
            // fill in special struct for compatiblity with 16-bit WM_CTLCOLOR
            /*__CTLCOLOR ctl;
             ctl.hDC = (HDC)wparam;
             ctl.nCtlType = uMsg - WM_CTLCOLORMSGBOX;
             //ASSERT(ctl.nCtlType >= CTLCOLOR_MSGBOX);
             ASSERT(ctl.nCtlType <= CTLCOLOR_STATIC);

             // reflect the message through the message ::collection::map as OCM_CTLCOLOR
             bool bResult = interaction_impl::OnWndMsg(WM_REFLECT_BASE+WM_CTLCOLOR, 0, (LPARAM)&ctl, pResult);
             if ((HBRUSH)*pResult == NULL)
             bResult = FALSE;
             return bResult;*/
            return false;
         }
         break;
      }

      return false;   // let the parent handle it
   }

   void interaction_impl::OnParentNotify(UINT message, LPARAM lparam)
   {
      if ((LOWORD(message) == WM_CREATE || LOWORD(message) == WM_DESTROY))
      {
         if (ReflectLastMsg((oswindow)lparam))
            return;     // eat it
      }
      // not handled - do default
      Default();
   }

   void interaction_impl::OnSetFocus(::user::interaction *)
   {
      bool bHandled;

      bHandled = FALSE;
      if( !bHandled )
      {
         Default();
      }
   }


   LRESULT interaction_impl::OnActivateTopLevel(WPARAM wparam, LPARAM)
   {

      return 0;

   }

   void interaction_impl::OnSysColorChange()
   {
      _throw(not_implemented(get_app()));

      /*      application* pApp = &System;
       if (pApp != NULL && pApp->GetMainWnd() == this)
       {
       // recolor global brushes used by control bars
       afxData.UpdateSysColors();
       }

       // forward this message to all other child windows
       if (!(GetStyle() & WS_CHILD))
       send_message_to_descendants(WM_SYSCOLORCHANGE, 0, 0L, TRUE, TRUE);

       Default();*/
   }

   bool gen_GotScrollLines;

   void interaction_impl::OnSettingChange(UINT uFlags, const char * lpszSection)
   {
      //      UNUSED_ALWAYS(uFlags);
      //    UNUSED_ALWAYS(lpszSection);

      // force refresh of settings that we cache
      gen_GotScrollLines = FALSE;


      interaction_impl::OnDisplayChange(0, 0);    // to update system metrics, etc.
   }

   /*   void interaction_impl::OnDevModeChange(__in LPTSTR lpDeviceName)
    {
    UNREFERENCED_PARAMETER(lpDeviceName);
    _throw(not_implemented(get_app()));
    application* pApp = &System;
    if (pApp != NULL && pApp->GetMainWnd() == this)
    pApp->DevModeChange(lpDeviceName);

    // forward this message to all other child windows
    if (!(GetStyle() & WS_CHILD))
    {
    const MESSAGE* pMsg = GetCurrentMessage();
    send_message_to_descendants(pMsg->message, pMsg->wparam, pMsg->lparam,
    TRUE, TRUE);
    }*/
   //}


   //
   //   bool interaction_impl::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
   //   {
   //      if (!(GetStyle() & WS_CHILD))
   //      {
   //         ::user::interaction* pMainWnd = System.GetMainWnd();
   //         if (pMainWnd != NULL &&
   //            GetKeyState(VK_SHIFT) >= 0 &&
   //            GetKeyState(VK_CONTROL) >= 0 &&
   //            GetKeyState(VK_MENU) >= 0)
   //         {
   //            //            pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
   //            return TRUE;
   //         }
   //      }
   //      return Default() != 0;
   //   }
   //
   LRESULT interaction_impl::OnDisplayChange(WPARAM, LPARAM)
   {
      if (!(GetStyle() & WS_CHILD))
      {
         const MESSAGE* pMsg = GetCurrentMessage();
         send_message_to_descendants(pMsg->message, pMsg->wParam, pMsg->lParam, TRUE, TRUE);
      }

      return Default();
   }

   LRESULT interaction_impl::OnDragList(WPARAM, LPARAM lparam)
   {

      _throw(not_implemented(get_app()));
      //
      //      LPDRAGLISTINFO lpInfo = (LPDRAGLISTINFO)lparam;
      //      ASSERT(lpInfo != NULL);
      //
      //      LRESULT lResult;
      //      if (ReflectLastMsg(lpInfo->hWnd, &lResult))
      //         return (int32_t)lResult;    // eat it
      //
      //      // not handled - do default
      //      return (int32_t)Default();
   }

   
   void interaction_impl::prodevian_task()
   {
      
      ::user::interaction_impl::prodevian_task();
      
//      if (m_pthreadProDevian.is_null())
//      {
//         
//         m_pthreadProDevian = fork([&]()
//                                   {
//                                      
//                                      DWORD dwStart;
//                                      
//                                      bool bUpdateScreen = false;
//                                      
//                                      while (::get_thread_run())
//                                      {
//                                         
//                                         try
//                                         {
//                                            
//                                            dwStart = ::get_tick_count();
//                                            
//                                            if(m_pui == NULL)
//                                            {
//                                               
//                                               break;
//                                               
//                                            }
//                                            
////                                            if(m_oswindow != NULL)
////                                            {
////                                               
////                                               m_oswindow->m_bNsWindowRect = false;
////                                               
////                                            }
//                                            
//                                            if (!m_pui->m_bLockWindowUpdate)
//                                            {
//                                               
//                                               bool bUpdateBuffer =
//                                               m_pui->m_bProDevian ||
//                                               m_pui->check_need_layout()||
//                                               m_pui->check_need_zorder() ||
//                                               m_pui->check_show_flags();
//                                               
//                                               if(bUpdateBuffer)
//                                               {
//                                                  
//                                               }
//                                               else if(m_pui->IsWindowVisible())
//                                               {
//                                                  
//                                                  bUpdateBuffer = m_pui->has_pending_graphical_update();
//                                                  
//                                               }
//                                               
//                                               if(bUpdateBuffer)
//                                               {
//                                                  
//                                                  _001UpdateBuffer();
//                                                  
//                                                  if(m_pui == NULL)
//                                                  {
//                                                     
//                                                     break;
//                                                     
//                                                  }
//                                                  
//                                                  m_pui->on_after_graphical_update();
//                                                  
//                                                  bUpdateScreen = true;
//                                                  
//                                               }
//                                               
//                                            }
//                                            
//                                            if(bUpdateScreen)
//                                            {
//                                               
//                                               u64 now = get_nanos();
//                                               
//                                               u64 delta1 = now - m_uiLastUpdateBeg;
//                                               
//                                               i64 delta2 = (i64) m_uiLastUpdateBeg - (i64) m_uiLastUpdateEnd;
//                                               
//                                               u64 frameNanos = 1000000000LL / m_dFps;
//                                               
//                                               if(delta1 < frameNanos || (delta2 > 0 && delta2 < 10000000000LL))
//                                               {
//                                                  
//                                                  output_debug_string("opt_out set need redraw");
//                                                  
//                                               }
//                                               else
//                                               {
//                                                  
//                                                  bUpdateScreen = false;
//                                                  
//                                                  _001UpdateScreen();
//                                                  
//                                               }
//                                               
//                                            }
//                                            
//                                            DWORD dwSpan = ::get_tick_count() - dwStart;
//                                            
//                                            if (dwSpan < 50)
//                                            {
//                                               
//                                               Sleep(50 - dwSpan);
//                                               
//                                            }
//                                            
//                                         }
//                                         catch(...)
//                                         {
//                                            
//                                            break;
//                                            
//                                         }
//                                         
//                                      }
//                                      
//                                      output_debug_string("m_pthreadDraw has finished!");
//                                      
//                                   });
//         
//      }

      
//   if(m_pthreadProDevian.is_null())
//   {
//
//      m_pthreadProDevian = m_pui->fork([&]()
//                                       {
//
//                                          DWORD dwStart;
//
//                                          bool bUpdateScreen = false;
//
//                                          while (::get_thread_run())
//                                          {
//
//                                             try
//                                             {
//
//                                                dwStart = ::get_tick_count();
//
//                                                if(m_pui == NULL)
//                                                {
//
//                                                   break;
//
//                                                }
//
//                                                if (!m_pui->m_bLockWindowUpdate)
//                                                {
//
//                                                   bool bUpdateBuffer = m_pui->check_need_layout()
//                                                   || m_pui->check_need_zorder() || m_pui->check_show_flags();
//
//                                                   if(bUpdateBuffer)
//                                                   {
//
//                                                   }
//                                                   else if(m_pui->IsWindowVisible())
//                                                   {
//
//                                                      bUpdateBuffer = m_pui->has_pending_graphical_update();
//
//                                                   }
//
//                                                   if(bUpdateBuffer)
//                                                   {
//
//                                                      _001UpdateBuffer();
//
//                                                      m_pui->on_after_graphical_update();
//
//                                                      bUpdateScreen = true;
//
//                                                   }
//
//                                                }
//
//                                                if(bUpdateScreen)
//                                                {
//
//                                                   bUpdateScreen = false;
//
//                                                   _001UpdateScreen();
//
//                                                }
//
//                                                DWORD dwSpan = ::get_tick_count() - dwStart;
//
//                                                if (dwSpan < 20)
//                                                {
//
//                                                   Sleep(20 - dwSpan);
//
//                                                }
//
//                                             }
//                                             catch(...)
//                                             {
//
//                                                break;
//
//                                             }
//
//                                          }
//
//                                          output_debug_string("m_pthreadDraw has finished!");
//
//                                          m_pthreadProDevian.release();
//
//                                          //release_graphics_resources();
//
//
//                                       });
//
//   }
   
   }


   void interaction_impl::_001OnCreate(::message::message * pobj)
   {

      UNREFERENCED_PARAMETER(pobj);

      Default();

      //{
      // SetTimer(2049, 184, NULL);
      //}


      if(m_pui->is_message_only_window())
      {

         TRACE("good : opt out!");

      }
      else
      {

      }

   }


   void interaction_impl::_001OnTimer(::timer * ptimer)
   {

      ::user::interaction_impl::_001OnTimer(ptimer);;

//      if(ptimer->m_nIDEvent == 2049)
//      {
//
//         set_need_redraw();
//
//      }

   }


//   void interaction_impl::set_need_redraw()
//   {

   //set_need_redraw();

//   }


//   bool interaction_impl::round_window_key_down(::user::e_key ekey)
//   {
//
//      return false;
//
//   }


//   bool interaction_impl::round_window_key_up(::user::e_key ekey)
//   {
//
//      return false;
//
//   }


//   bool interaction_impl::round_window_on_text(const char * pszText)
//   {
//
//      return false;
//
//   }

   /*


    void interaction_impl::OnHScroll(UINT, UINT, CScrollBar* pScrollBar)
    {
    UNREFERENCED_PARAMETER(pScrollBar);
    Default();
    }

    void interaction_impl::OnVScroll(UINT, UINT, CScrollBar* pScrollBar)
    {
    UNREFERENCED_PARAMETER(pScrollBar);
    Default();
    }
    */
   bool CALLBACK interaction_impl::GetAppsEnumWindowsProc(oswindow hwnd, LPARAM lparam)
   {
      user::oswindow_array * phwnda = (user::oswindow_array *) lparam;
      phwnda->add(hwnd);
      return TRUE;
   }

   void interaction_impl::get_app_wnda(user::oswindow_array & wnda)
   {
      _throw(not_implemented(::get_app()));
      //      EnumWindows(GetAppsEnumWindowsProc, (LPARAM) &wnda);
   }

   /*   void interaction_impl::_001OnDeferPaintLayeredWindowBackground(::draw2d::dib * pdib)
    {
    _001DeferPaintLayeredWindowBackground(pgraphics);
    }*/


   class print_window :
      virtual ::object
   {
   public:



      manual_reset_event m_event;
      oswindow m_hwnd;
      HDC m_hdc;

      print_window(::aura::application * papp, oswindow hwnd, HDC hdc, DWORD dwTimeout) :
         ::object(papp),
         m_event(papp)

      {
         m_event.ResetEvent();
         m_hwnd = hwnd;
         m_hdc = hdc;
         __begin_thread(papp, &print_window::s_print_window, (LPVOID) this, ::multithreading::priority_normal);
         if(m_event.wait(millis(dwTimeout)).timeout())
         {
            TRACE("print_window::time_out");
         }
      }


      static_function UINT c_cdecl s_print_window(LPVOID pvoid)
      {
         //         print_window * pprintwindow = (print_window *) pvoid;
         //         try
         //         {
         //            HANDLE hevent = (HANDLE) pprintwindow->m_event.get_handle();
         //            _throw(not_implemented(pprintwindow->get_app()));
         //            /*            ::PrintWindow(pprintwindow->m_hwnd, pprintwindow->m_hdc, 0);
         //             ::SetEvent(hevent);*/
         //         }
         //         catch(...)
         //         {
         //         }
         return 0;
      }
   };

   void interaction_impl::_001DeferPaintLayeredWindowBackground(HDC hdc)
   {

      rect rectClient;

      GetClientRect(rectClient);


      //pgraphics->FillSolidRect(rectClient, 0x00000000);

      //return;
      rect rectUpdate;
      GetWindowRect(rectUpdate);
      //      SetViewportOrgEx(hdc, 0, 0, NULL);
      rect rectPaint;
      rectPaint = rectUpdate;
      ScreenToClient(rectPaint);
      user::oswindow_array wndaApp;


      //      HRGN rgnWindow;
      //      HRGN rgnIntersect;
      //      HRGN rgnUpdate = NULL;

      _throw(not_implemented(get_app()));
   }

   void interaction_impl::_001OnProdevianSynch(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void interaction_impl::_001OnPaint(::message::message * pobj)
   {

      //lock lock(m_pui, 1984);

      _throw(not_implemented(get_app()));

   }


   void interaction_impl::_001OnPrint(::message::message * pobj)
   {
      _throw(not_implemented(get_app()));
      //      SCAST_PTR(::message::base, pbase, pobj);
      //
      //      if(pbase->m_wparam == NULL)
      //         return;
      //
      //      ::draw2d::graphics_sp graphics(get_app());
      //      WIN_DC(graphics.m_p)->Attach((HDC) pbase->m_wparam);
      //      rect rectx;
      //      ::draw2d::bitmap * pbitmap = &graphics->GetCurrentBitmap();
      //      ::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);
      //      //      DWORD dw = ::get_last_error();
      //      class size size = pbitmap->get_size();
      //      rectx.left = 0;
      //      rectx.top = 0;
      //      rectx.right = size.cx;
      //      rectx.bottom = size.cy;
      //      try
      //      {
      //         rect rectWindow;
      //         GetWindowRect(rectWindow);
      //
      //         ::draw2d::dib_sp dib(get_app());
      //         if(!dib->create(rectWindow.bottom_right()))
      //            return;
      //
      //         ::draw2d::graphics * pgraphics = dib->get_graphics();
      //
      //         if(pgraphics->get_handle() == NULL)
      //            return;
      //
      //         rect rectPaint;
      //         rect rectUpdate;
      //         rectUpdate = rectWindow;
      //         rectPaint = rectWindow;
      //         rectPaint.offset(-rectPaint.top_left());
      //         (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //         if(m_pui != NULL && m_pui != this)
      //         {
      //            m_pui->_001OnDeferPaintLayeredWindowBackground(pgraphics);
      //         }
      //         else
      //         {
      //            _001OnDeferPaintLayeredWindowBackground(pgraphics);
      //         }
      //         (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //         (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //         _000OnDraw(pgraphics);
      //         (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //         //(dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
      //         (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->SelectClipRgn(NULL);
      //         (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->SetViewportOrg(point(0, 0));
      //
      //         graphics->SelectClipRgn( NULL);
      //         graphics->BitBlt(rectPaint.left, rectPaint.top,
      //            rectPaint.width(), rectPaint.height(),
      //            pgraphics, rectUpdate.left, rectUpdate.top,
      //            SRCCOPY);
      //
      //         graphics->text_out(0, 0, "Te Amo Thommy!!", 11);
      //      }
      //      catch(...)
      //      {
      //      }
      //      graphics->FillSolidRect(rectx, RGB(255, 255, 255));
      //      WIN_DC(graphics.m_p)->Detach();
      //      pobj->m_bRet = true;
      //      pbase->set_lresult(0);
   }


   void interaction_impl::OnEnterIdle(UINT /*nWhy*/, ::user::interaction * /*pWho*/)
   {
      // In some OLE inplace active scenarios, OLE will post a
      // message instead of sending it.  This causes so many WM_ENTERIDLE
      // messages to be sent that tasks running in the background stop
      // running.  By dispatching the pending WM_ENTERIDLE messages
      // when the first one is received, we trick oswindows into thinking
      // that only one was really sent and dispatched.
      {
         //         MESSAGE msg;
         _throw(not_implemented(get_app()));
         //while (PeekMessage(&msg, NULL, WM_ENTERIDLE, WM_ENTERIDLE, PM_REMOVE))
         //while (PeekMessage(&msg, ::caNULL, WM_ENTERIDLE, WM_ENTERIDLE, TRUE))
         // DispatchMessage(&msg);
      }

      //Default();
   }

   HBRUSH interaction_impl::OnCtlColor(::draw2d::graphics *, ::user::interaction * pWnd, UINT)
   {
      ASSERT(pWnd != NULL && get_handle() != NULL);
      LRESULT lResult;
//      if (IOS_WINDOW(pWnd)->SendChildNotifyLastMsg(&lResult))
      //       return (HBRUSH)lResult;     // eat it
      return (HBRUSH)Default();
   }

   // implementation of OnCtlColor for default gray backgrounds
   //   (works for any user::interaction containing controls)
   //  return value of FALSE means caller must call DefWindowProc's default
   //  TRUE means that 'hbrGray' will be used and the appropriate text
   //    ('clrText') and background colors are set.
   bool PASCAL interaction_impl::GrayCtlColor(HDC hDC, oswindow hWnd, UINT nCtlColor,
         HBRUSH hbrGray, COLORREF clrText)
   {
      _throw(not_implemented(::get_app()));
      //      if (hDC == NULL)
      //      {
      //         // sometimes Win32 passes a NULL hDC in the WM_CTLCOLOR message.
      //         //         TRACE(::ca2::trace::category_AppMsg, 0, "Warning: hDC is NULL in interaction_impl::GrayCtlColor; WM_CTLCOLOR not processed.\n");
      //         return FALSE;
      //      }
      //
      //      if (hbrGray == NULL ||
      //         nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_MSGBOX ||
      //         nCtlColor == CTLCOLOR_SCROLLBAR)
      //      {
      //         return FALSE;
      //      }
      //
      //      if (nCtlColor == CTLCOLOR_LISTBOX)
      //      {
      //         // only handle requests to draw the space between edit and drop button
      //         //  in a drop-down combo (not a drop-down list)
      //         if (!__is_combo_box_control(hWnd, (UINT)CBS_DROPDOWN))
      //            return FALSE;
      //      }
      //
      //      // set background color and return handle to brush
      //      LOGBRUSH logbrush;
      //      VERIFY(::GetObject(hbrGray, sizeof(LOGBRUSH), (LPVOID)&logbrush));
      //      ::SetBkColor(hDC, logbrush.lbColor);
      //      if (clrText == (COLORREF)-1)
      //         clrText = ::GetSysColor(COLOR_WINDOWTEXT);  // normal text
      //      ::SetTextColor(hDC, clrText);
      //      return TRUE;
   }



   /////////////////////////////////////////////////////////////////////////////
   // Centering dialog support (works for any non-child user::interaction)

   void interaction_impl::CenterWindow(::user::interaction *  pAlternateOwner)
   {
      _throw(not_implemented(get_app()));
   }

   bool interaction_impl::CheckAutoCenter()
   {
      return TRUE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Dialog initialization support

   bool interaction_impl::ExecuteDlgInit(const char * lpszResourceName)
   {
      // find resource handle
      LPVOID lpResource = NULL;
      HGLOBAL hResource = NULL;
      if (lpszResourceName != NULL)
      {
         //         HINSTANCE hInst = ::ca2::FindResourceHandle(lpszResourceName, RT_DLGINIT);
         //       HRSRC hDlgInit = ::FindResource(hInst, lpszResourceName, RT_DLGINIT);
         /*     if (hDlgInit != NULL)
          {
          // load it
          hResource = LoadResource(hInst, hDlgInit);
          if (hResource == NULL)
          return FALSE;
          // lock it
          lpResource = LockResource(hResource);
          ASSERT(lpResource != NULL);
          }*/
      }

      // execute it
      bool bResult = ExecuteDlgInit(lpResource);

      // cleanup
      if (lpResource != NULL && hResource != NULL)
      {
         _throw(not_implemented(get_app()));
         //         UnlockResource(hResource);
         //         FreeResource(hResource);
      }
      return bResult;
   }

   bool interaction_impl::ExecuteDlgInit(LPVOID lpResource)
   {
      _throw(not_implemented(get_app()));
      //      bool bSuccess = TRUE;
      //      if (lpResource != NULL)
      //      {
      //         UNALIGNED WORD* lpnRes = (WORD*)lpResource;
      //         while (bSuccess && *lpnRes != 0)
      //         {
      //            WORD nIDC = *lpnRes++;
      //            WORD nMsg = *lpnRes++;
      //            DWORD dwLen = *((UNALIGNED DWORD*&)lpnRes)++;
      //
      //            // In Win32 the WM_ messages have changed.  They have
      //            // to be translated from the 32-bit values to 16-bit
      //            // values here.
      //
      //#define WIN16_LB_ADDSTRING  0x0401
      //#define WIN16_CB_ADDSTRING  0x0403
      //#define __CB_ADDSTRING   0x1234
      //
      //            // unfortunately, WIN16_CB_ADDSTRING == CBEM_INSERTITEM
      //            if (nMsg == __CB_ADDSTRING)
      //               nMsg = CBEM_INSERTITEM;
      //            else if (nMsg == WIN16_LB_ADDSTRING)
      //               nMsg = LB_ADDSTRING;
      //            else if (nMsg == WIN16_CB_ADDSTRING)
      //               nMsg = CB_ADDSTRING;
      //
      //            // check for invalid/unknown message types
      //            ASSERT(nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING ||
      //               nMsg == CBEM_INSERTITEM);
      //
      //#ifdef DEBUG
      //            // For AddStrings, the count must exactly delimit the
      //            // string, including the NULL termination.  This check
      //            // will not catch all mal-formed ADDSTRINGs, but will
      //            // catch some.
      //            if (nMsg == LB_ADDSTRING || nMsg == CB_ADDSTRING || nMsg == CBEM_INSERTITEM)
      //               ASSERT(*((LPBYTE)lpnRes + (UINT)dwLen - 1) == 0);
      //#endif
      //
      //            if (nMsg == CBEM_INSERTITEM)
      //            {
      //               COMBOBOXEXITEM item = {0};
      //               item.mask = CBEIF_TEXT;
      //               item.iItem = -1;
      //               string strText(reinterpret_cast<LPTSTR>(lpnRes));
      //               item.pszText = const_cast<LPTSTR>(strText.GetString());
      //               if (::SendDlgItemMessage(get_handle(), nIDC, nMsg, 0, (LPARAM) &item) == -1)
      //                  bSuccess = FALSE;
      //            }
      //            {
      //               // List/Combobox returns -1 for error
      //               if (::SendDlgItemMessageA(get_handle(), nIDC, nMsg, 0, (LPARAM) lpnRes) == -1)
      //                  bSuccess = FALSE;
      //            }
      //
      //
      //            // skip past data
      //            lpnRes = (WORD*)((LPBYTE)lpnRes + (UINT)dwLen);
      //         }
      //      }
      //
      //      // send update message to all controls after all other siblings loaded
      //      if (bSuccess)
      //         send_message_to_descendants(WM_INITIALUPDATE, 0, 0, FALSE, FALSE);
      //
      //      return bSuccess;
   }

   void interaction_impl::UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler)
   {
      UNREFERENCED_PARAMETER(pTarget);
      UNREFERENCED_PARAMETER(bDisableIfNoHndler);
      ::user::command state(get_app());
      user::interaction wndTemp;       // very temporary user::interaction just for CmdUI update

      // walk all the kids - assume the IDs are for buttons
      /* xxx   for (oswindow hWndChild = ::GetTopWindow(get_handle()); hWndChild != NULL;
       hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
       {
       // send to buttons
       wndTemp.set_handle(hWndChild); // quick and dirty attach
       state.m_nID = __get_dialog_control_id(hWndChild);
       state.m_pOther = &wndTemp;

       // check for reflect handlers in the child user::interaction
       ::user::interaction * pWnd = ::macos::interaction_impl::FromHandlePermanent(hWndChild);
       if (pWnd != NULL)
       {
       // call it directly to disable any routing
       if (IOS_WINDOW(pWnd)->interaction_impl::_001OnCommand(0, MAKELONG(0xffff,
       WM_COMMAND+WM_REFLECT_BASE), &state, NULL))
       continue;
       }

       // check for handlers in the parent user::interaction
       if (interaction_impl::_001OnCommand((UINT)state.m_nID, CN_UPDATE_::user::command, &state, NULL))
       continue;

       // determine whether to disable when no handler exists
       bool bDisableTemp = bDisableIfNoHndler;
       if (bDisableTemp)
       {
       if ((wndTemp.SendMessage(WM_GETDLGCODE) & DLGC_BUTTON) == 0)
       {
       // non-button controls don't get automagically disabled
       bDisableTemp = FALSE;
       }
       else
       {
       // only certain button controls get automagically disabled
       UINT nStyle = (UINT)(wndTemp.GetStyle() & 0x0F);
       if (nStyle == (UINT)BS_AUTOCHECKBOX ||
       nStyle == (UINT)BS_AUTO3STATE ||
       nStyle == (UINT)BS_GROUPBOX ||
       nStyle == (UINT)BS_AUTORADIOBUTTON)
       {
       bDisableTemp = FALSE;
       }
       }
       }
       // check for handlers in the target (owner)
       state.DoUpdate(pTarget, bDisableTemp);
       }
       wndTemp.set_handle(NULL);      // quick and dirty detach */
   }


   bool interaction_impl::IsFrameWnd()
   {
      return FALSE;
   }

   bool interaction_impl::SubclassWindow(oswindow hWnd)
   {
      if (!Attach(hWnd))
         return FALSE;

      // allow any other subclassing to occur
      pre_subclass_window();

      _throw(not_implemented(get_app()));
      //      m_pfnSuper = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC);

      // now hook into the AFX WndProc
      //      WNDPROC* lplpfn = GetSuperWndProcAddr();
      //      WNDPROC oldWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC,
      //         (int_ptr)__get_window_procedure());
      //      ASSERT(oldWndProc != __get_window_procedure());
      //
      //      if (*lplpfn == NULL)
      //         *lplpfn = oldWndProc;   // the first control of that type created
      //#ifdef DEBUG
      //      else if (*lplpfn != oldWndProc)
      //      {
      //         TRACE(::ca2::trace::category_AppMsg, 0, "p: Trying to use SubclassWindow with incorrect user::interaction\n");
      //         TRACE(::ca2::trace::category_AppMsg, 0, "\tderived class.\n");
      //         TRACE(::ca2::trace::category_AppMsg, 0, "\thWnd = $%08X (nIDC=$%08X) is not a %hs.\n", (UINT)(uint_ptr)hWnd,
      //            __get_dialog_control_id(hWnd), typeid(*this).name());
      //         ASSERT(FALSE);
      //         // undo the subclassing if continuing after assert
      //         ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (int_ptr)oldWndProc);
      //      }
      //#endif
      //      ::message::size size(get_app());
      //      _001OnSize(&size);
      //      return TRUE;
      //   }
      //
      //   bool interaction_impl::SubclassDlgItem(UINT nID, ::user::interaction * pParent)
      //   {
      //      ASSERT(pParent != NULL);
      //      ASSERT(::is_window(IOS_WINDOW(pParent)->get_handle()));
      //
      //      // check for normal dialog control first
      //      oswindow hWndControl = ::GetDlgItem(IOS_WINDOW(pParent)->get_handle(), nID);
      //      if (hWndControl != NULL)
      //         return SubclassWindow(hWndControl);
      //
      //
      //      return FALSE;   // control not found
   }

   oswindow interaction_impl::UnsubclassWindow()
   {
      ASSERT(::is_window(get_handle()));

      _throw(not_implemented(get_app()));
      //      // set WNDPROC back to original value
      //      WNDPROC* lplpfn = GetSuperWndProcAddr();
      //      SetWindowLongPtr(get_handle(), GWLP_WNDPROC, (int_ptr)*lplpfn);
      //      *lplpfn = NULL;
      //
      //      // and Detach the oswindow from the user::interaction object
      //      return Detach();
   }


   /*   view_update_hint::view_update_hint(::aura::application * papp) :
    ::object(papp)
    {
    }
    */



   bool interaction_impl::IsChild(::user::interaction *  pWnd)
   {
      ASSERT(::is_window(get_handle()));
      if(get_handle() == NULL)
      {
         return ::user::interaction_impl::IsChild(pWnd);
      }
      else
      {
         //         return ::IsChild(get_handle(), IOS_WINDOW(pWnd)->get_handle()) != FALSE;
         return FALSE;
      }
   }

   
   bool interaction_impl::IsWindow() const
   {
      
      return ::is_window(m_oswindow) != FALSE;
      
   }


   ::user::interaction * interaction_impl::get_wnd() const
   {

      return m_pui;

   }


   void interaction_impl::MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, bool bRepaint)
   {

      ASSERT(::is_window(get_handle()));

      SetWindowPos(0, x, y, nWidth, nHeight, bRepaint ? SWP_SHOWWINDOW : 0);

   }


   bool interaction_impl::ClientToScreen(LPRECT lprect)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   += (LONG) rectWindow.left;
      lprect->right  += (LONG) rectWindow.left;
      lprect->top    += (LONG) rectWindow.top;
      lprect->bottom += (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(LPPOINT lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     += (LONG) rectWindow.left;
      lppoint->y     += (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(RECT64 * lprect)
   {

      class rect rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   += rectWindow.left;
      lprect->right  += rectWindow.left;
      lprect->top    += rectWindow.top;
      lprect->bottom += rectWindow.top;

      return true;

   }


   bool interaction_impl::ClientToScreen(POINT64 * lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;
      }

      lppoint->x     += rectWindow.left;
      lppoint->y     += rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(LPRECT lprect)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lprect->left   -= (LONG) rectWindow.left;
      lprect->right  -= (LONG) rectWindow.left;
      lprect->top    -= (LONG) rectWindow.top;
      lprect->bottom -= (LONG) rectWindow.top;

      return true;

   }


   bool interaction_impl::ScreenToClient(LPPOINT lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     -= (LONG) rectWindow.left;
      lppoint->y     -= (LONG) rectWindow.top;


      return true;

   }


   bool interaction_impl::ScreenToClient(RECT64 * lprect)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }


      lprect->left   -= rectWindow.left;
      lprect->right  -= rectWindow.left;
      lprect->top    -= rectWindow.top;
      lprect->bottom -= rectWindow.top;

      return true;

   }

   bool interaction_impl::ScreenToClient(POINT64 * lppoint)
   {

      class rect64 rectWindow;

      if(!m_pui->GetWindowRect(rectWindow))
      {

         return false;

      }

      lppoint->x     -= rectWindow.left;
      lppoint->y     -= rectWindow.top;

      return true;

   }


   bool interaction_impl::GetWindowRect(RECT64 * lprect)
   {
      //      if(!::is_window(get_handle()))
      //       _throw(simple_exception(get_app(), "no more a user::interaction"));
      if(!::is_window(get_handle()))
      {

         return false;

      }

      return ::user::interaction_impl::GetWindowRect(lprect);

      // if it is temporary user::interaction - probably not ca2 wrapped user::interaction

//      rect rect32;
//
//      if(m_pui == m_papp->m_psystem->m_possystemwindow->m_pui)
//      {
//
//         if(!GetMainScreenRect(rect32))
//         {
//
//            return false;
//
//         }
//
//      }
//      else
//      {
//
//         if(!::GetWindowRect(get_handle(), rect32))
//         {
//
//            return false;
//
//         }
//
//      }
//
//      ::copy(lprect, rect32);
//
//      return true;

   }

   bool interaction_impl::GetClientRect(RECT64 * lprect)
   {

      if(!::is_window(get_handle()))
         return false;

//      ASSERT(::is_window(get_handle()));

      rect rect32;

      if(m_pui == m_papp->m_psystem->m_possystemwindow->m_pui)
      {

         if(!GetMainScreenRect(rect32))
         {

            return false;

         }

      }
      else
      {

         if(!::GetWindowRect(get_handle(), rect32))
         {

            return false;

         }

      }

      ::copy(lprect, rect32);

      lprect->top += 16;

      return true;

   }


   id interaction_impl::SetDlgCtrlId(id id)
   {

      return m_pui->SetDlgCtrlId((id));

   }


   id interaction_impl::GetDlgCtrlId()
   {

      return m_pui->GetDlgCtrlId();

   }


   void interaction_impl::_001WindowMaximize()
   {
      ::user::interaction_impl::_001WindowMaximize();
   }

   void interaction_impl::_001WindowRestore()
   {
      m_pui->m_eappearance = user::appearance_normal;
      if(m_pui != NULL)
         m_pui->m_eappearance = user::appearance_normal;
      //      ::ShowWindow(get_handle(), SW_RESTORE);
   }


   bool interaction_impl::ShowWindow(int32_t nCmdShow)
   {

      if(!::is_window(get_handle()))
      {

         return false;

      }

      if(nCmdShow != SW_HIDE)
      {

         round_window_show();

      }
      else
      {

         round_window_hide();

      }

      return m_pui->IsWindowVisible();

   }


   bool interaction_impl::WfiIsIconic()
   {
      ASSERT(::is_window(get_handle()));
      if(GetExStyle() & WS_EX_LAYERED)
      {
         return m_pui->m_eappearance == user::appearance_iconic;
      }
      else
      {
         return ::IsIconic(get_handle()) != FALSE;
      }
   }

   bool interaction_impl::WfiIsZoomed()
   {
      ASSERT(::is_window(get_handle()));
      return m_pui->m_eappearance == user::appearance_zoomed;
   }


   ::user::interaction * interaction_impl::GetParent() const
   {
      return NULL;
      //      if(!::is_window(get_handle()))
      //       return NULL;
      //  if(get_handle() == NULL)
      //   return NULL;
      // return ::macos::interaction_impl::from_handle(::GetParent(get_handle()));
   }


   LONG interaction_impl::GetWindowLong(int32_t nIndex)
   {

      return ::GetWindowLong(get_handle(), nIndex);

   }


   LONG interaction_impl::SetWindowLong(int32_t nIndex, LONG lValue)
   {

      return ::SetWindowLong(get_handle(), nIndex, lValue);

   }


   /*


      DWORD interaction_impl::GetStyle() const
      {
         ASSERT(::is_window(get_handle()));
         return (DWORD)::GetWindowLong(get_handle(), GWL_STYLE);
      }

      DWORD interaction_impl::GetExStyle() const
      {
         ASSERT(::is_window(get_handle()));
         return (DWORD)::GetWindowLong(get_handle(), GWL_EXSTYLE);
      }

      bool interaction_impl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
      {
         ASSERT(::is_window(get_handle()));
         return ModifyStyle(get_handle(), dwRemove, dwAdd, nFlags);
      }

      bool interaction_impl::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
      {
         ASSERT(::is_window(get_handle()));
         return ModifyStyleEx(get_handle(), dwRemove, dwAdd, nFlags);
      }
   */

   ::user::interaction *  interaction_impl::SetOwner(::user::interaction *  pOwnerWnd)
   {
      //      m_puiOwner = pOwnerWnd;
      return NULL;
   }

   LRESULT interaction_impl::send_message(UINT message, WPARAM wparam, lparam lparam)
   {

      return ::user::interaction_impl::send_message(message, wparam, lparam);

      //      ::smart_pointer < ::message::base > spbase;

      //    spbase = get_base(message, wparam, lparam);

      /*      try
       {
       ::user::interaction * pui = m_pui;
       while(pui != NULL)
       {
       try
       {
       pui->pre_translate_message(spbase);
       }
       catch(...)
       {
       break;
       }
       if(spbase->m_bRet)
       return spbase->get_lresult();
       try
       {
       pui = pui->get_parent();
       }
       catch(...)
       {
       break;
       }
       }
       }
       catch(...)
       {
       }*/
      //      message_handler(spbase);
      //    return spbase->get_lresult();

      //_throw(todo(get_app()));

      ////ASSERT(::is_window(get_handle()));
      //return ::SendMessage(get_handle(), message, wParam, lParam);
   }

   bool interaction_impl::post_message(UINT message, WPARAM wparam, lparam lparam)
   {
      if(m_papp != NULL)
      {
         return m_papp->post_message(m_pui, message, wparam, lparam);
      }
      else
      {
         return FALSE;
      }
   }

   bool interaction_impl::DragDetect(POINT pt) const
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));

      //return ::DragDetect(get_handle(), pt) != FALSE;

   }


   void interaction_impl::set_window_text(const char * lpszString)
   {

      m_strWindowText = lpszString;

   }

   /*
   strsize interaction_impl::get_window_text(char * lpszString, strsize nMaxCount)
   {

      strncpy(lpszString, m_strWindowText, nMaxCount);

      return MIN(nMaxCount, m_strWindowText.get_length());

   }

   strsize interaction_impl::get_window_text_length()()
   {

      return m_strWindowText.get_length();

   }

   */



   /*

    void interaction_impl::SetFont(::draw2d::font* pfont, bool bRedraw)
    {
    UNREFERENCED_PARAMETER(bRedraw);
    ASSERT(::is_window(get_handle())); m_pfont = new ::draw2d::font(*pfont);
    }

    ::draw2d::font* interaction_impl::GetFont()
    {
    ASSERT(::is_window(get_handle()));
    return m_pfont;
    }

    */

   void interaction_impl::DragAcceptFiles(bool bAccept)
   {

      _throw(not_implemented(get_app()));

      //ASSERT(::is_window(get_handle()));
      //::DragAcceptFiles(get_handle(), bAccept);

   }

   /*

    sp(::user::frame_window) interaction_impl::EnsureParentFrame()
    {
    ::user::frame_window * pFrameWnd=GetParentFrame();
    ENSURE_VALID(pFrameWnd);
    return pFrameWnd;

    }


    ::user::interaction *  interaction_impl::EnsureTopLevelParent()
    {

    ::user::interaction * pWnd = GetTopLevelParent();

    ENSURE_VALID(pWnd);

    return pWnd;

    }

    */

   void interaction_impl::MoveWindow(LPCRECT lpRect, bool bRepaint)
   {
      MoveWindow(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, bRepaint);
   }

   UINT interaction_impl::ArrangeIconicWindows()
   {
      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle())); return ::ArrangeIconicWindows(get_handle());
   }

   int32_t interaction_impl::SetWindowRgn(HRGN hRgn, bool bRedraw)
   {
      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle())); return ::SetWindowRgn(get_handle(), hRgn, bRedraw);
   }

   int32_t interaction_impl::GetWindowRgn(HRGN hRgn)
   {
      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()) && hRgn != NULL); return ::GetWindowRgn(get_handle(), hRgn);
   }


   void interaction_impl::BringToTop(int nCmdShow)
   {

      if(nCmdShow != SW_HIDE)
      {

         BringWindowToTop();

         round_window_show();

      }

   }


   bool interaction_impl::BringWindowToTop()
   {

      bool b = ::BringWindowToTop(get_handle()) != FALSE;

      return b;

   }


   void interaction_impl::MapWindowPoints(::user::interaction * pwndTo, LPPOINT lpPoint, UINT nCount)
   {
      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::MapWindowPoints(get_handle(), (oswindow) pwndTo->get_handle(), lpPoint, nCount);
   }

   void interaction_impl::MapWindowPoints(::user::interaction * pwndTo, LPRECT lpRect)
   {
      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::MapWindowPoints(get_handle(), (oswindow) pwndTo->get_handle(), (LPPOINT)lpRect, 2);
   }

   ::draw2d::graphics * interaction_impl::GetDC()
   {
      ::draw2d::graphics_sp g(allocer());
      oswindow oswindow;
      if(get_handle() == NULL)
      {
         //         oswindow = ::get_desktop_window();

      }
      else
      {
         oswindow = (::oswindow) get_handle();
      }
      rect rectClient;
      //oswindow.get_client_rect(rectClient);
      rectClient.left = 0;
      rectClient.top = 0;
      rectClient.right = 500;
      rectClient.bottom = 500;
      //      (dynamic_cast < ::draw2d_quartz2d::graphics * >(g.m_p))->attach(cairo_create(cairo_xlib_surface_create(oswindow.display(), oswindow.user::interaction(), DefaultVisual(oswindow.display(), 0), rectClient.width(), rectClient.height())));
      return g.detach();
   }

   ::draw2d::graphics * interaction_impl::GetWindowDC()
   {
      //ASSERT(::is_window(get_handle()));
      //::draw2d::graphics_sp g(allocer());
      //g->attach(::GetWindowDC(get_handle()));
      //return g.detach();

      return NULL;

   }

   bool interaction_impl::ReleaseDC(::draw2d::graphics * pgraphics)
   {

      if(pgraphics == NULL)
         return false;

      //      cairo_t * pcairo = (cairo_t *) pgraphics->get_handle();
      //
      //      cairo_surface_t * psurface = cairo_get_target(pcairo);
      //
      //      cairo_destroy(pcairo);
      //
      //      cairo_surface_destroy(psurface);

      //      if(((Gdiplus::Graphics *)(dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->get_handle()) == NULL)
      //       return false;

      //::ReleaseDC(get_handle(), (dynamic_cast < ::draw2d_quartz2d::graphics * > (pgraphics))->detach());

      //      (dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->m_hdc = NULL;

      //      pgraphics->release();

      return true;

   }

   void interaction_impl::UpdateWindow()
   {
      _throw(not_implemented(get_app()));
      //::UpdateWindow(get_handle());
   }

   void interaction_impl::SetRedraw(bool bRedraw)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::SendMessage(get_handle(), WM_SETREDRAW, bRedraw, 0);
   }

   bool interaction_impl::GetUpdateRect(LPRECT lpRect, bool bErase)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //return ::GetUpdateRect(get_handle(), lpRect, bErase) != FALSE;
   }

   int32_t interaction_impl::GetUpdateRgn(::draw2d::region* pRgn, bool bErase)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //return ::GetUpdateRgn(get_handle(), (HRGN)pRgn->get_handle(), bErase);
   }

   void interaction_impl::Invalidate(bool bErase)
   {
      m_bNeedsUpdate = true;
      //      round_window_invalidate();
      //      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::InvalidateRect(get_handle(), NULL, bErase);
   }

   void interaction_impl::InvalidateRect(LPCRECT lpRect, bool bErase)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::InvalidateRect(get_handle(), lpRect, bErase);
   }

   void interaction_impl::InvalidateRgn(::draw2d::region* pRgn, bool bErase)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::InvalidateRgn(get_handle(), (HRGN)pRgn->get_handle(), bErase);
   }

   void interaction_impl::ValidateRect(LPCRECT lpRect)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::ValidateRect(get_handle(), lpRect);
   }

   void interaction_impl::ValidateRgn(::draw2d::region* pRgn)
   {
      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::ValidateRgn(get_handle(), (HRGN)pRgn->get_handle());
   }

   bool interaction_impl::IsWindowVisible()
   {

      if(!::is_window(get_handle()))
         return false;

      if(m_pui != NULL)
      {

         if(!m_pui->is_this_visible())
            return false;

         if(m_pui->GetParent() != NULL && !m_pui->GetParent()->IsWindowVisible())
            return false;

      }

      if(!::IsWindowVisible(get_handle()))
         return false;

      return true;

   }


   void interaction_impl::ShowOwnedPopups(bool bShow)
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::ShowOwnedPopups(get_handle(), bShow);

   }

   void interaction_impl::send_message_to_descendants(UINT message, WPARAM wparam, lparam lparam, bool bDeep, bool bOnlyPerm)
   {
      ASSERT(::is_window(get_handle()));
      //interaction_impl::send_message_to_descendants(get_handle(), message, wparam, lparam, bDeep, bOnlyPerm);

      // walk through oswindows to avoid creating temporary user::interaction objects
      // unless we need to call this function recursively
      user::interaction * pui = m_pui->top_child();
      while(pui != NULL)
      {
         try
         {
            pui->send_message(message, wparam, lparam);
         }
         catch(...)
         {
         }
         if (bDeep)
         {
            // send to child windows after parent
            try
            {
               pui->send_message_to_descendants(message, wparam, lparam, bDeep, bOnlyPerm);
            }
            catch(...)
            {
            }
         }
         try
         {
            pui = pui->under_sibling();
         }
         catch(...)
         {
            break;
         }
      }
   }

   ::user::interaction *  interaction_impl::GetDescendantWindow(id id)
   {
      //      ASSERT(::is_window(get_handle()));
      //    return interaction_impl::GetDescendantWindow(this, id);

      single_lock sl(m_papp->m_pmutex, TRUE);
      for(int32_t i = 0; i < m_pui->m_uiptraChild.get_count(); i++)
      {
         if(m_pui->m_uiptraChild[i]->GetDlgCtrlId() == id)
         {
            if(m_pui->m_uiptraChild[i]->GetDescendantWindow(id))
               return m_pui->m_uiptraChild[i]->GetDescendantWindow(id);
            else
               return m_pui->m_uiptraChild[i];
         }
      }

      return NULL;
   }


   ::draw2d::graphics * interaction_impl::GetDCEx(::draw2d::region* prgnClip, DWORD flags)
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::draw2d::graphics_sp g(get_app());
      //g->attach(::GetDCEx(get_handle(), (HRGN)prgnClip->get_handle(), flags));
      //return g.detach();

   }

   bool interaction_impl::LockWindowUpdate()
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //return ::LockWindowUpdate(get_handle()) != FALSE;

   }

   void interaction_impl::UnlockWindowUpdate()
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //::LockWindowUpdate(NULL);

   }

   bool interaction_impl::RedrawWindow(LPCRECT lpRectUpdate, ::draw2d::region* prgnUpdate, UINT flags)
   {

      if(flags & RDW_UPDATENOW)
      {

         //debug_break();

         if(m_bShowFlags)
         {

            if(!IsWindowVisible() && (m_iShowFlags & SWP_SHOWWINDOW))
            {

               round_window_show();

            }
            else if(IsWindowVisible() && (m_iShowFlags & SWP_HIDEWINDOW))
            {

               round_window_hide();

            }

         }

         if (m_rectLastPos != m_rectParentClientRequest)
         {

            ::SetWindowPos(m_oswindow, NULL,
                           (int) m_rectParentClientRequest.left,
                           (int) m_rectParentClientRequest.top,
                           (int) m_rectParentClientRequest.width(),
                           (int) m_rectParentClientRequest.height(),
                           SWP_NOZORDER
                           | SWP_NOREDRAW
                           | SWP_NOCOPYBITS
                           | SWP_NOACTIVATE
                           | SWP_NOOWNERZORDER
                           | SWP_NOSENDCHANGING
                           | SWP_DEFERERASE);

            m_rectLastPos = m_rectParentClientRequest;

         }

         if (IsWindowVisible())
         {

            round_window_redraw();

         }
         else
         {

//            _001UpdateWindow();

         }


      }
      else
      {

         //debug_break();

         m_pui->m_bRedraw = true;

      }

      return true;

   }

   /*
    bool interaction_impl::EnableScrollBar(int32_t nSBFlags, UINT nArrowFlags)
    {

    ASSERT(::is_window(get_handle()));

    return ::EnableScrollBar(get_handle(), nSBFlags, nArrowFlags) != FALSE;

    }
    */

   bool interaction_impl::DrawAnimatedRects(int32_t idAni, CONST RECT *lprcFrom, CONST RECT *lprcTo)
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //return ::DrawAnimatedRects(get_handle(), idAni, lprcFrom, lprcTo) != FALSE;

   }

   bool interaction_impl::DrawCaption(::draw2d::graphics * pgraphics, LPCRECT lprc, UINT uFlags)
   {

      _throw(not_implemented(get_app()));
      //ASSERT(::is_window(get_handle()));
      //return ::DrawCaption(get_handle(), (HDC)(dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->get_handle(), lprc, uFlags) != FALSE;

   }

   bool interaction_impl::SetTimer(uint_ptr nIDEvent, UINT nElapse, PFN_TIMER pfnTimer)
   {
      return ::user::interaction_impl::SetTimer(nIDEvent, nElapse, pfnTimer);
      /*
       UNREFERENCED_PARAMETER(lpfnTimer);

       m_pui->m_papp->set_timer(m_pui, nIDEvent, nElapse);

       return nIDEvent;

       //_throw(not_implemented(get_app()));
       //ASSERT(::is_window(get_handle()));
       //return ::SetTimer(get_handle(), nIDEvent, nElapse, lpfnTimer);
       //return true;*/

   }

   bool interaction_impl::KillTimer(uint_ptr nIDEvent)
   {
      return ::user::interaction_impl::KillTimer(nIDEvent);

      /*
       //_throw(not_implemented(get_app()));
       //ASSERT(::is_window(get_handle()));
       //return ::KillTimer(get_handle(), nIDEvent)  != FALSE;

       m_pui->m_papp->unset_timer(m_pui, nIDEvent);


       return true;*/

   }

   bool interaction_impl::IsWindowEnabled()
   {

      return true;

      /*      if(!::is_window(get_handle()))
       return false;

       return ::IsWindowEnabled(get_handle()) != FALSE;
       */

   }


   bool interaction_impl::EnableWindow(bool bEnable)
   {

      /*      ASSERT(::is_window(get_handle()));

       return ::EnableWindow(get_handle(), bEnable) != FALSE;
       */

      return true;

   }

   ::user::interaction *  interaction_impl::GetActiveWindow()
   {

      oswindow oswindow = ::get_active_window();

      if(oswindow == NULL)
      {

         return NULL;

      }

      ::user::interaction_impl * pimpl = ::ios::interaction_impl::from_handle(oswindow);

      if(pimpl == NULL)
      {

         return NULL;

      }

      return pimpl->m_pui;

   }

   ::user::interaction *  interaction_impl::SetActiveWindow()
   {

      ASSERT(::is_window(get_handle()));

      oswindow oswindow = ::set_active_window(get_handle());

      if(oswindow == NULL)
      {

         return NULL;

      }

      ::user::interaction_impl * pimpl = ::ios::interaction_impl::from_handle(oswindow);

      if(pimpl == NULL)
      {

         return NULL;

      }

      return pimpl->m_pui;

   }





   ::user::interaction *  interaction_impl::GetFocus()
   {

      oswindow oswindow = ::get_focus();

      if(oswindow == NULL)
      {

         return NULL;

      }

      ::user::interaction_impl * pimpl = ::ios::interaction_impl::from_handle(oswindow);

      if(pimpl == NULL)
      {

         return NULL;

      }

      return pimpl->m_pui;

   }


   bool interaction_impl::SetFocus()
   {

      if(!::is_window(get_handle()))
         return NULL;

      oswindow oswindow = ::set_focus(get_handle());

      if(oswindow == NULL)
      {

         return NULL;

      }

      ::user::interaction_impl * pimpl = ::ios::interaction_impl::from_handle(oswindow);

      if(pimpl == NULL)
      {

         return NULL;

      }

      return pimpl->m_pui;

   }


   ::user::interaction *  interaction_impl::get_desktop_window()
   {

      return NULL;

   }


   // Helper for radio buttons
   int32_t interaction_impl::GetCheckedRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton)
   {

      for (int32_t nID = nIDFirstButton; nID <= nIDLastButton; nID++)
      {
         if (IsDlgButtonChecked(nID))
            return nID; // id that matched
      }
      return 0; // invalid ID
   }

   void interaction_impl::CheckDlgButton(int32_t nIDButton, UINT nCheck)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::CheckDlgButton(get_handle(), nIDButton, nCheck);

   }

   void interaction_impl::CheckRadioButton(int32_t nIDFirstButton, int32_t nIDLastButton, int32_t nIDCheckButton)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::CheckRadioButton(get_handle(), nIDFirstButton, nIDLastButton, nIDCheckButton);

   }

   int32_t interaction_impl::DlgDirList(LPTSTR lpPathSpec, int32_t nIDListBox, int32_t nIDStaticPath, UINT nFileType)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::DlgDirList(get_handle(), lpPathSpec, nIDListBox, nIDStaticPath, nFileType);

   }

   int32_t interaction_impl::DlgDirListComboBox(LPTSTR lpPathSpec, int32_t nIDComboBox, int32_t nIDStaticPath, UINT nFileType)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::DlgDirListComboBox(get_handle(), lpPathSpec, nIDComboBox, nIDStaticPath, nFileType);

   }

   bool interaction_impl::DlgDirSelect(LPTSTR lpString, int32_t nSize, int32_t nIDListBox)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::DlgDirSelectEx(get_handle(), lpString, nSize, nIDListBox) != FALSE;

   }

   bool interaction_impl::DlgDirSelectComboBox(LPTSTR lpString, int32_t nSize, int32_t nIDComboBox)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::DlgDirSelectComboBoxEx(get_handle(), lpString, nSize, nIDComboBox) != FALSE;

   }

   /*
    void interaction_impl::GetDlgItem(id id, oswindow* phWnd) const
    {

    ASSERT(::is_window(get_handle()));
    ASSERT(phWnd != NULL);
    *phWnd = ::GetDlgItem(get_handle(), (int32_t) id);

    }
    */

   /*
    UINT interaction_impl::GetDlgItemInt(int32_t nID, WINBOOL * lpTrans, bool bSigned) const
    {

    ASSERT(::is_window(get_handle()));

    return ::GetDlgItemInt(get_handle(), nID, lpTrans, bSigned);

    }
    */

   //   int32_t interaction_impl::GetDlgItemText(int32_t nID, LPTSTR lpStr, int32_t nMaxCount) const
   //   {
   //
   //      _throw(not_implemented(get_app()));
   //      ASSERT(::is_window(get_handle())); return ::GetDlgItemText(get_handle(), nID, lpStr, nMaxCount);}

   ::user::interaction * interaction_impl::GetNextDlgGroupItem(::user::interaction * pWndCtl, bool bPrevious) const
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::macos::interaction_impl::from_handle(::GetNextDlgGroupItem(get_handle(), (oswindow) pWndCtl->get_handle(), bPrevious));

   }

   ::user::interaction * interaction_impl::GetNextDlgTabItem(::user::interaction * pWndCtl, bool bPrevious) const
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::macos::interaction_impl::from_handle(::GetNextDlgTabItem(get_handle(), (oswindow) pWndCtl->get_handle(), bPrevious));

   }

   UINT interaction_impl::IsDlgButtonChecked(int32_t nIDButton) const
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::IsDlgButtonChecked(get_handle(), nIDButton);

   }

   LPARAM interaction_impl::SendDlgItemMessage(int32_t nID, UINT message, WPARAM wparam, LPARAM lparam)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::SendDlgItemMessage(get_handle(), nID, message, wparam, lparam);

   }

   void interaction_impl::SetDlgItemInt(int32_t nID, UINT nValue, bool bSigned)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::SetDlgItemInt(get_handle(), nID, nValue, bSigned);

   }

   void interaction_impl::SetDlgItemText(int32_t nID, const char * lpszString)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::SetDlgItemText(get_handle(), nID, lpszString);

   }

   int32_t interaction_impl::ScrollWindowEx(int32_t dx, int32_t dy, LPCRECT lpRectScroll, LPCRECT lpRectClip, ::draw2d::region* prgnUpdate, LPRECT lpRectUpdate, UINT flags)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::ScrollWindowEx(get_handle(), dx, dy, lpRectScroll, lpRectClip, (HRGN)prgnUpdate->get_handle(), lpRectUpdate, flags);

   }

   void interaction_impl::ShowScrollBar(UINT nBar, bool bShow)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::ShowScrollBar(get_handle(), nBar, bShow);

   }

   ::user::interaction *  interaction_impl::ChildWindowFromPoint(POINT point)
   {


      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::macos::interaction_impl::from_handle(::ChildWindowFromPoint(get_handle(), point));

   }

   ::user::interaction *  interaction_impl::ChildWindowFromPoint(POINT point, UINT nFlags)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::macos::interaction_impl::from_handle(::ChildWindowFromPointEx(get_handle(), point, nFlags));

   }

   ::user::interaction * PASCAL interaction_impl::FindWindow(const char * lpszClassName, const char * lpszWindowName)
   {

      //      _throw(not_implemented(get_app()));
      //      return ::macos::interaction_impl::from_handle(::FindWindow(lpszClassName, lpszWindowName));
      return NULL;

   }

   ::user::interaction * interaction_impl::FindWindowEx(oswindow hwndParent, oswindow hwndChildAfter, const char * lpszClass, const char * lpszWindow)
   {

      _throw(not_implemented(::get_app()));
      //      return ::macos::interaction_impl::from_handle(::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow));

   }

   ::user::interaction *  interaction_impl::GetNextWindow(UINT nFlag)
   {

      return NULL;

   }

   ::user::interaction *  interaction_impl::GetTopWindow()
   {

      if(m_pui->m_uiptraChild.get_size() <= 0)
         return NULL;

      return m_pui->m_uiptraChild[0];

   }

   ::user::interaction *  interaction_impl::GetWindow(UINT nCmd)
   {

      ASSERT(::is_window(get_handle()));
      //      return ::macos::interaction_impl::from_handle(::GetWindow(get_handle(), nCmd));
      return NULL;

   }

   ::user::interaction *  interaction_impl::GetLastActivePopup()
   {


      _throw(todo(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::macos::interaction_impl::from_handle(::GetLastActivePopup(get_handle()));

   }

   ::user::interaction * interaction_impl::SetParent(::user::interaction * pWndNewParent)
   {

      ASSERT(::is_window(get_handle()));
      return from_handle(::SetParent(get_handle(), (oswindow) pWndNewParent->get_handle()))->m_pui;

   }

   ::user::interaction * PASCAL interaction_impl::oswindowFromPoint(POINT point)
   {


      _throw(not_implemented(::get_app()));
      //      return ::macos::interaction_impl::from_handle(::oswindowFromPoint(point));

   }

   bool interaction_impl::FlashWindow(bool bInvert)
   {


      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::FlashWindow(get_handle(), bInvert) != FALSE;

   }

   bool interaction_impl::ChangeClipboardChain(oswindow hWndNext)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::ChangeClipboardChain(get_handle(), hWndNext) != FALSE;

   }

   oswindow interaction_impl::SetClipboardViewer()
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::SetClipboardViewer(get_handle());

   }

   bool interaction_impl::OpenClipboard()
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::OpenClipboard(get_handle()) != FALSE;

   }

   ::user::interaction * PASCAL interaction_impl::GetOpenClipboardWindow()
   {

      _throw(not_implemented(::get_app()));
      //      return ::macos::interaction_impl::from_handle(::GetOpenClipboardWindow());

   }

   ::user::interaction * PASCAL interaction_impl::GetClipboardOwner()
   {

      _throw(not_implemented(::get_app()));
      //      return ::macos::interaction_impl::from_handle(::GetClipboardOwner());

   }

   ::user::interaction * PASCAL interaction_impl::GetClipboardViewer()
   {

      _throw(not_implemented(::get_app()));
      //      return ::macos::interaction_impl::from_handle(::GetClipboardViewer());

   }

   void interaction_impl::CreateCaret(::draw2d::bitmap* pBitmap)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::CreateCaret(get_handle(), (HBITMAP)pBitmap->get_handle(), 0, 0);

   }

   void interaction_impl::CreateSolidCaret(int32_t nWidth, int32_t nHeight)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::CreateCaret(get_handle(), (HBITMAP)0, nWidth, nHeight);

   }

   void interaction_impl::CreateGrayCaret(int32_t nWidth, int32_t nHeight)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::CreateCaret(get_handle(), (HBITMAP)1, nWidth, nHeight);

   }

   point PASCAL interaction_impl::GetCaretPos()
   {

      _throw(not_implemented(::get_app()));
      //      point point;
      //      ::GetCaretPos((LPPOINT)&point); return point;

   }

   void PASCAL interaction_impl::SetCaretPos(POINT point)
   {

      _throw(not_implemented(::get_app()));
      //      ::SetCaretPos(point.x, point.y);

   }

   void interaction_impl::HideCaret()
   {

      _throw(not_implemented(get_app()));
      //      ::HideCaret(get_handle());

   }

   void interaction_impl::ShowCaret()
   {

      _throw(not_implemented(get_app()));
      //    ::ShowCaret(get_handle());

   }

   bool interaction_impl::SetForegroundWindow()
   {

      bool b = ::SetForegroundWindow(get_handle()) != FALSE;

      //      round_window_show();

      return b;

   }

   ::user::interaction * PASCAL interaction_impl::GetForegroundWindow()
   {

      return NULL;
      // return ::macos::interaction_impl::from_handle(::GetForegroundWindow());

   }

   bool interaction_impl::SendNotifyMessage(UINT message, WPARAM wparam, LPARAM lparam)
   {

      _throw(not_implemented(get_app()));
      //      return ::SendNotifyMessage(get_handle(), message, wparam, lparam) != FALSE;

   }


   void interaction_impl::Print(::draw2d::graphics * pgraphics, DWORD dwFlags) const
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      const_cast < user::interaction * > (this)->send_message(WM_PRINT, (WPARAM)(dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->get_handle(), dwFlags);

   }

   void interaction_impl::PrintClient(::draw2d::graphics * pgraphics, DWORD dwFlags) const
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      const_cast < user::interaction * > (this)->send_message(WM_PRINTCLIENT, (WPARAM)(dynamic_cast<::draw2d_quartz2d::graphics * >(pgraphics))->get_handle(), dwFlags);

   }

   bool interaction_impl::SetWindowContextHelpId(DWORD dwContextHelpId)
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::SetWindowContextHelpId(get_handle(), dwContextHelpId) != FALSE;

   }

   DWORD interaction_impl::GetWindowContextHelpId() const
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::GetWindowContextHelpId(get_handle());

   }


   // Default message ::collection::map implementations
   void interaction_impl::OnActivateApp(bool, DWORD)
   { Default(); }
   void interaction_impl::OnActivate(UINT, ::user::interaction *, bool)
   { Default(); }
   void interaction_impl::OnCancelMode()
   { Default(); }
   void interaction_impl::OnChildActivate()
   { Default(); }
   void interaction_impl::OnClose()
   { Default(); }
   void interaction_impl::OnContextMenu(::user::interaction *, point)
   { Default(); }

   bool interaction_impl::OnCopyData(::user::interaction *, COPYDATASTRUCT*)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnEnable(bool)
   { Default(); }
   void interaction_impl::OnEndSession(bool)
   { Default(); }

   bool interaction_impl::OnEraseBkgnd(::draw2d::graphics *)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnGetMinMaxInfo(MINMAXINFO*)
   { Default(); }
   void interaction_impl::OnIconEraseBkgnd(::draw2d::graphics *)
   { Default(); }


   LRESULT interaction_impl::OnMenuChar(UINT, UINT, ::user::menu*)
   {
      
      return Default();
      
   }
   
   void interaction_impl::OnMenuSelect(UINT, UINT, HMENU)
   { Default(); }
   void interaction_impl::OnMove(int32_t, int32_t)
   { Default(); }
   HCURSOR interaction_impl::OnQueryDragIcon()
   { return (HCURSOR)Default(); }

   bool interaction_impl::OnQueryEndSession()
   {

      return Default() != FALSE;

   }

   bool interaction_impl::OnQueryNewPalette()
   {

      return Default() != FALSE;

   }

   bool interaction_impl::OnQueryOpen()
   {

      return Default() != FALSE;

   }

   void interaction_impl::_001OnSetCursor(::message::message * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(Session.get_cursor() != NULL
            && Session.get_cursor()->m_ecursor != ::visual::cursor_system)
      {

         _throw(not_implemented(get_app()));
         //         ::SetCursor(NULL);
      }
      pbase->set_lresult(1);
      pbase->m_bRet = true;
      //(bool)Default();
   }
   void interaction_impl::OnShowWindow(bool, UINT)
   { Default(); }
   void interaction_impl::OnSize(UINT, int32_t, int32_t)
   { Default(); }
   void interaction_impl::OnTCard(UINT, DWORD)
   { Default(); }
   void interaction_impl::OnWindowPosChanging(WINDOWPOS*)
   { Default(); }
   void interaction_impl::OnWindowPosChanged(WINDOWPOS*)
   { Default(); }
   void interaction_impl::OnDropFiles(HDROP)
   { Default(); }
   void interaction_impl::OnPaletteIsChanging(::user::interaction *)
   { Default(); }

   bool interaction_impl::OnNcActivate(bool)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnNcCalcSize(bool, NCCALCSIZE_PARAMS*)
   { Default(); }

   bool interaction_impl::OnNcCreate(::user::create_struct *)
   {

      return Default() != FALSE;

   }

   LRESULT interaction_impl::OnNcHitTest(point)
   { return Default(); }
   void interaction_impl::OnNcLButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnNcLButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnNcLButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnNcMouseMove(UINT, point)
   { Default(); }
   void interaction_impl::OnNcPaint()
   { Default(); }
   void interaction_impl::OnNcRButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnNcRButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnNcRButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnSysChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnSysCommand(UINT, LPARAM)
   { Default(); }
   void interaction_impl::OnSysDeadChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnSysKeyDown(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnSysKeyUp(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnCompacting(UINT)
   { Default(); }
   void interaction_impl::OnFontChange()
   { Default(); }
   void interaction_impl::OnPaletteChanged(::user::interaction *)
   { Default(); }
   void interaction_impl::OnSpoolerStatus(UINT, UINT)
   { Default(); }
   void interaction_impl::OnTimeChange()
   { Default(); }
   void interaction_impl::OnChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnDeadChar(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnKeyDown(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnKeyUp(UINT, UINT, UINT)
   { Default(); }
   void interaction_impl::OnLButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnLButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnLButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnMButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnMButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnMButtonUp(UINT, point)
   { Default(); }
   int32_t interaction_impl::OnMouseActivate(::user::interaction *, UINT, UINT)
   { return (int32_t)Default(); }
   void interaction_impl::OnMouseMove(UINT, point)
   { Default(); }

   bool interaction_impl::OnMouseWheel(UINT, short, point)
   {

      return Default() != FALSE;

   }

   LRESULT interaction_impl::OnRegisteredMouseWheel(WPARAM, LPARAM)
   { return Default(); }
   void interaction_impl::OnRButtonDblClk(UINT, point)
   { Default(); }
   void interaction_impl::OnRButtonDown(UINT, point)
   { Default(); }
   void interaction_impl::OnRButtonUp(UINT, point)
   { Default(); }
   void interaction_impl::OnTimer(uint_ptr)
   { Default(); }
   void interaction_impl::OnInitMenu(::user::menu*)
   { Default(); }
   void interaction_impl::OnInitMenuPopup(::user::menu*, UINT, bool)
   { Default(); }
   void interaction_impl::OnAskCbFormatName(UINT nMaxCount, LPTSTR pszName)
   {
      //      (nMaxCount);
      if(nMaxCount>0)
      {
         /* defwindow proc should do this for us, but to be safe, we'll do it here too */
         pszName[0]='\0';
      }
      Default();
   }
   void interaction_impl::OnChangeCbChain(oswindow, oswindow)
   { Default(); }
   void interaction_impl::OnDestroyClipboard()
   { Default(); }
   void interaction_impl::OnDrawClipboard()
   { Default(); }
   void interaction_impl::OnHScrollClipboard(::user::interaction *, UINT, UINT)
   { Default(); }
   void interaction_impl::OnPaintClipboard(::user::interaction *, HGLOBAL)
   { Default(); }
   void interaction_impl::OnRenderAllFormats()
   { Default(); }
   void interaction_impl::OnRenderFormat(UINT)
   { Default(); }
   void interaction_impl::OnSizeClipboard(::user::interaction *, HGLOBAL)
   { Default(); }
   void interaction_impl::OnVScrollClipboard(::user::interaction *, UINT, UINT)
   { Default(); }
   UINT interaction_impl::OnGetDlgCode()
   { return (UINT)Default(); }
   void interaction_impl::OnMDIActivate(bool, ::user::interaction *, ::user::interaction *)
   { Default(); }
   void interaction_impl::OnEnterMenuLoop(bool)
   { Default(); }
   void interaction_impl::OnExitMenuLoop(bool)
   { Default(); }
   // Win4 support
   //   void interaction_impl::OnStyleChanged(int32_t, LPSTYLESTRUCT)
   //   { Default(); }
   //   void interaction_impl::OnStyleChanging(int32_t, LPSTYLESTRUCT)
   //   { Default(); }
   void interaction_impl::OnSizing(UINT, LPRECT)
   { Default(); }
   void interaction_impl::OnMoving(UINT, LPRECT)
   { Default(); }
   void interaction_impl::OnCaptureChanged(::user::interaction *)
   { Default(); }

   bool interaction_impl::OnDeviceChange(UINT, uint_ptr)
   {

      return Default() != FALSE;

   }

   void interaction_impl::OnWinIniChange(const char *)
   { Default(); }
   void interaction_impl::OnChangeUIState(UINT, UINT)
   { Default(); }
   void interaction_impl::OnUpdateUIState(UINT, UINT)
   { Default(); }

   UINT interaction_impl::OnQueryUIState()
   {

      return (UINT)Default();

   }

   // user::interaction dialog data support
   //    void interaction_impl::DoDataExchange(CDataExchange*)
   //   { } // default does nothing

   // user::interaction modality support

   void interaction_impl::BeginModalState()
   {

      _throw(todo(get_app()));

      //::EnableWindow(get_handle(), FALSE);

   }

   void interaction_impl::EndModalState()
   {

      _throw(todo(get_app()));

      //::EnableWindow(get_handle(), TRUE);

   }

   // frame_window
   /*    void frame_window::DelayUpdateFrameTitle()
    { m_nIdleFlags |= idleTitle; }
    void frame_window::DelayRecalcLayout(bool bNotify)
    { m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };
    bool frame_window::InModalState() const
    { return m_cModalStack != 0; }
    void frame_window::set_title(const char * lpszTitle)
    { m_strTitle = lpszTitle; }
    string frame_window::get_title() const
    { return m_strTitle; }
    */



   void interaction_impl::CloseWindow()
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      ::CloseWindow(get_handle());

   }

   bool interaction_impl::OpenIcon()
   {

      _throw(not_implemented(get_app()));
      //      ASSERT(::is_window(get_handle()));
      //      return ::OpenIcon(get_handle()) != FALSE;

   }

   ////////////////////////////////////////////////////////////////////////////
   // UI related ::user::interaction functions

   oswindow PASCAL interaction_impl::GetSafeOwner_(oswindow hParent, oswindow* pWndTop)
   {
      // get ::user::interaction to start with
      oswindow hWnd = hParent;
      if (hWnd == NULL)
      {
         /* trans      frame_window* pFrame = command_target::GetRoutingFrame_();
          if (pFrame != NULL)
          hWnd = pFrame->get_handle();
          else
          hWnd = System.GetMainWnd()->get_handle();*/
      }

      // a popup ::user::interaction cannot be owned by a child ::user::interaction
      while (hWnd != NULL && (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD))
         hWnd = ::GetParent(hWnd);

      // determine toplevel ::user::interaction to disable as well
      oswindow hWndTop = hWnd, hWndTemp = hWnd;
      for (;;)
      {
         if (hWndTemp == NULL)
            break;
         else
            hWndTop = hWndTemp;
         hWndTemp = ::GetParent(hWndTop);
      }

      _throw(todo(::get_app()));
      // get last active popup of first non-child that was found
      //    if (hParent == NULL && hWnd != NULL)
      //       hWnd = ::GetLastActivePopup(hWnd);

      // disable and store top level parent ::user::interaction if specified
//      if (pWndTop != NULL)
//      {
//         /*         if (hWndTop != NULL && ::IsWindowEnabled(hWndTop) && hWndTop != hWnd)
//          {
//          *pWndTop = hWndTop;
//          ::EnableWindow(hWndTop, FALSE);
//          }
//          else
//          *pWndTop = ::caNULL;*/
//      }
//
//      return hWnd;    // return the owner as oswindow
   }





   void interaction_impl::_001OnEraseBkgnd(::message::message * pobj)
   {
      SCAST_PTR(::message::erase_bkgnd, perasebkgnd, pobj);
      perasebkgnd->m_bRet = true;
      perasebkgnd->set_result(TRUE);
   }

   
   void interaction_impl::_001BaseWndInterfaceMap()
   {

      System.window_map().set((int_ptr)get_handle(), this);

   }


   void interaction_impl::_001OnTriggerMouseInside()
   {


      //_throw(not_implemented(get_app()));
      //      m_bMouseHover = true;
      //      TRACKMOUSEEVENT tme = { sizeof(tme) };
      //      tme.dwFlags = TME_LEAVE;
      //      tme.hwndTrack = get_handle();
      //      TrackMouseEvent(&tme);

   }


   void interaction_impl::_001UpdateWindow(bool bUpdateBuffer)
   {

      ::user::interaction_impl::_001UpdateWindow(bUpdateBuffer);

   }


   void interaction_impl::_001UpdateScreen()
   {

      if(m_bShowFlags)
      {

         if(!IsWindowVisible() && (m_iShowFlags & SWP_SHOWWINDOW))
         {

            round_window_show();

         }
         else if(IsWindowVisible() && (m_iShowFlags & SWP_HIDEWINDOW))
         {

            round_window_hide();

         }

      }

      if (m_rectLastPos != m_rectParentClientRequest)
      {

         ::SetWindowPos(m_oswindow, NULL,
                        (int) m_rectParentClientRequest.left,
                        (int) m_rectParentClientRequest.top,
                        (int) m_rectParentClientRequest.width(),
                        (int) m_rectParentClientRequest.height(),
                        SWP_NOZORDER
                        | SWP_NOREDRAW
                        | SWP_NOCOPYBITS
                        | SWP_NOACTIVATE
                        | SWP_NOOWNERZORDER
                        | SWP_NOSENDCHANGING
                        | SWP_DEFERERASE);

         m_rectLastPos = m_rectParentClientRequest;

      }

      if (IsWindowVisible())
      {

         round_window_redraw();

      }

   }
   

   void interaction_impl::offset_viewport_org(LPRECT lprectScreen)
   {

   }


   void interaction_impl::set_viewport_org(::draw2d::graphics * pgraphics)
   {

      // graphics will be already set its view port to the user::interaction for linux - cairo with xlib

      pgraphics->SetViewportOrg(point(0, 0));

   }

   
   bool interaction_impl::round_window_become_first_responder()
   {
      
   
   }
   

   void interaction_impl::round_window_draw(CGContextRef cgc, int cx, int cy)
   {

      {

         single_lock sl(m_pui->m_pmutex, true);
         
         m_rectParentClientRequest.set_size(cx, cy);
         
         m_rectParentClient.set_size(cx, cy);

         if(m_bUpdateGraphics)
         {

            update_graphics_resources();

         }

      }

      cslock slDisplay(cs_display());

      window_buffer * pbuffer = m_spgraphics.cast < window_buffer >();

      synch_lock sl1(pbuffer->m_pmutex);

      ::draw2d::dib_sp & spdibBuffer = pbuffer->get_buffer();

      if(spdibBuffer.is_null())
      {

         return;

      }

      if(spdibBuffer->get_data() == NULL)
      {

         return;

      }

      ::draw2d::graphics_sp g(allocer());

      g->attach(cgc);

      ::rect rectClient;

      GetWindowRect(rectClient);
      
      g->BitBlt(0, 0, spdibBuffer->m_size.cx, spdibBuffer->m_size.cy, spdibBuffer->get_graphics(), 0, 0, SRCCOPY);
      
   }

   
   int interaction_impl::round_window_get_x()
   {

      return (int) m_rectParentClient.left;

   }

   int interaction_impl::round_window_get_y()
   {

      return (int) m_rectParentClient.top;

   }


   bool interaction_impl::round_window_key_down(::user::e_key ekey)
   {

      sp(::message::base) spbase;

      ::message::key * pkey = canew(::message::key(get_app()));

      pkey->m_id = WM_KEYDOWN;

      pkey->m_ekey = ekey;

      spbase = pkey;

      send(spbase);

      return spbase->m_bRet;

   }


   bool interaction_impl::round_window_key_up(::user::e_key ekey)
   {

      sp(::message::base) spbase;

      ::message::key * pkey = canew(::message::key(get_app()));

      pkey->m_id = WM_KEYUP;
      pkey->m_ekey = ekey;

      spbase = pkey;

      send(spbase);

      return spbase->m_bRet;

   }


   void interaction_impl::defer_update_text_view()
   {

      sp(::user::edit_text) ptext = Session.get_keyboard_focus();

      if(ptext.is_null())
      {

         return;

      }

      string strTextPrevious;

      long iSelBegPrevious = -1;

      long iSelEndPrevious = -1;

      string strText;

      strsize iSelBeg = -1;

      strsize iSelEnd = -1;

      {

         long iSize = round_window_get_text_length();

         char * psz = strTextPrevious.GetBufferSetLength(iSize);

         round_window_get_text(psz, iSize);

         strTextPrevious.ReleaseBuffer(iSize);

      }

      ptext->_001GetText(strText);

      if(strText != strTextPrevious)
      {

         round_window_set_text(strText);

      }

      round_window_get_sel(iSelBegPrevious, iSelEndPrevious);

      ptext->_001GetSel(iSelBeg, iSelEnd);

      if(iSelEndPrevious != iSelEnd || iSelBegPrevious != iSelBeg)
      {

         round_window_set_sel(iSelBeg, iSelEnd);

      }

   }


   bool interaction_impl::round_window_on_text(const char * pszText)
   {

      sp(::user::edit_text) ptext = Session.get_keyboard_focus();

      if(ptext.is_set())
      {

         string strText;

         long iSize = round_window_get_text_length();

         char * pszText = strText.GetBufferSetLength(iSize);

         round_window_get_text(pszText, iSize);

         strText.ReleaseBuffer(iSize);

         ptext->_001SetText(strText, ::action::source_user);

         long iSelBeg = -1;

         long iSelEnd = -1;

         round_window_get_sel(iSelBeg, iSelEnd);

         ptext->_001SetSel(iSelBeg, iSelEnd);

         return true;

      }
      else
      {

         sp(::message::base) spbase;

         ::message::key * pkey = canew(::message::key(get_app()));

         pkey->m_id = WM_KEYDOWN;

         string strText(pszText);

         if(strText.is_empty())
         {

            pkey->m_ekey = ::user::key_back;

         }
         else if(strText == '\t')
         {

            pkey->m_ekey = ::user::key_tab;

         }
         else if(strText == '\n')
         {

            pkey->m_ekey = ::user::key_return;

         }
         else
         {

            pkey->m_ekey = ::user::key_refer_to_text_member;

            pkey->m_strText = strText;

         }

         round_window_set_text("");

         spbase = pkey;

         send(spbase);

         return spbase->m_bRet;

      }

      return true;

   }


   bool interaction_impl::round_window_on_sel_text(long iBeg, long iEnd)
   {

      sp(::user::interaction) pui = Session.get_keyboard_focus();

      sp(::user::edit_text) pedit = pui;

      if(pedit.is_set())
      {

         pedit->MacroBegin();

         pedit->_001SetSel(iBeg, iEnd);

         pedit->MacroEnd();

         return true;

      }

      return true;

   }


   void interaction_impl::show_software_keyboard(bool bShow, string str, strsize iBeg, strsize iEnd)
   {
      
      ns_main_async(
      ^{
      
         if(bShow)
         {
      
            round_window_set_text(str);
      
            round_window_set_sel(iBeg, iEnd);
         
         }
      
         round_window_show_keyboard(bShow);
                       
      });
      
   }


   void interaction_impl::_001OnSetFocus(::message::message * pmessage)
   {

   }


   void interaction_impl::_001OnKillFocus(::message::message * pmessage)
   {
      
   }
   
   
   void interaction_impl::round_window_mouse_down(double x, double y)
   {

      sp(::message::base) spbase;

      if(!is_active())
      {

         try
         {

            ::message::mouse_activate * pmouseactivate = canew(::message::mouse_activate(get_app()));

            pmouseactivate->m_id = WM_MOUSEACTIVATE;

            spbase = pmouseactivate;

            send(spbase);

            if(spbase->get_lresult() == MA_ACTIVATE || spbase->get_lresult() == MA_ACTIVATEANDEAT)
            {

               ::message::activate * pactivate = canew(::message::activate(get_app()));

               pactivate->m_id = WM_ACTIVATE;
               pactivate->m_wparam = WA_CLICKACTIVE;
               pactivate->m_nState = WA_CLICKACTIVE;
               pactivate->m_bMinimized = false;

               spbase = pactivate;

               send(spbase);

            }

         }
         catch(...)
         {
         }

      }

      {

         ::message::mouse * pmouse = canew(::message::mouse(get_app()));

         pmouse->m_id = WM_LBUTTONDOWN;
         pmouse->m_pt.x = (LONG) x;
         pmouse->m_pt.y = (LONG) y;
         pmouse->m_bTranslated = true;
         //pmouse->m_bTranslateMouseMessageCursor = true;

         spbase = pmouse;

         send(spbase);

      }

   }


   void interaction_impl::round_window_mouse_up(double x, double y)
   {

      sp(::message::base) spbase;

      ::message::mouse * pmouse = canew(::message::mouse(get_app()));

      pmouse->m_id = WM_LBUTTONUP;
      pmouse->m_pt.x = (LONG) x;
      pmouse->m_pt.y = (LONG) y;
      pmouse->m_bTranslated = true;
      //      pmouse->m_bTranslateMouseMessageCursor = true;

      spbase = pmouse;

      send(spbase);

   }


   void interaction_impl::round_window_mouse_moved(double x, double y)
   {

      sp(::message::base) spbase;

      ::message::mouse * pmouse = canew(::message::mouse(get_app()));

      pmouse->m_id = WM_MOUSEMOVE;
      pmouse->m_pt.x = (LONG) x;
      pmouse->m_pt.y = (LONG) y;
      pmouse->m_bTranslated = true;
      //      pmouse->m_bTranslateMouseMessageCursor = true;

      spbase = pmouse;

      send(spbase);

   }


   void interaction_impl::round_window_mouse_dragged(double x, double y)
   {

      sp(::message::base) spbase;

      ::message::mouse * pmouse = canew(::message::mouse(get_app()));

      pmouse->m_id = WM_MOUSEMOVE;
      pmouse->m_pt.x = (LONG) x;
      pmouse->m_pt.y = (LONG) y;
      pmouse->m_bTranslated = true;
      //      pmouse->m_bTranslateMouseMessageCursor = true;

      spbase = pmouse;

      send(spbase);

   }
   void interaction_impl::round_window_resized(CGRect rect)
   {


      ::size sz;

      point64 pt(rect.origin.x, rect.origin.y);

      bool bMove = false;

      {

         synch_lock sl(m_pui->m_pmutex);

         if (pt != m_rectParentClientRequest.top_left())
         {

            bMove = true;

         }

         m_rectParentClientRequest.move_to(pt);

         m_rectParentClientRequest.set_size(rect.size.width, rect.size.height);

         sz = m_rectParentClientRequest.get_size();

      }

      ::rect r;

      copy(r, m_rectParentClientRequest);

      SetWindowRect(m_oswindow, r);



      m_pui->send_message(WM_SIZE, 0, sz.lparam());

      if (bMove)
      {

         m_pui->send_message(WM_MOVE, 0, pt.lparam());

      }

   }

   bool interaction_impl::has_focus()
   {
      
      return get_handle() == ::get_focus();
      
   }
   
   bool interaction_impl::is_active()
   {
      
      return get_handle() == ::get_active_window();
      
   }

   void interaction_impl::round_window_moved(CGPoint point)
   {

      ::point pt;

      {

         synch_lock sl(m_pui->m_pmutex);

         m_rectParentClient.move_to(point.x, point.y);

         pt = m_rectParentClient.top_left();

      }

      m_pui->send_message(WM_MOVE, 0, pt.lparam());

   }


   void interaction_impl::round_window_on_show()
   {

      m_pui->message_call(WM_SHOWWINDOW, 1);

   }


   void interaction_impl::round_window_on_hide()
   {

      m_pui->message_call(WM_SHOWWINDOW, 0);

   }


   void interaction_impl::round_window_activate()
   {

      ::set_active_window(get_handle());

      m_pui->set_need_redraw();

   }


   void interaction_impl::round_window_deactivate()
   {

      ::deactivate_window(get_handle());

      m_pui->set_need_redraw();

   }

   bool interaction_impl::has_pending_graphical_update()
   {

      if (::user::interaction_impl::has_pending_graphical_update())
      {

         return true;

      }

      synch_lock sl(m_pui->m_pmutex);

      for (auto p : m_pui->m_uiptraChild)
      {

         if (p->has_pending_graphical_update())
         {

            return true;

         }

      }


      return false;

   }

} // namespace ios





















namespace aura
{


   void * system::initialize_native_window2(LPCRECT lpcrect)
   {

      return m_possystemwindow->m_pui->m_pimpl.cast < ::ios::interaction_impl >()->m_proundwindow;

   }


} // namespace base
