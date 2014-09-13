#pragma once


namespace macos
{
   
   
   class CLASS_DECL_BASE window_draw :
   virtual public ::user::window_draw,
   virtual public aura::message_queue_listener
   {
   public:
      
      
      ::aura::message_queue_sp               m_spqueue;
      user::interaction_ptr_array            m_wndpaOut;
      mutex                                  m_mutexRendering;
      mutex                                  m_mutexRgnUpdate;
      semaphore                              m_semaphoreBuffer;
      bool                                   m_bRender;
      DWORD                                  m_dwLastRedrawRequest;
      mutex                                  m_mutexRender;
      DWORD                                  m_dwLastUpdate;
      DWORD                                  m_dwLastDelay;
      
      
      window_draw(::aura::application * papp);
      virtual ~window_draw();
      
      
      bool UpdateBuffer(LPCRECT lpcrect);
      virtual bool UpdateBuffer();
      
      user::interaction_ptr_array get_wnda();
      
      virtual int32_t run();
      
      virtual UINT RedrawProc();
      
      virtual void asynch_redraw();
      virtual void _asynch_redraw();
      virtual void synch_redraw();
      virtual void _synch_redraw();
      
      virtual void message_queue_message_handler(signal_details * pobj);
      
      virtual bool pre_run();
      
      
   };

   
} // namespace macos







