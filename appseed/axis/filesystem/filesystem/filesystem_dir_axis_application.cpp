#include "framework.h"


namespace zip
{

   class Util;

}


namespace file
{


   namespace dir
   {


      namespace axis
      {


         application::application(::aura::application * papp) :
            ::object(papp),
            ::file::dir::application(papp)
         {

         }


         application::~application()
         {

         }

         //string application::locale_schema(const char * pszLocale,const char * pszStyle)
         //{
         //   return m_papp->m_psystem->m_spdir->locale_schema(get_app(),pszLocale,pszStyle);
         //}

         //stringa application::locale_schema_matter(const char * pszLocale,const char * pszStyle,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->locale_schema_matter(get_app(),pszLocale,pszStyle,pszRoot,pszApp);
         //}

         //void application::matter_ls(const string & str,stringa & stra)
         //{
         //   m_papp->m_psystem->m_spdir->matter_ls(get_app(),str,stra);
         //}

         //void application::matter_ls_file(const string & str,stringa & stra)
         //{
         //   m_papp->m_psystem->m_spdir->matter_ls_file(get_app(),str,stra);
         //}

         //string application::matter(const string & str,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->matter(get_app(),str,bDir,pszRoot,pszApp);
         //}

         //string application::matter(const string & str,const string & str2,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->matter(get_app(),str,str2,bDir,pszRoot,pszApp);
         //}

         //string application::matter(const string & str,const char * psz,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->matter(get_app(),str,psz,bDir,pszRoot,pszApp);
         //}

         //string application::matter(const char * psz,const string & str,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->matter(get_app(),psz,str,bDir,pszRoot,pszApp);
         //}

         //string application::matter(const char * lpcsz,const char * lpcsz2,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->matter(get_app(),lpcsz,lpcsz2,bDir,pszRoot,pszApp);
         //}

         ::file::path application::matter_file(const ::file::path & lpcsz, bool bDir)
         {

            string strPath = m_papp->m_psystem->m_spdir->matter(get_app(), lpcsz, bDir);

            if (strPath.begins_ci("http://") || strPath.begins_ci("https://"))
            {
               string strFileExists = strPath;
               strFileExists.replace(":", "_");
               strFileExists = System.dir().cache() / strFileExists + ".exists_question";
               strFileExists.replace("\\\\", "\\", 2);
               string strFile = strPath;
               strFile.replace(":", "_");
               strFile = System.dir().cache() / strFile + ".local_copy";
               strFile.replace("\\\\", "\\", 2);
               if (Application.file().exists(strFileExists))
               {
                  if (Application.file().as_string(strFileExists) == "yes")
                     return strFile;
                  else
                     return "";
               }
               if (Application.file().transfer(strFile, strPath))
               {
                  Application.file().put_contents(strFileExists, "yes");
                  return strFile;
               }
               else
               {
                  Application.file().put_contents(strFileExists, "no");
                  return "";
               }
            }
            else
            {
               return strPath;
            }

         }

         //string application::matter(const stringa & stra,bool bDir,const char * pszRoot,const char * pszApp)
         //{
         //   return m_papp->m_psystem->m_spdir->matter(get_app(),stra,bDir,pszRoot,pszApp);
         //}

         //string application::usersystemappdata(const char * lpcszPrefix,const char * lpcsz,const char * lpcsz2)
         //{
         //   return m_papp->m_psystem->m_spdir->usersystemappdata(get_app(),lpcszPrefix,lpcsz,lpcsz2);
         //}

         //string application::userappdata(const char * lpcsz,const char * lpcsz2)
         //{
         //   return m_papp->m_psystem->m_spdir->userappdata(get_app(),lpcsz,lpcsz2);
         //}

         //string application::userdata(const char * lpcsz,const char * lpcsz2)
         //{
         //   return m_papp->m_psystem->m_spdir->userdata(get_app(),lpcsz,lpcsz2);
         //}

         //string application::userfolder(const char * lpcsz,const char * lpcsz2)
         //{
         //   return m_papp->m_psystem->m_spdir->userfolder(get_app(),lpcsz,lpcsz2);
         //}

