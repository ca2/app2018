#pragma once


namespace file
{


   class CLASS_DECL_ca2 system :
      public object
   {
   public:


      class CLASS_DECL_ca2 path :
         virtual public object
      {
      public:
         
         ::file::system * m_pfile;


         bool is_equal(const char * lpszFilPathA, const char * lpszFilPathB);
         bool eat_end_level(string & str, int32_t iLevelCount, const char * lpSeparator);
         void split(stringa & stra, const char * lpcszPath);
         bool is_relative(const char * psz);

         bool rename(const char * pszNew, const char * psz, sp(base_application) papp);

      };


      system();

      path & path();

      virtual void copy(const char * pszNew, const char * psz, bool bFailIfExists, e_extract eextract, sp(base_application) app);
      virtual void move(const char * pszNew, const char * psz);
      virtual void del(const char * psz);
      virtual string copy(const char * psz, sp(base_application) papp);
      virtual string paste(const char * pszLocation, const char * path, sp(base_application) papp);

      virtual void trash_that_is_not_trash(const char * psz, sp(base_application) papp);
      virtual void trash_that_is_not_trash(stringa & stra, sp(base_application) papp);

      virtual string title_(const char * path);
      virtual string name_(const char * path);
      virtual string extension(const char * path);

      virtual var length(const char * path);

      virtual void replace(const char * pszContext, const char * pszFind, const char * pszReplace, sp(base_application) papp);

      virtual bool exists(const char * path, sp(base_application) papp);
      virtual bool exists(const string & path, sp(base_application) papp);

      virtual bool exists(const char * path, var * pvarQuery, sp(base_application) papp);
      virtual bool exists(const string & path, var * pvarQuery, sp(base_application) papp);

      void  get_ascendants_path(const char * lpcsz, stringa & stra);
      void  get_ascendants_name(const char * lpcsz, stringa & stra);

      template < class T >
      bool output(sp(base_application) papp, const char * pszOutput, T * p, bool (T::*lpfnOuput)(::file::output_stream &, const char *), const char * lpszSource)
#if defined(LINUX) || defined(MACOS) || defined(ANDROID)
         ;
#else
      {
         System.dir().mk(System.dir().name(pszOutput), papp);
         ::file::binary_buffer_sp fileOut = App(papp).file().get_file(pszOutput, ::file::mode_create | ::file::type_binary | ::file::mode_write);
         if(fileOut.is_null())
            return false;
         ::file::output_stream ostream(fileOut);
         return (p->*lpfnOuput)(ostream, lpszSource);
      }
#endif

      template < class T >
      bool output(sp(base_application) papp, const char * pszOutput, T * p, bool (T::*lpfnOuput)(::file::output_stream &, ::file::input_stream &), const char * lpszInput)
#if defined(LINUX) || defined(MACOS) || defined(ANDROID)
         ;
#else
      {

         App(papp).dir().mk(System.dir().name(pszOutput));

         ::file::binary_buffer_sp fileOut = App(papp).file().get_file(pszOutput, ::file::mode_create | ::file::type_binary | ::file::mode_write);

         if(fileOut.is_null())
            return false;

         ::file::binary_buffer_sp fileIn = App(papp).file().get_file(lpszInput, ::file::type_binary | ::file::mode_read);

         if(fileIn.is_null())
            return false;

         ::file::output_stream ostream(fileOut);

         ::file::input_stream istream(fileIn);

         return (p->*lpfnOuput)(ostream, istream);

      }
#endif

      template < class T >
      bool output(sp(base_application) papp, const char * pszOutput, T * p, bool (T::*lpfnOuput)(::file::output_stream &, ::file::input_stream &), ::file::input_stream & istream)
      {
         
         ::file::output_stream ostream(get(pszOutput, papp));

         return (p->*lpfnOuput)(ostream, istream);

      }

      string time(sp(base_application) papp, const char * pszBasePath, int32_t iDepth, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
      string time_square(sp(base_application) papp, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
      string time_log(sp(base_application) papp, const char * pszId);

      virtual ::file::binary_buffer_sp time_square_file(sp(base_application) papp, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
      virtual ::file::binary_buffer_sp get(const char * name, sp(base_application) papp);

      template < class T >
      string time_square(sp(base_application) papp, T * p, bool (T::*lpfnOutput)(::file::output_stream &, const char *), const char * lpszSource)
      {
         string strTime = time_square(papp);
         if(strTime.has_char())
            if(!output(strTime, p, lpfnOutput, lpszSource))
               return "";
         return strTime;
      }

      int32_t filterex_time_square(const char * pszPrefix, stringa & stra);
      bool mk_time(const char * lpcszCandidate);

      string as_string(var varFile, sp(base_application) papp);
      string as_string(var varFile, var & varQuery, sp(base_application) papp);
      void as_memory(var varFile, primitive::memory_base & mem, sp(base_application) papp);
      void lines(stringa & stra, var varFile, sp(base_application) papp);

      bool put_contents(var varFile, const void * pvoidContents, ::count count, sp(base_application) papp);
      bool put_contents(var varFile, const char * lpcszContents, sp(base_application) papp);
      bool put_contents(var varFile, ::file::binary_buffer & file, sp(base_application) papp);
      bool put_contents(var varFile, primitive::memory & mem, sp(base_application) papp);
      bool put_contents_utf8(var varFile, const char * lpcszContents, sp(base_application) papp);

      bool is_read_only(const char * psz);

      string sys_temp(const char * pszName, const char * pszExtension, sp(base_application) papp);
      string sys_temp_unique(const char * pszName);


      string replace_extension(const char * pszFile, const char * pszExtension);
      void set_extension(string & str, const char * pszExtension);


      void normalize(string & str);
      int32_t cmp(const char * psz1, const char* psz2);



      string md5(const char * psz);
      string nessie(const char * psz);

      string nessie(::file::buffer_sp  pfile);

//      path & path36();

      void dtf(const char * pszFile, const char * pszDir, sp(base_application) papp);
      void dtf(const char * pszFile, stringa & stra, stringa & straRelative, sp(base_application) papp);
      void ftd(const char * pszDir, const char * pszFile, sp(base_application) papp);

      void is_valid_fileset(const char * pszFile, sp(base_application) papp);

      // 'n' (natural) terminated ascii number, example: 245765487n
      static void write_n_number(::file::buffer_sp  pfile, ::crypto::md5::context * pctx, int64_t iNumber);
      static void read_n_number(::file::buffer_sp  pfile, ::crypto::md5::context * pctx, int64_t & iNumber);

      static void write_gen_string(::file::buffer_sp  pfile, ::crypto::md5::context * pctx, string & str);
      static void read_gen_string(::file::buffer_sp  pfile, ::crypto::md5::context * pctx, string & str);




   protected:

      class system::path m_path;

   };


   typedef smart_pointer < system > system_sp;




} // namespace file





