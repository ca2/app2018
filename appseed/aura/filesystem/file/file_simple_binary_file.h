#pragma once


namespace file
{


   class exception;
   struct file_status;


   class CLASS_DECL_AURA simple_binary_file :
      virtual public ::file::binary_file
   {
      public:


         FILE *         m_pfile;
         string       m_strFileName;


         simple_binary_file(::aura::application * papp);
         virtual ~simple_binary_file();


#ifdef DEBUG
         virtual void assert_valid() const override;
         virtual void dump(dump_context & dumpcontext) const override;
#endif



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
         //virtual bool GetStatus(const char * lpszFileName, file_status& rStatus);
         //virtual void SetStatus(const char * lpszFileName, const file_status& status);


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

         //virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);



         using ::file::binary_file::write;
         void write(ostream & ostream);


         using ::file::binary_file::read;
         void read(istream & istream);


         //virtual bool read_string(string & str);

         virtual void write_string(const char * lpsz);
         virtual LPTSTR read_string(LPTSTR lpsz, UINT nMax);
         virtual bool read_string(string & rString);

   };


   void CLASS_DECL_AURA throw_exception(::aura::application * papp, ::file::exception::e_cause cause,LONG lOsError,const char * lpszFileName = NULL);

} // namespace file






