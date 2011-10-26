// $Id: io_decorators.cpp,v 1.4 2002/07/02 22:13:40 t1mpy Exp $

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



#include "io_decorators.h" //has "readers.h" "io_helpers.h" "utils.h"
#include "zlib/zlib.h"

using namespace dami;

void io::WindowedReader::setWindow(pos_type beg, size_type size)
{
  ID3D_NOTICE( "WindowedReader::setWindow() [beg, size] = [" <<
               this->getBeg() << ", " << size << "]" );
  pos_type cur = this->getCur();

  // reset the end marker so as to avoid errors
  this->setEnd(_reader.getEnd());

  // set the beginning marker
  this->setBeg(beg);

  // since the characters might be more than a byte in size, we need to
  // manually get all the chars to set the window appropriately
  this->setCur(beg);
  ID3D_NOTICE( "WindowedReader::setWindow(): after setCur(beg), cur = "<<
               this->getCur() );

  this->skipChars((::primitive::memory_size)size);
  ID3D_NOTICE( "WindowedReader::setWindow(): after skipChars, cur = " <<
               this->getCur() );

  this->setEnd(this->getCur());

  ID3D_NOTICE( "WindowedReader::setWindow() [beg, cur, end] = [" << this->getBeg() << ", " << this->getCur() << ", " << this->getEnd() << "]" );


  // reset the stream
  this->setCur(cur);
}

file_position io::WindowedReader::setBeg(file_position beg)
{
  // make sure the position we want to set to isn't past the current
  // end position or the superclass's beginning position
  if (beg <= this->getEnd() && beg >= _reader.getBeg())
  {
    _beg = beg;
  }
  else if (beg > this->getEnd())
  {
    ID3D_WARNING( "WindowedReader::setBeg() failed, [beg, _end] = " <<
                  beg << ", " << this->getEnd() << "]" );
  }
  else
  {
    ID3D_WARNING( "WindowedReader::setBeg() failed, [beg, _beg] = " <<
                  beg << ", " << this->getBeg() << "]" );
  }
  return _beg;
}

file_position io::WindowedReader::setEnd(file_position end)
{
  // make sure the position we want to set to isn't beforen the current
  // beginning position or the superclass's end position
  if (this->getBeg() <= end && end <= _reader.getEnd())
  {
    _end = end;
  }
  else
  {
    ID3D_WARNING( "WindowedReader::setEnd() failed, end = " << end );
    ID3D_WARNING( "WindowedReader::setEnd() failed, beg = " <<
                  this->getBeg() );
    ID3D_WARNING( "WindowedReader::setEnd() failed, super.end = " <<
                  _reader.getEnd() );

  }
  return _end;
}

bool io::WindowedReader::read(char * pch)
{
  if (this->inWindow())
  {
    return _reader.read(pch);
  }
  else
  {
    ID3D_WARNING( "io::WindowedReader::read: not in window, " <<
                  "pos = " << this->getCur() << ", window = [" <<
                  this->getBeg() << ", " << this->getEnd() << "]");
    return false;
  }
}

bool io::WindowedReader::peek(char * pch)
{
   if (this->inWindow())
   {
      return _reader.peek(pch);
   }
   else
   {
      return false;
   }
}

::primitive::memory_size io::WindowedReader::readChars(char buf[], ::primitive::memory_size len)
{
  pos_type cur = this->getCur();
  ::primitive::memory_size size = 0;
  if (inWindow(cur))
  {
    size = _reader.readChars(buf, (primitive::memory_size)(min(len, _end - cur)));
  }
  return size;
}

::primitive::memory_size io::CharReader::readChars(char buf[], ::primitive::memory_size len)
{
   ::primitive::memory_size numChars = 0;
  ID3D_NOTICE( "CharReader::readChars(): len = " << len );
  char ch;
  for (; numChars < len; ++numChars)
  {
    if (this->atEnd())
    {
      break;
    }
    this->read(&ch);
    if (buf != NULL)
    {
      buf[numChars] = ch;
    }
  }
  ID3D_NOTICE( "CharReader::readChars(): numChars = " << len );
  return numChars;
}

