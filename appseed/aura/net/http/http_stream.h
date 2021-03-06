#pragma once


namespace http
{


   class CLASS_DECL_AURA stream :
      virtual public ::file::plain_text_stream
   {
   public:


      stream();
      stream(::file::file * pbuffer);
      virtual ~stream();


      using ::file::plain_text_stream::write;
      virtual void write (const string & str);


      using ::file::plain_text_stream::operator<<;
      stream & operator << (::memory_file & memfile);
      stream & operator << (::file::file_sp filesp);


   };


} // namespace http



