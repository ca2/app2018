#pragma once

namespace userbase
{
   class frame_window;
   class ::userbase::control_bar;

} // namespace userbase
class BaseDockBar;
class BaseDockContext;
class BaseDockState;

#define CBRS_DRAGMOVE            0x01000000L
/*#undef CBRS_ALL
#define CBRS_ALL            0x0140ffffL*/


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

#define CBRS_ORIENT_HORZ    (CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM)
#define CBRS_ORIENT_VERT    (CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT)
#define CBRS_ORIENT_ANY     (CBRS_ORIENT_HORZ|CBRS_ORIENT_VERT)

//#define CBRS_ALL            0x0040FFFFL
#define CBRS_ALL            0x0140FFFFL

// the CBRS_ style is made up of an alignment style and a draw border style
//  the alignment styles are mutually exclusive
//  the draw border styles may be combined
#define CBRS_NOALIGN        0x00000000L
#define CBRS_LEFT           (CBRS_ALIGN_LEFT|CBRS_BORDER_RIGHT)
#define CBRS_TOP            (CBRS_ALIGN_TOP|CBRS_BORDER_BOTTOM)
#define CBRS_RIGHT          (CBRS_ALIGN_RIGHT|CBRS_BORDER_LEFT)
#define CBRS_BOTTOM         (CBRS_ALIGN_BOTTOM|CBRS_BORDER_TOP)



// Standard control bars (IDW = ::ca::window ID)
#define AFX_IDW_CONTROLBAR_FIRST        0xE800
#define AFX_IDW_CONTROLBAR_LAST         0xE8FF

#define AFX_IDW_TOOLBAR                 0xE800  // main Toolbar for ::ca::window
//#define "status_bar"              0xE801  // Status bar ::ca::window
#define AFX_IDW_PREVIEW_BAR             0xE802  // PrintPreview Dialog Bar
#define AFX_IDW_RESIZE_BAR              0xE803  // OLE in-place resize bar
#define AFX_IDW_REBAR                   0xE804  // COMCTL32 "rebar" Bar
#define AFX_IDW_DIALOGBAR               0xE805  // CDialogBar

// Note: If your application supports docking toolbars, you should
//  not use the following IDs for your own toolbars.  The IDs chosen
//  are at the top of the first 32 such that the bars will be hidden
//  while in print preview mode, and are not likely to conflict with
//  IDs your application may have used succesfully in the past.

#define AFX_IDW_DOCKBAR_TOP             0xE81B
#define AFX_IDW_DOCKBAR_LEFT            0xE81C
#define AFX_IDW_DOCKBAR_RIGHT           0xE81D
#define AFX_IDW_DOCKBAR_BOTTOM          0xE81E
#define AFX_IDW_DOCKBAR_FLOAT           0xE81F

// Macro for mapping standard control bars to bitmask (limit of 32)
#define AFX_CONTROLBAR_MASK(nIDC)   (1L << (nIDC - AFX_IDW_CONTROLBAR_FIRST))



// layout Modes for CalcDynamicLayout
#define LM_STRETCH  0x01    // same meaning as bStretch in CalcFixedLayout.  If set, ignores nLength
                     // and returns dimensions based on LM_HORZ state, otherwise LM_HORZ is used
                     // to determine if nLength is the desired horizontal or vertical length
                     // and dimensions are returned based on nLength
#define LM_HORZ     0x02    // same as bHorz in CalcFixedLayout
#define LM_MRUWIDTH 0x04    // Most Recently Used Dynamic width
#define LM_HORZDOCK 0x08    // Horizontal Docked Dimensions
#define LM_VERTDOCK 0x10    // Vertical Docked Dimensions
#define LM_LENGTHY  0x20    // Set if nLength is a height instead of a width
#define LM_COMMIT   0x40    // Remember MRUWidth


/////////////////////////////////////////////////////////////////////////////
// BaseControlBarInfo - used for docking serialization

class BaseControlBarInfo
{
public:
// Implementation
   BaseControlBarInfo();

// Attributes
   UINT m_nBarID;      // ID of this bar
   BOOL m_bVisible;    // visibility of this bar
   BOOL m_bFloating;   // whether floating or not
   BOOL m_bHorz;       // orientation of floating dockbar
   BOOL m_bDockBar;    // TRUE if a dockbar
   point m_pointPos;  // topleft point of ::ca::window

   UINT m_nMRUWidth;   // MRUWidth for Dynamic Toolbars
   BOOL m_bDocking;    // TRUE if this bar has a DockContext
   UINT m_uMRUDockID;  // most recent docked dockbar
   rect m_rectMRUDockPos; // most recent docked position
   DWORD m_dwMRUFloatStyle; // most recent floating orientation
   point m_ptMRUFloatPos; // most recent floating position

