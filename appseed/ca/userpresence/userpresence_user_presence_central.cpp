#include "framework.h"

namespace userpresence
{

   presence_central::presence_central(::ca::application * papp) :
      ca(papp),
      message_window_simple_callback(papp)
   {
   }

   presence_central::~presence_central()
   {
   }

   bool presence_central::initialize()
   {

      if(!initialize_message_window("ca5::user::presence_central::message_window"))
         return false;

      m_spuiMessage->SetTimer(1984, 1000, ::null());

      if(ApplicationUser.m_ppresence == ::null())
      {
         presence * ppresence = new presence(get_app());
         ppresence->report_activity();
         ppresence->pulse_user_presence();
         ApplicationUser.m_ppresence = ppresence;
      }

      return true;

   }

   bool presence_central::finalize()
   {

      m_spuiMessage->KillTimer(1984);

      finalize_message_window();

      if(ApplicationUser.m_ppresence != ::null())
      {
         delete ApplicationUser.m_ppresence;
         ApplicationUser.m_ppresence = ::null();
      }

      return true;

   }

   bool presence_central::is_initialized()
   {

      if(!m_spuiMessage->IsWindow())
         return false;

      return true;

   }

   void presence_central::message_window_message_handler(::ca::signal_object * pobj)
   {
      SCAST_PTR(::ca::message::base, pbase, pobj);
      if(pbase->m_uiMessage == WM_TIMER)
      {
         SCAST_PTR(::ca::message::timer, ptimer, pobj);
         if(&ApplicationUser != ::null())
         {
            presence * ppresence = ApplicationUser.m_ppresence;
            if(ptimer->m_nIDEvent == 1984 && ppresence != ::null())
            {
               ppresence->defer_pulse_user_presence();
            }
         }
      }
   }


} // namespace ca5
