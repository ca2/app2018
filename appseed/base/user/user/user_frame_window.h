#pragma once


#ifndef TBSTYLE_FLAT
#define TBSTYLE_BUTTON          0x0000  // obsolete; use BTNS_BUTTON instead
#define TBSTYLE_SEP             0x0001  // obsolete; use BTNS_SEP instead
#define TBSTYLE_CHECK           0x0002  // obsolete; use BTNS_CHECK instead
#define TBSTYLE_GROUP           0x0004  // obsolete; use BTNS_GROUP instead
#define TBSTYLE_CHECKGROUP      (TBSTYLE_GROUP | TBSTYLE_CHECK)     // obsolete; use BTNS_CHECKGROUP instead
#define TBSTYLE_DROPDOWN        0x0008  // obsolete; use BTNS_DROPDOWN instead
#define TBSTYLE_AUTOSIZE        0x0010  // obsolete; use BTNS_AUTOSIZE instead
#define TBSTYLE_NOPREFIX        0x0020  // obsolete; use BTNS_NOPREFIX instead

#define TBSTYLE_TOOLTIPS        0x0100
#define TBSTYLE_WRAPABLE        0x0200
#define TBSTYLE_ALTDRAG         0x0400
#define TBSTYLE_FLAT            0x0800
#define TBSTYLE_LIST            0x1000
#define TBSTYLE_CUSTOMERASE     0x2000
#define TBSTYLE_REGISTERDROP    0x4000
#define TBSTYLE_TRANSPARENT     0x8000
#endif

/////////////////////////////////////////////////////////////////////////////
// General style bits etc

// ControlBar styles
#define CBRS_ALIGN_LEFT     0x1000L
#define CBRS_ALIGN_TOP      0x2000L
#define CBRS_ALIGN_RIGHT    0x4000L
#define CBRS_ALIGN_BOTTOM   0x8000L
#define CBRS_ALIGN_ANY      0xF000L

#define CBRS_BORDER_LEFT    0x0100L
#define CBRS_BORDER_TOP     0x0200L
#define CBRS_BORDER_RIGHT   0x0400L
#define CBRS_BORDER_BOTTOM  0x0800L
#define CBRS_BORDER_ANY     0x0F00L

#define CBRS_TOOLTIPS       0x0010L
#define CBRS_FLYBY          0x0020L
#define CBRS_FLOAT_MULTI    0x0040L
#define CBRS_BORDER_3D      0x0080L
#define CBRS_HIDE_INPLACE   0x0008L
#define CBRS_SIZE_DYNAMIC   0x0004L
#define CBRS_SIZE_FIXED     0x0002L
#define CBRS_FLOATING       0x0001L

#define CBRS_GRIPPER        0x00400000L
#define CBRS_LEAVEONFULLSCREEN     0x10000000L

#define CBRS_ORIENT_HORZ    (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM)
#define CBRS_ORIENT_VERT    (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT)
#define CBRS_ORIENT_ANY     (CBRS_ORIENT_HORZ|CBRS_ORIENT_VERT)

#define CBRS_ALL            0x0040FFFFL
//#define CBRS_ALL            0x1140FFFFL

// the CBRS_ style is made up of an alignment style and a draw border style
//  the alignment styles are mutually exclusive
//  the draw border styles may be combined
#define CBRS_NOALIGN        0x00000000L
#define CBRS_LEFT           (CBRS_ALIGN_LEFT|CBRS_BORDER_RIGHT)
#define CBRS_TOP            (CBRS_ALIGN_TOP|CBRS_BORDER_BOTTOM)
#define CBRS_RIGHT          (CBRS_ALIGN_RIGHT|CBRS_BORDER_LEFT)
#define CBRS_BOTTOM         (CBRS_ALIGN_BOTTOM|CBRS_BORDER_TOP)


// Frame interaction_impl styles
#define FWS_ADDTOTITLE  0x00008000L // modify title based on content
#define FWS_PREFIXTITLE 0x00004000L // show document name before cast name
#define FWS_SNAPTOBARS  0x00002000L // snap size to size of contained bars


namespace user
{


   class interaction_impl;
   class frame_window;
   class mdi_child_window;
   class control_bar;


   struct PrintPreviewState;  // forward reference (see afxext.h)
   class CReBar;               // forward reference (see afxext.h)

   class BaseDockBar;             // forward reference (see afxpriv.h)
   class mini_dock_frame_window;    // forward reference (see afxpriv.h)
   class CDockState;           // forward reference (see afxpriv.h)


   class FrameInitialUpdate
   {
   public:


      bool                          m_bMakeVisible;
      ::user::document *            m_pdoc;


   };

