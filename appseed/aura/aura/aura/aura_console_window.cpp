#include "framework.h"
#include <stdio.h>


void std_out_buffer::write(const void * lpBuf,memory_size_t nCount)
{

   DWORD dw;

#ifdef WINDOWS

   WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), lpBuf, (DWORD) nCount, &dw, NULL);

#else

   fwrite(lpBuf, nCount, 1, stdout);

#endif

}



namespace console
{


   void window::redirect_io()
   {

   }


} // namespace console


