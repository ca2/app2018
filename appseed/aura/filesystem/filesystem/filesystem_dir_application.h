#pragma once


namespace zip
{


   class util;


}



namespace file
{


   namespace dir
   {


      class CLASS_DECL_AURA application:
         virtual public ::object
      {
      public:


         application(::aura::application * papp);
         virtual ~application();


         //virtual stringa locale_schema_matter(string & strLocale, const string & strStyle);
         //virtual stringa locale_schema_matter(string & strLocale, const string & strStyle, const string & pathRoot, const ::file::path & pathDomain);
         virtual string matter(const ::file::patha & patha, bool bDir = false);
         virtual string matter(const ::file::path & path, bool bDir = false);

         virtual void   matter_ls(const ::file::path & str, ::file::patha & stra);
         virtual ::file::path matter_file(const ::file::path & path,bool bDir = false);
         virtual void   matter_ls_file(const ::file::path & str,::file::patha & stra);

         //virtual ::file::path usersystemappdata(const string & lpcszPrefix);
         //virtual ::file::path userappdata();
         //virtual ::file::path userdata();
         //virtual ::file::path userfolder();
         //virtual ::file::path default_os_user_path_prefix();
         //virtual ::file::path default_userappdata(const string & lpcszPrefix, const string & lpcszLogin );
         //virtual ::file::path default_userdata(const string & lpcszPrefix, const string & lpcszLogin);
         //virtual ::file::path default_userfolder(const string & lpcszPrefix, const string & lpcszLogin);


         virtual ::file::path userquicklaunch();
         virtual ::file::path userprograms();


         virtual listing & ls(listing & listing);
         virtual listing & ls_relative_name(listing & listing);


         virtual bool  has_subdir(const ::file::path & lpcsz);
         virtual bool  is(const ::file::path & lpcsz);
         //virtual bool  is(const string & str);
         //virtual bool  is(const var & var);
         virtual bool  is_inside(const ::file::path & lpcszDir, const ::file::path & lpcszPath);
         virtual bool  is_inside_time(const ::file::path & lpcsz);
         virtual ::file::listing & root_ones(::file::listing & listing);
         virtual bool  mk(const ::file::path & lpcsz);
         virtual bool  rm(const ::file::path & psz, bool bRecursive = true);

         virtual ::file::path pathfind(const string & pszEnv,const string & pszTopic,const string & pszMode);

         virtual ::file::path dropbox();


      };


      typedef smart_pointer < application > application_sp;


   } // namespace dir


} // namespace file







