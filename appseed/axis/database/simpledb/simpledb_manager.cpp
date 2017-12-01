﻿#include "framework.h"
#include "simpledb_socket.h"




namespace simpledb
{


   manager::manager(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      m_mutexSession(papp),
      m_mutexTagId(papp),
      m_mutexTagName(papp)
   {
      m_dwBuildTimeWindow = 770;
      m_dwBuildTimeRandomWindow = 840;

      m_iDatabaseWaitTimeOut = 1000 * 1000 * 60;

      begin();

   }


   manager::~manager()
   {

   }


   bool manager::init_thread()
   {

      return true;

   }


   bool manager::handle(socket * psocket)
   {
      memory memory;
      if(!get_output_internal(psocket->inattr("request_uri"), memory))
      {
         string strError;
         strError = "Error processing command";
         psocket->response().file().write(strError, strError.get_length());
         return false;
      }
      psocket->response().file().write(memory.get_data(), memory.get_size());
      psocket->outheader(__id(content_type)) = "application/x-core-var";
      return true;
   }

   bool manager::get_output_internal(const char * pszCommand, memory & memory)
   {
      UNREFERENCED_PARAMETER(pszCommand);
      UNREFERENCED_PARAMETER(memory);
      return true;
   }



   void manager::message_queue_message_handler(::message::message * pobj)
   {
      SCAST_PTR(::message::base, pbase, pobj);
      if(pbase->m_id == WM_APP + 13)
      {
         //if(wparam == 0)
         // ((script *) lparam)->Load(false);
         //else if(wparam == 13)
         // ((script *) lparam)->Unload(false);
         pbase->m_bRet = true;
      }
      else if(pbase->m_id == WM_APP + 14)
      {
      }
   }





   session * & manager::get_session(const char * pszId)
   {
      single_lock sl(&m_mutexSession, TRUE);
      ::simpledb::session * & psession = m_mapSession[pszId];
      return psession;
   }



} // namespace simpledb



