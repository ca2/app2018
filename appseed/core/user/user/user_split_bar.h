#pragma once


namespace user
{

   class split_layout;

   class CLASS_DECL_CORE split_bar :
      public ::user::interaction
   {
   public:


      index                         m_iIndex;
      split_layout *                m_pparent;
      uint32_t                      m_dwPosition;
      double                        m_dRate;
      double                        m_dMinimumRate;
      double                        m_dMaximumRate;
      uint32_t                      m_dwMaxPosition;



      split_bar(::aura::application * papp);
      virtual ~split_bar();


      void install_message_routing(::message::sender * pinterface);

      virtual void _001OnDraw(::draw2d::graphics * pgraphics);

      using ::user::interaction::create_window;
      bool create_window(const RECT & rect, split_layout * pparent);
      virtual bool pre_create_window(::user::create_struct& cs);

      
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);

      virtual void on_layout();

   };


} // namespace user


