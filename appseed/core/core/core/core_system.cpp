﻿#include "framework.h"
//#include "core/user/user/user.h"
////#include "axis/net/net_sockets.h"
//#include "charguess.h"



#ifdef WINDOWS
#undef new
////#include <GdiPlus.h>
#define new AURA_NEW
#endif

#ifdef LINUX
#include "basecore/basecore.h"
#include <dlfcn.h>

void * g_pbasecore = NULL;


typedef void BASECORE_INIT();
#endif // LINUX


namespace core
{


   system::system(::aura::application * papp, app_core * pappcore):
      ::object(papp),
      ::aura::system(papp, pappcore),
      ::axis::system(papp, pappcore),
      ::base::system(papp, pappcore),
      m_mutexDelete(this),
      m_mutex(this)
   {

      m_ppatch = new core::patch();
      g_pszCooperativeLevel = "core";



#ifdef METROWIN
      m_window                                  = nullptr;
#endif

      m_pcoresystem                            = this;
      set_app(this);


      if(papp == NULL)
      {

         oprop("parent_system") = (sp(object)) NULL;

      }
      else
      {

         oprop("parent_system") = papp->m_pcoresystem;

      }

      string strId;
      //strId = m_strAppName;
      //strId += ::str::has_char(m_strAppId, ".");
      //strId += ::str::has_char(m_strBaseSupportId, ".");










      m_userset.set_app(this);
      //      m_email.set_app(this);






      m_bInitApplication         = false;
      m_bInitApplicationResult   = FALSE;
      m_bProcessInitialize       = false;
      m_bProcessInitializeResult = false;

      //m_puserstr                 = NULL;

      m_pparserfactory           = NULL;

      m_bLicense                 = false;

      //m_prunstartinstaller       = NULL;
      m_bLicense                 = false;

#ifdef WINDOWSEX

      m_uiWindowsTaskbarCreatedMessage = 0;

#endif



   }


   system::~system()
   {

      ::aura::del(m_ppatch);

   }


   void system::discard_to_factory(sp(object) pca)
   {

      UNREFERENCED_PARAMETER(pca);

   }


   bool system::is_system()
   {

      return true;

   }

   bool system::init2()
   {

      if(!::core::application::init2())
         return false;

      if(!::base::system::init2())
         return false;

      return true;

   }


   bool system::InitApplication()
   {

      if(m_bInitApplication)
         return m_bInitApplicationResult;

      m_bInitApplicationResult      = FALSE;
      m_bInitApplication            = true;

      m_bInitApplicationResult = ::core::application::InitApplication();

      return m_bInitApplicationResult;
   }


   bool system::process_init()
   {

      if (m_bProcessInitialize)
      {

         return m_bProcessInitializeResult;

      }

      thisinfo << "start";

      m_bProcessInitializeResult    = false;

      m_bProcessInitialize          = true;

#ifdef WINDOWSEX

      if (m_uiWindowsTaskbarCreatedMessage == 0)
      {

         m_uiWindowsTaskbarCreatedMessage = RegisterWindowMessageW(L"TaskbarCreated");

      }

#endif

      if (!::core::application::process_init())
      {

         thiserr << "end failure (1)";

         return false;

      }

      if (!::base::system::process_init())
      {

         thiserr << "end failure (2)";

         return false;

      }

      ::aura::profiler::initialize();

#ifdef LINUX

      fork([=]()
      {

         ::get_thread()->unregister_from_required_threads();

         g_pbasecore = dlopen("libbasecore.so", RTLD_LOCAL | RTLD_NOW);

         BASECORE_INIT * f =  (BASECORE_INIT *) dlsym(g_pbasecore, "basecore_init");

         if(f != NULL)
         {

            output_debug_string("basecore_init entry point not found at basecore library");

            (*f)();

            output_debug_string("gtk_main exited");

         }

      });

#endif

      //m_phtml = create_html();

      //m_phtml->add_ref();

      //if(m_phtml == NULL)
      //   return false;

      //m_phtml->construct(this);

      m_bProcessInitializeResult = true;

      thisinfo << "end";

      return true;

   }


   bool system::init()
   {

#ifndef APPLEOS

      if(m_pparserfactory == NULL)
      {

      }

#endif

      if (!::core::application::init())
      {

         return false;

      }

      return true;

   }


