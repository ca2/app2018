#include "framework.h"

namespace zip
{
   class Util;
}

namespace ca
{

   namespace dir
   {

      application::application()
      {
      }

      application::~application()
      {
      }

      string application::locale_schema(const char * pszLocale, const char * pszStyle)
      {
         return System.dir().locale_schema(get_app(), pszLocale, pszStyle);
      }

      string application::locale_schema_matter(const char * pszLocale, const char * pszStyle)
      {
         return System.dir().locale_schema_matter(get_app(), pszLocale, pszStyle);
      }

      string application::matter(const char * lpcsz, const char * lpcsz2)
      {
         return System.dir().matter(get_app(), lpcsz, lpcsz2);
      }

      string application::usersystemappdata(const char * lpcszPrefix, const char * lpcsz, const char * lpcsz2)
      {
         return System.dir().usersystemappdata(get_app(), lpcszPrefix, lpcsz, lpcsz2);
      }

      string application::userappdata(const char * lpcsz, const char * lpcsz2)
      {
         return System.dir().userappdata(get_app(), lpcsz, lpcsz2);
      }

      string application::userdata(const char * lpcsz, const char * lpcsz2)
      {
         return System.dir().userdata(get_app(), lpcsz, lpcsz2);
      }

      string application::userfolder(const char * lpcsz, const char * lpcsz2)
      {
         return System.dir().userfolder(get_app(), lpcsz, lpcsz2);
      }

      string application::default_os_user_path_prefix()
      {
         return System.dir().default_os_user_path_prefix(get_app());
      }

      string application::default_userappdata(const char * lpcszPrefix, const char * lpcszLogin , const char * pszRelativePath)
      {
         return System.dir().default_userappdata(get_app(), lpcszPrefix, lpcszLogin, pszRelativePath);
      }

      string application::default_userdata(const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      {
         return System.dir().default_userdata(get_app(), lpcszPrefix, lpcszLogin, pszRelativePath);
      }

      string application::default_userfolder(const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath)
      {
         return System.dir().default_userfolder(get_app(), lpcszPrefix, lpcszLogin, pszRelativePath);
      }

      string application::userquicklaunch(const char * pszRelativePath, const char * lpcsz2)
      {
         return System.dir().userquicklaunch(get_app(), pszRelativePath, lpcsz2);
      }

      string application::userprograms(const char * pszRelativePath, const char * lpcsz2)
      {
         return System.dir().userprograms(get_app(), pszRelativePath, lpcsz2);
      }


      void application::ls_pattern(const char * lpcsz, const char * lpcszPattern, stringa * pstraPath, stringa * pstraTitle, base_array < bool, bool > * pbaIsDir, base_array < int64_t, int64_t > * piaSize)
      {
         return m_psystem->m_spdir.m_p->ls_pattern(m_papp, lpcsz, lpcszPattern, pstraPath, pstraTitle, pbaIsDir, piaSize);
      }

      void application::ls(const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, base_array < bool, bool > * pbaIsDir, base_array < int64_t, int64_t > * piaSize)
      {
         return m_psystem->m_spdir.m_p->ls(m_papp, lpcsz, pstraPath, pstraTitle, pbaIsDir, piaSize);
      }

      void application::rls_pattern(const char * lpcsz, const char * lpcszPattern, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative, base_array < bool, bool > * pbaIsDir, base_array < int64_t, int64_t > * piaSize)
      {
         return m_psystem->m_spdir.m_p->rls_pattern(m_papp, lpcsz, lpcszPattern, pstraPath, pstraTitle, pstraRelative, pbaIsDir, piaSize);
      }

      void application::rls(const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative)
      {
         return m_psystem->m_spdir.m_p->rls(m_papp, lpcsz, pstraPath, pstraTitle, pstraRelative);
      }

      void application::rls_dir(const char * lpcsz, stringa * pstraPath, stringa * pstraTitle, stringa * pstraRelative)
      {
         return m_psystem->m_spdir.m_p->rls_dir(m_papp, lpcsz, pstraPath, pstraTitle, pstraRelative);
      }

      void application::ls_dir(const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
      {
         return m_psystem->m_spdir.m_p->ls_dir(m_papp, lpcsz, pstraPath, pstraTitle);
      }

      bool application::has_subdir(const char * lpcsz)
      {
         return m_psystem->m_spdir.m_p->has_subdir(m_papp, lpcsz);
      }

      void application::ls_file(const char * lpcsz, stringa * pstraPath, stringa * pstraTitle)
      {
         return m_psystem->m_spdir.m_p->ls_file(m_papp, lpcsz, pstraPath, pstraTitle);
      }

      bool application::is(const char * lpcsz)
      {
         return m_psystem->m_spdir.m_p->is(lpcsz, m_papp);
      }

      bool application::is(const string & str)
      {
         return m_psystem->m_spdir.m_p->is(str, m_papp);
      }

      bool application::is(const var & var)
      {
         return m_psystem->m_spdir.m_p->is(var, m_papp);
      }

      bool application::is_inside(const char * lpcszDir, const char * lpcszPath)
      {
         return m_psystem->m_spdir.m_p->is_inside(lpcszDir, lpcszPath, m_papp);
      }

      bool application::is_inside_time(const char * lpcsz)
      {
         return m_psystem->m_spdir.m_p->is_inside_time(lpcsz, m_papp);
      }

      void application::root_ones(stringa & stra)
      {
         return m_psystem->m_spdir.m_p->root_ones(stra, m_papp);
      }

      bool application::mk(const char * lpcsz)
      {
         return m_psystem->m_spdir.m_p->mk(lpcsz, m_papp);
      }

      bool application::rm(const char * psz, bool bRecursive)
      {
         return m_psystem->m_spdir.m_p->rm(m_papp, psz, bRecursive);
      }


   } // namespace dir


} // namespace ca


