#pragma once


class CLASS_DECL_AURA timer_event:
   virtual public timer
{
public:


   manual_reset_event   m_ev;


   timer_event(::aura::application * papp,int iTimer);
   virtual ~timer_event();


   virtual bool wait(int millis);


   virtual void on_timer() override;


};






