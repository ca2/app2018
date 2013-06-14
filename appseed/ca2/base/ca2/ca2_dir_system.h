#pragma once


#if defined(METROWIN) || defined(MACOS)
   
#define MATTER_CACHE_FROM_HTTP_SERVER

#endif


namespace zip
{


   class Util;


}


namespace ca2
{


   class CLASS_DECL_ca2 path :
      virtual public ::ca2::object

   {
   public:
      virtual bool  is_equal(const char * lpszFilPathA, const char * lpszFilPathB);
   };


   namespace dir
   {


      class CLASS_DECL_ca2 system :
         virtual public ::ca2::object
      {
      public:



         class CLASS_DECL_ca2 is_dir
         {
         public:


            bool        m_bIsDir;
            uint32_t       m_dwLastCheck;
            uint32_t       m_dwError;


         };


         class CLASS_DECL_ca2 is_dir_map :
            virtual public string_map < is_dir >
         {
         public:


            mutex             m_mutex;
            uint32_t             m_dwTimeOut;


            is_dir_map(sp(::ca2::application) papp);


            bool lookup(const string & strPath, bool &bIsDir, uint32_t & dwLastError);
            bool lookup(const string & strPath, bool &bIsDir, uint32_t & dwLastError, int32_t iLast);
            bool lookup(const char * pszPath, bool &bIsDir, uint32_t &dwLastError);
            void set(const char * pszPath, bool bIsDir, uint32_t dwLastError);
            void set(const string & strPath, bool bIsDir, uint32_t dwLastError);


         };


         zip::Util *       m_pziputil;
         is_dir_map        m_isdirmap;
         mutex             m_mutex;


         system(sp(::ca2::application) papp);
         virtual ~system();


         virtual class path & path();


         virtual string path(const string & strFolder, const string & strRelative, bool bUrl);
         virtual string path(const string & strFolder, const string & strRelative, const string & str2, bool bUrl);

         virtual string path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl);

         virtual string path(const string & strFolder, const string & strRelative);
         virtual string path(const string & strFolder, const string & strRelative, const char * psz2);
         virtual string path(const string & strFolder, const string & strRelative, const string & str2);

         virtual string path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2);

         virtual string simple_path(const string & str1, const string & str2);
         virtual string simple_path(const string & str1, const string & str2, const string & str3);


