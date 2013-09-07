// LockedStream.cpp

#include "framework.h"

namespace core
{

   ::primitive::memory_size locked_in_stream::read(file_position startPos, void *data, ::primitive::memory_size size)
   {
      single_lock sl(&_criticalSection, TRUE);
      _stream->seek((file_offset) startPos, ::file::seek_begin);
      return _stream->read(data, size);
   }

   ::primitive::memory_size locked_reader::read(void *data, ::primitive::memory_size size)
   {
      ::primitive::memory_size realProcessedSize = _lockedInStream->read((file_position) _pos, data, size);
      _pos += realProcessedSize;
      return realProcessedSize;
   }

} // namespace core
