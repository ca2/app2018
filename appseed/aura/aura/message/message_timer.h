#pragma once


namespace message
{


   class CLASS_DECL_AURA timer : virtual public base
   {
   public:


      UINT m_nIDEvent;


      timer(::aura::application * papp) : ::message::base(papp) {}

      using ::message::base::set;
      virtual void set(::user::primitive * pwnd, UINT uiMessage, WPARAM wparam, ::lparam lparam, LRESULT & lresult);


   };



} // namespace message