   bool system::init1()
   {

      m_pfilehandler = new ::filehandler::handler(this);

      if (!::core::application::init1())
      {

         return false;

      }

      if (!::base::system::init1())
      {

         return false;

      }

      //if(Session.fontopus()->create_system_user("system") == NULL)
      // return false;

#if !defined(CUBE) && !defined(VSNORD)

#if !defined(DEBUG) || defined(WINDOWS)
      try
      {
#endif
         find_applications_from_cache();
#if !defined(DEBUG) || defined(WINDOWS)
      }
      catch(...)
      {
      }
#endif

#endif

      //if(!m_phtml->initialize())
      //   return false;

      return true;

   }


   ::filehandler::handler & system::filehandler()
   {

      return *m_pfilehandler;

   }

   void system::on_start_find_applications_from_cache()
   {

      m_pfilehandler->m_sptree->remove_all();

   }

   void system::on_end_find_applications_from_cache(::file::byte_istream & is)
   {

      is >> *m_pfilehandler;

   }

   void system::on_end_find_applications_to_cache(::file::byte_ostream & os)
   {

      os << *m_pfilehandler;

   }

   void system::on_map_application_library(::aura::library & library)
   {

      m_pfilehandler->defer_add_library(library.m_pca2library);

   }

   bool system::init3()
   {

      if(!::core::application::init3())
      {

         return false;

      }

      if(m_phistory == NULL)
      {
         m_phistory = canew(os_history(this));
      }

      return true;

   }


   bool system::init_application()
   {

      if(!::base::system::init_application())
      {

         return false;

      }

      /*set_enum_name(var::type_null      , "NULL");
      set_enum_name(var::type_empty     , "is_empty");
      set_enum_name(var::type_string    , "string");
      set_enum_name(var::type_int32   , "integer");
      set_enum_name(var::type_uint32     , "ulong");
      set_enum_name(var::type_element       , "ca2");
      set_enum_name(var::type_bool      , "bool");
      set_enum_name(var::type_double    , "double");*/

      if (!::core::application::init_application())
      {

         return false;

      }

      m_pbergedgemap = canew(::core::session::map);

      if (!Session.on_initial_update())
      {

         return false;

      }

      return true;

   }


   bool system::bergedge_start()
   {

      return true;

   }


   void system::term_application()
   {



      __wait_threading_count(::millis((5000) * 8));

#ifdef LINUX

      BASECORE_INIT * f =  (BASECORE_INIT *) dlsym(g_pbasecore, "basecore_term");

      if(f != NULL)
      {

         output_debug_string("basecore_term entry point not found at basecore library");

         (*f)();

      }

#endif

      try
      {

         ::base::system::term_application();

      }
      catch(...)
      {

         m_error.set_if_not_set();

      }

   }


   ::aura::session * system::on_create_session()
   {

      return canew(::core::session(this));

   }


   ::aura::session * system::query_session(index iEdge)
   {

      ::core::session * pbergedge = NULL;

      if (m_pbergedgemap == NULL)
      {

         return NULL;

      }

      if(!m_pbergedgemap->Lookup(iEdge,pbergedge))
      {

         return NULL;

      }

      return pbergedge;

   }


   ::core::session * system::get_platform(index iEdge,application_bias * pbiasCreation)
   {

      if (iEdge == 0)
      {

         return System.m_pcoresession;

      }

      ::core::session * pbergedge = NULL;

      if (m_pbergedgemap == NULL)
      {

         return NULL;

      }

      if(!m_pbergedgemap->Lookup(iEdge,pbergedge))
      {

         ::aura::application * papp = create_application("session", true, pbiasCreation);

         if (papp == NULL)
         {

            return NULL;

         }

         pbergedge = dynamic_cast <::core::session *> (papp);

         if (pbergedge == NULL)
         {

            ::aura::del(papp);

            return NULL;

         }

         pbergedge->m_iEdge = iEdge;

         m_pbergedgemap->set_at(iEdge,pbergedge);

      }

      return pbergedge;

   }


   ::core::history & system::hist()
   {

      return *m_phistory;

   }


   bool system::set_history(::core::history * phistory)
   {

      m_phistory = phistory;

      return true;

   }


   void system::term()
   {

      ::aura::del(m_phistory);

      ::aura::del(m_pfilehandler);

      try
      {

         ::base::system::term();

      }
      catch(...)
      {

         m_error.set_if_not_set();

      }

   }


   void system::on_allocation_error(::aura::application * papp,::type * ptype)
   {

      UNREFERENCED_PARAMETER(papp);

      string strMessage;

      strMessage.Format("Implement \"%s\" allocation\n", ptype->friendly_name());

      App(papp).post_critical_error_message(strMessage);

   }


   void system::on_request(::create * pcreate)
   {

      get_platform(pcreate->m_spCommandLine->m_iEdge,pcreate->m_spCommandLine->m_pbiasCreate);

      ::base::system::on_request(pcreate);

   }


