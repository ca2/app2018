#include "framework.h"
#ifdef WINDOWSEX
//#include "aura/node/windows/windows.h"
#endif



namespace fs
{


   link::link(::aura::application * papp):
      ::object(papp),
      ::data::data(papp),
      ::fs::native(papp),
      ::fs::data(papp)
   {
      
   }


   // optional if ls_dir is implemented
   bool link::has_subdir(const ::file::path & path)
   {

      if(path == m_path)
      {

         return true;

      }
      else
      {

         return native::has_subdir(path);

      }

   }


   ::file::listing & link::ls(::file::listing & listing)
   {

      listing.clear_results();


      if(listing.m_path == m_path)
      {

         listing = *this;

      }

      return listing;

   }


   int link::is_dir(const ::file::path & path)
   {

      if(path == m_path)
      {

         return 1;

      }
      else
      {

         return native::is_dir(path);

      }

   }


   ::file::listing & link::root_ones(::file::listing & listing)
   {

      ::file::path & path = listing.add(m_path);
      path.m_iDir = 1;
      listing.m_straTitle.add(m_strTitle);
      

      return listing;

   }


/*   

   void link::get_ascendants_path(const char * pszPath,stringa & stra)
   {

      return System.file().get_ascendants_path(pszPath,stra);

   }


   void link::get_ascendants_name(const char * lpcsz,stringa & straParam)
   {

      return System.file().get_ascendants_name(lpcsz,straParam);

   }


   string link::eat_end_level(const char * pszPath,int32_t iCount)
   {

      string strPath(pszPath);

      while(iCount > 0)
      {

         strPath = System.dir().name(strPath);

         iCount--;

      }

      return strPath;

   }


   string link::file_name(const char * pszPath)
   {

      return System.file().name_(pszPath);

   }


   string link::dir_path(const char * psz1,const char * psz2)
   {

      return psz1,psz2);

   }


   bool link::file_move(const char * pszDst,const char * pszSrc)
   {

      System.file().move(pszDst,pszSrc);

      return true;

   }


   ::file::file_sp link::get_file(var varFile,UINT nOpenFlags)
   {

      ::file::file_sp spfile(allocer());

      // ::file::mode_read | ::file::share_deny_none | ::file::type_binary
      if(spfile->open(varFile.get_string(),nOpenFlags).failed())
      {

         _throw(::file::exception(get_app(),::file::exception::none,01,varFile.get_string()));

      }

      return spfile;

   }


   bool link::file().exists(const char * pszPath)
   {

      return ::file_exists_dup(pszPath) != FALSE;

   }

*/


   bool link::tree_show_subdir(const ::file::path & path)
   {

      if(path == m_path)
      {

         return true;

      }
      else
      {

         return false;

      }

   }

   void link::defer_fill_folder(string strTitle, ::file::path pathFolder)
   {

      if (strTitle.is_empty() || pathFolder.is_empty())
      {

         return;

      }
      
      m_strTitle = strTitle;

      m_path = pathFolder;

      m_pprovider = get_app();

      ::file::listing::ls();

   }


   void link::fill_os_user()
   {

      defer_fill_folder("Favorites", System.dir().get_favorites_folder());

   }


   void link::fill_os_user_desktop()
   {

      defer_fill_folder("Desktop", System.dir().get_desktop_folder());


   }


} // namespace fs



