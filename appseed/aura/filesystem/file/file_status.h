#pragma once


namespace file
{


   struct file_status
   {


      ::datetime::time     m_ctime;         // creation date/time of file
      ::datetime::time     m_mtime;         // last modification date/time of file
      ::datetime::time     m_atime;         // last access date/time of file
      file_size_t            m_size;          // logical size of file in bytes
      BYTE                 m_attribute;     // logical OR of ::file::file_sp::Attribute enum values
      BYTE                 m_padding;       // pad the structure to a WORD
      ::file::path         m_strFullName;   // absolute path name


#ifdef DEBUG

      void dump(dump_context & dumpcontext) const;

#endif

   };


} // namespace file





