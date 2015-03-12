#pragma once


namespace file
{


   class CLASS_DECL_AURA plain_text_stream :
      virtual public plain_text_ostream,
      virtual public plain_text_istream,
      virtual public stream
   {
   public:

      
      plain_text_stream();
      plain_text_stream(stream_buffer * pbuffer);
      plain_text_stream(const stream & stream);
      virtual ~plain_text_stream();

      
      stream & operator = (const stream & stream);

      virtual bool is_stream_null();
      virtual bool is_stream_set();

      virtual void close();



   };


} // namespace file




using std_string_stream = ::file::plain_text_stream;