#pragma once

#include "id3lib_streams.h"
#include "reader.h"

class CLASS_DECL_ca ID3_FileReader : public ID3_Reader
{
   ex1::file & m_file;
protected:
   ex1::file & getReader() const { return m_file; }
public:

   ID3_FileReader(ex1::file & reader) : m_file(reader) { ; }
   virtual ~ID3_FileReader() { ; }
   virtual void close() { m_file.close(); }

   virtual bool peek(char * pch) { return m_file.peek(pch); }

   /** read up to \c len chars into buf and advance the internal position
   ** accordingly.  Returns the number of characters read into buf.
   **/
   virtual ::primitive::memory_size readChars(char buf[], ::primitive::memory_size len)
   {
      return m_file.read((char *)buf, len);
   }

   virtual file_position getBeg() { return 0; }
   virtual file_position getCur() { return m_file.get_position(); }
   virtual file_position getEnd() 
   { 
      pos_type cur = this->getCur();
      m_file.seek_to_end();
      pos_type end = this->getCur();
      this->setCur(cur);
      return end;
   }

   /** Set the value of the internal position for reading.
   **/
   virtual file_position setCur(file_position pos) { m_file.seek((file_offset) pos, ::ex1::seek_begin); return pos; }


};

class CLASS_DECL_ca ID3_MemoryReader : public ID3_Reader
{
   const char * m_pchBeg;
   const char * m_pchCur;
   const char * m_pchEnd;
protected:
   void setBuffer(const char* buf, size_type size)
   {
      m_pchBeg = buf;
      m_pchCur = buf;
      m_pchEnd = buf + (::primitive::memory_position)size;
   };
public:
   ID3_MemoryReader()
   {
      this->setBuffer(NULL, 0);
   }
   ID3_MemoryReader(const char* buf, size_type size)
   {
      this->setBuffer(buf, size);
   };
   virtual ~ID3_MemoryReader() { ; }
   virtual void close() { ; }

   virtual bool peek(char * pch) 
   { 
      if (!this->atEnd())
      {
         *pch = *m_pchCur; 
         return true;
      }
      return false;
   }

   /** read up to \c len chars into buf and advance the internal position
   ** accordingly.  Returns the number of characters read into buf.
   **/
   virtual ::primitive::memory_size readChars(char buf[], ::primitive::memory_size len);

   virtual file_position getCur() 
   { 
      return m_pchCur - m_pchBeg; 
   }

   virtual file_position getBeg()
   {
      return m_pchBeg - m_pchBeg;
   }

   virtual file_position getEnd()
   {
      return m_pchEnd - m_pchBeg;
   }

   /** Set the value of the internal position for reading.
   **/
   virtual file_position setCur(file_position pos)
   {
      pos_type end = getEnd();
      size_type size = (pos < end) ? pos : end;
      m_pchCur = m_pchBeg + (::primitive::memory_position)size;
      return getCur();
   }
};

