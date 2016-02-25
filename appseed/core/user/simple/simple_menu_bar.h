#pragma once


namespace user
{


   class toolbar_control;


} // namespace user


class SimpleMenu;
class CMenuTrackingThreadV033;
class CMenuBarHookV033;

class CLASS_DECL_CORE simple_menu_bar :
   public simple_toolbar
{
public:


   int32_t                       m_iHover;
   sp(image_list)                m_pimagelist;
   sp(image_list)                m_pimagelistDisabled;
   int_int_spreadset *           m_prel;
   bool                          m_bTracking;
   int32_t                       m_iTracking;
   UINT                          m_uiResourceID;
   int32_t                       m_iTopMenuCount;
   ::user::menu *                m_pmenu;


   simple_menu_bar(::aura::application * papp);
   virtual ~simple_menu_bar();


   using ::simple_toolbar::create_window;
   bool create_window(sp(::user::interaction) pParentWnd,uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,UINT nID = __IDW_TOOLBAR);
   using ::simple_toolbar::create_window_ex;
   bool create_window_ex(sp(::user::interaction) pParentWnd, uint32_t dwCtrlStyle = TBSTYLE_FLAT,uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,const RECT & rect = ::null_rect(),UINT nID = __IDW_TOOLBAR);


   LRESULT CALLBACK MessageProc(
        int32_t code,       // hook code
        WPARAM wParam,  // undefined
        LPARAM lParam   // address of structure with message data
        );

   virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);
   virtual void pre_translate_message(signal_details * pobj);
   protected:
   virtual bool pre_create_window(::user::create_struct& cs);

   virtual int32_t _001GetHoverItem();
   virtual void OnUpdateHover();
   void _001OnClick(index iItem);
   void _001OnDropDown(int32_t iItem);
   //int32_t _001GetHoverItem();
   //void _001Hover(point pt);
   //void _001Hover();
   //void _001DrawItem(::draw2d::graphics * pdc, int32_t iItem);
   //int32_t _001HitTest(const POINT * point);
   //bool _001CheckItem(int32_t iItem, bool bCheck);
   //bool _001GetItemRect(int32_t iItem, LPRECT lprect, e_element eelement);
   //virtual void _001OnDraw(::draw2d::dib * pdib);
   bool ReloadMenuBar();
   virtual void OnUpdateCmdUI(sp(::user::frame_window) pTarget, bool bDisableIfNoHndler);
   void RemoveAllButtons();

   bool Initialize(
      sp(image_list) pimagelist,
      sp(image_list) pimagelistDisabled,
      int_int_spreadset * prel,
      ::draw2d::font * pfont);

   //size CalcLayout(uint32_t dwMode, int32_t nLength);

   //size CalcFixedLayout(bool bStretch, bool bHorz);

   //size CalcDynamicLayout(int32_t nLength, uint32_t dwMode);
   //static ::music::e_result CalcSize(::user::toolbar_control & tbc, size & size);
   //static ::music::e_result CalcSize(CToolBarCtrl & tbc, size & size);
   bool _TrackPopupMenu(point point);
   bool _TrackPopupMenu(int32_t iItem);
   bool LoadMenuBar(UINT nIDResource);
   void SetMenuID(UINT nIDResource);

   DECL_GEN_SIGNAL(_001OnMouseMove);
   DECL_GEN_SIGNAL(_001OnNcMouseMove);
   DECL_GEN_SIGNAL(_001OnCreate);
   DECL_GEN_SIGNAL(_001OnKeyDown);
   DECL_GEN_SIGNAL(_001OnDestroy);
   DECL_GEN_SIGNAL(_001OnMenuChar);
   DECL_GEN_SIGNAL(_001OnLButtonDown);
   void _001OnTimer(::timer * ptimer);

   virtual int32_t OnMessage(MPARAM mparam, NPARAM nparam, OPARAM oparam);

   DECL_GEN_SIGNAL(_001OnAppLanguage);

   virtual void install_message_handling(::message::dispatch * pdispatch);


};


