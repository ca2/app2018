#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   interaction_impl_base::interaction_impl_base() :
      m_rectParentClient(0,0,0,0),
      m_rectParentClientRequest(0, 0, 0, 0),
      m_mutexLongPtr(get_app())
   {

      m_iPendingRectMatch  = -1;
      m_bPendingRedraw     = false;
      m_bZ                 = false;
      m_pui                = NULL;
      m_bIgnoreSizeEvent   = false;
      m_bIgnoreMoveEvent   = false;
      m_bShowWindow        = false;
      m_iShowWindow        = -1;
      m_bShowFlags         = false;
      m_iShowFlags         = 0;
      m_bUserElementalOk   = true;

   }


   interaction_impl_base::~interaction_impl_base()
   {

      m_pui = NULL;

   }

   bool interaction_impl_base::create_message_queue(::user::interaction * pui, const char * pszName)
   {

      ::exception::throw_interface_only(get_app());

      return true;

   }


   bool interaction_impl_base::create_window(::user::interaction * pui, const RECT & rect, ::user::interaction *pparent, id id)
   {

      ::exception::throw_interface_only(get_app());

      return true;

   }


   bool interaction_impl_base::create_window(::user::interaction * pui, const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT & rect, ::user::interaction * pParentWnd, id id, ::create * pcreate)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


   bool interaction_impl_base::create_window_ex(::user::interaction * pui, ::user::create_struct & cs, ::user::interaction * puiParent, id id)
   {

      ::exception::throw_interface_only(get_app());

      return true;

   }


   ::user::interaction_impl * interaction_impl_base::get_user_interaction_impl()
   {

      return NULL;

   }


   ::user::interaction_child * interaction_impl_base::get_user_interaction_child()
   {

      return NULL;

   }



   bool interaction_impl_base::check_need_layout()
   {

      if(m_pui == NULL)
      {

         return false;

      }

      if(m_pui->m_bNeedLayout)
      {

         return true;

      }

      if(m_rectParentClientRequest.get_size() != m_rectParentClient.get_size())
      {

         return true;

      }

      return false;

   }


   void interaction_impl_base::clear_need_layout()
   {

      if(m_iPendingRectMatch < 0 || m_rectParentClient == m_rectParentClientRequest)
      {

         m_rectParentClient = m_rectParentClientRequest;

         m_bShowFlags = false;

         m_iShowFlags = 0;

         m_pui->m_bNeedLayout = false;

      }

   }


   bool interaction_impl_base::check_need_translation()
   {

      return m_rectParentClientRequest.top_left() != m_rectParentClient.top_left();

   }


   void interaction_impl_base::clear_need_translation()
   {

      if(m_iPendingRectMatch < 0 || m_rectParentClient == m_rectParentClientRequest)
      {

         m_rectParentClient = m_rectParentClientRequest;

         m_bShowFlags = false;

      }

   }


   bool interaction_impl_base::check_show_flags()
   {

      return m_bShowFlags || m_bShowWindow;

   }


   void interaction_impl_base::clear_show_flags()
   {

      if(m_iPendingRectMatch < 0 || m_rectParentClient == m_rectParentClientRequest)
      {

         m_rectParentClient = m_rectParentClientRequest;

         m_bShowFlags = false;

         m_bShowWindow = false;

      }

   }


   bool interaction_impl_base::check_need_zorder()
   {

      return m_bZ;

   }


   void interaction_impl_base::clear_need_zorder()
   {

      if(m_iPendingRectMatch < 0 || m_rectParentClient == m_rectParentClientRequest)
      {

         m_bZ = false;

      }

   }


   void interaction_impl_base::on_zorder()
   {

   }


   void interaction_impl_base::set_need_layout()
   {

      m_pui->m_bNeedLayout = true;

      if (m_pui->m_pparent != NULL)
      {

         m_pui->m_pparent->set_need_layout();

      }

   }


   bool interaction_impl_base::has_pending_redraw_flags()
   {

      bool bNeedLayout = m_pui->m_bNeedLayout;

      bool bNeedRedraw = m_pui->m_bRedraw;

      if(bNeedLayout || bNeedRedraw)
      {

         return true;

      }

      return m_bZ
             || m_bShowFlags
             || m_rectParentClient != m_rectParentClientRequest;

   }


   void interaction_impl_base::on_layout()
   {

      if (!m_bUserElementalOk)
      {

         return;

      }

      ::rect64 rectOld = m_rectParentClient;

      ::rect64 rectNew = m_rectParentClientRequest;

      m_rectParentClient = m_rectParentClientRequest;

      if (rectOld.top_left() != rectNew.top_left() || (m_iShowFlags & SWP_CHANGEVIEWPORTOFFSET))
      {

         m_pui->message_call(WM_MOVE);

         sp(::user::interaction) pui;

         while (m_pui->get_child(pui))
         {

            pui->on_translate();

         }

      }

      m_pui->message_call(WM_SIZE);

      m_pui->m_dwLastSizeMove = ::get_tick_count();

      m_pui->m_bSizeMove = true;

   }


   void interaction_impl_base::on_translate()
   {

      if (::is_null(m_pui))
      {

         return;

      }

      m_rectParentClient.move_to(m_rectParentClientRequest.top_left());

      m_pui->message_call(WM_MOVE);

      m_pui->m_dwLastSizeMove = ::get_tick_count();

      m_pui->m_bSizeMove = true;

      sp(::user::interaction) pui;

      while (m_pui->get_child(pui))
      {

         pui->on_translate();

      }

   }


   void interaction_impl_base::on_do_show_flags()
   {

      if ((m_bShowFlags && (m_iShowFlags & SWP_SHOWWINDOW)) ||
            (m_bShowWindow && (m_iShowWindow != SW_HIDE)))
      {

         ModifyStyle(0, WS_VISIBLE);

         m_pui->message_call(WM_SHOWWINDOW, 1);

      }
      else if ((m_bShowFlags && (m_iShowFlags & SWP_HIDEWINDOW)) ||
               (m_bShowWindow && (m_iShowWindow == SW_HIDE)))
      {

         ModifyStyle(WS_VISIBLE, 0);

         if(m_pui != NULL)
         {

            m_pui->message_call(WM_SHOWWINDOW, 0);

         }

      }

   }


   void interaction_impl_base::_001WindowMinimize(bool bNoActivate)
   {

      m_pui->set_appearance(appearance_iconic);

      rect rectNormal;

      m_pui->GetWindowRect(rectNormal);

      m_pui->good_iconify(NULL,rectNormal,true, (bNoActivate ? SWP_NOACTIVATE : 0) | SWP_FRAMECHANGED);

   }


   void interaction_impl_base::_001WindowDock(::user::e_appearance eappearance)
   {

      ASSERT(is_docking_appearance(eappearance));

      if(is_docking_appearance(eappearance))
      {

         m_pui->make_zoneing(NULL, null_rect(), true, &eappearance);

      }

   }


   void interaction_impl_base::_001WindowMaximize()
   {

      m_pui->set_appearance(appearance_zoomed);

      rect rectNormal;

      m_pui->GetWindowRect(rectNormal);

      m_pui->best_wkspace(NULL,rectNormal,true);

   }


   void interaction_impl_base::_001WindowFullScreen(LPCRECT lpcrectHint)
   {

      m_pui->set_appearance(appearance_full_screen);

      rect rectNormal;

      if(lpcrectHint != NULL)
      {

         rectNormal = *lpcrectHint;

      }
      else
      {

         m_pui->GetWindowRect(rectNormal);

      }

      m_pui->best_monitor(NULL,rectNormal,true);

   }


   void interaction_impl_base::_001WindowRestore()
   {

      m_pui->set_appearance(appearance_normal);

      rect rectNormal;

      m_pui->GetWindowRect(rectNormal);

      m_pui->good_restore(NULL,rectNormal,true);

   }


   void interaction_impl_base::RepositionBars(UINT nIDFirst,UINT nIDLast, id idLeft, UINT nFlags,LPRECT lpRectParam,LPCRECT lpRectClient,bool bStretch)
   {

      if (!IsWindow())
      {

         return;

      }

      UNREFERENCED_PARAMETER(nIDFirst);
      UNREFERENCED_PARAMETER(nIDLast);

      ASSERT(nFlags == 0 || (nFlags & ~reposNoPosLeftOver) == reposQuery || (nFlags & ~reposNoPosLeftOver) == reposExtra);

      SIZEPARENTPARAMS sizeparentparams;

      sp(::user::interaction) puiLeft = NULL;

      sizeparentparams.bStretch = bStretch;

      sizeparentparams.sizeTotal.cx = sizeparentparams.sizeTotal.cy = 0;

      if(lpRectClient != NULL)
      {

         sizeparentparams.rect = *lpRectClient;

      }
      else
      {

         m_pui->GetClientRect(&sizeparentparams.rect);

      }

      if (::IsRectEmpty(sizeparentparams.rect))
      {

         return;

      }

      sp(::user::interaction) pui;

      while(m_pui->get_child(pui))
      {

         id id = pui->GetDlgCtrlId();

         if (id == idLeft)
         {

            puiLeft = pui;

         }
         else
         {

            pui->send_message(WM_SIZEPARENT, 0, (LPARAM)&sizeparentparams);

         }

      }

      if((nFlags & ~reposNoPosLeftOver) == reposQuery)
      {

         ASSERT(lpRectParam != NULL);

         if (bStretch)
         {

            ::CopyRect(lpRectParam, &sizeparentparams.rect);

         }
         else
         {

            lpRectParam->left = lpRectParam->top = 0;
            lpRectParam->right = sizeparentparams.sizeTotal.cx;
            lpRectParam->bottom = sizeparentparams.sizeTotal.cy;

         }

         return;

      }


      if(!idLeft.is_empty() && puiLeft != NULL)
      {

         if((nFlags & ~reposNoPosLeftOver) == reposExtra)
         {

            ASSERT(lpRectParam != NULL);

            sizeparentparams.rect.left += lpRectParam->left;
            sizeparentparams.rect.top += lpRectParam->top;
            sizeparentparams.rect.right -= lpRectParam->right;
            sizeparentparams.rect.bottom -= lpRectParam->bottom;

         }

         if((nFlags & reposNoPosLeftOver) != reposNoPosLeftOver)
         {

            puiLeft->CalcWindowRect(&sizeparentparams.rect);

            puiLeft->SetPlacement(sizeparentparams.rect, SWP_SHOWWINDOW | SWP_NOZORDER);

         }

      }

   }


   bool interaction_impl_base::GetWindowRect(LPRECT lprect)
   {

      if(m_pui == NULL)
      {

         return false;

      }

      if(!m_pui->GetWindowRect(lprect))
      {

         return false;

      }

      return true;

   }


   bool interaction_impl_base::GetClientRect(LPRECT lprect)
   {

      if(m_pui == NULL)
      {

         return false;

      }

      if(!m_pui->GetClientRect(lprect))
      {

         return false;

      }

      return true;

   }


   bool interaction_impl_base::GetClientRect(RECT64 * lprect)
   {

      if(m_pui == NULL)
      {

         return false;

      }

      //if(!m_pui->GetClientRect(lprect))
      //{

      // return false;

      //}

      lprect->left = 0;
      lprect->top = 0;
      lprect->right = m_rectParentClient.width();
      lprect->bottom = m_rectParentClient.height();

      return true;

   }


   bool interaction_impl_base::GetWindowRect(RECT64 * lprect)
   {

//      if(!IsWindow())
//      {
//
//         return false;
//
//      }
//


      *lprect = m_rectParentClient;


      return true;

   }


   rect interaction_impl_base::GetWindowRect()
   {

      return m_pui->GetWindowRect();

   }


   rect64 interaction_impl_base::GetWindowRect64()
   {

      return m_pui->GetWindowRect64();

   }


   bool interaction_impl_base::SetPlacement(const RECT & rect,UINT nFlags)
   {

      return m_pui->SetPlacement(rect, nFlags);

   }


   bool interaction_impl_base::RepositionWindow(const RECT & rect,UINT nFlags)
   {

      return m_pui->RepositionWindow(rect, nFlags);

   }


   bool interaction_impl_base::RepositionWindow(int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      return m_pui->RepositionWindow(x, y, cx, cy, nFlags);

   }


   bool interaction_impl_base::MoveWindow(int32_t x,int32_t y,UINT nFlags)
   {

      return m_pui->MoveWindow(x, y, nFlags);

   }


   bool interaction_impl_base::MoveWindow(POINT pt,UINT nFlags)
   {

      return m_pui->MoveWindow(pt, nFlags);

   }


   bool interaction_impl_base::SizeWindow(int32_t cx,int32_t cy,UINT nFlags)
   {

      return m_pui->SizeWindow(cx, cy, nFlags);

   }


   bool interaction_impl_base::SizeWindow(SIZE sz,UINT nFlags)
   {

      return m_pui->SizeWindow(sz, nFlags);

   }


   bool interaction_impl_base::ResizeWindow(int32_t cx,int32_t cy,UINT nFlags)
   {

      return m_pui->ResizeWindow(cx, cy, nFlags);

   }


   bool interaction_impl_base::ResizeWindow(SIZE sz,UINT nFlags)
   {

      return m_pui->ResizeWindow(sz, nFlags);

   }


   bool interaction_impl_base::SetWindowPos(int_ptr z,const RECT & rect,UINT nFlags)
   {

      return m_pui->SetWindowPos(z, rect, nFlags);

   }


   bool interaction_impl_base::SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      if(is_null(m_pui))
      {

         return false;

      }

      synch_lock sl(m_pui->m_pmutex);

      if (!(nFlags & SWP_NOZORDER))
      {

         m_bZ = true;

         m_iZ = z;

      }

      LONG lNoZ = ~(SWP_NOZORDER);

      if(m_bShowFlags)
      {

         if (nFlags & SWP_SHOWWINDOW)
         {

            m_iShowFlags &= ~SWP_HIDEWINDOW;

         }
         else if (nFlags & SWP_HIDEWINDOW)
         {

            m_iShowFlags &= ~SWP_SHOWWINDOW;

         }

         m_iShowFlags |= nFlags & lNoZ;

      }
      else
      {

         m_iShowFlags = nFlags & lNoZ;

      }

      if(strstr(typeid(*m_pui).name(), "main_frame") != NULL)
      {

         output_debug_string("\nmainframe m_iShowFlags = 0x" + ::hex::upper_from(m_iShowFlags));

      }

      bool bShowFlags = m_bShowFlags;

      if (!(nFlags & SWP_NOREDRAW) && (IsWindowVisible() || (nFlags & SWP_SHOWWINDOW)))
      {

         bShowFlags = !is_this_visible();

         if (nFlags & SWP_SHOWWINDOW)
         {

            if (m_iShowWindow <= SW_HIDE)
            {

               m_iShowWindow = SW_NORMAL;

            }

         }

      }

      ::rect64 rect = m_rectParentClientRequest;

      if (!(nFlags & SWP_NOMOVE) && (rect.left != x || rect.top != y))
      {

         rect.move_to(x, y);

      }

      if (!(nFlags & SWP_NOSIZE) && (rect.width() != cx || rect.height() != cy))
      {

         rect.set_size(cx, cy);

      }

      m_rectParentClientRequest = rect;

      m_bShowFlags = bShowFlags;

      deferred_on_change_visibility();

      set_need_redraw();

      return true;

   }

   bool interaction_impl_base::defer_set_window_pos(int_ptr z, const RECT & rect, UINT nFlags) // only set_windows_pos if get_parent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)
   {

      return m_pui->defer_set_window_pos(z, rect, nFlags);

   }


   bool interaction_impl_base::defer_set_window_pos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags) // only set_windows_pos if get_parent()->ScreenToClient(get_window_rect) different of rect(x, y, cx, cy)
   {

      return m_pui->defer_set_window_pos(z, x, y, cx, cy, nFlags);

   }


   void interaction_impl_base::_001Print(::draw2d::graphics * pgraphics)
   {

      windowing_output_debug_string("\ninteraction_impl_base::_001Print 1");

      ::user::interaction * pui = get_wnd();

      windowing_output_debug_string("\ninteraction_impl_base::_001Print 2");

//      if (GetActiveWindow() == pui)
//      {
//
//         //TRACE("active");
//
//      }
//      else
//      {
//
//         //TRACE("not active");
//
//      }

      windowing_output_debug_string("\ninteraction_impl_base::_001Print 3");

      m_pui->_001Print(pgraphics);

   }


   void interaction_impl_base::_000OnDraw(::draw2d::graphics * pgraphics)
   {

      m_pui->_000OnDraw(pgraphics);

   }


   void interaction_impl_base::_001DrawThis(::draw2d::graphics * pgraphics)
   {

      m_pui->_001DrawThis(pgraphics);

   }


   void interaction_impl_base::_001DrawChildren(::draw2d::graphics * pgraphics)
   {

      m_pui->_001DrawChildren(pgraphics);

   }


   void interaction_impl_base::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      m_pui->_001OnDraw(pgraphics);

   }


   void interaction_impl_base::draw_control_background(::draw2d::graphics * pgraphics)
   {

      m_pui->draw_control_background(pgraphics);

   }


   //::user::interaction * interaction_impl_base::get_wnd() const
   //{

   //   return m_pui->get_wnd();

   //}



   void interaction_impl_base::set_viewport_org(::draw2d::graphics * pgraphics)
   {

      //::exception::throw_interface_only(get_app());

   }


   void interaction_impl_base::viewport_client_to_screen(POINT * ppt)
   {

      ClientToScreen(ppt);

   }


   void interaction_impl_base::viewport_screen_to_client(POINT * ppt)
   {

      ScreenToClient(ppt);

   }


   void interaction_impl_base::viewport_client_to_screen(RECT * prect)
   {

      viewport_client_to_screen((POINT *)&prect->left);
      viewport_client_to_screen((POINT *)&prect->right);

   }


   void interaction_impl_base::viewport_screen_to_client(RECT * prect)
   {

      viewport_screen_to_client((POINT *)&prect->left);
      viewport_screen_to_client((POINT *)&prect->right);

   }


   bool interaction_impl_base::RedrawWindow(LPCRECT lpRectUpdate,::draw2d::region* prgnUpdate,UINT flags)
   {

      return m_pui->RedrawWindow(lpRectUpdate,prgnUpdate,flags);

   }


   uint32_t interaction_impl_base::GetStyle() const
   {

      return get_window_long(GWL_STYLE);

   }


   uint32_t interaction_impl_base::GetExStyle() const
   {

      return get_window_long(GWL_EXSTYLE);

   }


   bool interaction_impl_base::ModifyStyle(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      LONG l = GetStyle();

      //LONG lAdd = dwAdd;

      l |= dwAdd;

      LONG lRemove = ~dwRemove;

      l &= lRemove;

      set_window_long(GWL_STYLE, l);

      return true;

   }


   bool interaction_impl_base::ModifyStyleEx(uint32_t dwRemove,uint32_t dwAdd,UINT nFlags)
   {

      set_window_long(GWL_EXSTYLE,(GetExStyle() | dwAdd) & ~dwRemove);

      return true;

   }


   LONG interaction_impl_base::get_window_long(int32_t nIndex) const
   {

      return (LONG)get_window_long_ptr(nIndex);

   }


   LONG interaction_impl_base::set_window_long(int32_t nIndex,LONG lValue)
   {

      return (LONG)set_window_long_ptr(nIndex, lValue);

   }


   LONG_PTR interaction_impl_base::get_window_long_ptr(int32_t nIndex) const
   {

      synch_lock sl(&((interaction_impl_base*)this)->m_mutexLongPtr);

      return (LONG_PTR)m_longptr[nIndex];

   }


   LONG_PTR interaction_impl_base::set_window_long_ptr(int32_t nIndex,LONG_PTR lValue)
   {

      synch_lock sl(&m_mutexLongPtr);

      m_longptr[nIndex] = lValue;

      return lValue;

   }


   id interaction_impl_base::GetDlgCtrlId() const
   {

      return m_pui->GetDlgCtrlId();

   }


   id interaction_impl_base::SetDlgCtrlId(id id)
   {

      return m_pui->SetDlgCtrlId(id);

   }



   ::user::interaction * interaction_impl_base::first_child()
   {

      return m_pui->first_child();

   }


   ::user::interaction * interaction_impl_base::top_child()
   {

      return m_pui->first_child();

   }


   ::user::interaction * interaction_impl_base::under_sibling()
   {

      return m_pui->under_sibling();

   }


   ::user::interaction * interaction_impl_base::above_sibling()
   {

      return m_pui->above_sibling();

   }


   ::user::interaction * interaction_impl_base::above_sibling(::user::interaction * pui)
   {

      return m_pui->above_sibling(pui);

   }


   ::user::interaction * interaction_impl_base::under_sibling(::user::interaction * pui)
   {

      return m_pui->under_sibling(pui);

   }


   UINT interaction_impl_base::ArrangeIconicWindows()
   {

//      return m_pui->ArrangeIconicWindows();
      return 0;

   }


   void interaction_impl_base::BringToTop(int nCmdShow)
   {

      //return m_pui->BringToTop(nCmdShow);

   }


   bool interaction_impl_base::BringWindowToTop()
   {

      return true;

   }


   bool interaction_impl_base::IsAscendant(const ::user::interaction * puiIsAscendant) const
   {

      if(puiIsAscendant == NULL)
         return false;

      return puiIsAscendant->IsDescendant(m_pui);

   }


   bool interaction_impl_base::IsParent(const ::user::interaction * puiIsParent) const
   {

      if(puiIsParent == NULL)
         return false;

      return puiIsParent->IsChild(m_pui);

   }


   bool interaction_impl_base::IsChild(const ::user::interaction * puiIsChild) const
   {

      if(puiIsChild == NULL)
         return false;

      ::user::interaction * puiProbe = puiIsChild->GetParent();

      return puiProbe == m_pui;

   }


   bool interaction_impl_base::IsDescendant(const ::user::interaction * puiIsDescendant) const
   {

      ::user::interaction * puiProbe = puiIsDescendant->GetParent();

      if(puiProbe == NULL)
         return false;

      do
      {

         if(puiProbe == m_pui)
            return true;

         puiProbe = puiProbe->GetParent();

      }
      while(puiProbe != NULL);

      return false;

   }

   ::user::interaction * interaction_impl_base::get_wnd() const
   {

      if (m_pui == NULL)
      {

         return NULL;

      }

      return m_pui->get_wnd();

   }


   ::user::interaction * interaction_impl_base::get_wnd(UINT nCmd) const
   {

      if (m_pui == NULL)
      {

         return NULL;

      }

      return m_pui->get_wnd(nCmd);

   }


   ::user::interaction * interaction_impl_base::GetTopWindow() const
   {

      sp(::user::interaction) pui = get_wnd();

      if(pui.is_null())
         return NULL;

      return pui->GetTopWindow();

   }


   ::user::interaction * interaction_impl_base::GetParent() const
   {

      if (m_pui == NULL)
      {

         return NULL;

      }

      return m_pui->GetParent();

   }


   ::user::interaction * interaction_impl_base::GetOwner() const
   {

      if (m_pui == NULL)
      {

         return NULL;

      }

      return m_pui->m_puiOwner;

   }


   ::user::frame * interaction_impl_base::GetFrame() const
   {

      return m_pui->GetFrame();

   }


   ::user::frame * interaction_impl_base::GetParentFrame() const
   {

      return m_pui->GetParentFrame();

   }


   ::user::interaction * interaction_impl_base::GetParentOwner() const
   {

      return m_pui->GetParentOwner();

   }


   ::user::interaction * interaction_impl_base::GetParentOrOwner() const
   {

      return m_pui->GetParentOrOwner();

   }


   ::user::interaction * interaction_impl_base::GetTopLevelOwner() const
   {

      return m_pui->GetTopLevelOwner();

   }


   ::user::frame * interaction_impl_base::GetTopLevelFrame() const
   {

      return m_pui->GetTopLevelFrame();

   }

   ::user::frame * interaction_impl_base::EnsureParentFrame()
   {

      return m_pui->EnsureParentFrame();

   }


   ::user::interaction * interaction_impl_base::GetTopLevel() const
   {

      return m_pui->GetTopLevel();

   }


   ::user::interaction * interaction_impl_base::GetParentTopLevel() const
   {

      return m_pui->GetParentTopLevel();

   }


   ::user::frame * interaction_impl_base::GetParentTopLevelFrame() const
   {

      return m_pui->GetParentTopLevelFrame();

   }


   ::user::interaction * interaction_impl_base::EnsureTopLevel()
   {

      return m_pui->EnsureTopLevel();

   }


   ::user::interaction * interaction_impl_base::EnsureParentTopLevel()
   {

      return m_pui->EnsureParentTopLevel();

   }


   LRESULT interaction_impl_base::message_call(UINT uiMessage, WPARAM wparam, lparam lparam)
   {

      if(m_pui == NULL)
      {

         return 0;

      }

      smart_pointer < ::message::base > spbase;

      spbase = m_pui->get_message_base(uiMessage, wparam, lparam);

      if (m_pui->WfiIsMoving())
      {
         TRACE("moving: skip walk pre translate tree");
      }
      else if (m_pui->WfiIsSizing())
      {
         TRACE("sizing: skip walk pre translate tree");
      }
      else
      {

         m_pui->walk_pre_translate_tree(spbase);

         if (spbase->m_bRet)
         {

            return spbase->get_lresult();

         }

      }

      message_handler(spbase);

      return spbase->get_lresult();

   }


   void interaction_impl_base::send_message_to_descendants(UINT message,WPARAM wparam,lparam lparam,bool bDeep,bool bOnlyPerm)
   {

      return m_pui->send_message_to_descendants(message, wparam, lparam, bDeep, bOnlyPerm);

   }


   void interaction_impl_base::pre_translate_message(::message::message * pobj)
   {

      m_pui->pre_translate_message(pobj);

   }


   bool interaction_impl_base::SetCapture(::user::interaction * pui)
   {

      return get_wnd()->SetCapture(pui);

   }


   ::user::interaction * interaction_impl_base::GetCapture()
   {

      ::user::interaction * pui = get_wnd();

      if (pui == NULL)
      {

         return NULL;

      }

      return pui->GetCapture();

   }


   bool interaction_impl_base::ReleaseCapture()
   {

      return get_wnd()->ReleaseCapture();

   }


   bool interaction_impl_base::SetFocus()
   {

      return m_pui->SetFocus();

   }


   bool interaction_impl_base::get_rect_normal(LPRECT lprect)
   {

      GetWindowRect(lprect);

      return true;

   }


   bool interaction_impl_base::SetTimer(uint_ptr nIDEvent,UINT nEllapse, PFN_TIMER pfnTimer)
   {

      if(nEllapse < 500)
      {

//         string str;
//
//         str.Format("creating fast timer: %d\n", nEllapse);
//
//         ::output_debug_string(str);

      }

      if (m_ptimerarray.is_null())
      {

         m_ptimerarray = canew(::aura::timer_array(get_app()));

         m_ptimerarray->m_pcallback = m_pui;

      }

      return m_ptimerarray->create_timer(nIDEvent, nEllapse, pfnTimer, true);

   }


   bool interaction_impl_base::KillTimer(uint_ptr nIDEvent)
   {

      if(m_ptimerarray.is_null())
      {

         return true;

      }

      return m_ptimerarray->delete_timer(nIDEvent);

   }


   void interaction_impl_base::_001OnTimer(::timer * ptimer)
   {

      if(m_pui == NULL)
      {

         return;

      }

      m_pui->_001OnTimer(ptimer);

   }


   void interaction_impl_base::defer_start_prodevian()
   {

      m_pui->post_simple_command(simple_command_defer_start_prodevian);

   }


   void interaction_impl_base::_defer_start_prodevian()
   {

   }


   bool interaction_impl_base::DestroyWindow()
   {

      m_bUserElementalOk = false;

      if (m_pui == NULL)
      {

         return false;

      }

      try
      {

         m_ptimerarray.release();

      }
      catch(...)
      {

      }

      try
      {

         send_message(WM_DESTROY);

      }
      catch(...)
      {

      }

      try
      {

         send_message(WM_NCDESTROY);

      }
      catch(...)
      {

      }

      return true;

   }


   void interaction_impl_base::mouse_hover_add(::user::interaction * pinterface)
   {

      ::user::interaction * pui = get_wnd();

      if(pui != NULL)
      {

         pui->mouse_hover_add(pinterface);

      }

   }

   void interaction_impl_base::mouse_hover_remove(::user::interaction * pinterface)
   {

      ::user::interaction * pui = get_wnd();

      if(pui != NULL)
      {

         pui->mouse_hover_remove(pinterface);

      }

   }


   void interaction_impl_base::register_drop_target()
   {
   }


   ::user::elemental * interaction_impl_base::get_focus_elemental()
   {

      return NULL;

   }


   bool interaction_impl_base::set_focus_elemental(::user::elemental * pelemental)
   {

      UNREFERENCED_PARAMETER(pelemental);

      return false;

   }


   void interaction_impl_base::PostNcDestroy()
   {

      ::user::interaction_base::PostNcDestroy();

   }


   bool interaction_impl_base::ShowWindow(int32_t nCmdShow)
   {

      if (nCmdShow != SW_HIDE)
      {

         m_bShowFlags = true;

         m_iShowFlags &= ~SWP_HIDEWINDOW;
         m_iShowFlags |= SWP_SHOWWINDOW;

         m_bShowWindow = true;
         m_iShowWindow = nCmdShow;

         m_pui->set_need_redraw();

      }
      else if(nCmdShow == SW_HIDE && is_this_visible())
      {

         m_bShowFlags = true;

         m_iShowFlags &= ~SWP_SHOWWINDOW;
         m_iShowFlags |= SWP_HIDEWINDOW;

         m_bShowWindow = true;
         m_iShowWindow = nCmdShow;

         m_pui->set_need_redraw();

      }


      return true;

   }


   void interaction_impl_base::_001OnShowWindow(::message::message * pobj)
   {

      SCAST_PTR(::message::show_window, pshowwindow, pobj);


      set_need_redraw();

   }


   void interaction_impl_base::_001OnDestroyWindow(::message::message * pobj)
   {

      try
      {

         m_ptimerarray.release();

      }
      catch(...)
      {

      }

   }


   void interaction_impl_base::prio_install_message_routing(::message::sender * pinterface)
   {

      IGUI_MSG_LINK(WM_SHOWWINDOW, pinterface, this, &interaction_impl_base::_001OnShowWindow);
      IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &interaction_impl_base::_001OnDestroyWindow);

   }


   void interaction_impl_base::last_install_message_routing(::message::sender * pinterface)
   {


   }


   void interaction_impl_base::show_task(bool bShow)
   {

      UNREFERENCED_PARAMETER(bShow);

   }

   void interaction_impl_base::redraw_add(::object * p)
   {

   }


   void interaction_impl_base::redraw_remove(::object * p)
   {

   }


   bool interaction_impl_base::has_redraw()
   {

      return false;

   }


   void interaction_impl_base::queue_message_handler(::message::base * pbase)
   {

      return m_pui->message_handler(pbase);

   }


   void interaction_impl_base::show_software_keyboard(bool bShow, string str, strsize iBeg, strsize iEnd)
   {


   }


   bool interaction_impl_base::has_focus()
   {

      if(m_pui == NULL)
      {

         return NULL;

      }

      sp(::user::interaction) pui = m_pui->get_wnd();

      if(pui.is_null())
      {


         return false;

      }

      if(!pui->has_focus())
      {

         return false;

      }

      sp(::user::interaction_impl) pimpl = pui->m_pimpl;

      if(pimpl.is_null())
      {

         return false;

      }

      return pimpl->m_pelementalFocus == m_pui;

   }


   bool interaction_impl_base::is_active()
   {

      if(m_pui == NULL)
      {

         return NULL;

      }

      return m_pui == m_pui->GetActiveWindow();

   }


   void interaction_impl_base::deferred_on_change_visibility()
   {

   }


} // namespace user



