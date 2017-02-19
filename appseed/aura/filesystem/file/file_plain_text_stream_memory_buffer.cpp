//#include "framework.h"


namespace file
{


   plain_text_stream_memory_buffer::plain_text_stream_memory_buffer(::aura::application * papp)
   {

      m_spmemorybuffer = canew(memory_buffer(papp));

      m_spfile = m_spmemorybuffer;

   }


   plain_text_stream_memory_buffer::plain_text_stream_memory_buffer(::primitive::memory_base * pbase)
   {

      m_spmemorybuffer = canew(memory_buffer(pbase->get_app(), pbase));

      m_spfile = m_spmemorybuffer;
      

   }


   plain_text_stream_memory_buffer::plain_text_stream_memory_buffer(::aura::application * papp, const void * data, memory_size_t c)
   {

      m_spmemorybuffer = canew(memory_buffer(papp, (void *) data, c));

      m_spfile = m_spmemorybuffer;

   }


   plain_text_stream_memory_buffer::~plain_text_stream_memory_buffer()
   {

   }


   file_size_t plain_text_stream_memory_buffer::get_length()
   {
      
      return m_spmemorybuffer->get_size();

   }


   void * plain_text_stream_memory_buffer::get_data()
   {
      
      return m_spmemorybuffer->get_data();

   }

   string plain_text_stream_memory_buffer::to_string()
   {
      
      return m_spmemorybuffer->str();

   }

} // namespace file




