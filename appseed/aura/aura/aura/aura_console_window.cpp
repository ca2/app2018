#include "framework.h"


void std_out_buffer::write(const void * lpBuf,memory_size_t nCount)
{

#ifdef METROWIN

   throw todo(get_app());

#elif defined(WINDOWS)

   write_memory_to_file(GetStdHandle(STD_OUTPUT_HANDLE),lpBuf,nCount,NULL);

#else

   fwrite(lpBuf,1,nCount,stdout);

#endif

}



namespace console
{


   void window::redirect_io()
   {

   }


} // namespace console