   class CLASS_DECL_BASE frame_window :
      virtual public ::user::box
   {
   public:



      enum EColumn
      {

         ColumnWindowRect

      };

      // idle update of frame ::fontopus::user interface
      enum IdleFlags
      {

         idleMenu = 1,
         idleTitle = 2,
         idleNotify = 4,
         idleLayout = 8

      };

      bool                                m_bblur_Background;

      bool                       m_bAutoWindowFrame;
      bool                       m_bWindowFrame;
      bool                       m_bLayered;
      int32_t                    m_iFrameData;

      ::id                       m_idHelp;         // xxx mrs
      ::user::impact_system *    m_pdocumenttemplate;

      int32_t                    m_nWindow;  // general purpose interaction_impl number - display as ":n"
      // -1 => unknown, 0 => only interaction_impl viewing ::user::document
      // 1 => first of many windows viewing ::user::document, 2=> second

      HMENU                      m_hMenuDefault;       // default menu resource for this frame
      HACCEL                     m_hAccelTable;       // accelerator table
      uint32_t                   m_dwPromptContext;    // current help prompt context for message box
      bool                       m_bHelpMode;           // if TRUE, then Shift+F1 help mode is active
      ::user::frame_window *     m_pNextFrameWnd; // next frame_window in cast global list
      rect                       m_rectBorder;         // for OLE border space negotiation

      spa(control_bar)           m_barptra; // array of all control bars that have this interaction_impl as their dock site
      int32_t                    m_nShowDelay;           // SW_ command for delay show/hide

      bool                       m_bFrameMoveEnable;


      string                     m_strMatterHelp;             // Help ID (0 for none, see HID_BASE_RESOURCE)
      UINT                       m_nIDTracking;         // tracking command ID or string IDS
      UINT                       m_nIDLastMessage;      // last displayed message string IDS
      ::user::impact *           m_pviewActive;       // current active ::user::impact
      UINT                       m_cModalStack;         // BeginModalState depth
      ::user::interaction_ptra   m_uiptraDisable;       // windows disabled because of BeginModalState
      HMENU                      m_hMenuAlt;           // menu to update to (NULL means default)
      string                     m_strTitle;         // default title (original)
      bool                       m_bInRecalcLayout;     // avoid recursion in on_layout
      sp(type)                   m_pFloatingFrameClass;

      UINT                       m_nIdleFlags;          // set of bit flags for idle processing

      ::user::impact *           m_pviewMain;

      


      frame_window();
      virtual ~frame_window();

      void CommonConstruct();


      virtual void OnInitialFrameUpdate(bool bMakeVisible);


      virtual void SetBorderRect(const RECT & rect);
      virtual void GetBorderRect(LPRECT lprect);
      virtual bool _001IsFrameWnd();


      //virtual ::user::OleFrameHook * GetNotifyHook();
      virtual void NotifyFloatingWindows(uint32_t dwFlags);

      virtual string get_window_default_matter();
      virtual ::user::interaction::e_type get_window_type();


      virtual bool on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult);
      //virtual void _000OnDraw(::draw2d::graphics * pgraphics);
      //virtual void _001OnDraw(::draw2d::graphics * pgraphics);
      virtual void install_message_handling(::message::dispatch * pinterface);

      DECL_GEN_SIGNAL(_guserbaseOnInitialUpdate);
      DECL_GEN_SIGNAL(guserbaseOnInitialUpdate);

      virtual void on_set_parent(::user::interaction * puiParent) override;

      virtual void defer_synch_layered();
      virtual bool calc_layered();

      virtual void assert_valid() const;
      virtual void dump(dump_context & dc) const;




      bool LoadAccelTable(const char * lpszResourceName);
      virtual bool create_window(const char * lpszClassName, const char * lpszWindowName, uint32_t dwStyle, const RECT & rect = ::null_rect(), ::user::interaction * puiParent = NULL, const char * lpszMenuName = NULL, uint32_t dwExStyle = 0, ::create * pcreate = NULL);

      // dynamic creation - load frame and associated resources
      virtual bool LoadFrame(const char * pszMatter,
         uint32_t dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
         ::user::interaction * pParentWnd = NULL,
         ::create * pcreate = NULL);

      virtual bool ShowWindow(int32_t nCmdShow);


      // Attributes
      virtual ::user::document * GetActiveDocument();

      // Active child ::user::impact maintenance
      sp(::user::impact) GetActiveView() const;           // active ::user::impact or NULL
      void SetActiveView(sp(::user::impact) pViewNew, bool bNotify = TRUE);
      // active ::user::impact or NULL, bNotify == FALSE if focus should not be set

      // Active frame (for frames within frames -- MDI)
      virtual sp(::user::frame_window) GetActiveFrame();

      // For customizing the default messages on the status bar
      virtual void GetMessageString(UINT nID, string & rMessage) const;

      bool m_bAutoMenuEnable;
      // TRUE => menu items without handlers will be disabled

      bool IsTracking() const;

      // Operations
      virtual void on_layout();
      virtual void ActivateFrame(int32_t nCmdShow = -1);
      virtual void InitialUpdateFrame(::user::document * pDoc, bool bMakeVisible);
      virtual void InitialFramePosition(bool bForceRestore = false);
      void set_title(const char * lpszTitle);
      string get_title() const;

