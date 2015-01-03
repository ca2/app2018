#include "framework.h"


//#include "framework.h"
#ifdef WINDOWS
#define SECURITY_WIN32
#include <Shlobj.h>
#include <Security.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif


#ifdef LINUX
#include <link.h>
#elif defined(APPLEOS)
#include <dlfcn.h>
#include <mach-o/dyld.h>
#endif



#ifdef WINDOWSEX


bool  SHGetSpecialFolderPath(oswindow oswindow,string &str,int32_t csidl,bool fCreate)
{

   return ::SHGetSpecialFolderPathW(oswindow,wtostring(str,MAX_PATH * 8),csidl,fCreate) != FALSE;

}


#endif


#ifdef WINDOWS


bool __win_file_find_is_dots(WIN32_FIND_DATA & data)
{

   if(data.cFileName[0] == '.')
   {

      if(data.cFileName[1] == '\0' ||
         (data.cFileName[1] == '.' &&
         data.cFileName[2] == '\0'))
      {

         return true;

      }

   }

   return false;

}


#endif


//O volume na unidade Z � eco
 //O N�mero de S�rie do Volume � 4A91-94E4

 //Pasta de Z:\nordroid\app\appseed\aura\filesystem\file

#include "file.cpp"
#include "file_binary_buffer.cpp"
#include "file_buffered_buffer.cpp"
#include "file_byte_input_stream.cpp"
#include "file_byte_input_stream_binary_buffer.cpp"
#include "file_byte_output_stream.cpp"
#include "file_byte_output_stream_binary_buffer.cpp"
#include "file_byte_stream.cpp"
#include "file_byte_stream_memory_buffer.cpp"
#include "file_c.cpp"
#include "file_circular_buffer.cpp"
#include "file_composite.cpp"
#include "file_dir.cpp"
#include "file_dir2.cpp"
#include "file_exception.cpp"
#include "file_file_input_stream.cpp"
#include "file_file_output_stream.cpp"
#include "file_file_stream.cpp"
#include "file_file_stream_base.cpp"
#include "file_folder_watch.cpp"
#include "file_input_stream.cpp"
#include "file_memory_buffer.cpp"
#include "file_output_stream.cpp"
#include "file_plain_text_input_stream.cpp"
#include "file_plain_text_output_stream.cpp"
#include "file_plain_text_output_stream_string_buffer.cpp"
#include "file_plain_text_stream.cpp"
#include "file_plain_text_stream_base.cpp"
#include "file_plain_text_stream_memory_buffer.cpp"
#include "file_plain_text_stream_string_buffer.cpp"
#include "file_reader.cpp"
#include "file_seekable.cpp"
#include "file_serializable.cpp"
#include "file_set.cpp"
#include "file_simple_binary_buffer.cpp"
#include "file_stream.cpp"
#include "file_stream2.cpp"
#include "file_streambuf.cpp"
#include "file_stream_base.cpp"
#include "file_stream_buffer.cpp"
#include "file_stream_buffer_base.cpp"
#include "file_string_buffer.cpp"
#include "file_text_buffer.cpp"
#include "file_timeout_buffer.cpp"
#include "file_transfer_buffer.cpp"
#include "file_writer.cpp"
#include "file_writer_flush.cpp"
//2015-01-03  18:10                 0 _file.cpp"
//              48 arquivo(s)        172.623 bytes
//
//     Total de Arquivos na Lista:
//              48 arquivo(s)        172.623 bytes
//               0 pasta(s)   36.845.842.432 bytes dispon�veis
