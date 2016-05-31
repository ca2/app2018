#pragma once


#undef new



namespace message
{


   class CLASS_DECL_ACE base :
      public signal_details
   {
   public:


      ::user::primitive *      m_pwnd;
      bool                       m_bConditional;
      bool                       m_bReflect;
      bool                       m_bDestroyed;
      bool                       m_bDoSystemDefault;

      base(::ace::application * papp,class ::signal * psignal = NULL);
      base(::ace::application * papp,::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
      base(const base & base);
      virtual ~base();



      virtual void set_lresult(LRESULT lresult);
      virtual LRESULT & get_lresult();
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam);

      LRESULT * m_plresult;
      LRESULT  m_lresult;
   };


   class CLASS_DECL_ACE timer: public base
   {
   public:


      timer(::ace::application * papp): ::message::base(papp) {}
      using ::message::base::set;
      virtual void set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult);
      UINT m_nIDEvent;
   };



} // namespace message


#define new ACE_NEW