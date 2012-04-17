#include "StdAfx.h"


namespace planebase
{


   application::application()
   {
      m_pservice = NULL;
   }


   application::~application()
   {
   }


   bool application::is_serviceable()
   {

      return false;

   }


   bool application::create_service()
   {

      return System.os().create_service(this);

   }


   bool application::remove_service()
   {

      return System.os().create_service(this);

   }


   bool application::start_service()
   {

      if(m_strAppName.is_empty()
      || m_strAppName.CompareNoCase("bergedge") == 0
      || !is_serviceable())
         return false;

      SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

      if(hdlSCM == 0)
      {
         //::GetLastError();
         return false;
      }

      SC_HANDLE hdlServ = ::OpenService(
         hdlSCM,                    // SCManager database
         "TBSCSTvotagusCa2FontopusMain-" + m_strAppName,               // name of service
         SERVICE_START);                     // no password


      if (!hdlServ)
      {
         CloseServiceHandle(hdlSCM);
         //Ret = ::GetLastError();
         return FALSE;
      }

      BOOL bOk = StartService(hdlServ, 0, NULL);

      CloseServiceHandle(hdlServ);
      CloseServiceHandle(hdlSCM);

      return bOk != FALSE;
   }

   bool application::stop_service()
   {

      if(m_strAppName.is_empty()
      || m_strAppName.CompareNoCase("bergedge") == 0
      || !is_serviceable())
         return false;

      SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

      if(hdlSCM == 0)
      {
         //::GetLastError();
         return false;
      }

      SC_HANDLE hdlServ = ::OpenService(
         hdlSCM,                    // SCManager database
         "TBSCSTvotagusCa2FontopusMain-" + m_strAppName,               // name of service
         SERVICE_STOP);                     // no password

      if (!hdlServ)
      {
         // Ret = ::GetLastError();
         CloseServiceHandle(hdlSCM);
         return false;
      }

      SERVICE_STATUS ss;

      memset(&ss, 0, sizeof(ss));

      BOOL bOk = ::ControlService(hdlServ, SERVICE_CONTROL_STOP, &ss);

      ::DeleteService(hdlServ);

      CloseServiceHandle(hdlServ);

      CloseServiceHandle(hdlSCM);

      return bOk != FALSE;
   }


   BOOL application::run()
   {
      if (command().m_varTopicQuery.has_property("service"))
      {
         create_new_service();
         service_base::run(*m_pservice);
      }
      else if(command().m_varTopicQuery.has_property("run") || is_serviceable())
      {
         create_new_service();
         m_pservice->Start(0);
         return fontopus::application::run();
      }
      else
      {
         return fontopus::application::run();
      }
      return TRUE;
   }

   bool application::on_install()
   {
      if(is_serviceable())
      {
         create_service();
         start_service();
      }
      return ::fontopus::application::on_install();
   }

   bool application::on_uninstall()
   {
      if(is_serviceable())
      {
         stop_service();
         remove_service();
      }
      return ::fontopus::application::on_uninstall();
   }


   service_base * application::get_service()
   {
      return m_pservice;
   }


   service_base * application::allocate_new_service()
   {
      if(!is_serviceable())
         throw "only a serviceable application should/can allocate a new service";
      throw "a serviceable application should implement this function returning a newly allocated application service";
   }


   bool application::create_new_service()
   {

      if(m_pservice != NULL)
         return false;

      m_pservice = allocate_new_service();

      if(m_pservice == NULL)
         return false;

      return true;

   }

   void application::on_service_request(::ca::create_context * pcreatecontext)
   {

      if(!is_serviceable())
         return;

      if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("create_service"))
      {
         create_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("start_service"))
      {
         start_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("stop_service"))
      {
         stop_service();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove_service"))
      {
         remove_service();
      }

   }


} //namespace fontopus


