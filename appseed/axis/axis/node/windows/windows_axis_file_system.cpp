#include "framework.h"
//#include "windows_file_system.h"


namespace windows
{


   namespace axis
   {


      file_system::file_system(::aura::application * papp):
         ::object(papp),
         ::file::system(papp),
         ::windows::file_system(papp),
         ::file::axis::system(papp)
      {

      }


      file_system::~file_system()
      {

      }


      bool file_system::FullPath(string &str,const char * lpszFileIn)
      {

         //if(::core::file_system::FullPath(str, lpszFileIn))
         // return true;

         if(::str::begins_ci(lpszFileIn,"http://"))
         {

            str = lpszFileIn;

            return true;

         }
         else if(::str::begins_ci(lpszFileIn,"https://"))
         {

            str = lpszFileIn;

            return true;

         }
         wstring wstrFileIn;
         wstrFileIn = ::str::international::utf8_to_unicode(lpszFileIn);
         wstring wstrFileOut;
         bool b = vfxFullPath(wstrFileOut.alloc(MAX_PATH * 8),wstrFileIn) != FALSE;
         if(b)
         {
            ::str::international::unicode_to_utf8(str,wstrFileOut);
         }
         return b;
      }

      bool file_system::FullPath(wstring & wstrFullPath,const wstring & wstrPath)
      {

         /*      if(::core::file_system::FullPath(wstrFullPath, wstrPath))
         return true;*/

         if(::str::begins_ci(wstrPath,L"http://"))
         {
            wstrFullPath = wstrPath;
            return true;
         }
         else if(::str::begins_ci(wstrPath,L"https://"))
         {
            wstrFullPath = wstrPath;
            return true;
         }

         return vfxFullPath(wstrFullPath,wstrPath) != FALSE;

      }


      UINT file_system::GetFileName(const char * lpszPathName,string & str)
      {
         int32_t nMax = MAX_PATH * 8;
         wstring wstrPathName;
         wstrPathName = ::str::international::utf8_to_unicode(lpszPathName);
         wstring wstrTitle;
         UINT user = vfxGetFileName(wstrPathName,wstrTitle.alloc(nMax),nMax);
         str = ::str::international::unicode_to_utf8(wstrTitle);
         return user;
      }

      void file_system::GetModuleShortFileName(HINSTANCE hInst,string & strShortName)
      {
         vfxGetModuleShortFileName(hInst,strShortName);
      }

      var file_system::length(const string & strPath,::aura::application * papp)
      {

         var varRet;

         varRet = ::file::system::length(strPath,papp);

         if(!varRet.is_null())
            return varRet;


#ifdef WINDOWS

         WIN32_FILE_ATTRIBUTE_DATA data;

         if(!GetFileAttributesExW(::str::international::utf8_to_unicode(strPath),GetFileExInfoStandard,&data))
         {
            varRet.set_type(var::type_null);
         }
         else
         {
            varRet = (uint32_t) data.nFileSizeLow;
         }

#else

         struct stat stat;

         if(::stat(strPath, &stat)  == -1)
         {
            varRet.set_type(var::type_null);
         }
         else
         {
            varRet = stat.st_size;
         }

#endif

         return varRet;

      }


      ::file::file_sp file_system::get_file(var varFile,UINT nOpenFlags,cres * pfesp,::aura::application * papp)
      {

         ::file::file_sp spfile;

         spfile = ::file::axis::system::get_file(varFile,nOpenFlags,pfesp,papp);

         if(spfile.is_set())
         {

            return spfile;

         }

         spfile = ::windows::file_system::get_file(varFile,nOpenFlags,pfesp,papp);

         if(spfile.is_set())
         {

            return spfile;

         }

         return NULL;

      }


   } // namespace axis


} // namespace windows
























