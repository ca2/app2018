#include "framework.h"
//#include "base/user/user.h"


namespace userpresence
{


   userpresence::userpresence(::aura::application * papp):
      ::object(papp),
      ::aura::department(papp),
      ::aura::timer_array(papp)
   {

      m_bUserPresenceFeatureRequired = false;


      m_bInit = false;

   }


   userpresence::~userpresence()
   {

   }


   bool userpresence::initialize()
   {


      if(!(bool)System.oprop("do_not_initialize_user_presence"))
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
      delete_all_timers();

      return true;

      if(!is_initialized())
      {

         //if(m_spuiMessage.is_null())
         {
            // m_spuiMessage = canew(::user::interaction());
         }

         //         if(!m_spqueue->create_message_queue("ca5::user::userpresence::message_queue"))
         //          return false;

      }


      string strQuery = Application.handler()->m_varTopicQuery["app"];

      if(Application.handler()->m_varTopicQuery.has_property("install")
         || Application.handler()->m_varTopicQuery.has_property("uninstall"))
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "simpledbcfg"
         || Application.handler()->m_varTopicQuery["app"] == "app-core/netnodelite"
         || Application.handler()->m_varTopicQuery["app"] == "netshareclient")
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "app-core/mydns")
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "app-core/netnodecfg")
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "app-core/netnode_dynamic_web_server_cfg")
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "app-core/netnode_dynamic_web_server")
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "app-gtech/sensible_netnode")
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "app-gtech/sensible_service")
         return true;

      // it may not be initialized, due
      // licensing for example
      if(!Session.is_licensed("user_presence",m_bUserPresenceFeatureRequired))
      {
         TRACE("user presence not licensed for this user");
         return false;
      }
      else
      {
         TRACE("user presence is licensed for this user");
      }


      create_timer(8888,1000, NULL);

      if(ApplicationUser.m_ppresence == NULL)
      {
         presence * ppresence = new presence(get_app());
         ppresence->report_activity();
         ppresence->pulse_user_presence();
         ApplicationUser.m_ppresence = ppresence;
      }

      m_bInit = true;

      return true;

   }

   bool userpresence::defer_finalize_user_presence()
   {

      if(!is_initialized())
         return true;

      if(Application.handler()->m_varTopicQuery.has_property("install")
         || Application.handler()->m_varTopicQuery.has_property("uninstall"))
         return true;

      if(Application.handler()->m_varTopicQuery["app"] == "simpledbcfg"
         || Application.handler()->m_varTopicQuery["app"] == "app-core/netnodelite")
         return true;

      if(!is_initialized())
      {
         return true;
      }


      delete_timer(1984);

      delete_timer(8888);

      if(ApplicationUser.m_ppresence != NULL)
      {

         ::release(ApplicationUser.m_ppresence);

      }

      m_bInit = false;

      return true;

   }




   bool userpresence::is_initialized()
   {

      return m_bInit;

   }

   void userpresence::_001OnTimer(::timer * ptimer)
   {
      if(&ApplicationUser != NULL)
      {

         presence * ppresence = ApplicationUser.m_ppresence;

         if(ptimer->m_nIDEvent == 8888 && ppresence != NULL)
         {

            ppresence->defer_pulse_user_presence();

         }

      }

   }

   void userpresence::message_queue_message_handler(::message::message * pobj)
   {
      
      UNREFERENCED_PARAMETER(
                             pobj);

      //SCAST_PTR(::message::base, paxis, pobj);


   }



} //namespace userpresence











