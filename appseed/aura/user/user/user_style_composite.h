#pragma once


namespace user
{


//   class CLASS_DECL_AURA style_composite :
//      virtual public style_base
//   {
//   public:
//
//
//      style_composite();
//      virtual ~style_composite();
//
//
//      // these are style_base pure virtual functions
//      // "implementation"
//      // these are style_base pure virtual functions
//      // composition
//      // these function should be always be a composition
//      // based on userstyle()
//      virtual void initialize_style(e_schema eschema) override;
//      virtual void initialize_style() override;
//      virtual void initialize_style_set() override;
//      virtual ::user::menu_interaction * create_menu_button(menu_item * pitem) override;
//      virtual bool prepare_menu(::user::menu_item * pitem) override;
//      virtual bool prepare_menu_button(::user::menu_item * pitem) override;
//
//
//      virtual bool _001OnDrawMainFrameBackground(::draw2d::graphics * pgraphics,::user::frame * pframe) override;
//      virtual void DrawCheck(check::e_check echeck, const RECT & rect, ::draw2d::graphics * pgraphics) override;
//      virtual bool simple_ui_draw_focus_rect(::user::interaction * pui,::draw2d::graphics * pgraphics) override;
//      virtual bool on_ui_event(e_event eevent, e_object eobject, ::user::interaction * pui) override;
//      virtual bool _001TabOnDrawSchema01(::draw2d::graphics * pgraphics,tab * ptab) override;
//      virtual bool _001OnDrawMenuInteraction(::draw2d::graphics * pgraphics, menu_interaction * pbutton) override;
//      virtual void _001OnTabPaneDrawTitle(::user::tab_pane & pane,::user::tab * ptab,::draw2d::graphics * pgraphics,LPCRECT lpcrect,::draw2d::brush_sp & brushText) override;
//      virtual bool _001OnTabLayout(::user::tab * ptab) override;
//      virtual void _001DrawCheckBox(::draw2d::graphics * pgraphics, LPCRECT lpcrectClient, ::check::e_check echeck) override;
//      virtual bool _001DrawToolbarItem(::draw2d::graphics * pgraphics, int32_t iItem, ::user::toolbar * ptoolbar) override;
//      virtual bool _001DrawSimpleScrollBar(::draw2d::graphics * pgraphics, simple_scroll_bar * pbar) override;
//      virtual bool _001DrawScrollBar(::draw2d::graphics * pgraphics, ::user::scroll_bar * pbar) override;
//
//      //virtual bool get_color(COLORREF & cr,e_color ecolor) override;
//      //virtual bool get_font(::draw2d::font_sp & sp, e_font efont) override;
//      //virtual bool get_translucency(e_translucency & etranslucency, e_element eelement) override;
//      //virtual bool get_flag(bool & bSet, ::user::e_flag eflag) override;
//      //virtual bool get_rect(RECT & rect, ::user::e_rect erect) override;
//      //virtual bool get_int(int & i, ::user::e_int eint) override;
//
//      //virtual e_style get_style() override;
//
//
//      // these are exception of rule above
////      virtual sp(style) & operator[](e_style estyle) override;
//
//
////      virtual bool impl_get_color(COLORREF & cr, e_color eusercolor) = 0;
////      virtual bool impl_get_font(::draw2d::font_sp & sp, e_font efont) = 0;
////      virtual bool impl_get_translucency(e_translucency & etranslucency, e_element element) = 0;
////      virtual bool impl_has_flag(e_flag eflag) = 0;
////      virtual rect impl_get_rect(e_rect erect) = 0;
////      virtual int impl_get_int(e_int eint) = 0;
//
//
//
//      // these are utility functions
//      // the utility functions should be based on this class functions
//      // specially the style_base pure function overrides
//      // that way, if some styled class overriding any style_base pure function
//      // will affect the return of the utility function.
//      // so, it should be very avoided using the m_pstylebase compositor
//      // to implement the utility functions
//      //virtual bool _001GetMainFrameTranslucency(::user::e_translucency & etranslucency);
//
//      virtual bool select_text_color(::user::interaction * pui, ::draw2d::graphics * pgraphics, e_color ecolor = color_text);
//      virtual bool select_solid_brush(::user::interaction * pui, ::draw2d::graphics * pgraphics, e_color ecolor);
//      virtual bool select_font(::user::interaction * pui, ::draw2d::graphics * pgraphics, e_font efont = font_default);
//
//
//
//      virtual bool select_text_color(::user::interaction * pui, e_color ecolor = color_text);
//      virtual bool select_solid_brush(::user::interaction * pui, e_color ecolor);
//      virtual bool select_font(::user::interaction * pui, e_font efont = font_default);
//
//
//
//      virtual COLORREF           _001GetColor(::user::interaction * pui, e_color ecolor, COLORREF crDefault = 0);
//      virtual ::draw2d::font_sp  _001GetFont(::user::interaction * pui, e_font efont, ::draw2d::font * pfont = NULL);
//      virtual e_translucency     _001GetTranslucency(::user::interaction * pui, e_element eelement = element_none, e_translucency etranslucencyDefault = translucency_undefined);
//      virtual bool               _001GetFlag(::user::interaction * pui, ::user::e_flag eflag, bool bDefault = false);
//      virtual RECT               _001GetRect(::user::interaction * pui, ::user::e_rect erect, rect rectDefault = ::null_rect());
//      virtual int                _001GetInt(::user::interaction * pui, ::user::e_int eint, int iDefault = 0);
//      virtual double             _001GetDouble(::user::interaction * pui, ::user::e_double edouble, double dDefault = 0.0);
//
//      virtual bool _001IsBackgroundBypass(::user::interaction * pui, e_element eelement = element_none);
//      virtual bool _001IsTransparent(::user::interaction * pui, e_element eelement = element_none);
//      virtual bool _001IsTranslucent(::user::interaction * pui, e_element eelement = element_none);
//      virtual bool _001HasTranslucency(::user::interaction * pui, e_element eelement = element_none);
//
//
//
//
//
//
//      // special utility functions
//      // they are very suitable for style containers that contains that style
//      // by deriving from this style class
//      virtual ::user::style * userstyle() = 0;
//
//
//
//      /// style selection generally done at initialization
//      virtual void on_select_user_style();
//      virtual ::user::style * parent_userstyle();
//      virtual style * style_get(e_schema eschema) = 0;
//
//
//      virtual ::draw2d::graphics * style_get_graphics();
//
//
//      // userstyle() composition
//      virtual bool userstyle_color(COLORREF & cr, e_color ecolor, ::user::interaction * pui);
//      virtual bool userstyle_font(::draw2d::font_sp & sp, e_font efont, ::user::interaction * pui);
//      virtual bool userstyle_translucency(e_translucency & etranslucency, e_element eelement, ::user::interaction * pui);
//      virtual bool userstyle_flag(bool & bSet, ::user::e_flag eflag, ::user::interaction * pui);
//      virtual bool userstyle_rect(RECT & rect, ::user::e_rect erect, ::user::interaction * pui);
//      virtual bool userstyle_int(int & i, ::user::e_int eint, ::user::interaction * pui);
//      virtual bool userstyle_double(double & d, ::user::e_double edouble, ::user::interaction * pui);
//
//
//      // e_style composition
//      virtual bool style_color(COLORREF & cr,e_color ecolor, ::user::interaction * pui);
//      virtual bool style_font(::draw2d::font_sp & sp, e_font efont, ::user::interaction * pui);
//      virtual bool style_translucency(e_translucency & etranslucency, e_element eelement, ::user::interaction * pui);
//      virtual bool style_flag(bool & bSet, ::user::e_flag eflag, ::user::interaction * pui);
//      virtual bool style_rect(RECT & rect, ::user::e_rect erect, ::user::interaction * pui);
//      virtual bool style_int(int & i, ::user::e_int eint, ::user::interaction * pui);
//      virtual bool style_double(double & d, ::user::e_double edouble, ::user::interaction * pui);
//
//
//
//   };
//

} // namespace user




