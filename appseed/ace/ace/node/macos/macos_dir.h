#pragma once


namespace macos
{
   

   class CLASS_DECL_ACE dir :
      virtual public ::file::dir::system
   {
   public:
      
      
      
       ::file::path            m_strTimeFolder;
       ::file::path            m_strNetSeedFolder;
       
       ::file::path            m_strCa2;
       ::file::path            m_strCommonAppData;
       ::file::path            m_strAppData;
       ::file::path            m_strPrograms;
       ::file::path            m_strCommonPrograms;
       ::file::path            m_strCa2AppData;
      
       dir(::ace::application * papp);
       
       
       //virtual void update_module_path();
       
       
       // rls fetchs should set a meaningful m_iRelative value at each returned path
       virtual ::file::listing & ls(::ace::application * papp, ::file::listing & path);
       
       
       virtual bool  is(const ::file::path & path, ::ace::application * papp);
       virtual bool  is_inside(const ::file::path & pathFolder,const ::file::path & path,::ace::application * papp);
       virtual bool  is_inside_time(const ::file::path & path,::ace::application * papp);
       virtual bool  name_is(const ::file::path & path,::ace::application * papp);
       virtual bool  has_subdir(::ace::application * papp, const ::file::path & path);
       
       virtual ::file::listing & root_ones(::file::listing & listing,::ace::application * papp);
       virtual bool mk(const ::file::path & path,::ace::application * papp);
       virtual bool rm(::ace::application * papp, const ::file::path & path, bool bRecursive = true);
       
       
       virtual ::file::path name(const ::file::path & path);
       
       virtual ::file::path time();
       virtual ::file::path stage();
       virtual ::file::path stageapp();
       virtual ::file::path netseed();
       virtual ::file::path element();
//       virtual ::file::path profile();
       
       virtual ::file::path module();
       virtual ::file::path ca2module();
       virtual ::file::path time_square(::ace::application * papp, const string & strPrefix = cnull, const string & strSuffix = cnull);
       virtual ::file::path time_log();
       
       
       virtual ::file::path trash_that_is_not_trash(const ::file::path & path);
       
       
       
       virtual ::file::path appdata();
       virtual ::file::path commonappdata_root();
       virtual ::file::path commonappdata();
       
       virtual ::file::path usersystemappdata(::ace::application * papp, const char * lpcszPrefix);
       virtual ::file::path userappdata(::ace::application * papp);
       virtual ::file::path userdata(::ace::application * papp);
       //virtual ::file::path userfolder(::ace::application * papp);
       virtual ::file::path default_os_user_path_prefix(::ace::application * papp);
       virtual ::file::path default_userappdata(::ace::application * papp,const string & lpcszPrefix,const string & lpcszLogin );
       virtual ::file::path default_userdata(::ace::application * papp,const string & lpcszPrefix,const string & lpcszLogin);
       virtual ::file::path default_userfolder(::ace::application * papp,const string & lpcszPrefix,const string & lpcszLogin);
       virtual ::file::path userquicklaunch(::ace::application * papp);
       virtual ::file::path userprograms(::ace::application * papp);
       
       virtual ::file::path commonprograms();
       
       virtual bool initialize();
       

   };
    
   
} // namespace macos







