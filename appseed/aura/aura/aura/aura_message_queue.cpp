#include "framework.h"


message_queue::message_queue(sp(::aura::application) papp) :
   element(papp)
{

   m_plistener          = NULL;

}

message_queue::~message_queue()
{

}


bool message_queue::create_message_queue(const char * pszName, ::message_queue_listener * plistener)
{

   m_plistener = plistener;

   return true;

}


void message_queue::message_handler(signal_details * pobj)
{
   
   message_queue_message_handler(pobj);

   if(pobj->m_bRet)
      return;

}


void message_queue::message_queue_message_handler(signal_details * pobj)
{

   if(m_plistener != NULL)
   {

      m_plistener->message_queue_message_handler(pobj);

   }

}


bool message_queue::is_initialized()
{

   return false;

}


bool message_queue::message_queue_set_timer(uint_ptr uiId,DWORD dwMillis)
{

   throw interface_only_exception(get_app());

   return false;

}