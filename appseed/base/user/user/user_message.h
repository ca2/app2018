#pragma once


namespace user
{


   class CLASS_DECL_BASE message :
      virtual public ::object
   {
   public:


      sp(::user::interaction)    m_pui;
      UINT                       m_uiMessage;
      WPARAM                     m_wparam;
      LPARAM                     m_lparam;


      LRESULT send();
      static_function UINT ThreadProcSendMessage(LPVOID lp);
      static_function void post(sp(::user::interaction) puie, UINT uiMessage, WPARAM wparam, LPARAM lparam, int32_t epriority = ::multithreading::priority_normal);

   };


} // namespace user
