#pragma once


namespace file
{


   class CLASS_DECL_BASE system :
      public object
   {
   public:


      class CLASS_DECL_BASE path :
         virtual public ::object
      {
      public:

         ::file::system * m_pfile;


         bool is_equal(const char * lpszFilPathA, const char * lpszFilPathB);
         bool eat_end_level(string & str, int32_t iLevelCount, const char * lpSeparator);
         void split(stringa & stra, const char * lpcszPath);
         bool is_relative(const char * psz);

         bool rename(const char * pszNew, const char * psz, ::aura::application * papp);

      };


      system();

      path & path();

      virtual void copy(const char * pszNew, const char * psz, bool bFailIfExists, e_extract eextract, ::aura::application * papp);
      virtual void move(const char * pszNew, const char * psz);
      virtual void del(const char * psz);
      virtual string copy(const char * psz, ::aura::application * papp);
      virtual string paste(const char * pszLocation, const char * path, ::aura::application * papp);

      virtual void trash_that_is_not_trash(const char * psz, ::aura::application * papp);
      virtual void trash_that_is_not_trash(stringa & stra, ::aura::application * papp);

      virtual string title_(const char * path);
      virtual string name_(const char * path);
      virtual string extension(const char * path);

      

      virtual void replace(const char * pszContext, const char * pszFind, const char * pszReplace, ::aura::application * papp);

      virtual bool exists(const char * path, ::aura::application * papp);
      virtual bool exists(const string & path, ::aura::application * papp);

      virtual bool exists(const char * path, var * pvarQuery, ::aura::application * papp);
      virtual bool exists(const string & path, var * pvarQuery, ::aura::application * papp);

      virtual var length(const char * path, ::aura::application * papp);
      virtual var length(const string & path, ::aura::application * papp);

      virtual var length(const char * path, var * pvarQuery, ::aura::application * papp);
      virtual var length(const string & path, var * pvarQuery, ::aura::application * papp);

      void  get_ascendants_path(const char * lpcsz, stringa & stra);
      void  get_ascendants_name(const char * lpcsz, stringa & stra);

      template < class T >
      bool output(::aura::application * papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,const char *),const char * lpszSource);

      template < class T >
      bool output(::aura::application * papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,::file::input_stream &),const char * lpszInput);

      template < class T >
      bool output(::aura::application * papp,const char * pszOutput,T * p,bool (T::*lpfnOuput)(::file::output_stream &,::file::input_stream &),::file::input_stream & istream);








      string time(::aura::application * papp, const char * pszBasePath, int32_t iDepth, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
      string time_square(::aura::application * papp, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
      string time_log(::aura::application * papp, const char * pszId);

      virtual ::file::buffer_sp time_square_file(::aura::application * papp, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
      virtual ::file::buffer_sp get(const char * name, ::aura::application * papp);

      template < class T >
      string time_square(::aura::application * papp, T * p, bool (T::*lpfnOutput)(::file::output_stream &, const char *), const char * lpszSource)
      {
         string strTime = time_square(papp);
         if(strTime.has_char())
            if(!output(strTime, p, lpfnOutput, lpszSource))
               return "";
         return strTime;
      }



      int32_t filterex_time_square(const char * pszPrefix, stringa & stra);
      bool mk_time(const char * lpcszCandidate);

      string as_string(var varFile, ::aura::application * papp);
      string as_string(var varFile, var & varQuery, ::aura::application * papp);
      void as_memory(var varFile, primitive::memory_base & mem, ::aura::application * papp);
      void lines(stringa & stra, var varFile, ::aura::application * papp);

      bool put_contents(var varFile, const void * pvoidContents, ::count count, ::aura::application * papp);
      bool put_contents(var varFile, const char * lpcszContents, ::aura::application * papp);
      bool put_contents(var varFile, ::file::reader & reader, ::aura::application * papp);
      bool put_contents(var varFile, primitive::memory & mem, ::aura::application * papp);
      bool put_contents_utf8(var varFile, const char * lpcszContents, ::aura::application * papp);

      bool is_read_only(const char * psz);

      string sys_temp(const char * pszName, const char * pszExtension, ::aura::application * papp);
      string sys_temp_unique(const char * pszName);


      string replace_extension(const char * pszFile, const char * pszExtension);
      void set_extension(string & str, const char * pszExtension);


      void normalize(string & str);
      int32_t cmp(const char * psz1, const char* psz2);



      string md5(const char * psz);
      string nessie(const char * psz);

      string nessie(::file::buffer_sp  pfile);

//      path & path36();

      void dtf(const char * pszFile, const char * pszDir, ::aura::application * papp);
      void dtf(const char * pszFile, stringa & stra, stringa & straRelative, ::aura::application * papp);
      void ftd(const char * pszDir, const char * pszFile, ::aura::application * papp);

      void is_valid_fileset(const char * pszFile, ::aura::application * papp);

      // 'n' (natural) terminated ascii number, example: 245765487n
      static void write_n_number(::file::buffer_sp  pfile, ::md5::md5 * pctx, int64_t iNumber);
      static void read_n_number(::file::buffer_sp  pfile, ::md5::md5 * pctx, int64_t & iNumber);

      static void write_gen_string(::file::buffer_sp  pfile, ::md5::md5 * pctx, string & str);
      static void read_gen_string(::file::buffer_sp  pfile, ::md5::md5 * pctx, string & str);


      virtual bool resolve_link(string & strTarget,const char * pszSource,sp(::user::interaction) puiMessageParentOptional = NULL);



   protected:

      class system::path m_path;

   };


   typedef smart_pointer < system > system_sp;




} // namespace file





