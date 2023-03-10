#pragma once


namespace file
{


   class CLASS_DECL_AURA system :
      virtual public object
   {
   public:


      path         m_pathCa2Module;
      path         m_pathModule;


      system(::aura::application * papp);
      virtual ~system();


      virtual cres copy(const path & pszNew,const path & psz,bool bFailIfExists,e_extract eextract,::aura::application * papp);
      virtual cres move(const path & pszNew,const path & psz,::aura::application * papp);
      virtual cres del(const path & psz,::aura::application * papp);
      virtual path duplicate(const path & psz,::aura::application * papp);
      virtual path paste(const path & pszLocation,const path & path,::aura::application * papp);
      virtual cres rename(const path & pszNew,const path & psz,::aura::application * papp);

      virtual void trash_that_is_not_trash(const path & psz,::aura::application * papp);
      virtual void trash_that_is_not_trash(patha & stra, ::aura::application * papp);

      virtual bool get_status(const path & path,file_status & status,::aura::application * papp);
      virtual cres set_status(const path & path,const file_status & status,::aura::application * papp);

      virtual cres replace(const path & pszContext,const string & pszFind,const string & pszReplace,::aura::application * papp);

      virtual bool exists(path path, ::aura::application * papp, bool bOptional = true, bool bNoCache = false);

      virtual bool is_file_or_dir(path path, var * pvarQuery, e_type * petype, ::aura::application * papp, bool bOptional = true, bool bNoCache = false);

      virtual var length(const path & path,::aura::application * papp);

      virtual var length(const path & path,var * pvarQuery,::aura::application * papp);

      virtual path module();

      virtual path ca2module();

      virtual bool update_module_path();

      path time(::aura::application * papp,const path & pszBasePath,int32_t iDepth,const string & pszPrefix = cnull,const string & pszSuffix = cnull);
      path time_square(::aura::application * papp,const string & pszPrefix = cnull,const string & pszSuffix = cnull);
      path time_log(::aura::application * papp,const string & pszId);

      virtual file_sp time_square_file(::aura::application * papp, const string & pszPrefix = cnull, const string & pszSuffix = cnull);
      virtual file_sp get(const path & name,::aura::application * papp);

      template < class T >
      string time_square(::aura::application * papp,T * p,bool (T::*lpfnOutput)(::stream &,const path &),const path & lpszSource)
      {
         string strTime = time_square(papp);
         if(strTime.has_char())
            if(!output(strTime, p, lpfnOutput, lpszSource))
               return "";
         return strTime;
      }




      virtual int32_t filterex_time_square(const char * pszPrefix,patha & stra);
      virtual bool mk_time(const ::file::path & pathCandidate);

      virtual var as_json(var varFile, ::aura::application * papp);
      virtual string as_string(var varFile,::aura::application * papp);
      virtual string as_string(var varFile,var & varQuery,::aura::application * papp);
      virtual void as_memory(var varFile,primitive::memory_base & mem,::aura::application * papp);

      virtual void lines(stringa & stra, var varFile, ::aura::application * papp);
      virtual void put_lines(var varFile, const stringa & stra, ::aura::application * papp);
      virtual void put_lines_utf8(var varFile, const stringa & stra, ::aura::application * papp);
      virtual void _put_lines(::file::file * pfile, const stringa & stra);

      virtual bool put_contents(var varFile,const void * pvoidContents,::count count,::aura::application * papp);
      virtual bool put_contents(var varFile,const char * lpcszContents,::aura::application * papp);
      virtual bool put_contents(var varFile,reader & reader,::aura::application * papp);
      virtual bool put_contents(var varFile,memory & mem,::aura::application * papp);
      virtual bool put_contents_utf8(var varFile,const char * lpcszContents,::aura::application * papp);

      virtual bool add_contents(var varFile,const void * pvoidContents,::count count,::aura::application * papp);
      virtual bool add_contents(var varFile,const char * lpcszContents,::aura::application * papp);

      virtual bool is_read_only(const path & psz);

      virtual path sys_temp(const path & pszName,const char * pszExtension,::aura::application * papp);
      virtual path sys_temp_unique(const path & pszName);


      virtual path replace_extension(const path & pszFile,const char * pszExtension);
      virtual void set_extension(path & str,const char * pszExtension);


      virtual void normalize(string & str);
      virtual int32_t cmp(const path & psz1,const path & psz2);


      virtual string md5(file * pfile);
      virtual string md5(var varFile, ::aura::application * papp);

      virtual string nessie(file *  pfile);
      virtual string nessie(var varFile, ::aura::application * papp);

      virtual bool resolve_link(::file::path & pathTarget, string & strFolder, string & strParams, const string & strSource, ::user::primitive * puiMessageParentOptional = NULL);

      virtual bool is_link(string strPath);

      virtual bool get_last_write_time(FILETIME * pfiletime,const string & strFilename);

      virtual void dtf(const path & pszFile,const path & pszDir,::aura::application * papp);

      virtual void dtf(const path & pszFile,patha & stra,::aura::application * papp);

      virtual void ftd(const path & pszDir,const path & pszFile,::aura::application * papp);

      // 'n' (natural) terminated ascii number, example: 245765487n
      // pmd5ctx = openssl/md5.h's MD5_CTX
      virtual void write_n_number(file *  pfile, void * pmd5ctx, int64_t iNumber);
      virtual void read_n_number(file *  pfile, void * pmd5ctx, int64_t & iNumber);

      virtual void write_gen_string(file *  pfile, void * pmd5ctx, string & str);
      virtual void read_gen_string(file *  pfile, void * pmd5ctx, string & str);

      virtual bool initialize();


      virtual file_sp get_file(var varFile,UINT nOpenFlags,cres * pfesp,::aura::application * papp);

      virtual bool prepare_output(::aura::application * papp, path & pathDownloading, const path & pathOut, ::stream & os);

      virtual bool prepare_input(::aura::application * papp, ::stream & is, const path & pathIn);

      virtual bool prepare_input(::aura::application * papp, ::stream & is, file * pfileIn);

      virtual bool prepare_input(::aura::application * papp, ::stream & is);

      virtual bool post_output(::aura::application * papp, path pathOut, path pathDownloading);

      template < class T >
      bool output(::aura::application * papp, const path & pathOut, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), const path & pathIn);

