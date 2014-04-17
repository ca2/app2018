#pragma once


#define TBSTYLE_ALIGN_LEFT     0x10000
#define TBSTYLE_ALIGN_RIGHT    0x30000
#define TBSTYLE_ALIGN_CENTER   0x20000
#define TBSTYLE_ALIGN_TOP      0x40000
#define TBSTYLE_ALIGN_BOTTOM   0x80000
#define TBSTYLE_ALIGN_VCENTER  0xC0000


class image_list;


class simple_toolbar_item :
      virtual public element
{
public:


   int32_t                  m_iIndex;
   int32_t                  m_iImage;
   ::visual::dib_sp     m_spdib;
   id                   m_id;
   BYTE                 m_fsState;
   BYTE                 m_fsStyle;
   string               m_str;
   rect                 m_rect;
   bool                 m_bEnableIfHasCommandHandler;


   simple_toolbar_item();


};


class CLASS_DECL_CORE simple_toolbar :
   public ::user::control_bar
{

public:

   enum EElement
   {
      ElementItem,
      ElementItemHover,
      ElementItemPress,
      ElementImage,
      ElementImageHover,
      ElementImagePress,
      ElementText,
      ElementTextHover,
      ElementTextPress,
   };


   bool                       m_bTransparentBackground;
   bool                       m_bSimpleLayout;
   spa(simple_toolbar_item)   m_itema;
   int32_t                    m_iHover;

   size                       m_sizeButton;
   size                       m_sizeImage;
#ifdef WINDOWS
    HRSRC           m_hRsrcImageWell; // handle to loaded resource for image well
#endif
   HINSTANCE        m_hInstImageWell; // instance handle to load image well from
   HBITMAP          m_hbmImageWell; // contains color mapped button images
//   sp(image_list)    m_pimagelist;
//   sp(image_list)    m_pimagelistHue;
//   sp(image_list)    m_pimagelistBlend;
//   sp(image_list)    m_pimagelistHueLight;
   //bool           m_bInternalImageList;
   int32_t              m_iButtonPressItem;
   bool             m_bDelayedButtonLayout;
   ::draw2d::dib_sp     m_dibDraft;

   ::draw2d::font_sp    m_font;



   simple_toolbar(sp(base_application) papp);
   virtual ~simple_toolbar();


   using ::user::control_bar::create;
   bool create(sp(::user::interaction) pParentWnd,
      uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
      id nID = "__IDW_TOOLBAR");
   using ::user::interaction::CreateEx;
#if defined(WINDOWSEX) || defined(LINUX)
   bool CreateEx(sp(::user::interaction) pParentWnd, uint32_t dwCtrlStyle = TBSTYLE_FLAT,
      uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
      rect rcBorders = rect(0, 0, 0, 0),
      id nID = "__IDW_TOOLBAR");
#else
   bool CreateEx(sp(::user::interaction) pParentWnd, uint32_t dwCtrlStyle = 0,
      uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
      rect rcBorders = rect(0, 0, 0, 0),
      id nID = "__IDW_TOOLBAR");
#endif


   int32_t WrapToolBar(int32_t nCount, int32_t nWidth);
   void SizeToolBar( int32_t nCount, int32_t nLength, bool bVert = FALSE);

   size CalcDynamicLayout(int32_t nLength, uint32_t dwMode);
   size CalcLayout(uint32_t dwMode, int32_t nLength = -1);
   //bool CalcSize(size & size, bool bHorz);
   size CalcSize(int32_t nCount);
   virtual void OnBarStyleChange(uint32_t dwOldStyle, uint32_t dwNewStyle);
   virtual size CalcFixedLayout(bool bStretch, bool bHorz);


   void RemoveAllTools();
   virtual int32_t _001GetHoverItem();
   virtual void OnUpdateHover();
   void SetItemImage(int32_t iItem, int32_t iImage);
   void GetButtonText(int32_t i, string & str);
   void SetButtonStyle(int32_t nIndex, UINT nStyle);
   UINT GetButtonStyle(int32_t iButton);
   bool SetItemStyle(int32_t iItem, BYTE bStyle);
   int32_t GetItemStyle(int32_t iItem);
   void _001SetImageList(sp(image_list) pimagelist);
   void _001DiscardImageList();
   void _001OnClick(index iItem);
   int32_t _001HitTest(point pt);
   void _001Hover(point pt, bool bRedraw = true);
   void _001Hover(bool bRedraw = true);


   void layout();


   void _001DrawItem(::draw2d::graphics * pdc, int32_t iItem);
   bool _001GetItemRect(int32_t iItem, LPRECT lprect);
   bool _001GetItemRect(int32_t iItem, LPRECT lprect, EElement eelement);
   bool _001SetItem(int32_t iItem, simple_toolbar_item * pitem);
   bool _001GetItem(int32_t iItem, simple_toolbar_item * pitem);
   //bool _SetButton(int32_t nIndex, TBBUTTON * pbutton);
   //bool _GetButton(int32_t nIndex, TBBUTTON * pbutton);
   int32_t _001GetItemCount();

/*
   bool LoadToolBar(const char * lpszResourceName);
   bool LoadToolBar(UINT nIDResource);
   bool LoadBitmap(const char * lpszResourceName);
   bool LoadBitmap(UINT nIDResource);
*/

   bool LoadXmlToolBar(const char * lpszFileName);



//   bool SetButtons(const UINT* lpIDArray, int32_t nIDCount);
   void SetSizes(SIZE sizeButton, SIZE sizeImage);

   void OnUpdateCmdUI(sp(::user::frame_window) pTarget, bool bDisableIfNoHndler);


   void TransparentEraseNonClient(::draw2d::graphics * pdc);
   void SetTransparentBackground(bool bSet);
   virtual void _001OnDraw(::draw2d::graphics * pdc);
   size CalcSimpleLayout();

   void _001OnImageListAttrib();
   DECL_GEN_SIGNAL(_001OnCreate);
   DECL_GEN_SIGNAL(_001OnMouseMove);
   DECL_GEN_SIGNAL(_001OnLButtonDown);
   DECL_GEN_SIGNAL(_001OnTimer);
   DECL_GEN_SIGNAL(_001OnLButtonUp);
   DECL_GEN_SIGNAL(_001OnNcCalcSize);
   DECL_GEN_SIGNAL(_001OnNcHitTest);
   DECL_GEN_SIGNAL(_001OnMove);
   DECL_GEN_SIGNAL(_001OnMouseLeave);

   virtual void install_message_handling(::message::dispatch * pdispatch);


};
