// InOutTempBuffer.h
#pragma once


/*#include "../../Common/MyCom.h"
#include "../../Windows/FileDir.h"
#include "../../Windows/FileIO.h"

#include "../IStream.h"*/

namespace ca2
{

   class temp_io_buffer : 
      virtual public ::ca2::object

   {
   public:
      //NWindows::NFile::NDirectory::CTempFile _tempFile;
      //NWindows::NFile::NIO::COutFile _outFile;
      ::ca2::filesp                      _tempFile;
      ::ca2::filesp                      _outFile;
      string                           _tempFileName;
      bool                             _tempFileCreated;
      uint32_t                           _crc;
      ::primitive::memory              m_memory;
      ::primitive::memory_position     _bufPos;
      ::primitive::memory_size         _size;


      temp_io_buffer();
      virtual ~temp_io_buffer();

      bool write_to_file(const void * data, ::primitive::memory_size size);
      using ::ca2::request_interface::create;
      void create();

      void InitWriting();
      bool write(const void *data, ::primitive::memory_size size);

      HRESULT write_to_stream(writer *stream);
      ::primitive::memory_size GetDataSize() const 
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
      void write(const void *data, ::primitive::memory_size size, ::primitive::memory_size * processedSize);
   };

} // namespace ca2
