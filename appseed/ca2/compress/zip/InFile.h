#pragma once


#if defined(LINUX) || defined(ANDROID)


namespace ca2
{


    class file;


} // namespace ca2



typedef ::file::file * HFILE;


#endif


namespace zip
{

   class File;

   class CLASS_DECL_ca2 InFile :
      public ::file::file
   {
   public:

      enum e_mode
      {
         mode_undefined,
         mode_zip,
         mode_unzip,
      };

      unz_file_info                       m_fi;
      bool                                m_bCloseOnDelete;
      string                              m_strFileName;
      uint64_t                            m_iPosition;
      stringa                             m_straPath;
      spa(File)                           m_filea;
      spa(InFile)                         m_izfilea;
      stringa                             m_straPrefix;
      e_mode                              m_emode;
      string                              m_strZipFile;


      InFile(sp(::application) papp);
      virtual ~InFile();



      operator HFILE() const;

      virtual file_position get_position() const;

      virtual bool zip_open(const char *,UINT);
      virtual bool zip_open(File * pzfile, const char * lpcszFileName);

      virtual bool unzip_open(sp(::file::file) pfile);
      virtual bool unzip_open(const char *,UINT);
      virtual bool unzip_open(File * pzfile, const char * lpcszFileName);

      virtual bool locate(const char * pszFileName);

      virtual void add_file(const char * pszDir, const char * pszRelative);

      bool dump(sp(::file::file) pfile);

      virtual sp(::file::file) Duplicate() const;

      File * get_zip_file();
      const File * get_zip_file() const;

      virtual file_position seek(file_offset lOff, ::file::e_seek  nFrom);
      virtual void set_length(file_size dwNewLen);
      virtual file_size get_length() const;

      virtual ::primitive::memory_size read(void * lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);

      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);

      virtual void Abort();
      virtual void flush();
      virtual void close();

   // Implementation
   public:
      virtual bool IsOpened();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
      virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);


   private:
      using ::file::file::open;

   };

} // namespace zip

