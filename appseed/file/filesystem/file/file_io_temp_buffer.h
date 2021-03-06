// InOutTempBuffer.h
#pragma once


/*#include "../../Common/MyCom.h"
#include "../../Windows/FileDir.h"
#include "../../Windows/FileIO.h"

#include "../IStream.h"*/

namespace file
{

   class temp_io_buffer : 
      virtual public ::object

   {
   public:
      //NWindows::NFile::NDirectory::CTempFile _tempFile;
      //NWindows::NFile::NIO::COutFile _outFile;
      ::file::file_sp                      _tempFile;
      ::file::file_sp                      _outFile;
      string                           _tempFileName;
      bool                             _tempFileCreated;
      uint32_t                           _crc;
      memory              m_memory;
      memory_position_t     _bufPos;
      memory_size_t         _size;


      temp_io_buffer();
      virtual ~temp_io_buffer();

      bool write_to_file(const void * data, memory_size_t size);
      void create();

      void InitWriting();
      bool write(const void *data, memory_size_t size);

      HRESULT write_to_stream(writer *stream);
      memory_size_t GetDataSize() const 
      { 
         return _size; 
      }
   };

   class temp_io_writer :
      virtual public writer
   {
      temp_io_buffer *_buf;
   public:
      void Init(temp_io_buffer *buffer) 
      {
         _buf = buffer; 
      }
      void write(const void *data, memory_size_t size, memory_size_t * processedSize);
   };


} // namespace file