   primitive_array < void * > m_arrBarID;   // bar IDs for bars contained within this one
   ::userbase::control_bar * m_pBar;    // bar which this refers to (transient)

//   void Serialize(CArchive& ar, BaseDockState* pDockState);
   BOOL LoadState(const char * lpszProfileName, int nIndex, BaseDockState* pDockState);
   BOOL SaveState(const char * lpszProfileName, int nIndex);
};

namespace userbase
{

   class CLASS_DECL_ca2 control_bar :
      virtual public ::user::interaction
   {
   public:
      control_bar();

   // Attributes
   public:
      //int get_count();

      // for styles specific to ::userbase::control_bar
      DWORD GetBarStyle();
      void SetBarStyle(DWORD dwStyle);

      BOOL m_bAutoDelete;

      // getting and setting border space
      void SetBorders(LPCRECT lpRect);
      void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);
      rect GetBorders();

      userbase::frame_window* GetDockingFrame();
      BOOL IsFloating();
      virtual size CalcFixedLayout(BOOL bStretch, BOOL bHorz);
      virtual size CalcDynamicLayout(int nLength, DWORD nMode);

   // Operations
      void EnableDocking(DWORD dwDockStyle);

   // Overridables
      virtual void OnUpdateCmdUI(userbase::frame_window* pTarget, BOOL bDisableIfNoHndler) = 0;

   // Implementation
   public:
      virtual void _001OnDraw(::ca::graphics * pdc);
      virtual void message_handler(gen::signal_object * pobj);
      virtual ~control_bar();
   #ifdef _DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif
      virtual void DelayShow(BOOL bShow);
      virtual BOOL IsVisible();
      virtual DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout);

      virtual BOOL IsDockBar();
      virtual BOOL DestroyWindow();
      virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

      // info about bar (for status bar and toolbar)
      int m_cxLeftBorder, m_cxRightBorder;
      int m_cyTopBorder, m_cyBottomBorder;
      int m_cxDefaultGap;         // default gap value
      UINT m_nMRUWidth;   // For dynamic resizing.
      bool  m_bDockTrack;

      // base_array of elements
      //int m_nCount;
      //void * m_pData;        // m_nCount elements - type depends on derived class

      // support for delayed hide/show
      enum StateFlags
         { delayHide = 1, delayShow = 2, tempHide = 4, statusSet = 8 };
      UINT m_nStateFlags;

      // support for docking
      DWORD m_dwStyle;    // creation style (used for layout)
      DWORD m_dwDockStyle;// indicates how bar can be docked
      userbase::frame_window * m_pDockSite; // current dock site, if dockable
      BaseDockBar* m_pDockBar;   // current dock bar, if dockable
      BaseDockContext* m_pDockContext;   // used during dragging
      DWORD m_dwCtrlStyle;

      virtual void pre_translate_message(gen::signal_object * pobj);
      virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
      virtual void PostNcDestroy();

      virtual void DoPaint(::ca::graphics * pgraphics);
      void DrawBorders(::ca::graphics * pgraphics, rect& rect);
      void DrawGripper(::ca::graphics * pgraphics, const rect& rect);

      // implementation helpers
      void CalcInsideRect(rect& rect, BOOL bHorz) const; // adjusts borders etc
      //BOOL AllocElements(int nElements, int cbElement);
      virtual BOOL SetStatusText(int nHit);
      void ResetTimer(UINT nEvent, UINT nTime);
      void EraseNonClient();
      void EraseNonClient(::ca::graphics * pdc);

      void GetBarInfo(BaseControlBarInfo* pInfo);
      void SetBarInfo(BaseControlBarInfo* pInfo, userbase::frame_window* pFrameWnd);

      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnDestroy)
      DECL_GEN_SIGNAL(_001OnCtlColor)
      DECL_GEN_SIGNAL(_001OnWindowPosChanging)
      DECL_GEN_SIGNAL(_001OnSizeParent)
      DECL_GEN_SIGNAL(_001OnHelpHitTest)
      DECL_GEN_SIGNAL(_001OnInitialUpdate)
      DECL_GEN_SIGNAL(_001OnIdleUpdateCmdUI)
      DECL_GEN_SIGNAL(_001OnLButtonDown)
      DECL_GEN_SIGNAL(_001OnLButtonDblClk)
      DECL_GEN_SIGNAL(_001OnMouseActivate)
      DECL_GEN_SIGNAL(_001OnMouseMove)
      DECL_GEN_SIGNAL(_001OnLButtonUp)
      //DECL_GEN_SIGNAL(_001OnShowWindow)
      //DECL_GEN_SIGNAL(_001OnCancelMode)

   //   DECL_GEN_SIGNAL(_001OnPaint)
   //   virtual void _001OnDraw(::ca::graphics * pdc);

      virtual void install_message_handling(::gen::message::dispatch * pinterface);

      friend class userbase::frame_window;
      friend class BaseDockBar;
   };


} // namespace userbase
