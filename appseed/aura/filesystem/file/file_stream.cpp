//#include "framework.h"


namespace file
{


   stream::stream()
   {

   }


   stream::stream(buffer_sp pbuffer):
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


   stream & stream::operator = (const stream & stream)
   {
      istream::operator = (stream);
      ostream::operator = (stream);
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
      istream::close();
      ostream::close();
   }


   void * stream::get_internal_data()
   {
      
      return m_spbuffer->get_internal_data();

   }


   ::primitive::memory_size stream::get_internal_data_size() const
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


