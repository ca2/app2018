#pragma once


namespace user
{


   class CLASS_DECL_CORE elastic_slider :
      public ::user::interaction

   {
   public:



      scalar_base *  m_pscalarVelocity;
      scalar_base *  m_pscalarPosition;

      e_scalar       m_escalar;

      uint32_t       m_dwLastTime;

      double         m_dPosition;// 0.0 = MIN 1.0 = MAX
      double         m_dTensionPosition;// 0.0 = MIN 1.0 = MAX
      bool           m_bSlide;
      index          m_iScalar;
      double_array   m_daScalar;

      elastic_slider(::aura::application * papp);
      virtual ~elastic_slider();


      virtual void install_message_handling(::message::dispatch * pdispatch);

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnTimer);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);


      void Slide();




      void SetStreamingVelocityMode(scalar_base * pscalarVelocity, scalar_base * pscalarPosition);


      void UpdatePosition();
      void CalcTension(point & pt);
      void SetSliderPos(double dPos);

      double CalcScalar();
      void CalcTension();


      virtual void _001OnDraw(::draw2d::graphics * pdc);

      double GetForce();

      void GetSliderRect(rect & rect);

   };

} // namespace user