         virtual string relpath(const char * lpcszSource, const char * lpcszRelative, const char * lpcsz2 = NULL);
         virtual void  ls_pattern(sp(::ca2::application) papp, const char * lpcsz, const char * lpcszPattern, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, array < bool, bool > * pbaIsDir = NULL, array < int64_t, int64_t > * piaSize = NULL);
         virtual void  ls(sp(::ca2::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, array < bool, bool > * pbaIsDir = NULL, array < int64_t, int64_t > * piaSize = NULL);
         virtual void  rls_pattern(sp(::ca2::application) papp, const char * lpcsz, const char * lpcszPattern, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL, array < bool, bool > * pbaIsDir = NULL, array < int64_t, int64_t > * piaSize = NULL, e_extract eextract = extract_first);
         virtual void  rls(sp(::ca2::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL, e_extract eextract = extract_first);
         virtual void  rls_dir(sp(::ca2::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL);
         virtual void  ls_dir(sp(::ca2::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL);
         virtual bool  has_subdir(sp(::ca2::application) papp, const char * lpcsz);
         virtual void  ls_file(sp(::ca2::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL);

         virtual bool  is(const char * lpcsz, sp(::ca2::application) papp);
         virtual bool  is(const string & str, sp(::ca2::application) papp);
         virtual bool  is(const var & var, sp(::ca2::application) papp);

         virtual bool  name_is(const char * lpcsz, sp(::ca2::application) papp);
         virtual bool  name_is(const string & str, sp(::ca2::application) papp);
         virtual bool  name_is(const var & var, sp(::ca2::application) papp);

         virtual bool  is_inside(const char * lpcszDir, const char * lpcszPath, sp(::ca2::application) papp);
         virtual bool  is_inside_time(const char * lpcsz, sp(::ca2::application) papp);
         virtual void  root_ones(stringa & stra, sp(::ca2::application) papp);
         virtual bool  mk(const char * lpcsz, sp(::ca2::application) papp);
         virtual bool  rm(sp(::ca2::application) papp, const char * psz, bool bRecursive = true);

         virtual string name(const char * psz);

         virtual string time(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string stage(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string stageapp(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string netseed(const char * lpcsz = NULL, const char * lpcsz2 = NULL);

         virtual string ca2(const char * lpcsz, const char * lpcsz2 = NULL);
         virtual string ca2();
         virtual string ca2(const string & str);
         virtual string ca2(const string & str, const string & str2);
         virtual string ca2(const char * lpcsz, const string & str2);
         virtual string ca2(const string & str, const char * lpcsz2);

         virtual string module(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string ca2module(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual void time_square(string &str);
         virtual string time_log(const char * pszId);

         virtual string locale_schema(sp(::ca2::application) papp, const string & strLocale, const string & strStyle);
         virtual string locale_schema_matter(sp(::ca2::application) papp, const string & strLocale, const string & strStyle);
         virtual string locale_schema_matter(const string & strLocator, const string & strLocale, const string & strStyle);
         virtual string matter(sp(::ca2::application) papp, const stringa & stra, bool bDir = false);
         virtual string matter(sp(::ca2::application) papp, const string & str, const string & str2, bool bDir = false);
         virtual string matter(sp(::ca2::application) papp, const string & str, const char * psz, bool bDir = false);
         virtual string matter(sp(::ca2::application) papp, const char * psz, const string & str, bool bDir = false);
         virtual string matter(sp(::ca2::application) papp, const char * psz, const char * psz2, bool bDir = false);
         virtual string matter(sp(::ca2::application) papp, const string & str, bool bDir = false);
         virtual string matter(sp(::ca2::application) papp, const char * psz, bool bDir = false);

         virtual void   matter_ls(sp(::ca2::application) papp, const string & str, stringa & stra);
         virtual void   matter_ls_file(sp(::ca2::application) papp, const string & str, stringa & stra);

         virtual string matter(sp(::ca2::application) papp);

         virtual string matter_from_locator(::user::str_context * pcontext, const string & strLocator);
         virtual string matter_from_locator(::user::str_context * pcontext, const string & strLocator, const string & str);
         virtual string matter_from_locator(::user::str_context * pcontext, const string & strLocator, const string & str, const string & str2);
         virtual void appmatter_locators(string & strRoot, string & strDomain, sp(::ca2::application) papp);
         virtual void appmatter_locators(string & strRoot, string & strDomain, const string & strLibraryName, const string & strAppName);
         virtual void appmatter_locators(string & strRoot, string & strDomain, const string & strAppName);
         virtual string appmatter_locator(sp(::ca2::application) papp);
         virtual string appmatter_locator(const string & strLibraryName, const string & strAppName);
         virtual string appmatter_locator(const string & strAppName);
         virtual string base_appmatter_locator(const string & strBase, const string & strLibraryName, const string & strAppName);
         virtual string base_appmatter_locator(const string & strBase, const string & strAppName);



         virtual string trash_that_is_not_trash(const char * psz);

         virtual string appdata(const char * lpcsz = NULL, const char * lpcsz2 = NULL);

         virtual string usersystemappdata(sp(::ca2::application) papp, const char * lpcszPrefix, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string userappdata(sp(::ca2::application) papp, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string userdata(sp(::ca2::application) papp, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string userfolder(sp(::ca2::application) papp, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
         virtual string default_os_user_path_prefix(sp(::ca2::application) papp);
         virtual string default_userappdata(sp(::ca2::application) papp, const char * lpcszPrefix, const char * lpcszLogin , const char * pszRelativePath = NULL);
         virtual string default_userdata(sp(::ca2::application) papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath = NULL);
         virtual string default_userfolder(sp(::ca2::application) papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath = NULL);
         virtual string userquicklaunch(sp(::ca2::application) papp, const char * pszRelativePath = NULL, const char * lpcsz2 = NULL);
         virtual string userprograms(sp(::ca2::application) papp, const char * pszRelativePath = NULL, const char * lpcsz2 = NULL);

         virtual string commonprograms(const char * pszRelativePath = NULL, const char * lpcsz2 = NULL);

         virtual string pathfind(const char * pszEnv, const char * pszTopic, const char * pszMode, sp(::ca2::application) papp);

         virtual bool initialize();
      };

      typedef ::ca::smart_pointer < system > system_sp;


   } // namespace dir


} // namespace ca2
