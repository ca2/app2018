#pragma once


namespace ca2
{

   struct file_status
   {
      ::datetime::time  m_ctime;         // creation date/time of file
      ::datetime::time  m_mtime;         // last modification date/time of file
      ::datetime::time  m_atime;         // last access date/time of file
      file_size     m_size;          // logical size of file in bytes
      BYTE        m_attribute;     // logical OR of ::ca2::filesp::Attribute enum values
      BYTE        m_padding;       // pad the structure to a WORD
      string      m_strFullName;   // absolute path name

      void dump(dump_context & dumpcontext) const;

   };


} // namespace ca2