         //string application::default_os_user_path_prefix()
         //{
         //   return m_papp->m_psystem->m_spdir->default_os_user_path_prefix(get_app());
         //}

         //string application::default_userappdata(const char * lpcszPrefix,const char * lpcszLogin,const char * pszRelativePath)
         //{
         //   return m_papp->m_psystem->m_spdir->default_userappdata(get_app(),lpcszPrefix,lpcszLogin,pszRelativePath);
         //}

         //string application::default_userdata(const char * lpcszPrefix,const char * lpcszLogin,const char * pszRelativePath)
         //{
         //   return m_papp->m_psystem->m_spdir->default_userdata(get_app(),lpcszPrefix,lpcszLogin,pszRelativePath);
         //}

         //string application::default_userfolder(const char * lpcszPrefix,const char * lpcszLogin,const char * pszRelativePath)
         //{
         //   return m_papp->m_psystem->m_spdir->default_userfolder(get_app(),lpcszPrefix,lpcszLogin,pszRelativePath);
         //}

         //string application::userquicklaunch(const char * pszRelativePath,const char * lpcsz2)
         //{
         //   return m_papp->m_psystem->m_spdir->userquicklaunch(get_app(),pszRelativePath,lpcsz2);
         //}

         //string application::userprograms(const char * pszRelativePath,const char * lpcsz2)
         //{
         //   return m_papp->m_psystem->m_spdir->userprograms(get_app(),pszRelativePath,lpcsz2);
         //}


         //bool application::ls_pattern(const char * lpcsz,const char * lpcszPattern,::file::patha & patha,bool bSize)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->ls_pattern(m_papp,lpcsz,lpcszPattern,ppatha,ppathaName,pbaIsDir,piaSize);
         //}

         //void application::ls(const char * lpcsz,::file::patha & patha,bool bSize)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->ls(m_papp,lpcsz,ppatha,ppathaName,pbaIsDir,piaSize);
         //}

         //bool application::rls_pattern(const char * lpcsz,const char * lpcszPattern,::file::patha & patha,bool bSize)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->rls_pattern(m_papp,lpcsz,lpcszPattern,ppatha,ppathaName,ppathaRelative,pbaIsDir,piaSize);
         //}

         //bool application::rls(const char * lpcsz,::file::patha & patha)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->rls(m_papp,lpcsz,ppatha,ppathaName,ppathaRelative);
         //}

         //void application::rls_dir(const char * lpcsz,::file::patha & patha)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->rls_dir(m_papp,lpcsz,ppatha,ppathaName,ppathaRelative);
         //}

         //void application::ls_dir(const char * lpcsz,::file::patha & patha)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->ls_dir(m_papp,lpcsz,ppatha,ppathaName);
         //}

         //bool application::has_subdir(const char * lpcsz)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->has_subdir(m_papp,lpcsz);
         //}

         //void application::ls_file(const char * lpcsz,::file::patha & patha)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->ls_file(m_papp,lpcsz,ppatha,ppathaName);
         //}

         //bool application::is(const char * lpcsz)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->is(lpcsz,m_papp);
         //}

         //bool application::is(const string & str)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->is(str,m_papp);
         //}

         //bool application::is(const var & var)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->is(var,m_papp);
         //}

         //bool application::is_inside(const char * lpcszDir,const char * lpcszPath)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->is_inside(lpcszDir,lpcszPath,m_papp);
         //}

         //bool application::is_inside_time(const char * lpcsz)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->is_inside_time(lpcsz,m_papp);
         //}


         //void application::root_ones(stringa & patha,stringa & straTitle)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->root_ones(patha,straTitle,m_papp);
         //}


         //bool application::mk(const char * lpcsz)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->mk(lpcsz,m_papp);
         //}


         //bool application::rm(const char * psz,bool bRecursive)
         //{
         //   return m_papp->m_psystem->m_spdir.m_p->rm(m_papp,psz,bRecursive);
         //}


         //string application::pathfind(const char * pszEnv,const char * pszTopic,const char * pszMode)
         //{
         //   return m_papp->m_psystem->m_spdir->pathfind(pszEnv,pszTopic,pszMode,m_papp);
         //}


      } // namespace axis


   } // namespace dir


} // namespace file






