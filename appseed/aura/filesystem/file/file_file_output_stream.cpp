//#include "framework.h"



namespace file
{


   file_output_stream::file_output_stream()
   {

   }


   file_output_stream::file_output_stream(const char * lpszFileName,uint32_t nOpenFlags,::aura::application * papp)
   {

      open(lpszFileName,nOpenFlags,papp);

   }


   file_output_stream::file_output_stream(stream_buffer * preader)
   {

      m_spbuffer = preader;

   }

   file_output_stream::file_output_stream(const file_output_stream & istream)
   {

      m_spbuffer = istream.m_spbuffer;

   }

   file_output_stream::~file_output_stream()
   {


   }


} // namespace file









