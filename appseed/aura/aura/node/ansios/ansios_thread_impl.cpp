//#include "framework.h"
//#include "ansios.h"
//#include <fcntl.h>
//#include <sys/time.h>

namespace ansios
{


   thread_impl::thread_impl(::aura::application * papp) :
   element(papp),
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


} // namespace macos










void __end_thread_impl(aura::application*, unsigned int, bool)

{

}


void __term_thread_impl(aura::application*, HINSTANCE__*)
{
}











void CLASS_DECL_AURA __init_thread()
{

}






void CLASS_DECL_AURA __term_thread()
{

}






