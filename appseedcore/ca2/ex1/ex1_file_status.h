#pragma once


namespace ex1
{

   struct file_status
   {
      class time  m_ctime;         // creation date/time of file
      class time  m_mtime;         // last modification date/time of file
      class time  m_atime;         // last access date/time of file
      file_size     m_size;          // logical size of file in bytes
      BYTE        m_attribute;     // logical OR of ex1::filesp::Attribute enum values
      BYTE        m_padding;       // pad the structure to a WORD
      string      m_strFullName;   // absolute path name

   #ifdef _DEBUG
      void dump(dump_context & dumpcontext) const;
   #endif

   };


} // namespace ex1



