#pragma once


namespace file
{


   class exception;
   struct file_status;


   class CLASS_DECL_AURA binary_buffer :
      virtual public ::file::file
   {
   public:


      



      binary_buffer();
      virtual ~binary_buffer();

      virtual file_position_t get_position() const;
      virtual bool GetStatus(file_status& rStatus) const;
      virtual string GetFileName() const;
      virtual string GetFileTitle() const;
      virtual string GetFilePath() const;
      virtual void SetFilePath(const char * lpszNewName);

      
   // Operations
      virtual cres open(const ::file::path & lpszFileName,UINT nOpenFlags);

      //virtual void Rename(const char * lpszOldName, const char * lpszNewName);
      //virtual void remove(const char * lpszFileName);
      //virtual bool GetStatus(const ::file::path & lpszFileName,file_status& rStatus);
      //virtual void SetStatus(const ::file::path & * lpszFileName,const file_status& status);


   // Overridables
      virtual ::file::file_sp  Duplicate() const;

      virtual file_position_t seek(file_offset_t lOff, ::file::e_seek  nFrom);
      virtual void set_length(file_size_t dwNewLen);
      virtual file_size_t get_length() const;

      virtual void LockRange(file_position_t dwPos, file_size_t dwCount);
      virtual void UnlockRange(file_position_t dwPos, file_size_t dwCount);

      virtual void Abort();
      virtual void flush();
      virtual void close();


      // io_stream
      virtual memory_size_t read(void *lpBuf, memory_size_t nCount);
      virtual void write(const void * lpBuf, memory_size_t nCount);
      virtual string get_location() const;


      virtual bool IsOpened();

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);



      using ::file::file::write;
      void write(ostream & ostream);


      using ::file::file::read;
      void read(istream & istream);


      //virtual bool read_string(string & str);



   };


   typedef smart_pointer < binary_buffer > binary_buffer_sp;

   inline void binary_buffer::SetFilePath(const char * lpszNewName)
   {
      UNREFERENCED_PARAMETER(lpszNewName);
   }



} // namespace file






