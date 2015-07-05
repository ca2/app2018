//#include "framework.h"
//#include "base/user/user.h"


namespace userpresence
{


   userpresence::userpresence(::aura::application * papp) :
      ::object(papp),
      ::aura::departament(papp),
      m_spqueue(allocer())
   {

      m_bUserPresenceFeatureRequired = false;

   }


   userpresence::~userpresence()
   {

   }


   bool userpresence::initialize()
   {


      if (!(bool)System.oprop("do_not_initialize_user_presence"))
      {
         // xxx
         // defer_initialize_user_presence();
         //
      }

      

      return true;

   }

   bool userpresence::finalize()
   {

      if(!m_pauraapp->is_system())
      {
         defer_finalize_user_presence();
      }

      return true;

   }

   bool userpresence::defer_initialize_user_presence()
   {
      return true;

      if(!is_initialized())
      {

         //if(m_spuiMessage.is_null())
         {
           // m_spuiMessage = canew(::user::interaction());
         }

         if(!m_spqueue->create_message_queue("ca5::user::userpresence::message_queue"))
            return false;

      }


      string strQuery = Application.command()->m_varTopicQuery["app"];

      if(Application.command()->m_varTopicQuery.has_property("install")
      || Application.command()->m_varTopicQuery.has_property("uninstall"))
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "simpledbcfg"
      || Application.command()->m_varTopicQuery["app"] == "app-core/netnodelite"
      || Application.command()->m_varTopicQuery["app"] == "netshareclient")
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "app-core/mydns")
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "app-core/netnodecfg")
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "app-core/netnode_dynamic_web_server_cfg")
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "app-core/netnode_dynamic_web_server")
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "app-gtech/sensible_netnode")
         return true;
	  
      if(Application.command()->m_varTopicQuery["app"] == "app-gtech/sensible_service")
         return true;
	  
	  // it may not be initialized, due
      // licensing for example
      if(!Session.is_licensed("user_presence", m_bUserPresenceFeatureRequired))
      {
         TRACE("user presence not licensed for this user");
         return false;
      }
      else
      {
         TRACE("user presence is licensed for this user");
      }


      m_spqueue->message_queue_set_timer(8888, 1000);

      if(ApplicationUser.m_ppresence == NULL)
      {
         presence * ppresence = new presence(get_app());
         ppresence->report_activity();
         ppresence->pulse_user_presence();
         ApplicationUser.m_ppresence = ppresence;
      }

      return true;

   }

   bool userpresence::defer_finalize_user_presence()
   {

      if(!is_initialized())
         return true;

      if(Application.command()->m_varTopicQuery.has_property("install")
      || Application.command()->m_varTopicQuery.has_property("uninstall"))
         return true;

      if(Application.command()->m_varTopicQuery["app"] == "simpledbcfg"
      || Application.command()->m_varTopicQuery["app"] == "app-core/netnodelite")
         return true;

      if(!is_initialized())
      {
         return true;
      }

      if(m_spqueue->message_queue_is_initialized())
      {

         m_spqueue->message_queue_del_timer(1984);

         m_spqueue->message_queue_destroy();

      }

      if(ApplicationUser.m_ppresence != NULL)
      {
         
         ::release(ApplicationUser.m_ppresence);

      }

      return true;

   }




   bool userpresence::is_initialized()
   {

      if(!m_spqueue->message_queue_is_initialized())
         return false;

      return true;

   }


   void userpresence::message_queue_message_handler(signal_details * pobj)
   {

      SCAST_PTR(::message::base, paxis, pobj);

      if(paxis->m_uiMessage == WM_TIMER)
      {

         SCAST_PTR(::message::timer, ptimer, pobj);

         if(&ApplicationUser != NULL)
         {

            presence * ppresence = ApplicationUser.m_ppresence;

            if(ptimer->m_nIDEvent == 8888 && ppresence != NULL)
            {

               ppresence->defer_pulse_user_presence();

            }

         }

      }

   }



} //namespace userpresence











