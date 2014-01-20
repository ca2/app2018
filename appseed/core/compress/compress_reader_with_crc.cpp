// InStreamWithCRC.cpp


#include "framework.h"

/*#include "InStreamWithCRC.h"*/

namespace libcompress
{


::primitive::memory_size reader_with_crc::read(void *data, ::primitive::memory_size size)
{
  ::primitive::memory_size realProcessedSize = _stream->read(data, size);
  _size += realProcessedSize;
  if (size > 0 && realProcessedSize == 0)
    _wasFinished = true;
  _crc = crc_update(_crc, data, realProcessedSize);
  return realProcessedSize;
}

::primitive::memory_size input_stream_with_crc::read(void *data, ::primitive::memory_size size)
{
  ::primitive::memory_size realProcessedSize = _stream->read(data, size);
  /*
  if (size > 0 && realProcessedSize == 0)
    _wasFinished = true;
  */
  _size += realProcessedSize;
  _crc = crc_update(_crc, data, realProcessedSize);
  return realProcessedSize;
}

file_position input_stream_with_crc::seek(file_offset offset, ::file::e_seek seekOrigin)
{
  if (seekOrigin != ::file::seek_begin || offset != 0)
    throw E_FAIL;
  _size = 0;
  _crc = CRC_INIT_VAL;
  return _stream->seek(offset, seekOrigin);
}

} // namespace libcompress
