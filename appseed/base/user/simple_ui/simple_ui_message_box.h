#pragma once


namespace simple_ui
{


   class CLASS_DECL_BASE message_box :
      virtual public ::simple_ui::top
   {
   public:


      stringa                 m_stra;
      spa(tap)                m_tapaA;
      spa(tap)                m_tapaB;
      int32_t                 m_iResult;
      uint32_t                m_uiFlags;
      string                  m_strMessage;
      string                  m_strTitle;
      manual_reset_event      m_evReady;


      message_box(::aura::application * papp,const char * pszMessage,const char * pszCaption,uint32_t uiFlags);
      virtual ~message_box();


      int32_t show();

      DRAWDD() override;

      virtual void on_layout() override;

      bool on_action(const char * pszId) override;

      virtual void create_a_button(id id,const char * pszText);
      virtual void create_b_button(id id,const char * pszText);
      virtual void create_button(spa(tap) & tapa, id id,const char * pszText);


      virtual void pre_translate_message(::message::message * pobj) override;

      //virtual void EndModalLoop(id nResult);

      virtual bool has_pending_graphical_update() override;


//      virtual void on_select_user_style() override;


   };


} // namespace simple_ui



CLASS_DECL_BASE int32_t simple_ui_message_box(oswindow interaction_impl, const char * lpText,const char * lpCaption, uint32_t uiFlags);






