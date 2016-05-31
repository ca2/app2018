//#include "framework.h"


namespace file
{


   file_stream::file_stream()
   {

   }


   file_stream::file_stream(const char * lpszFileName,uint32_t nOpenFlags,::aura::application * papp)
   {

      open(lpszFileName,nOpenFlags,papp);

   }


   file_stream::file_stream(stream_buffer * pbuffer)
   {
      
      m_spbuffer = pbuffer;

   }


   file_stream::file_stream(const file_stream & stream):
      file_ostream(stream),
      file_istream(stream)
   {

   }


   file_stream::~file_stream()
   {

   }


} // namespace file








