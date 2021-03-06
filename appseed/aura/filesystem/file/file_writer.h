#pragma once


#include "file_file_base.h"


namespace file
{

   class reader;

   class writer;

   typedef smart_pointer < writer > writer_sp;


   class CLASS_DECL_AURA writer :
      virtual public file_base
   {
   public:


      writer();
      virtual ~writer();


      virtual void write(const void *lpBuf, memory_size_t nCount);
      virtual void write(const void *lpBuf, memory_size_t nCount, memory_size_t * dwWritten);

      virtual void transfer_from(reader & reader, memory_size_t uiBufferSize = 16 * 1024);
      virtual void transfer_from_begin(reader & reader, memory_size_t uiBufSize = 16 * 1024);

      virtual void close();

      virtual void flush();


   };


   typedef writer writable;


   CLASS_DECL_AURA HRESULT write(writer * pwriter, const void * data, memory_size_t size);



} // namespace file




