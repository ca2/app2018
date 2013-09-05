#pragma once


namespace file
{


   class output_stream;

   typedef smart_pointer < output_stream > output_stream_sp;


   class CLASS_DECL_c output_stream
   {
   public:

      
      writer_sp         m_spwriter;
      writer_flush_sp   m_spflush;
   
      
      output_stream();
      output_stream(writer * pwriter);
      output_stream(writer & writer);
      output_stream(const output_stream & pwriter);
      virtual ~output_stream();

      
      virtual void write(const void *lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void *lpBuf, ::primitive::memory_size nCount, ::primitive::memory_size * dwWritten);

      //virtual void read(reader & reader);

      output_stream & operator = (const output_stream & writer);

      virtual bool is_writer_null();
      virtual bool is_writer_set();

      virtual void close();

      virtual void flush();

      inline output_stream & operator << (char ch                              ) { write(ch              ); return *this;}
      inline output_stream & operator << (uchar uch                            ) { write(uch             ); return *this;}
      inline output_stream & operator << (int16_t sh                           ) { write(sh              ); return *this;}
      inline output_stream & operator << (uint16_t ui                          ) { write(ui              ); return *this;}
      inline output_stream & operator << (wchar_t wch                          ) { write(wch             ); return *this;}
      inline output_stream & operator << (bool b                               ) { write(b               ); return *this;}
      inline output_stream & operator << (int32_t i                            ) { write(i               ); return *this;}
      inline output_stream & operator << (uint32_t ui                          ) { write(ui              ); return *this;}
      inline output_stream & operator << (int64_t i                            ) { write(i               ); return *this;}
      inline output_stream & operator << (uint64_t ui                          ) { write(ui              ); return *this;}
      inline output_stream & operator << (float f                              ) { write(f               ); return *this;}
      inline output_stream & operator << (double d                             ) { write(d               ); return *this;}
      inline output_stream & operator << (LPCRECT lpcrect                      ) { write(lpcrect         ); return *this;}
      inline output_stream & operator << (SIZE & size                          ) { write(size            ); return *this;}
      inline output_stream & operator << (sp(type) info                        ) { write(info            ); return *this;}
      inline output_stream & operator << (serializable & serializable          ) { write(serializable    ); return *this;}
      inline output_stream & operator << (const char * psz                     ) { write(psz             ); return *this;}
      inline output_stream & operator << (const id & id                        ) { write(id              ); return *this;}
      inline output_stream & operator << (const var & var                      ) { write(var             ); return *this;}
      inline output_stream & operator << (const string & str                   ) { write(str             ); return *this;}


      void write_arbitrary(int32_t i);
      void write_arbitrary(uint32_t ui);
      void write_arbitrary(int64_t i);
      void write_arbitrary(uint64_t ui);
      void write_arbitrary(uint64_t ui, int signal);



      virtual void write (char ch);
      virtual void write (uchar uch);
      virtual void write (int16_t sh);
      virtual void write (uint16_t uint16_t);
      virtual void write (wchar_t wch);
      virtual void write (bool b);
      virtual void write (int32_t i);
      virtual void write (uint32_t ui);
      virtual void write (int64_t i);
      virtual void write (uint64_t ui);
      virtual void write (float f);
      virtual void write (double d);
      virtual void write (LPCRECT lpcrect);
      virtual void write (SIZE & size);
      virtual void write (sp(type) info);
      virtual void write (serializable & serializable);
      virtual void write (const char * psz);
      virtual void write (const id & id);
      virtual void write (const var & var);
      virtual void write (const string & str);


   };


} // namespace file




