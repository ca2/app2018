#pragma once


class FileException;
struct FileStatus;

void CLASS_DECL_AURA vfxGetRoot(const wchar_t * lpszPath, string& wstrRoot);

namespace macos
{

   /////////////////////////////////////////////////////////////////////////////
   // File - raw unbuffered disk file I/O

   class CLASS_DECL_AURA file :
      virtual public ::file::binary_buffer
   {
   public:

      enum Attribute
      {
         normal =    0x00,
         readOnly =  0x01,
         hidden =    0x02,
         system =    0x04,
         volume =    0x08,
         directory = 0x10,
         archive =   0x20
      };
      
      enum
      {
         
         hFileNull = -1
         
      };
      
      enum BufferCommand
      {
         
         bufferRead,
         bufferWrite,
         bufferCommit,
         bufferCheck
         
      };
      
      string         m_strFileName;
      int32_t        m_iFile;
      
      
      file(::aura::application * papp);
      file(::aura::application * papp, int32_t hFile);
      file(::aura::application * papp, const char * lpszFileName, UINT nOpenFlags);
      virtual ~file();
      
      
      virtual file_position get_position() const;
      
      
      bool GetStatus(::file::file_status & rStatus) const;
      virtual string GetFileName() const;
      virtual string GetFileTitle() const;
      virtual string GetFilePath() const;
      virtual void SetFilePath(const char * lpszNewName);
      
      virtual cres open(const ::file::path & lpszFileName, UINT nOpenFlags);
      
      virtual bool PASCAL GetStatus(const char * lpszFileName, ::file::file_status& rStatus);
      
      uint64_t ReadHuge(void * lpBuffer, uint64_t dwCount);
      void WriteHuge(const void * lpBuffer, uint64_t dwCount);
      
      virtual sp(::file::stream_buffer) Duplicate() const;
      
      virtual file_position seek(file_offset lOff, ::file::e_seek nFrom);
      virtual void set_length(file_size dwNewLen);
      virtual file_size get_length() const;
      
      virtual ::primitive::memory_size read(void * lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);
      
      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);
      
      virtual void Abort();
      virtual void Flush();
      virtual void close();
      
      virtual bool IsOpened();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      
      virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);
      
      
   };


   namespace file_exception
   {
      
      
      ::file::exception::e_cause OsErrorToException(LONG lOsError);
      ::file::exception::e_cause ErrnoToException(int32_t nErrno);
      void ThrowOsError(::aura::application * papp, LONG lOsError, const char * lpszFileName = NULL);
      void ThrowErrno(::aura::application * papp, int32_t nErrno, const char * lpszFileName = NULL);
      
      
   }  // namespace file_exception



} // namespace macos
