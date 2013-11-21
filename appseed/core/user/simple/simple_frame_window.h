#pragma once


namespace frame
{


   class FrameSchema;


}


namespace user
{


   class control_bar;


} // namespace user


class BaseDockBar;
class BaseReBar;
class BaseMiniDockFrameWnd;

class CLASS_DECL_CORE simple_frame_window :
   virtual public ::user::frame_window,
   virtual public database::user::interaction,
   virtual public ::uinteraction::frame::WorkSetClientInterface,
   virtual public ::uinteraction::frame::WorkSetListener,
   virtual public ::uinteraction::frame::CWorkSetDownUpInterface
{
public:



   ::database::id          m_datakeyFrame;

   sp(::uinteraction::frame::frame)       m_pframeschema;


//   HDC                           m_hdcOpenGL;
//#ifdef WINDOWS
//   HGLRC                         m_hglrc;
//#endif
   bool                          m_bFullScreenAlt;
   bool                          m_bblur_Background;
   visual::dib_sp                m_dibBk;
   bool                          m_bCustomFrameBefore;
   rect                          m_FullScreenWindowRect;
   visual::fastblur              m_fastblur;

   mapsp(id, id, ::user::interaction)    m_toolbarmap;


   simple_frame_window(sp(base_application) papp);
   virtual ~simple_frame_window();


   using ::user::frame_window::create;
   virtual bool create(const char * lpszClassName,
            const char * lpszWindowName,
            uint32_t dwStyle = WS_OVERLAPPEDWINDOW,
            const RECT & rectParam = rect(0, 0, 0, 0),
            sp(::user::interaction) pParentWnd = NULL,        // != NULL for popups
            const char * lpszMenuName = NULL,
            uint32_t dwExStyle = 0,
            sp(::create_context) pContext = NULL);



   virtual void install_message_handling(::message::dispatch * pinterface);

   virtual void on_set_parent(sp(::user::interaction) pguieParent);

   virtual void GetClientRect(LPRECT lprect);

   virtual bool is_application_main_window();

   bool GetCustomFrame();
   void SetCustomFrame(bool bCustom);
   void SetBorderRect(LPCRECT lpcrect);
   virtual void GetBorderRect(LPRECT lprect);
   void ViewOnActivateFrame(sp(::user::impact) pview, UINT user, sp(::user::interaction) pframe);

   virtual void ToggleFullScreen();
   virtual bool IsFullScreen();
   virtual void WfiOnFullScreen(bool bFullScreen);
   virtual void ShowControlBars(bool bShow = true);


   void OnUpdateControlBarMenu(cmd_ui * pcmdui);

   virtual sp(::uinteraction::frame::frame) create_frame_schema();

   virtual bool LoadFrame(const char * pszMatter, uint32_t dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, sp(::user::interaction) pParentWnd = NULL, sp(::create_context) pContext = NULL);

   DECL_GEN_SIGNAL(_001OnNcActivate);
   DECL_GEN_SIGNAL(_001OnDisplayChange);

   void _001OnDeferPaintLayeredWindowBackground(::draw2d::graphics * pdc);




#ifdef WINDOWSEX
   void LoadToolBar(id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle = TBSTYLE_FLAT, uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP);
#else
   void LoadToolBar(id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle = 0, uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP);
#endif

   virtual void InitialFramePosition(bool bForceRestore = false);

   sp(::user::interaction) WindowDataGetWnd();
   virtual void layout();
   virtual void ActivateFrame(int32_t nCmdShow = -1);
   virtual bool OnCreateClient(LPCREATESTRUCT lpcs, sp(::create_context) pContext);
   virtual bool pre_create_window(CREATESTRUCT& cs);
   virtual void pre_translate_message(signal_details * pobj);

   virtual void _000OnDraw(::draw2d::graphics * pdc);
   virtual void _010OnDraw(::draw2d::graphics * pdc);

   virtual void _001OnDraw(::draw2d::graphics * pdc);
   virtual void _011OnDraw(::draw2d::graphics * pdc);






   virtual bool _001CanEnterScreenSaver();

   DECL_GEN_SIGNAL(_001OnCreate);
   DECL_GEN_SIGNAL(_001OnDdeInitiate);
   DECL_GEN_SIGNAL(_001OnDestroy)
   DECL_GEN_SIGNAL(_001OnSize)
   DECL_GEN_SIGNAL(_001OnMove)
   DECL_GEN_SIGNAL(_001OnClose)
   DECL_GEN_SIGNAL(_001OnSysCommand);
   DECL_GEN_SIGNAL(_001OnViewFullScreen)
   DECL_GEN_SIGNAL(_001OnMouseMove)
   DECL_GEN_SIGNAL(_001OnUpdateViewFullScreen)
   DECL_GEN_SIGNAL(_001OnToggleCustomFrame)
   DECL_GEN_SIGNAL(_001OnUpdateToggleCustomFrame)
   DECL_GEN_SIGNAL(_001OnGetMinMaxInfo)
   DECL_GEN_SIGNAL(_001OnUser184)

#ifdef WINDOWSEX
   void OnNcCalcSize(bool bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
#endif

   // persistent frame implemenation using updowntarget

   virtual bool WndFrameworkDownUpGetUpEnable();
   virtual bool WndFrameworkDownUpGetDownEnable();

   using ::uinteraction::frame::WorkSetListener::attach;
   DECL_GEN_VSIGNAL(guserbaseOnInitialUpdate);

   virtual class mini_dock_frame_window* CreateFloatingFrame(uint32_t dwStyle);
   virtual void NotifyFloatingWindows(uint32_t dwFlags);


   virtual void WfiOnDown();
   virtual void WfiOnUp();

   virtual bool calc_layered();


   virtual string get_window_default_matter();

   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;
   void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
   virtual bool on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult);

   virtual void OnDropFiles(HDROP hDropInfo);
   virtual bool OnQueryEndSession();
   virtual void OnEndSession(bool bEnding);

   LRESULT OnDDEInitiate(WPARAM wParam, LPARAM lParam);
   LRESULT OnDDEExecute(WPARAM wParam, LPARAM lParam);
   LRESULT OnDDETerminate(WPARAM wParam, LPARAM lParam);

   void _001OnQueryEndSession(signal_details * pobj);

   virtual bool BaseOnControlEvent(::user::control_event * pevent);

   virtual bool WfiOnMove(bool bTracking);
   virtual bool WfiOnSize(bool bTracking);

   virtual void WfiOnClose();
   virtual void WfiOnMaximize();
   virtual void WfiOnMinimize();
   virtual void WfiOnRestore();

   virtual bool DeferFullScreen(bool bFullScreen, bool bRestore);
   virtual bool ShowWindowFullScreen(bool bFullScreen = true, bool bRestore = true);

   virtual bool _001OnCmdMsg(base_cmd_msg * pcmdmsg);

   virtual void data_on_after_change(signal_details * pobj);




};



