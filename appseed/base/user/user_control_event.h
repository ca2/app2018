#pragma once


namespace user
{


   enum e_event
   {
      event_initialize_control,
      event_button_clicked,
      event_list_clicked,
      event_mouse_enter,
      event_mouse_leave,
      event_set_check,
      event_timer,
      event_enter_key,
      event_tab_key,
      event_key_down,
      event_after_change_text,
      event_after_change_cur_sel,
   };


   class CLASS_DECL_BASE control_event
   {
   public:


      e_event                       m_eevent;
      sp(::user::interaction)       m_puie;
      uint32_t                      m_uiEvent;
      ::action::context            m_actioncontext;
      uint32_t                      m_uiVKey;
      uint32_t                      m_uiFlags;
      
      
      control_event();
      virtual ~control_event();


   };


} // namespace user





