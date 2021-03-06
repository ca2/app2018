#pragma once


namespace ios
{
   
   
   class CLASS_DECL_AURA file_system :
   virtual public ::file::system
   {
   public:
      
      
      ::file::path            m_strUserFolder;
      
      
      file_system(::aura::application * papp);
      virtual ~file_system();
      
      
      //      virtual bool get_status(const ::file::path & path,::file::file_status & status);
      //      virtual cres set_status(const ::file::path & path,const ::file::file_status & status);
      //      
      //      
      //      
      //      virtual bool FullPath(string & str, const char * lpszFileIn);
      //      virtual bool FullPath(wstring & wstrFullPath, const wstring & wstrPath);
      //      virtual UINT GetFileName(const char * lpszPathName, string & str);
      //      virtual void GetModuleShortFileName(HINSTANCE hInst, string & strShortName);
      virtual var length(const string & strPath, ::aura::application * papp);
      //      
      //      virtual bool get_last_write_time(FILETIME * pfiletime,const string & strFilename);
      
      virtual bool update_module_path();
      
      virtual bool initialize();
      
      virtual ::file::file_sp get_file(var varFile,UINT nOpenFlags,cres * pfesp,::aura::application * papp);
      
      
      
   };
   
   
} // namespace ios



