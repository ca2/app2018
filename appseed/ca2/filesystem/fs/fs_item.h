#pragma once


namespace fs
{


   class CLASS_DECL_ca2 item :
      virtual public ::ca2::ca2
   {
   public:


      string                     m_strPath;
      flags < e_flag >           m_flags;


      item();
      item(const item & item);
      item(const string & strPath);
      item(const string & strPath, const flags < e_flag > & flags);


   };


} // namespace fs