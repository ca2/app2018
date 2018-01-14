#pragma once

CLASS_DECL_AURA int get_sync_io_error();
CLASS_DECL_AURA void set_sync_io_error(int iError);
CLASS_DECL_AURA int get_generate_sync_io_error();
CLASS_DECL_AURA void set_generate_sync_io_error(int iError);


class CLASS_DECL_AURA sync_io_error
{
   public:


      int   m_iGenerateBefore;
      int   m_iErrorBefore;
      int * m_piError;


      sync_io_error(int * piError = NULL)
      {
         m_iGenerateBefore = get_generate_sync_io_error();
         m_iErrorBefore = get_sync_io_error();
         set_sync_io_error(0);
         set_generate_sync_io_error(1);
         m_piError = piError;
      }

      ~sync_io_error()
      {
         if(m_piError != NULL)
         {
            *m_piError = get_sync_io_error();
         }
         set_generate_sync_io_error(m_iGenerateBefore);
         set_sync_io_error(m_iErrorBefore);
      }

      bool none() const { return get_sync_io_error() == 0; }


      operator bool() const { return get_sync_io_error() != 0; }

};

namespace file
{

   enum e_filestate
   {

      filestate_read_line_truncated = 1,

   };


   class CLASS_DECL_AURA file :
      virtual public ::file::reader,
      virtual public ::file::writer
   {
      public:


         ::file::path               m_strFileName;
         cflag < e_filestate >      m_efilestate;
         DWORD                      m_dwErrorBlockTimeout;

         file();
         virtual ~file();


         virtual void assert_valid() const override;
         virtual void dump(dump_context & dumpcontext) const override;



         virtual file_position_t get_position() const override;
         virtual file_position_t tell() const;
         virtual bool GetStatus(file_status& rStatus) const;
         virtual string GetFileName() const;
         virtual string GetFileTitle() const;
         virtual string GetFilePath() const;
         virtual void SetFilePath(const char * lpszNewName);


         virtual cres open(const ::file::path & lpszFileName,UINT nOpenFlags);

         //virtual bool GetStatus(const char * lpszFileName, file_status& rStatus);
         //virtual void SetStatus(const char * lpszFileName, const file_status& status);


         virtual ::file::file_sp  Duplicate() const;

         virtual file_position_t seek(file_offset_t lOff, ::file::e_seek  nFrom) override;
         virtual file_position_t seek_from_begin(file_position_t lPos = 0) override;
         virtual file_position_t seek_to_begin(file_position_t lPos = 0);
         virtual file_position_t seek_begin(file_position_t lPos = 0);
         virtual void set_length(file_size_t dwNewLen);
         virtual file_size_t get_length() const;

         virtual void LockRange(file_position_t dwPos, file_size_t dwCount);
         virtual void UnlockRange(file_position_t dwPos, file_size_t dwCount);

         virtual void Abort();
         virtual void flush() override;
         virtual void close() override;


         virtual memory_size_t read(void *lpBuf, memory_size_t nCount) override;
         virtual bool full_read(void *lpBuf, memory_size_t nCount);
         virtual void write_from_hex(const void * lpBuf,memory_size_t nCount);
         virtual void write(const void * lpBuf, memory_size_t nCount) override;
         virtual string get_location() const;


         virtual bool IsOpened();
         virtual bool is_open();
         virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);


         using ::file::writer::write;
         void write(ostream & ostream);


         using ::file::reader::read;
         void read(istream & istream);


         virtual bool read(char * pch);
         virtual bool read(uchar * puch);
         virtual bool peek(char * pch);
         virtual bool peek(uchar * puch);
         virtual bool read(char & ch);
         virtual bool read(uchar & uch);
         virtual bool peek(char & ch);
         virtual bool peek(uchar & uch);
         virtual int sgetc();
         virtual int sbumpc();
         virtual bool read_string(string & str);
         virtual bool read_string(::primitive::memory_base & mem);
         virtual bool full_read_string(string & str);

         virtual void write_string(const string & str);


         void writef(const char *format, ...);

         /** Common interface for write used by Tcp and Udp sockets. */
//      virtual void write(const void *, memory_size_t c);

         /** Common interface for Send used by Tcp and Udp sockets. */
         virtual void write(const string &);

         void copy_this(const file & file);

         file & operator = (const file & file);

   };



} // namespace file






