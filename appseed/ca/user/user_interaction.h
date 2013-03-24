#pragma once


#include "ca/ca/ca_holder.h"
#include "ca/user/user_window_interface.h"


namespace ca
{


   class window_callback;
   class live_object;

#ifdef METROWIN

   interface class system_window
   {
      virtual Windows::Foundation::Rect get_window_rect() = 0;
      virtual Windows::Foundation::Point get_cursor_pos() = 0;
   };

#endif

} // namespace ca


namespace user
{


   class control_event;


   class CLASS_DECL_ca interaction :
      virtual public window_interface,
      virtual public ::user::interaction_base
   {
   public:


      enum e_message
      {

         message_simple_command = WM_APP + 1985,

      };


      enum e_simple_command
      {

         simple_command_load_window_rect,
         simple_command_update_frame_title,
         simple_command_set_edit_file,
         simple_command_layout,

      };


      enum e_type
      {
         type_window,
         type_frame,
         type_view,

      };


      class CLASS_DECL_ca timer_item
      {
      public:


         interaction *        m_pguie;
         uint_ptr             m_uiId;
         UINT                 m_uiElapse;
         UINT                 m_uiLastSent;

         bool check(single_lock & sl);


      };

      class CLASS_DECL_ca timer_array :
         virtual public pha(interaction)
      {
      public:


         mutex                               m_mutex;
         array_ptr_alloc < timer_item >      m_timera;
         index                               m_iItem;


         timer_array(::ca::application * papp);


         virtual void on_delete(interaction * poc);
         uint_ptr set(interaction * pguie, uint_ptr uiId, UINT uiElapse);
         void check();
         bool unset(interaction * pguie, uint_ptr uiId);
         void unset(interaction * pguie);
         void detach(array_ptr_alloc < timer_item > & timera, interaction * pguie);
         void transfer(::ca::window * pwindow, interaction * pguie);
         interaction * find(::ca::ca * pca);
         index find(interaction * pguie, uint_ptr uiId);
         index find_from(interaction * pguie, index iStart);

         virtual void assert_valid() const;
         virtual void dump(dump_context & dc) const;

      };



      virtual bool create_message_window(const char * pszName, ::ca::window_callback * pcallback = NULL);
#ifdef METROWIN
      virtual bool initialize(Windows::UI::Core::CoreWindow ^ window, ::ca::system_window ^ pwindow);
#endif


      enum e_appearance
      {
         appearance_normal,
         appearance_zoomed,
         appearance_iconic,
         appearance_notify,
      };

      e_appearance                        m_eappearance;
      interaction *                       m_pimpl;
      static interaction *                g_pguieMouseMoveCapture;
      pha(interaction)                    m_uiptraChild;
      string                              m_strName;
      id                                  m_id;
      interaction *                       m_pguieOwner;
      UINT                                m_nFlags;      // see WF_ flags above
      bool                                m_bCursorInside;
      ::visual::e_cursor                  m_ecursor;
      int32_t                                 m_iModal;
      int32_t                                 m_iModalCount;
      bool                                m_bRectOk;
      ::plane::session *                  m_psession;

#ifdef WINDOWS

      comparable_array < int_ptr >        m_iaModalThread;

#else

      comparable_array < pthread_t >      m_iaModalThread;

#endif

      id                                  m_idModalResult; // for return values from ::ca::window::RunModalLoop
      COLORREF                            m_crDefaultBackgroundColor;



      interaction();
      interaction(::ca::application * papp);
      virtual ~interaction();


      virtual void on_select();


      ::visual::e_cursor get_cursor();
      void set_cursor(::visual::e_cursor ecursor);


      void set_timer(array_ptr_alloc < timer_item > timera);


      virtual bool IsWindow();

      virtual LONG GetWindowLong(int32_t nIndex);
      virtual LONG SetWindowLong(int32_t nIndex, LONG lValue);


      virtual void on_set_parent(interaction * pinterface);


      virtual interaction * get_bottom_child();
      virtual interaction * get_top_child();
      virtual interaction * under_sibling();
      virtual interaction * above_sibling();

      virtual interaction * above_sibling(interaction * pui);
      virtual interaction * under_sibling(interaction * pui);


      virtual void mouse_hover_add(::user::interaction* pinterface);
      virtual void mouse_hover_remove(::user::interaction* pinterface);


      virtual bool CheckAutoCenter();

      virtual void track_mouse_hover();
      virtual void track_mouse_leave();

      // dialog support
      void UpdateDialogControls(command_target* pTarget, bool bDisableIfNoHndler);
      virtual void CenterWindow(interaction * pAlternateOwner = NULL);
      virtual id   RunModalLoop(uint32_t dwFlags = 0, ::ca::live_object * pliveobject = NULL);
      virtual bool ContinueModal(int32_t iLevel);
      virtual void EndModalLoop(id nResult);
      virtual void EndAllModalLoops(id nResult);

      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      // Dialog data support
      virtual bool update_data(bool bSaveAndValidate = true);


      virtual ::user::interaction * get_os_focus_uie();


      virtual void _001WindowMaximize();
      virtual void _001WindowRestore();
      virtual bool IsZoomed();
      virtual bool IsFullScreen();
      virtual bool ShowWindowFullScreen(bool bFullScreen = true, bool bRestore = true);
      virtual bool IsIconic();
      virtual void MoveWindow(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight,
               bool bRepaint = TRUE);
      virtual void MoveWindow(LPCRECT lpRect, bool bRepaint = TRUE);
      virtual void GetClientRect(LPRECT lprect);
      virtual void GetClientRect(__rect64 * lprect);
      virtual void GetWindowRect(LPRECT lprect);
      virtual void GetWindowRect(__rect64 * lprect);
      virtual rect GetWindowRect();
      virtual rect64 GetWindowRect64();
      virtual void ClientToScreen(LPRECT lprect);
      virtual void ClientToScreen(__rect64 * lprect);
      virtual void ClientToScreen(LPPOINT lppoint);
      virtual void ClientToScreen(__point64 * lppoint);
      virtual void ScreenToClient(LPRECT lprect);
      virtual void ScreenToClient(__rect64 * lprect);
      virtual void ScreenToClient(LPPOINT lppoint);
      virtual void ScreenToClient(__point64 * lprect);
      virtual bool SetWindowPos(int32_t z, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags);
      virtual bool set_placement(LPRECT lprect);
      virtual int32_t SetWindowRgn(HRGN hRgn, bool bRedraw);
      virtual int32_t GetWindowRgn(HRGN hRgn);

      virtual void layout();

      virtual bool BringWindowToTop();

#ifdef WINDOWSEX
      virtual bool GetWindowPlacement(WINDOWPLACEMENT* lpwndpl);
      virtual bool SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);
#endif

