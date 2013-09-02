#pragma once


namespace file
{

   class writer;
   class reader;


   typedef smart_pointer < reader > reader_sp;


   class CLASS_DECL_c reader :
      virtual public seekable
   {
   public:


      ::file::reader_sp       m_spreader;


      reader();
      reader(reader * preader);
      reader(const reader & preader);
      virtual ~reader();


      virtual ::primitive::memory_size read(void *lpBuf, ::primitive::memory_size nCount);
      virtual file_position find(const void * pFind, ::primitive::memory_size size, const file_position * limit);

      virtual void write(writer & writer);

      reader & operator = (const reader & reader);

      virtual bool is_reader_null();
      virtual bool is_reader_set();

      virtual void close();

      virtual void to_hex(string & str, ::primitive::memory_position iStart = 0, ::primitive::memory_position iEnd = -1);

   };


   typedef reader readable;


} // namespace file







