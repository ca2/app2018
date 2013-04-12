#pragma once


namespace fs
{


   class list_item;


   class CLASS_DECL_ca item :
      virtual public ::ca::ca
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