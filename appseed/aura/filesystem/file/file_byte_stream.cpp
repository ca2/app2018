//#include "framework.h"


namespace file
{


   byte_stream::byte_stream()
   {

   }


   byte_stream::byte_stream(file * pbuffer) :
      stream(pbuffer)
   {

   }


   byte_stream::byte_stream(const stream & strm) :
      stream(strm)
   {

   }


   byte_stream::~byte_stream()
   {

   }


   stream & byte_stream::operator = (const stream & stream)
   {

      stream::operator = (stream);

      return *this;

   }


   bool byte_stream::is_stream_null()
   {

      return is_writer_null() && is_reader_null();

   }


   bool byte_stream::is_stream_set()
   {

      return is_writer_set() || is_reader_set();

   }


   void byte_stream::close()
   {

      istream::close();

      ostream::close();

   }


   file_position_t byte_stream::get_position() const
   {

      return m_spfile->get_position();

   }


} // namespace file