      virtual bool SendChildNotifyLastMsg(LRESULT* pResult = NULL);


      virtual bool pre_create_window(CREATESTRUCT& cs);


      virtual bool subclass_window(oswindow posdata);
      virtual oswindow unsubclass_window();


      virtual bool create(interaction *pparent, id id);
      virtual bool create_window(const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         const RECT& rect,
         interaction* pParentWnd, id id,
         ::ca::create_context* pContext = NULL);
      using ::ca::request_interface::create;
      virtual bool create(const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         const RECT& rect,
         interaction* pParentWnd, id id,
         ::ca::create_context* pContext = NULL);
      virtual bool CreateEx(uint32_t dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         const RECT& rect,
         interaction* pParentWnd, id id,
         LPVOID lpParam = NULL);
      virtual bool create_window_ex(uint32_t dwExStyle, const char * lpszClassName,
         const char * lpszWindowName, uint32_t dwStyle,
         const RECT& rect,
         interaction* pParentWnd, id id,
         LPVOID lpParam = NULL);
      enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
      virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
      virtual frame_window * GetParentFrame();

      virtual bool IsTopParentActive();
      virtual void ActivateTopParent();

      virtual bool DestroyWindow();


#ifdef WINDOWS
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::ca::region* prgnUpdate = NULL,
         UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
#else
      virtual bool RedrawWindow(LPCRECT lpRectUpdate = NULL,
         ::ca::region* prgnUpdate = NULL,
         UINT flags = 0);
#endif


   virtual void set_view_port_org(::ca::graphics * pgraphics);



      virtual void UpdateWindow();
      virtual void SetRedraw(bool bRedraw = TRUE);
      virtual bool GetUpdateRect(LPRECT lpRect, bool bErase = FALSE);
      virtual int32_t GetUpdateRgn(::ca::region* pRgn, bool bErase = FALSE);
      virtual void Invalidate(bool bErase = TRUE);
      virtual void InvalidateRect(LPCRECT lpRect, bool bErase = TRUE);
      virtual void InvalidateRgn(::ca::region* pRgn, bool bErase = TRUE);
      virtual void ValidateRect(LPCRECT lpRect);
      virtual void ValidateRgn(::ca::region* pRgn);
      virtual void ShowOwnedPopups(bool bShow = TRUE);


      virtual uint32_t GetStyle();
      virtual uint32_t GetExStyle();
      virtual LRESULT Default();

      virtual LRESULT send_message(::ca::message::base * pbase);
      virtual bool PostMessage(::ca::message::base * pbase);
      virtual LRESULT send_message(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0);

#ifdef LINUX

      virtual LRESULT send_message(XEvent * pevent);

#endif

      virtual bool PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
      virtual bool post_simple_command(e_simple_command ecommand, LPARAM lParam = 0);