bool io::LineFeedReader::read(char * pch)
{
  if (this->atEnd())
  {
    return false;
  }
  char ch;
  _reader.read(&ch);
  char ch2;
  if (ch == 0x0D && peek(&ch2) && ch2 == 0x0A)
  {
    ID3D_NOTICE( "LineFeedReader::read(): found CRLF at pos " <<
                 this->getCur() );
    _reader.read(&ch);
  }
  *pch = ch;
  return true;
};

bool io::UnsyncedReader::read(char * pch)
{
  if (atEnd())
  {
    return false;
  }
  char ch;
  _reader.read(&ch);
  char ch2;
  if (ch == 0xFF && peek(&ch2) && ch2 == 0x00)
  {
    ID3D_NOTICE( "UnsyncedReader::read(): found sync at pos " <<
                 this->getCur() );
    _reader.read(&ch);
  }
  *pch = ch;
  return true;
}

io::CompressedReader::CompressedReader(ID3_Reader& reader, ::primitive::memory_size newSize)
  : _uncompressed(new char[newSize])
{
   ::primitive::memory_size oldSize = reader.remainingBytes();

  primitive::memory binary = readBinary(reader, oldSize);

  ::uncompress((Bytef *) _uncompressed,
               reinterpret_cast<luint*>(&newSize),
               reinterpret_cast<const uchar*>(binary.get_data()),
               oldSize);
  this->setBuffer(_uncompressed, (size_type) newSize);
}

io::CompressedReader::~CompressedReader()
{
  delete [] _uncompressed;
}

ID3_Writer::int_type io::UnsyncedWriter::writeChar(char_type ch)
{
  if (_last == 0xFF && (ch == 0x00 || ch >= 0xE0))
  {
    _writer.writeChar('\0');
    _numSyncs++;
  }
  _last = _writer.writeChar(ch);
  return _last;
}

void io::UnsyncedWriter::flush()
{
  if (_last == 0xFF)
  {
    _last = _writer.writeChar('\0');
    _numSyncs++;
  }
  _writer.flush();
}

::primitive::memory_size io::UnsyncedWriter::writeChars(const char_type buf[], ::primitive::memory_size len)
{
  pos_type beg = this->getCur();
  ID3D_NOTICE( "UnsyncedWriter::writeChars(): len = " << len );
  for (size_t i = 0; i < len; ++i)
  {
    if (this->atEnd())
    {
      break;
    }
    this->writeChar(buf[i]);
  }
  ::primitive::memory_size numChars = (primitive::memory_size) (this->getCur() - beg);
  ID3D_NOTICE( "CharWriter::writeChars(): numChars = " << numChars );
  return numChars;
}

void io::CompressedWriter::flush()
{
  if (_data.get_size() == 0)
  {
    return;
  }
  const char_type* data = reinterpret_cast<const char_type*>(_data.get_data());
  ::primitive::memory_size dataSize = _data.get_size();
  _origSize = (size_type) dataSize;
  // The zlib documentation specifies that the destination size needs to
  // be an unsigned long at least 0.1% larger than the source buffer,
  // plus 12 bytes
  unsigned long newDataSize = dataSize + (dataSize / 10) + 12;
  char_type* newData = new char_type[newDataSize];
  if (::zlib_compress(newData, &newDataSize, data, dataSize) != Z_OK)
  {
    // log this
    ID3D_WARNING("io::CompressedWriter: error compressing");
    _writer.writeChars(data, dataSize);
  }
  else if (newDataSize < dataSize)
  {
    ID3D_NOTICE("io::CompressedWriter: compressed size = " << newDataSize << ", original size = " << dataSize );
    _writer.writeChars(newData, newDataSize);
  }
  else
  {
    ID3D_NOTICE("io::CompressedWriter: no compression!compressed size = " << newDataSize << ", original size = " << dataSize );
    _writer.writeChars(data, dataSize);
  }
  delete [] newData;
  _data.allocate(0);
}

::primitive::memory_size io::CompressedWriter::writeChars(const char_type buf[], ::primitive::memory_size len)
{
  ID3D_NOTICE("io::CompressedWriter: writing chars: " << len );
  _data.append(buf, len);
  return len;
}

