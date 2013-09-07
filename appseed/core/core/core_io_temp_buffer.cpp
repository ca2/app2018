// InOutTempBuffer.cpp

#include "framework.h"

/*#include "../../../C/7zCrc.h"

#include "InOutTempBuffer.h"
#include "StreamUtils.h"

using namespace NWindows;
using namespace NFile;
using namespace NDirectory;*/

namespace ca2
{

   static const uint32_t kTempBufSize = (1 << 20);

   static LPCSTR kTempFilePrefixString = "7zt";

   temp_io_buffer::temp_io_buffer()
   {
   }

   void temp_io_buffer::create()
   {
      m_memory.allocate(kTempBufSize);
   }

   temp_io_buffer::~temp_io_buffer()
   {
   }

   void temp_io_buffer::InitWriting()
   {
      _bufPos = 0;
      _tempFileCreated = false;
      _size = 0;
      _crc = CRC_INIT_VAL;
   }

   bool temp_io_buffer::write_to_file(const void *data, ::primitive::memory_size size)
   {
      if (size == 0)
         return true;
      if (!_tempFileCreated)
      {
         _tempFileName = Application.file().time_square(kTempFilePrefixString);
         _tempFile = Application.file().get(_tempFileName);
         _tempFileCreated = _tempFile.is_set();
      }
      primitive::memory_size processed = size;
      _outFile->write(data, size);
      _crc = crc_update(_crc, data, processed);
      _size += processed;
      return (processed == size);
   }

   bool temp_io_buffer::write(const void *data, ::primitive::memory_size size)
   {
      if (_bufPos < kTempBufSize)
      {
         primitive::memory_position cur = min(kTempBufSize - _bufPos, size);
         memcpy(m_memory.get_data() + _bufPos, data, cur);
         _crc = crc_update(_crc, data, cur);
         _bufPos += cur;
         size -= cur;
         data = ((const byte *)data) + cur;
         _size += cur;
      }
      return write_to_file(data, size);
   }

   HRESULT temp_io_buffer::write_to_stream(writer *stream)
   {
      _outFile->close();

      uint64_t size = 0;
      uint32_t crc = CRC_INIT_VAL;

      if (_bufPos > 0)
      {
         RINOK(WriteStream(stream, m_memory.get_data(), (::primitive::memory_size) _bufPos));
         //WriteStream(stream, m_memory.get_data(), _bufPos);
         crc = crc_update(crc, m_memory.get_data(), (::primitive::memory_size) _bufPos);
         size += _bufPos;
      }
      if (_tempFileCreated)
      {
         ::file::binary_buffer_sp inFile(get_app());
         if (!inFile->open(_tempFileName, ::file::mode_read | ::file::type_binary))
            return E_FAIL;
         while (size < _size)
         {
            uint32_t processed;
            throw "should implement below ReadPart from InFile";
            /*if (!inFile.ReadPart(m_memory.get_data(), kTempBufSize, processed))
               return E_FAIL;*/
            if (processed == 0)
               break;
            RINOK(WriteStream(stream, m_memory.get_data(), processed));
            crc = crc_update(crc, m_memory.get_data(), processed);
            size += processed;
         }
      }
      return (_crc == crc && size == _size) ? S_OK : E_FAIL;
   }

   void temp_io_writer::write(const void *data, ::primitive::memory_size size, ::primitive::memory_size * processed)
   {
      if (!_buf->write(data, size))
      {
         if (processed != NULL)
            *processed = 0;
         throw E_FAIL;
      }
      if (processed != NULL)
         *processed = size;
   }

} // namespace ca2
