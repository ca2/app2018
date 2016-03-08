#pragma once


#define ca2m_PRODEVIAN_SYNCH (WM_APP + 1985)


namespace user
{


   class buffer;
   class interaction_base;

}


namespace user
{


   class CLASS_DECL_BASE window_draw :
      virtual public thread
   {
   public:


      bool                 m_bProDevianMode;
      double               m_iFramesPerSecond;
      event                m_eventFree;
      bool                 m_bRunning;
      bool                 m_bRender;
      


      window_draw(::aura::application * papp);

      virtual bool UpdateBuffer();
      virtual bool ScreenOutput();

      // legacy name TWF ressurected here - TWF - Transparent Window Interface - The veriwell Musical Player lite translucent windows
      virtual bool twf_start();
      virtual bool twf_stop();

      virtual void asynch_redraw();
      virtual void synch_redraw();


      //virtual bool pre_run() override;
      virtual int32_t run() override;


      //ptr_array < ::user::interaction > get_wnda();


   };



   typedef smart_pointer < window_draw > window_draw_sp;


} // namespace user



