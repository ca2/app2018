// LimitedStreams.cpp
// from 7-zip on 2012-12-23, lunch time
#include "framework.h"


namespace file
{


   memory_size_t clustered_istream::read(void *data, memory_size_t size)
   {

      memory_size_t processedSize = 0;

      if (_virtPos >= Size)
      {
         if(_virtPos == Size)
            return 0;
         else
            _throw(system_exception(get_app(), E_FAIL));
      }

      if (_curRem == 0)
      {
         file_size_t blockSize = (file_size_t)1 << (file_size_t) BlockSizeLog;
         file_size_t virtBlock = _virtPos >> BlockSizeLog;
         file_size_t offsetInBlock = _virtPos & (blockSize - 1);
         file_size_t phyBlock = Vector[(index)virtBlock];
         file_size_t newPos = StartOffset + ((uint64_t)phyBlock << BlockSizeLog) + offsetInBlock;
         if (newPos != _physPos)
         {
            _physPos = newPos;
            SeekToPhys();
         }
         _curRem = blockSize - offsetInBlock;
         for (int32_t i = 1; i < 64 && (virtBlock + i) < (uint32_t)Vector.get_size() && phyBlock + i == Vector[(index)(virtBlock + i)]; i++)
            _curRem += (uint64_t)((uint64_t) 1 << (uint64_t) BlockSizeLog);
         uint64_t rem = Size - _virtPos;
         if (_curRem > rem)
            _curRem = rem;
      }
      if (size > _curRem)
         size = (memory_size_t) _curRem;
      size = Stream->read(data, size);
      processedSize = size;
      _physPos += size;
      _virtPos += size;
      _curRem -= size;
      return processedSize;
   }

   file_position_t clustered_istream::seek(file_offset_t offset, e_seek seekOrigin)
   {
      uint64_t newVirtPos = offset;
      switch(seekOrigin)
      {
      case STREAM_SEEK_SET: break;
      case STREAM_SEEK_CUR: newVirtPos += _virtPos; break;
      case STREAM_SEEK_END: newVirtPos += Size; break;
      default:
         _throw(system_exception(get_app(), STG_E_INVALIDFUNCTION));
      }
      if (_virtPos != newVirtPos)
         _curRem = 0;
      _virtPos = newVirtPos;
      return newVirtPos;
   }


} // namespace file