      virtual bool ModifyStyle(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags = 0);
      virtual bool ModifyStyleEx(uint32_t dwRemove, uint32_t dwAdd, UINT nFlags = 0);
      virtual bool ShowWindow(int32_t nCmdShow);
      virtual bool IsFrameWnd();

   // Timer Functions
      virtual uint_ptr SetTimer(uint_ptr nIDEvent, UINT nElapse,
         void (CALLBACK* lpfnTimer)(oswindow, UINT, uint_ptr, uint32_t));
      virtual bool KillTimer(uint_ptr nIDEvent);

      virtual bool IsWindowEnabled();
      virtual bool EnableWindow(bool bEnable = TRUE);

      virtual void _001Print(::ca::graphics * pdc);
      virtual void _000OnDraw(::ca::graphics *pdc);
      virtual void _001DrawThis(::ca::graphics *pdc);
      virtual void _001DrawChildren(::ca::graphics *pdc);
      virtual void _001OnDraw(::ca::graphics *pdc);
      virtual void draw_control_background(::ca::graphics *pdc);

      virtual ::ca::graphics * GetDC();
      virtual bool ReleaseDC(::ca::graphics *);

      virtual bool IsChild(interaction *  pWnd);
      virtual window_interface * window_interface_get_parent() const;
      virtual interaction * get_parent() const;
      virtual interaction * set_parent(interaction * pguieParent);

      virtual id GetDlgCtrlId();
      virtual id SetDlgCtrlId(class id id);

      virtual interaction * set_capture(interaction * pinterface = NULL);
      virtual interaction * release_capture();

      virtual bool has_focus();
      virtual interaction * SetFocus();
      virtual bool SetForegroundWindow();
      virtual interaction * GetActiveWindow();
      virtual interaction * SetActiveWindow();

      virtual void WalkPreTranslateTree(::ca::signal_object * pobj);
      virtual void WalkPreTranslateTree(::user::interaction * puiStop, ::ca::signal_object * pobj);

      virtual interaction * GetDescendantWindow(id id);

      virtual void SetWindowText(const char * lpszString);
      virtual strsize GetWindowText(LPTSTR lpszStringBuf, int32_t nMaxCount);
      virtual void GetWindowText(string & rString);
      virtual strsize GetWindowTextLength();
      virtual void SetFont(::ca::font* pFont, bool bRedraw = TRUE);
      virtual ::ca::font* GetFont();

      virtual void install_message_handling(::ca::message::dispatch * pinterface);
      virtual bool IsWindowVisible();

      virtual void _000OnMouse(::ca::message::mouse * pmouse);
      virtual void _000OnKey(::ca::message::key * pkey);
      DECL_GEN_SIGNAL(_001OnMouseMove)
      DECL_GEN_SIGNAL(_001OnMouseEnter)
      DECL_GEN_SIGNAL(_001OnMouseLeave)
      DECL_GEN_SIGNAL(_001OnKeyDown)
      DECL_GEN_SIGNAL(_001OnKeyUp)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnChar)
      DECL_GEN_SIGNAL(_001OnDestroy)
      DECL_GEN_SIGNAL(_001OnSize)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnMove)
      DECL_GEN_SIGNAL(_001OnUser184)
      DECL_GEN_SIGNAL(_001OnNcCalcSize)
      DECL_GEN_SIGNAL(_001OnClose)
      DECL_GEN_SIGNAL(_001OnSimpleCommand);



      virtual DECL_GEN_SIGNAL(_002OnLButtonDown)
      virtual DECL_GEN_SIGNAL(_002OnLButtonUp)
      virtual DECL_GEN_SIGNAL(_002OnMouseMove)
      virtual DECL_GEN_SIGNAL(_002OnMouseEnter)
      virtual DECL_GEN_SIGNAL(_002OnMouseLeave)
      virtual DECL_GEN_SIGNAL(_002OnKeyDown)
      virtual DECL_GEN_SIGNAL(_002OnKeyUp)
      virtual DECL_GEN_SIGNAL(_002OnTimer)


      virtual void on_delete(::ca::ca * poc);

      //virtual void _001SetWindowPos(const ::ca::window* pWndInsertAfter, int32_t x, int32_t y, int32_t cx, int32_t cy, UINT nFlags);
      //virtual void _001SetFocus();
      //virtual void _001ShowWindow(int32_t iShow);
      //virtual void _001ScreenToClient(LPPOINT lppoint);


   // trans   virtual bool _001IsVisible();
   // trans   virtual void _001SetVisible(bool bVisible);

      virtual bool _001IsPointInside(point64 pt);
      virtual interaction * _001FromPoint(point64 pt, bool bTestedIfParentVisible = false);

      virtual void OnLinkClick(const char * psz, const char * pszTarget = NULL);

      interaction * get_child_by_name(const char * pszName, int32_t iLevel = -1);
      interaction * get_child_by_id(id id, int32_t iLevel = -1);

      virtual ::frame_window * EnsureParentFrame();
      virtual interaction* GetTopLevelParent();
      virtual interaction* EnsureTopLevelParent();
      virtual ::frame_window * GetTopLevelFrame();
      virtual void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, bool bDeep = TRUE, bool bOnlyPerm = FALSE);
      virtual void pre_translate_message(::ca::signal_object * pobj);


      virtual int32_t get_descendant_level(::user::interaction * pui);
      virtual bool is_descendant(::user::interaction * pui, bool bIncludeSelf = false);
      virtual ::user::interaction * get_focusable_descendant(::user::interaction * pui = NULL);

