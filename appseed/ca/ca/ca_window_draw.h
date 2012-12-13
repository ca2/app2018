#pragma once


#include "ca/user/user_window_util.h"


#define ca2m_PRODEVIAN_SYNCH (WM_APP + 1985)

namespace user
{
   class buffer;
   class window_interface;
}

namespace ca
{

   class CLASS_DECL_ca window_draw :
      virtual public ::radix::object
   {
   public:


      bool                 m_bRun;
      bool                 m_bProDevianMode;
      ::user::buffer *     m_pbuffer;
      DWORD                m_iFramesPerSecond;
      event                m_eventFree;


      window_draw(::ca::application * papp);

      virtual bool start();
      virtual bool UpdateBuffer();
      virtual bool ScreenOutput();
      virtual bool stop();

      virtual void asynch_redraw();
      virtual void synch_redraw();

      static bool s_bRunning;
   };



   typedef smart_pointer < window_draw > window_draw_sp;

} // namespace ca
