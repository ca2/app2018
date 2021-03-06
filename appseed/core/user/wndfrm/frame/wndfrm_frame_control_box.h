#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         class  CLASS_DECL_CORE control_box :
            virtual public ::user::box
         {
         public:


            WorkSet *                     m_pworkset;
            DWORD                         m_dwShowTime;
            bool                          m_bShowAttempt;
            bool                          m_bDrag;
            point                         m_ptDrag;
            int32_t                       m_iDefaultButtonMargin;
            size                          m_sizeButtonDefault;
            ::draw2d::font_sp             m_fontMarlett;

            ::draw2d::brush_sp            m_brushButtonBack;
            ::draw2d::brush_sp            m_brushButtonBackSel;
            ::draw2d::brush_sp            m_brushButtonBackFocus;
            ::draw2d::brush_sp            m_brushButtonBackDisabled;

            ::draw2d::pen_sp              m_penButtonBack;
            ::draw2d::pen_sp              m_penButtonBackSel;
            ::draw2d::pen_sp              m_penButtonBackFocus;
            ::draw2d::pen_sp              m_penButtonBackDisabled;

            COLORREF                      m_crButtonFore;
            COLORREF                      m_crButtonForeSel;
            COLORREF                      m_crButtonForeFocus;
            COLORREF                      m_crButtonForeDisabled;

            map < e_button, e_button, id >           m_mapButtonId;
            id_map < e_button >           m_mapIdButton;
            map < e_button, e_button, bool, bool >       m_buttonmapPresent;


            COLORREF                      m_crBackground;


            map < e_button, e_button, sp(control_box_button), sp(control_box_button) >      m_buttonmap;


            control_box(::aura::application * papp);
            virtual ~control_box();


            virtual void set_button_color_system_default_001();


            virtual void install_message_routing(::message::sender *pinterface);

            virtual void _001OnClip(::draw2d::graphics * pgraphics);

            void drag(point pt);


            DECL_GEN_SIGNAL(_001OnShowWindow);
            DECL_GEN_SIGNAL(_001OnLButtonDown);
            DECL_GEN_SIGNAL(_001OnLButtonUp);
            DECL_GEN_SIGNAL(_001OnMouseMove);
            void _001OnTimer(::timer * ptimer);
            DECL_GEN_SIGNAL(_001OnCreate);
            DECL_GEN_SIGNAL(_001OnMove);
            DECL_GEN_SIGNAL(_001OnSize);

            virtual void on_layout();

            virtual size get_button_size(e_button ebutton);

            virtual rect get_button_margin(e_button ebutton);

            virtual int32_t calc_button_size();

            virtual int32_t calc_control_box_height();

            virtual int32_t calc_control_box_normal_width();

            virtual int32_t calc_control_box_zoomed_width();

            virtual int32_t calc_control_box_full_screen_width();

            ::user::wndfrm::frame::appearance * workset_get_appearance();

            virtual void reset_layout();

            virtual bool has_button(e_button ebutton);
            virtual void hide_button(e_button ebutton, bool bHide = true);
            virtual bool create_buttons();
            virtual void update_control_box_button(e_button ebutton);
            virtual void update_control_box_buttons();
            virtual bool create_button(e_button ebutton);
            virtual bool get_control_box_button_caption(e_button ebutton, string & str);
            virtual e_stock_icon get_control_box_button_stock_icon(e_button ebutton);
            virtual void set_control_box_button_id(e_button ebutton, id id);
            virtual id get_control_box_button_id(e_button ebutton);
            virtual e_button get_control_box_button_type(id id);
            sp(control_box_button) get_button(e_button ebutton);
            virtual sp(control_box_button) get_box_button(e_button ebutton);

            virtual bool get_font(::draw2d::font_sp & font);

         };


      } // namespace frame


   } // namespace wndfrm


} // namespace user






