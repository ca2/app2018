// $Id: utils.cpp,v 1.26 2002/07/02 22:04:36 t1mpy Exp $

// id3lib: a C++ library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug

// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
// License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where to
// send such submissions.  See the AUTHORS file for a list of people who have
// contributed to id3lib.  See the ChangeLog file for a list of changes to
// id3lib.  These files are distributed with id3lib at
// http://download.sourceforge.net/id3lib/
#include "StdAfx.h"
#include <ctype.h>

#if (defined(__GNUC__) && __GNUC__ == 2)
#define NOCREATE ios::nocreate
#else
#if defined(macintosh)  //not sure if this is still needed
#define toascii(X) (X)  //not sure if this is still needed
#endif                  //not sure if this is still needed
#define NOCREATE ((std::ios_base::openmode)0)
#endif

#include "utils.h" // has <config.h> "id3lib_streams.h" "globals.h" "id3lib_strings.h"

#if defined HAVE_ICONV_H
// check if we have all unicodes
#if (defined(ID3_ICONV_FORMAT_UTF16BE) && defined(ID3_ICONV_FORMAT_UTF16) && defined(ID3_ICONV_FORMAT_UTF8) && defined(ID3_ICONV_FORMAT_ASCII))
# include <iconv.h>
# include <errno.h>
#else
# undef HAVE_ICONV_H
#endif
#endif

  

using namespace dami;

size_t dami::renderNumber(uchar *buffer, uint32 val, size_t size)
{
  uint32 num = val;
  for (size_t i = 0; i < size; i++)
  {
    buffer[size - i - 1] = (uchar)(num & ID3LIB_MASK8);
    num >>= 8;
  }
  return size;
}



#if defined(HAVE_ICONV_H)

namespace 
{
  string convert_i(iconv_t cd, string source)
  {
    string target;
    size_t source_size = source.size();
#if defined(ID3LIB_ICONV_OLDSTYLE)
    const char* source_str = source.data();
#else
    char *source_str = new char[source.size()+1];
    source.copy(source_str, -1);
    source_str[source.length()] = 0;
#endif

#define ID3LIB_BUFSIZ 1024
    char buf[ID3LIB_BUFSIZ];
    char* target_str = buf;
    size_t target_size = ID3LIB_BUFSIZ;
    
    do
    {
      errno = 0;
      size_t nconv = iconv(cd, 
                           &source_str, &source_size, 
                           &target_str, &target_size);
      if (nconv == (size_t) -1 && errno != EINVAL && errno != E2BIG)
      {
// errno is probably EILSEQ here, which means either an invalid byte sequence or a valid but unconvertible byte sequence 
        return target;
      }
      target.append(buf, ID3LIB_BUFSIZ - target_size);
      target_str = buf;
      target_size = ID3LIB_BUFSIZ;
    }
    while (source_size > 0);
    return target;
  }

  const char* getFormat(ID3_TextEnc enc)
  {
    const char* format = NULL;
    switch (enc)
    {
      case ID3TE_ASCII:
        format = ID3_ICONV_FORMAT_ASCII;
        break;

      case ID3TE_UTF16:
        format = ID3_ICONV_FORMAT_UTF16;
        break;
        
      case ID3TE_UTF16BE:
        format = ID3_ICONV_FORMAT_UTF16BE;
        break;
        
      case ID3TE_UTF8:
        format = ID3_ICONV_FORMAT_UTF8;
        break;
        
      default:
        break;
    }
    return format;
  }
}
#endif


ID3_Err dami::createFile(string name, ex1::file & file)
{
  if (file.IsOpened())
  {
    file.close();
  }
    
  file.open(name, ex1::file::mode_read_write | ex1::file::type_binary | ex1::file::mode_create);
  if (!file)
  {
    return ID3E_ReadOnly;
  }
    
  return ID3E_NoError;
}


ID3_Err dami::openWritableFile(string name, ex1::file & file)
{
  
   if(!file.m_papp->m_psystem->file().exists(name))
   {
      return ID3E_NoFile;
   }
    
   if(file.IsOpened())
   {
      file.close();
   }
  
   if(!file.open(name,  ex1::file::mode_read_write | ex1::file::type_binary))
   {
      return ID3E_ReadOnly;
   }
    
   return ID3E_NoError;
}

ID3_Err dami::openReadableFile(string name, ex1::file & file)
{

   if(file.IsOpened())
   {
      file.close();
   }
  
   if(!file.open(name, ex1::file::type_binary | ex1::file::mode_read))
   {
      return ID3E_NoFile;
   }
    
   return ID3E_NoError;
}


