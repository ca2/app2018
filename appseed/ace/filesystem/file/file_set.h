#pragma once


namespace file
{


   class CLASS_DECL_ACE set :
      virtual public ::object
   {
   public:


      stringa           m_straSearch;
      bool_array        m_baRecursive;
      stringa           m_straFilter;
      ::file::listing   m_straFile;
      ::file::listing   m_straFileAddUp;


      set(::ace::application * papp);
      virtual ~set();


      // composite
      virtual void add_search(stringa & stra, bool_array & baRecursive);
      virtual void add_filter(stringa & stra);
      virtual ::count get_file_count();
      virtual void file_at(::index i, string & str);
      virtual ::index find_first_file(const char * lpcszFilePath, ::index iStart = 0);
      virtual void clear_file();
      virtual void clear_search();
      virtual void clear_filter();
      virtual void refresh();

      // derived
      virtual bool get_next_file(const char * lpcszCurrentFilePath, string & wstrNextFilePath);
      virtual string get_next_file(const char * psz);


   };


   typedef smart_pointer < set > set_sp;


} // namespace file





