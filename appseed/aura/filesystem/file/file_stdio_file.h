#pragma once


namespace file
{


   class CLASS_DECL_AURA stdio_file:
      virtual public ::file::file
   {
      public:


         FILE *        m_pfile;


         stdio_file();
         stdio_file(const char * lpszFileName,UINT nOpenFlags);
         virtual ~stdio_file();


#ifdef DEBUG
         virtual void assert_valid() const override;
         virtual void dump(dump_context & dumpcontext) const override;
#endif




         virtual file_position_t get_position() const;
         virtual bool GetStatus(file_status& rStatus) const;
         virtual string GetFileName() const;
         virtual string GetFileTitle() const;
         virtual string GetFilePath() const;
         //virtual void SetFilePath(const char * lpszNewName);


         virtual cres open(const ::file::path & lpszFileName,UINT nOpenFlags);

         //virtual bool GetStatus(const char * lpszFileName,file_status& rStatus);
         //virtual void SetStatus(const char * lpszFileName,const file_status& status);


         virtual ::file::file_sp  Duplicate() const;

         virtual file_position_t seek(file_offset_t lOff,::file::e_seek  nFrom);
         virtual void set_length(file_size_t dwNewLen);
         virtual file_size_t get_length() const;

         virtual void LockRange(file_position_t dwPos,file_size_t dwCount);
         virtual void UnlockRange(file_position_t dwPos,file_size_t dwCount);

         virtual void Abort();
         virtual void flush();
         virtual void close();


         virtual memory_size_t read(void *lpBuf,memory_size_t nCount);
         virtual void write(const void * lpBuf,memory_size_t nCount);
         virtual string get_location() const;

         virtual bool IsOpened();
         virtual bool is_open();
         virtual uint64_t GetBufferPtr(UINT nCommand,uint64_t nCount = 0,void ** ppBufStart = NULL,void ** ppBufMax = NULL);


         /*using ::file::writer::write;
         void write(ostream & ostream);


         using ::file::reader::read;
         void read(istream & istream);*/


         /*virtual bool read(char * pch);
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
         virtual bool full_read_string(string & str);*/


         //void writef(const char *format,...);

         /** Common interface for write used by Tcp and Udp sockets. */
         //      virtual void write(const void *, memory_size_t c);

         /** Common interface for Send used by Tcp and Udp sockets. */
         //virtual void write(const string &);



   };



} // namespace file