      template < class T >
      bool output(::aura::application * papp, const path & pathOut, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), file * pfileIn);

      template < class T >
      bool output(::aura::application * papp, const path & pathOut, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), ::stream & is);

      template < class T >
      bool output(::aura::application * papp, file * pfileOut, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), const path & pathIn);

      template < class T >
      bool output(::aura::application * papp, file * pfileOut, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), file * pfileIn);

      template < class T >
      bool output(::aura::application * papp, file * pfileOut, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), ::stream & is);

      template < class T >
      bool output(::aura::application * papp, ::stream & os, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), const path & pathIn);

      template < class T >
      bool output(::aura::application * papp, ::stream & os, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), file * pfileIn);

      template < class T >
      bool output(::aura::application * papp, ::stream & os, T * p, bool (T::*lpfnOuput)(::stream &, ::stream &), ::stream & is);

      virtual bool transfer(::aura::application * papp, const path & pathOut, const path & pathIn);

      virtual bool transfer(::aura::application * papp, const path & pathOut, file * pfileIn);

      virtual bool transfer(::aura::application * papp, const path & pathOut, ::stream & is);

      virtual bool transfer(::aura::application * papp, file * pfileOut, const path & pathIn);

      virtual bool transfer(::aura::application * papp, file * pfileOut, file * pfileIn);

      virtual bool transfer(::aura::application * papp, file * pfileOut, ::stream & is);

      virtual bool transfer(::aura::application * papp, ::stream & os, const path & pathIn);

      virtual bool transfer(::aura::application * papp, ::stream & os, file * pfileIn);

      virtual bool transfer(::aura::application * papp, ::stream & os, ::stream & is);

      virtual bool transfer(::stream & os, ::stream & is);

      virtual ::file::path dropbox_info_json(::aura::application * papp);


   };


   typedef smart_pointer < system > system_sp;




} // namespace file





