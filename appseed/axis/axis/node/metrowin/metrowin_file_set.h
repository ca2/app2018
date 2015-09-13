#pragma once


namespace metrowin
{


   class CLASS_DECL_AXIS file_set :
      public ::file::set
   {
   public:




      file_set(::aura::application * papp);
      virtual ~file_set();


      virtual void add_search(stringa & stra, bool_array & baRecursive);
      virtual void add_filter(stringa & stra);
      virtual ::count get_file_count();
      virtual void file_at(::index i, string & str);
      virtual ::index find_first_file(const char * lpcszFilePath, ::index iStart = 0);
      virtual void clear_search();
      virtual void clear_filter();
      virtual void clear_file();
      virtual void refresh();


   };


} // namespace metrowin



