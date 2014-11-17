#include "framework.h"

/*
class sys_message_queue
{
public:


   event                     m_event;
   mutex                     m_mutex;
   ptr_array < MESSAGE >           m_msgptra;


   int_bool GetMessage(MESSAGE * pmsg);


};




void _c_simple_message_loop()
{


   MESSAGE msg;

	while(GetMessage(&msg))
	{

		TranslateMessage(&msg);

		DispatchMessage(&msg);

	}


}




int_bool sys_message_queue::GetMessage(MESSAGE * pmsg)
{
   synch_lock lockMutex(m_mutex, false);
   while(true)
   {
      lockMutex.lock();
      if(m_msgptra.get_count() <= 0)
      {
         lockMutex.unlock();
         m_event.wait();
      }
      else
      {
         *pmsg = *m_msgptra[0];
         m_msgptra.remove_at(0);
         if(pmsg->message == 0xffff)
         {
            return FALSE;
         }
         else
         {
            return TRUE;
         }

      }
   }
}


sys_thread::sys_thread(pthread_t pthread) :
   m_pthread(pthread)
{
}

int_bool sys_thread::GetMessage(MESSAGE * pmsg)
{
      return m_messagequeue.GetMessage(pmsg);
}







sys_thread * sys_thread_pool::get_current()
{
   return get(pthread_self());
}

sys_thread * sys_thread_pool::get(pthread_t pthread)
{


   synch_lock lockMutex(m_mutex, false);


   for(int32_t i = 0; i < m_threadptra.get_count(); i++)
   {
      if(m_threadptra[i]->m_pthread == pthread)
         return m_threadptra[i];

   }

   m_threadptra.add(new sys_thread(pthread));

   return m_threadptra[m_threadptra.get_count() - 1];
}


*/


bool TranslateMessage(LPMESSAGE lpmessage)
{

   UNREFERENCED_PARAMETER(lpmessage);

   return true;

}


LRESULT DispatchMessage(LPMESSAGE lpmessage)
{

   if(lpmessage->hwnd == NULL)
      return false;

   return lpmessage->hwnd->get_user_interaction()->message_handler(lpmessage);

}



void _c_simple_message_loop()
{

   MESSAGE msg;

	while(GetMessage(&msg, NULL, 0, 0))
	{

		TranslateMessage(&msg);

		DispatchMessage(&msg);

	}


}


