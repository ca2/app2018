#pragma once

namespace user
{

   class button;

   class CLASS_DECL_CORE front_end_schema :
      virtual public ::object
   {
   public:

      class button;

      class CLASS_DECL_CORE system_menu
      {
      public:
         system_menu(sp(::base::application) papp);
         ::draw2d::font_sp    m_fontMarlett;
      };

      class CLASS_DECL_CORE menu
      {
      public:
         menu(sp(::base::application) papp);
         ~menu();
         ::draw2d::font_sp     m_font;
         button *          m_pschemaSysMenuButton;
         button *          m_pschemaItemButton;
         button *          m_pschemaPopupButton;
      };

      class CLASS_DECL_CORE button :
         public ::user::schema_simple_impl
      {
      public:


         bool                    m_bBorder;

         button(sp(::base::application) papp);


      };

      menu           m_menu;
      system_menu    m_sysmenu;
      button         m_button;
      button         m_buttonBaseWndMenuSysMenu;
      button         m_buttonBaseWndMenuItem;
      button         m_buttonBaseWndMenuItemPopup;
      COLORREF       m_crTextNormal;
      

      front_end_schema(sp(::base::application) app);
      ~front_end_schema();

      virtual void DrawCheck(check::check echeck, const RECT & rect, ::draw2d::graphics * pdc);

      

   };

} // namespace user