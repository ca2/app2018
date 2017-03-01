//#include "framework.h"


#ifdef WINDOWSEX


namespace windows
{


   ::file::path get_known_folder(REFKNOWNFOLDERID kfid);


} // namespace windows


#endif


::file::path dir::afterca2()
{

   return element();

}


::file::path dir::appdata(string strPlatform)
{

   return app() / strPlatform / app_relative();

}


::file::path dir::userappdata()
{

   return app() / app_relative() / "appdata";

}


::file::path dir::app()
{

   return root() / "app";

}


::file::path dir::public_system()
{

   return public_root() / "system";

}


::file::path dir::system()
{

   return root() / "system";

}


::file::path dir::local()
{

   return root() / "local";

}

#ifdef WINDOWSEX
::file::path dir::roaming_app_data()
{

   return ::windows::get_known_folder(FOLDERID_RoamingAppData);

}
#endif

::file::path dir::root()
{

   ::file::path path;

#ifdef WINDOWSEX

   path = ::windows::get_known_folder(FOLDERID_RoamingAppData);

   path /= "ca2";

#elif defined(METROWIN)

   path = begin(::Windows::Storage::ApplicationData::Current->LocalFolder->Path);

#elif defined(VSNORD)

   path = ::aura::system::g_p->m_pandroidinitdata->m_pszCacheDir;

#elif defined(APPLEOS)
   
   path = ::file::path(getenv("HOME")) / "Library/ca2";
   
#else

   path = ::file::path(getenv("HOME")) / ".config/ca2";

#endif

   return path;

}


::file::path dir::public_root()
{

   ::file::path path;

#ifdef WINDOWSEX

   path = ::windows::get_known_folder(FOLDERID_ProgramData);

   path /= "ca2";

#elif defined(METROWIN)

   path = begin(::Windows::Storage::ApplicationData::Current->LocalFolder->Path);

#elif defined(VSNORD)

   path = ::aura::system::g_p->m_pandroidinitdata->m_pszCacheDir;

#elif defined(APPLEOS)
   
   path = ::file::path(getenv("HOME")) / "Library/ca2";

#else

   path = ::file::path(getenv("HOME")) / ".config/ca2";

#endif

   return path;

}


::file::path dir::app_relative()
{

   string strRelative = dir::element();

   index iFind = strRelative.find(':');

   if (iFind >= 0)
   {

      index iFind1 = strRelative.reverse_find('\\', iFind);

      index iFind2 = strRelative.reverse_find('/', iFind);

      index iStart = MAX(iFind1 + 1, iFind2 + 1);

      strRelative = strRelative.substr(0, iFind - 1) + "_" + strRelative.substr(iStart, iFind - iStart) + strRelative.substr(iFind + 1);

   }

   return strRelative;

}
