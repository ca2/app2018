#include "framework.h"


#ifdef WINDOWSEX


namespace windows
{


   ::file::path get_known_folder(REFKNOWNFOLDERID kfid);


} // namespace windows

#elif defined(ANDROID)

#include "aura/aura/os/android/android_data_exchange.h"

#endif



::file::path dir::appdata()
{

   return ::dir::ca2config() / "appdata" / app_relative();

}


::file::path dir::public_system()
{

   return public_root() / "system";

}


::file::path dir::system()
{

   return ca2config() / "system";

}


::file::path dir::local()
{

   return ca2config() / "local";

}


::file::path dir::localconfig()
{

   return local() / "localconfig";

}


::file::path dir::program_data()
{

#ifdef WINDOWSEX

   return ::windows::get_known_folder(FOLDERID_ProgramData);

#else

   return ca2config() / "program_data";

#endif

}


::file::path dir::config()
{

   if (::aura::system::g_p != NULL)
   {

      synch_lock sl(::aura::system::g_p->m_pmutex);

      if (::aura::system::g_p->m_pathConfig.has_char())
      {

         return ::aura::system::g_p->m_pathConfig;

      }

   }

   ::file::path path;

#ifdef WINDOWSEX

   path = ::windows::get_known_folder(FOLDERID_RoamingAppData);

#elif defined(METROWIN)

   path = begin(::Windows::Storage::ApplicationData::Current->LocalFolder->Path);

#elif defined(VSNORD)

   if (is_null(::aura::system::g_p, 65535))
   {

      output_debug_string("dir::config(err1)\n");

   }
   else if (is_null(::aura::system::g_p->m_pappcore, 65535))
   {

      output_debug_string("dir::config(err1)\n");

   }
   else if (is_null(::aura::system::g_p->m_pappcore->m_pmaindata, 65535))
   {

      output_debug_string("dir::config(err2)\n");

   }
   else if (is_null(::aura::system::g_p->m_pappcore->m_pmaindata->m_pnodedataexchange, 65535))
   {

      output_debug_string("dir::config(err3)\n");

   }
   else if (is_null(::aura::system::g_p->m_pappcore->m_pmaindata->m_pnodedataexchange->m_pszCacheDir, 65535))
   {

      output_debug_string("dir::config(err3)\n");

   }
   else
   {

      path = ::aura::system::g_p->m_pappcore->m_pmaindata->m_pnodedataexchange->m_pszCacheDir;

   }

#elif defined(APPLEOS)

   path = ::file::path(getenv("HOME")) / "Library/Application Support";

#else

   path = ::file::path(getenv("HOME")) / ".config";

#endif

   if (::aura::system::g_p != NULL)
   {

      synch_lock sl(::aura::system::g_p->m_pmutex);

     ::aura::system::g_p->m_pathConfig = path;

   }

   return path;

}


::file::path dir::ca2config()
{

   return config() / "ca2";

}


::file::path dir::public_root()
{

   return program_data() / "ca2";

}


::file::path dir::relative(::file::path path)
{

   path.replace(":", "");

   ::str::ends_eat_ci(path, ".exe");

   return path;

}

#ifdef METROWIN


::file::path dir::app_relative()
{

   return "";

}


#else


::file::path dir::app_relative()
{

   ::file::path path = ::file::app_module();

   path = relative(path);

   return path;

}


#endif
