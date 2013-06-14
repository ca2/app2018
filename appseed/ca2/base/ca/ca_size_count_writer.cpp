#include "framework.h"

namespace ca2
{

   void size_count_writer::write(const void *data, ::primitive::memory_size size, ::primitive::memory_size * processedSize)
   {
      ::primitive::memory_size realProcessedSize;
      _stream->write(data, size, &realProcessedSize);
      _size += realProcessedSize;
      if (processedSize)
         *processedSize = realProcessedSize;
   }

} // namespace ca2
