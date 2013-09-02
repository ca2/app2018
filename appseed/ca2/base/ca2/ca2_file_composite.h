#pragma once


namespace ca2
{


   class CLASS_DECL_ca2 file_composite :
      virtual public ::file::file
   {
   public:



      ::ca2::filesp     m_spfile;


      file_composite();
      file_composite(sp(::file::file) pfile);
      virtual ~file_composite();

      virtual file_position get_position() const;
      virtual bool GetStatus(file_status& rStatus) const;
      virtual string GetFileName() const;
      virtual string GetFileTitle() const;
      virtual string GetFilePath() const;
      virtual void SetFilePath(const char * lpszNewName);


      virtual bool open(const char * lpszFileName, UINT nOpenFlags);

      //virtual void Rename(const char * lpszOldName, const char * lpszNewName);
      //virtual void remove(const char * lpszFileName);
      virtual bool GetStatus(const char * lpszFileName, file_status& rStatus);
      virtual void SetStatus(const char * lpszFileName, const file_status& status);


      virtual sp(::file::file) Duplicate() const;

      virtual file_position seek(file_offset lOff, ::file::e_seek  nFrom);
      virtual void set_length(file_size dwNewLen);
      virtual file_size get_length() const;

      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);

      virtual void Abort();
      virtual void flush();
      virtual void close();


      virtual ::primitive::memory_size read(void *lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);
      virtual string get_location() const;

      virtual bool read(char * pch);
      virtual bool read(uchar * pch);

      virtual bool read(char & pch);
      virtual bool read(uchar & pch);

      virtual bool peek(char * pch);
      virtual bool peek(uchar * pch);

      virtual bool peek(char & pch);
      virtual bool peek(uchar & pch);

      virtual bool IsOpened();

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);



      using ::file::reader::write;
      using ::file::writer::write;
      void write(byte_output_stream & ostream);


      using ::file::writer::read;
      using ::file::reader::read;
      void read(byte_input_stream & istream);



   };


} // namespace ca2


