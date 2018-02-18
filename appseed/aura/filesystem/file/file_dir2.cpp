#include "framework.h"


#ifdef WINDOWSEX


namespace windows
{


   ::file::path get_known_folder(REFKNOWNFOLDERID kfid);


} // namespace windows


#endif


//::file::path dir::afterca2()
//{
//
//   return element();
//
//}
//

//::file::path dir::appdata(string strPlatform)
//{
//
//   return app() / strPlatform / app_relative();
//
//}

::file::path dir::appdata()
{

   return ::dir::ca2config() / "appdata" / app_relative();

}

//::file::path dir::appdata()
//{
//
//   return appdata();
//
//}


//::file::path dir::app()
//{
//
//   return ca2config() / "app";
//
//}


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

   ::file::path path;

#ifdef WINDOWSEX

   path = ::windows::get_known_folder(FOLDERID_RoamingAppData);

#elif defined(METROWIN)

   path = begin(::Windows::Storage::ApplicationData::Current->LocalFolder->Path);

#elif defined(VSNORD)

   path = ::aura::system::g_p->m_pdataexchange->m_pszCacheDir;

#elif defined(APPLEOS)

   path = ::file::path(getenv("HOME")) / "Library/Application Support";

#else

   path = ::file::path(getenv("HOME")) / ".config";

#endif

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
