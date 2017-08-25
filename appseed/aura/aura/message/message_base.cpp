#include "framework.h"



namespace message
{


   //base::base(::aura::application * papp) :
   //   ::message::message(psignal)
   //{

   //   m_lresult = 0;
   //   m_bDestroyed = false;
   //   m_pwnd = NULL;
   //   m_plresult = &m_lresult;
   //   m_bDoSystemDefault = true;

   //}


   base::base(::aura::application * papp) :
      ::message::message(papp)
   {

      m_lresult            = 0;
      m_bDestroyed         = false;
      m_pwnd               = NULL;
      m_plresult           = &m_lresult;
      m_bDoSystemDefault   = true;

   }


   base::base(::aura::application * papp,::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult):
      ::message::message(papp)
   {

      m_lresult            = 0;
      m_bDestroyed         = false;
      m_bDoSystemDefault   = true;
      set(pwnd,uiMessage,wparam,lparam,lresult);

      //Application.tellme_destroyed(pwnd, &m_bDestroyed);

   }


   base::base(const base & base)
   {

      m_pwnd               = base.m_pwnd;
      m_id                 = base.m_id;
      m_wparam             = base.m_wparam;
      m_lparam             = base.m_lparam;
      m_bConditional       = base.m_bConditional;
      m_bReflect           = base.m_bReflect;
      m_bDoSystemDefault   = base.m_bDoSystemDefault;
      m_bDestroyed         = false;

      //Application.tellme_destroyed(m_pwnd, &m_bDestroyed);

   }

   base::~base()
   {

      //if(m_pwnd != NULL)
      //{

      //   synch_lock sl(get_ui_destroyed_mutex());

      //   if(m_bDestroyed)
      //   {

      //      debug_print("Window Already Destroyed");

      //   }
      //   else
      //   {

      //      Application.tellme_destroyed(m_pwnd, &m_bDestroyed, false);

      //   }

      //}

   }


   void base::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {

      m_pwnd            = pwnd;
      m_id              = uiMessage;
      m_wparam          = wparam;
      m_lparam          = lparam;
      m_plresult        = &lresult;

   }


   void base::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam)
   {

      set(pwnd,uiMessage,wparam,lparam,m_lresult);

   }


   void base::set_lresult(LRESULT lresult)
   {

      *m_plresult = lresult;

   }


   LRESULT & base::get_lresult()
   {

      return *m_plresult;

   }


   base & base::operator = (const base & base)
   {

      if (this != &base)
      {

         copy_this(base);

      }

      return *this;

   }


   //void base::copy_this(const base & base)
   //{

   //   m_pwnd = base.m_pwnd;
   //   m_bConditional = base.m_bConditional;
   //   m_bReflect = base.m_bReflect;
   //   m_bDestroyed = base.m_bDestroyed;
   //   m_bDoSystemDefault = base.m_bDoSystemDefault;
   //   m_plresult = base.m_plresult;
   //   m_lresult = base.m_lresult;

   //}


   void timer::set(::user::primitive * pwnd,UINT uiMessage,WPARAM wparam,::lparam lparam,LRESULT & lresult)
   {
      base::set(pwnd,uiMessage,wparam,lparam,lresult);
      m_nIDEvent = static_cast<UINT>(wparam);
   }



} // namespace message








#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)

#define IMPLEMENT_SIGNAL_OBJECT_FIXED_ALLOC(class_name) IMPLEMENT_FIXED_ALLOC(class_name, ROUND4(sizeof(class_name) * 32))