   bool system::sync_load_url(string & str,const char * lpszUrl,::fontopus::user * puser,::http::cookies * pcookies)
   {

      string filename = System.file().time_square(get_app());

      property_set set;

      set["user"] = puser;

      set["cookies"] = pcookies;

      if (!http().download(lpszUrl, filename, set))
      {

         return false;

      }

      string strLocation = set["get_headers"]["Location"];

      if(strLocation.has_char())
      {

         property_set set;

         set["user"] = puser;

         set["cookies"] = pcookies;

         Application.file().del(filename);

         return http().download(str,strLocation,set);

      }

      str = Application.file().as_string(filename);

      return true;

   }


   ::core::patch & system::patch()
   {

      return *m_ppatch;

   }


   ::http::system & system::http()
   {

      return m_httpsystem;

   }


   bool system::base_support()
   {

      return true;

   }


   index system::get_new_bergedge(application_bias * pbiasCreation)
   {

      index iNewEdge = m_iNewEdge;

      ::core::session * pbergedge = NULL;

      while(m_pbergedgemap->Lookup(iNewEdge,pbergedge))
      {

         iNewEdge++;

      }

      if (get_session(iNewEdge, pbiasCreation) == NULL)
      {

         return -1;

      }

      m_iNewEdge = iNewEdge + 1;

      return iNewEdge;

   }




   ::user::document * system::place_hold(::user::interaction * pui)
   {

      return NULL;

   }


   bool system::on_install()
   {

      try
      {

         xxdebug_box("system::on_install","system::on_install",0);

         if (!::core::application::on_install())
         {

            return false;

         }

      }
      catch(...)
      {

         return false;

      }

      return true;

   }


   string system::get_host_location_url()
   {

      return "";

   }


   bool system::add_library(::aura::library * plibrary)
   {

      m_libraryspa.add(plibrary);

      ::array < ::id > ida;

      plibrary->get_create_view_id_list(ida);

      for(int32_t i = 0; i < ida.get_count(); i++)
      {

         m_idmapCreateViewLibrary.set_at(ida[i],plibrary);

      }

      return true;

   }


   void system::post_fork_uri(const char * pszUri,application_bias * pbiasCreate)
   {

      handler()->add_fork_uri(pszUri,pbiasCreate);

      //if(handler()->m_varTopicQuery.has_property("version"))
      //{

      //   install().m_strVersion = handler()->m_varTopicQuery["version"];

      //}

      if(handler()->m_varTopicQuery["locale"].array_get_count() > 0)
      {

         Session.set_locale(handler()->m_varTopicQuery["locale"].stra()[0],::action::source::user());

      }

      if(handler()->m_varTopicQuery["schema"].array_get_count() > 0)
      {

         Session.set_schema(handler()->m_varTopicQuery["schema"].stra()[0],::action::source::user());

      }

   }


#ifdef METROWIN


   bool system::GetWindowRect(LPRECT lprect)
   {

      Windows::Foundation::Rect rect;

      try
      {

         rect = m_possystemwindow->m_pwindow->get_window_rect();

      }
      catch(...)
      {

         return false;

      }

      lprect->left      = (LONG)rect.X;
      lprect->top       = (LONG)rect.Y;
      lprect->right     = (LONG)(lprect->left + rect.Width);
      lprect->bottom    = (LONG)(lprect->top + rect.Height);

      return true;

   }


#endif


   bool system::process_command(::command::command * pcommand)
   {

      return base::system::process_command(pcommand);

   }


   void system::assert_valid() const
   {

      application::assert_valid();

   }


   void system::dump(dump_context & context) const
   {

      application::dump(context);

   }


   void system::main()
   {

      ::base::system::main();

   }


   spa(::core::session) & system::planesessionptra()
   {

      return m_planesessionptra;

   }


   void system::hist_hist(const char * psz)
   {

      hist().hist(psz);

   }


   sp(type) system::get_pane_tab_view_type_info()
   {

      return System.type_info < userex::pane_tab_view >();

   }




} // namespace plane


uint32_t _thread_proc_start_core_system_main(void * p);


uint32_t _thread_proc_start_core_system_main(void * p)
{

   ::base::system * psystem = (::base::system *)p;

   ::core::system * pplanesystem = dynamic_cast < ::core::system * > (psystem);

   ::set_thread(psystem);

   pplanesystem->::core::system::main();

   return pplanesystem->get_exit_code();

}


CLASS_DECL_CORE void __start_core_system_main(::base::system * psystem)
{

   ::create_thread(NULL,0,&_thread_proc_start_core_system_main,(LPVOID)psystem,0,0);

}
