#pragma once


namespace bergedge
{


   class view;
   class pane_view;


   class CLASS_DECL_CORE frame : 
      virtual public simple_frame_window,
      public message_queue
   {
   public:
      
      
      string                        m_strAnimatedStatusBarText;
      int32_t                       m_iAnimateStep;
      image_list  *                 m_pimagelist;
      sp(::bergedge::document)      m_pdocument;
      bool                          m_bHoverMouse;
      uint32_t                      m_dwLastHover;
      bool                          m_bTimerOn;

      bool                          m_bMouseOver;

      sp(::bergedge::view)          m_pview;
      sp(::bergedge::pane_view)     m_ppaneview;


      frame(sp(base_application) papp);
      virtual void install_message_handling(::message::dispatch * pinterface);

      DECL_GEN_SIGNAL(_001OnApp1);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      virtual void _000OnMouse(::message::mouse * pmouse);

      void OnHoverAction();

      virtual bool pre_create_window(CREATESTRUCT& cs);

      void ShowControlBars(bool bShow);
      void AnimateStatusBar();
      void SetAnimatedStatusBarText(const char * lpcsz);
      virtual ~frame();
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

      virtual void pre_translate_message(signal_details * pobj);

      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnApp2000)

      virtual void message_handler(signal_details * pobj);

      virtual void message_queue_message_handler(signal_details * pobj);


   };


} // namespace bergedge


