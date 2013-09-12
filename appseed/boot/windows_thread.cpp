#include "framework.h"


hthread::hthread()
{

   m_pevent = new event(get_thread_app(), false, true);

   m_pthread = NULL;

}

hthread::~hthread()
{

   delete m_pevent;

}


void hthread::wait()
{

   m_pevent->wait();

}




