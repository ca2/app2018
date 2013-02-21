#pragma once


namespace ca
{


   namespace file
   {


      class CLASS_DECL_ca application :
         public ::gen::object
      {
      public:


         application();


         virtual void copy(const char * pszNew, const char * psz, bool bFailIfExists = false, e_extract eextract = extract_first);


         virtual void trash_that_is_not_trash(const char * psz);
         virtual void trash_that_is_not_trash(stringa & stra);


         virtual void replace(const char * pszContext, const char * pszFind, const char * pszReplace);

         virtual bool exists(const char * pszPath);
         virtual bool exists(const string & strPath);
         virtual bool exists(const var & var);


         string time(const char * pszBasePath, int32_t iDepth, const char * pszPrefix = NULL, const char * pszSuffix = NULL);
         string time_square(const char * pszPrefix = NULL, const char * pszSuffix = NULL);
         string time_log(const char * pszId);


         virtual gen::filesp time_square_file(const char * pszPrefix = NULL, const char * pszSuffix = NULL);
         virtual gen::filesp get(const char * name);


         virtual string as_string(var varFile);
         virtual string as_string(var varFile, var & varQuery);
         virtual void as_memory(var varFile, primitive::memory & mem);
         virtual void lines(stringa & stra, var varFile);

         virtual bool put_contents(var varFile, const void * pvoidContents, count count);
         virtual bool put_contents(var varFile, const char * lpcszContents);
         virtual bool put_contents(var varFile, gen::file & file);
         virtual bool put_contents(var varFile, primitive::memory & mem);
         virtual bool put_contents_utf8(var varFile, const char * lpcszContents);

         
         string sys_temp(const char * pszName, const char * pszExtension);


         virtual ::gen::filesp get_file(var varFile, UINT nOpenFlags);
         virtual ::gen::byte_stream get_byte_stream(var varFile, UINT nOpenFlags);

         // get a file and if there are exceptions, should show end user friendly messages
         virtual ::gen::filesp friendly_get_file(var varFile, UINT nOpenFlags);

      };


   } // namespace file


} // namespace ca