#ifdef METROWIN
      virtual ::user::interaction * get_wnd() const;
#else
      virtual ::ca::window * get_wnd() const;
#endif

      enum RepositionFlags
      {
         reposDefault = 0,
         reposQuery = 1,
         reposExtra = 2,
         reposNoPosLeftOver=0x8000
      };

      virtual void RepositionBars(UINT nIDFirst, UINT nIDLast, id nIDLeftOver, UINT nFlag = reposDefault, LPRECT lpRectParam = NULL, LPCRECT lpRectClient = NULL, bool bStretch = TRUE);

      virtual interaction * get_owner() const;
      virtual void set_owner(interaction * pguie);

      virtual interaction * ChildWindowFromPoint(POINT point);
      virtual interaction * ChildWindowFromPoint(POINT point, UINT nFlags);


#ifdef WINDOWSEX
      virtual interaction * GetNextWindow(UINT nFlag = GW_HWNDNEXT);
#else
      virtual interaction * GetNextWindow(UINT nFlag = 0);
#endif

      virtual interaction * GetTopWindow();

      virtual interaction * get_next(bool bIgnoreChildren = false, int32_t * piLevel = NULL);

      virtual interaction * GetWindow(UINT nCmd);
      virtual interaction * GetLastActivePopup();

      virtual void pre_subclass_window();

      // for custom cleanup after WM_NCDESTROY
      virtual void PostNcDestroy();

      virtual LRESULT DefWindowProc(UINT uiMessage, WPARAM wparam, LPARAM lparam);
      virtual void message_handler(::ca::signal_object * pobj);
      virtual LRESULT message_handler(LPMESSAGE lpmessage);
      virtual void GuieProc(::ca::signal_object * pobj);

      virtual void _001DeferPaintLayeredWindowBackground(::ca::graphics * pdc);

      virtual void _001OnDeferPaintLayeredWindowBackground(::ca::graphics * pdc);


      oswindow get_safe_handle() const;
      virtual oswindow get_handle() const;
      virtual bool attach(oswindow oswindow_New);
      virtual oswindow detach();


      template < class T >
      T * GetTypedParent()
      {
         ASSERT_VALID(this);

         interaction * pParentWnd = get_parent();  // start with one parent up
         while (pParentWnd != NULL)
         {
            if(dynamic_cast < T * > (pParentWnd) != NULL)
            {
               return dynamic_cast < T * > (pParentWnd);
            }
            pParentWnd = pParentWnd->get_parent();
         }
         return NULL;
      }


      virtual bool can_merge(::user::interaction * pui);
      virtual bool merge(::user::interaction * pui);


      virtual COLORREF get_background_color();
      virtual void set_default_background_color(COLORREF crDefaultBackgroundColor);

      virtual void _001OnTriggerMouseInside();

#ifdef METROWIN
      Platform::Agile<Windows::UI::Core::CoreWindow> get_os_window();
#endif


      void offset_view_port_org(LPRECT lprect);

      virtual string get_window_default_matter();
      virtual string get_window_icon_matter();
      virtual uint32_t get_window_default_style();
      virtual e_type get_window_type();


      virtual bool on_simple_command(e_simple_command ecommand, LPARAM lparam, LRESULT & lresult);

   };

   inline oswindow interaction::get_safe_handle() const
   {
      if(((byte *)this) < (byte *) (((byte *) NULL) + (16 * 1024))) // consider invalid
      {
         return ::ca::null();
      }
      return get_handle();
   }



} // namespace user

CLASS_DECL_ca ::user::interaction * WINAPI CreateGuieEx(
   ::ca::application * papp,
    uint32_t dwExStyle,
    const char * lpClassName,
    const char * lpWindowName,
    uint32_t dwStyle,
    int32_t X,
    int32_t Y,
    int32_t nWidth,
    int32_t nHeight,
    ::user::interaction * pguieParent,
    id id,
    HINSTANCE hInstance,
    LPVOID lpParam);
