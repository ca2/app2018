// -*- C++ -*-
// $Id: io_strings.h,v 1.6 2002/06/29 17:43:24 t1mpy Exp $

// id3lib: a software library for creating and manipulating id3v1/v2 tags
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

#ifndef _ID3LIB_IO_stringS_H_
#define _ID3LIB_IO_stringS_H_

#include "id3lib_strings.h"
#include "reader.h"
#include "writer.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace dami
{
  namespace io
  {
    class CLASS_DECL_ca stringReader : public ID3_Reader
    {
      const string &  _string;
      strsize _cur;
     public:
      stringReader(const string & string) : _string(string), _cur(0) { ; }
      virtual ~stringReader() { ; }

      virtual void close() { ; }
      virtual bool peek(char * pch)
      {
        if (!this->atEnd())
        {
           *pch = _string.operator [](_cur);
          return true;
        }
        return false;
      }

      /** read up to \c len chars into buf and advance the internal position
       ** accordingly.  Returns the number of characters read into buf.
       **/
      virtual ::primitive::memory_size readChars(char buf[], ::primitive::memory_size len)
      {
        strsize size = min((unsigned int)len, (unsigned int)(_string.get_length() - _cur));
        memcpy(reinterpret_cast<char *>(buf), &((const char *)_string)[_cur], size);
        _cur += size;
        return size;
      }

      virtual file_position getCur()
      {
        return _cur;
      }

      virtual file_position getBeg()
      {
        return 0;
      }

      virtual file_position getEnd()
      {
        return _string.get_length();
      }

      /** Set the value of the internal position for reading.
       **/
      virtual file_position setCur(file_position pos)
      {
        pos_type end = this->getEnd();
        _cur = (strsize)((pos < end) ? pos : end);
        return _cur;
      }

      virtual bool atEnd()
      {
        return _cur >= _string.get_length();
      }

      virtual ::primitive::memory_size skipChars(::primitive::memory_size len)
      {
        strsize size = min((unsigned int)len, (unsigned int)(_string.get_length() - _cur));
        _cur += size;
        return size;
      }
    };

    class CLASS_DECL_ca BstringReader : public ID3_Reader
    {
      const primitive::memory&  _string;
      primitive::memory_position _cur;
     public:
      BstringReader(const primitive::memory& string) : _string(string), _cur(0) { ; }
      virtual ~BstringReader() { ; }

      virtual void close() { ; }
      virtual bool peek(char * pch)
      {
        if (!this->atEnd())
        {
          *pch = _string.get_data()[_cur];
          return true;
        }
        return false;
      }

      /** read up to \c len chars into buf and advance the internal position
       ** accordingly.  Returns the number of characters read into buf.
       **/
      virtual ::primitive::memory_size readChars(char buf[], ::primitive::memory_size len)
      {
        ::primitive::memory_size size = min((unsigned int)len, (unsigned int)(_string.get_size() - _cur));
        memcpy(reinterpret_cast<char *>(buf), &_string.get_data()[_cur], size);
        _cur += size;
        return size;
      }

      virtual file_position getCur()
      {
        return (file_position) _cur;
      }

      virtual file_position getBeg()
      {
        return 0;
      }

      virtual file_position getEnd()
      {
        return (file_position) _string.get_size();
      }

      /** Set the value of the internal position for reading.
       **/
      virtual file_position setCur(file_position pos)
      {
        file_position end = this->getEnd();
        _cur = (::primitive::memory_position) ((pos < end) ? pos : end);
        return (file_position) _cur;
      }

      virtual bool atEnd()
      {
        return _cur >= _string.get_size();
      }

      virtual ::primitive::memory_size skipChars(::primitive::memory_size len)
      {
         ::primitive::memory_position size = min((unsigned int)len,(unsigned int)( _string.get_size() - _cur));
        _cur += size;
        return (::primitive::memory_size) size;
      }
    };

    class CLASS_DECL_ca stringWriter : public ID3_Writer
    {
      string & _string;
     public:
      stringWriter(string & string) : _string(string) { ; }
      virtual ~stringWriter() { ; }

      void close() { ; }
      void flush() { ; }
      virtual ::primitive::memory_size writeChars(const char buf[], ::primitive::memory_size len)
      {
        _string += string(reinterpret_cast<const char *>(buf), len);
        return len;
      }
      ::primitive::memory_size writeChars(const char_type buf[], ::primitive::memory_size len)
      {
        _string += string(reinterpret_cast<const char *>(buf), len);
        return len;
      }

      file_position getCur()
      {
        return _string.get_length();
      }
    };

    class CLASS_DECL_ca BstringWriter : public ID3_Writer
    {
      primitive::memory& _string;
     public:
      BstringWriter(primitive::memory& string) : _string(string) { ; }
      virtual ~BstringWriter() { ; }

      void close() { ; }
      void flush() { ; }
      virtual ::primitive::memory_size writeChars(const char buf[], ::primitive::memory_size len)
      {
         _string.allocate_add_up(len);
        memcpy(&_string.get_data()[_string.get_size() - len],
           reinterpret_cast<const char *>(buf), len);
        return len;
      }
      ::primitive::memory_size writeChars(const char_type buf[], ::primitive::memory_size len)
      {
        memcpy(&_string.get_data()[_string.get_size() - len],
           reinterpret_cast<const char *>(buf), len);
        return len;
      }

      file_position getCur()
      {
        return (file_position) _string.get_size();
      }
    };
  };
};

#endif /* _ID3LIB_IO_stringS_H_ */