      // to set text of standard status bar
      void SetMessageText(const char * lpszText);
      void SetMessageText(UINT nID);

      ::user::control_bar* GetControlBar(UINT nID);

      // frame interaction_impl based modality
      virtual void BeginModalState();
      virtual void EndModalState();
      bool InModalState() const;
      void ShowOwnedWindows(bool bShow);


      virtual sp(::user::interaction) GetMessageBar();

      // border space negotiation
      enum BorderCmd { borderGet = 1, borderRequest = 2, borderSet = 3 };
      virtual bool NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder);
      virtual bool on_create_client(::user::create_struct * lpcs, ::create * pcreate);
      void OnContextHelp();   // for Shift+F1 help
      void OnUpdateControlBarMenu(cmd_ui* pCmdUI);
      bool OnBarCheck(UINT nID);

      virtual bool LoadToolBar(id idToolBar, const char * pszToolBar, uint32_t dwCtrlStyle = TBSTYLE_FLAT, uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP);

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);
      virtual void on_update_frame_title(bool bAddToTitle);
      virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
      virtual HACCEL GetDefaultAccelerator();
      virtual void pre_translate_message(signal_details * pobj);

      virtual void DelayUpdateFrameMenu(HMENU hMenuAlt);
      void DelayUpdateFrameTitle();
      void DelayRecalcLayout(bool bNotify = TRUE);

      // for Shift+F1 help support
      bool CanEnterHelpMode();
      virtual void ExitHelpMode();


      void UpdateFrameTitleForDocument(const char * lpszDocName);
      virtual bool pre_create_window(::user::create_struct& cs);
      //virtual bool OnCommand(WPARAM wParam, LPARAM lParam);
      virtual void PostNcDestroy();   // default to delete this.
      int32_t OnCreateHelper(::user::create_struct * lpcs, ::create * pcreate);
      void BringToTop(int32_t nCmdShow);
      // bring interaction_impl to top for SW_ commands which affect z-order

      // implementation helpers for Shift+F1 help mode
      bool ProcessHelpMsg(MESSAGE & msg, uint32_t * pContext);
      oswindow SetHelpCapture(POINT point, bool * pbDescendant);

      // frame_window list management
      void AddFrameWnd();
      void RemoveFrameWnd();

      friend class interaction_impl;  // for access to m_bModalDisable
      friend class CReBar; // for access to m_bInRecalcLayout

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnDestroy);
      DECL_GEN_SIGNAL(_001OnSysCommand);

      void OnClose();
      LRESULT OnPopMessageString(WPARAM wParam, LPARAM lParam);
      LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
      LRESULT OnHelpPromptAddr(WPARAM wParam, LPARAM lParam);
      //void OnIdleUpdateCmdUI(::signal_details * pobj);
      void OnEnterIdle(UINT nWhy, sp(::user::interaction) pWho);
      void OnSetFocus(sp(::user::interaction) pOldWnd);
      void OnSize(UINT nType, int32_t cx, int32_t cy);
      bool OnEraseBkgnd(::draw2d::dib * pdib);
      //void OnActivate(UINT nState, sp(::user::interaction) pWndOther, bool bMinimized);
      DECL_GEN_SIGNAL(_001OnActivate);
         DECL_GEN_SIGNAL(_001OnNcActivate);
         //bool OnNcActivate(bool bActive);
         void OnSysCommand(UINT nID, LPARAM lParam);
      bool OnQueryEndSession();
      void OnEndSession(bool bEnding);
#ifdef WINDOWSEX
      virtual void OnDropFiles(HDROP hDropInfo);
#endif
      bool OnSetCursor(sp(::user::interaction) pwindow, UINT nHitTest, UINT message);
      //LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
      //LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
      LRESULT OnActivateTopLevel(WPARAM wParam, LPARAM lParam);
      void OnEnable(bool bEnable);
      void OnPaletteChanged(sp(::user::interaction) pFocusWnd);
      bool OnQueryNewPalette();
      virtual LRESULT OnDDEInitiate(WPARAM wParam, LPARAM lParam);
      virtual LRESULT OnDDEExecute(WPARAM wParam, LPARAM lParam);
      virtual LRESULT OnDDETerminate(WPARAM wParam, LPARAM lParam);
      LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);







      virtual void _000OnDraw(::draw2d::graphics * pgraphics);


      virtual bool BaseOnControlEvent(::user::control_event * pevent);


      sp(::user::interaction) WindowDataGetWnd();


      DECL_GEN_SIGNAL(_001OnIdleUpdateCmdUI);
      DECL_GEN_SIGNAL(_001OnSetFocus);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnQueryEndSession);

      friend class user;
      virtual void AddControlBar(::user::control_bar *pBar);
      virtual void RemoveControlBar(::user::control_bar *pBar);


      void data_on_after_change(signal_details * pobj);


      virtual bool _001HasCommandHandler(id id);


      virtual bool get_window_minimum_size(::size & sizeMin);


      virtual ::user::front_end_schema * get_user_front_end_schema();


   };


} // namespace user



#pragma once

