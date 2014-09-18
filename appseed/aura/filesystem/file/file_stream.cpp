#include "framework.h"


namespace file
{


   stream::stream()
   {
   }

   stream::stream(sp(stream_buffer) pbuffer) :
      stream_base(pbuffer)
   {
   }

   stream::stream(const stream & stream) :
      stream_base(stream)
   {
   }

   stream::~stream()
   {
   }

   void stream::open(const char * lpszFileName,uint32_t nOpenFlags,sp(::aura::application) papp = ::get_thread_app())
   {

      if(m_spbuffer.is_null())
      {

         m_spbuffer = App(papp).alloc(System.type_info < file::stdio_fle >());

         if(m_spbuffer.is_null())
         {

            setf(failbit);

            return;

         }

      }

      if(!m_spbuffer->open(lpszFileName,nOpenFlags))
      {
         
         setf(failbit);

         return;

      }

   }

   stream & stream::operator = (const stream & stream)
   {
      input_stream::operator = (stream);
      output_stream::operator = (stream);
      return *this;
   }

   bool stream::is_stream_null()
   {
      return is_writer_null() && is_reader_null();
   }

   bool stream::is_stream_set()
   {
      return is_writer_set() || is_reader_set();
   }

   void stream::close()
   {
      input_stream::close();
      output_stream::close();
   }


   void * stream::get_internal_data()
   {
      
      return m_spbuffer->get_internal_data();

   }


   ::primitive::memory_size stream::get_internal_data_size()
   {
      
      return m_spbuffer->get_internal_data_size();

   }


   bool stream::set_internal_data_size(::primitive::memory_size c)
   {
      
      return m_spbuffer->set_internal_data_size(c);

   }


   file_position stream::get_position() const
   {
      
      return m_spbuffer->get_position();

   }


} // namespace file


