//
//  ansios_thread_impl.cpp
//  base
//
//  Created by Thomas Borregaard Sørensen on 21/05/14.
//
//

#include "framework.h"
#include <fcntl.h>


namespace ansios
{
   
   
   thread_impl::thread_impl(sp(::base::application) papp) :
   element(papp),
   message_queue(papp),
   ::thread(NULL),
   ::thread_impl(papp)
   {
      
      
   }
   
   
   
   
   
   
   thread_impl::~thread_impl()
   {
      
      
   }
   
   
   
   
   
	unsigned long MillisecondCounter ()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
   
   
} // namespace mac










void __end_thread_impl(base::application*, unsigned int, bool)

{
   
}


void __term_thread_impl(base::application*, HINSTANCE__*)
{
}

