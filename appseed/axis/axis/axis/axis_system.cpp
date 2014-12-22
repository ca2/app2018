#include "framework.h"
#include "framework.h" // from "axis/net/net_sockets.h"

#ifndef WINDOWS
#include "ft2build.h"
#include FT_FREETYPE_H
#endif



namespace axis
{



   system::system(::aura::application * papp):
      aura::system(this),
      m_httpsystem(this),
      m_visual(this)
   {

      m_purldepartament = new url::departament(this);

      m_paxissystem = this;

      m_spinstall = new ::install::install(this);

      __node_axis_factory_exchange(this);

      m_pcompress = new ::axis::compress;

      m_pcompress->set_app(this);



   }


   void system::construct(const char * pszAppId)
   {

      ::axis::application::construct(pszAppId);

   }


   system::~system()
   {

      delete m_purldepartament;

      delete m_pcompress;

   }





   bool system::process_initialize()
   {

#ifndef WINDOWS

      int32_t error = FT_Init_FreeType((FT_Library *)&m_ftlibrary);
      if(error)
      {
         TRACE("an error occurred during Free Type library initialization");
         return false;
      }

#endif

      enum_display_monitors();

      m_peengine = new ::exception::engine(this);


      if(!::axis::system::process_initialize())
         return false;

      m_spos.alloc(allocer());

      m_spcrypto.alloc(allocer());

      if(!m_spcrypto.is_set())
         return false;


      //#ifdef WINDOWSEX
      //
      //      dappy(string(typeid(*this).name()) + " : Going to ::axis::system::m_spwindow->create_window_ex : " + ::str::from(m_iReturnCode));
      //
      //      if(!m_spwindow->create_window_ex(0,NULL,NULL,0,null_rect(),NULL,"::axis::system::interaction_impl::no_twf"))
      //      {
      //
      //         dappy(string(typeid(*this).name()) + " : ::axis::system::m_spwindow->create_window_ex failure : " + ::str::from(m_iReturnCode));
      //
      //         return false;
      //
      //      }
      //
      //#endif

      dappy(string(typeid(*this).name()) + " : Going to ::axis::session " + ::str::from(m_iReturnCode));
      m_spfile.alloc(allocer());


      m_spdir.alloc(allocer());


      m_paxissession = new ::axis::session(this);

      m_paxissession = m_paxissession;

      m_paurasession = m_paxissession;

      if(m_paxissession == NULL)
         return false;

      if(!m_spdir->initialize())
         throw simple_exception(this,"failed to construct system m_spdir->initialize");

      m_paxissession->construct(this,0);



      if(!m_paxissession->begin_synch(&m_iReturnCode))
      {
         return false;
      }

      dappy(string(typeid(*this).name()) + " : ::axis::session OK " + ::str::from(m_iReturnCode));

      return true;

   }


   bool system::initialize2()
   {

      if(!::axis::application::initialize2())
         return false;

      return true;

   }


   bool system::initialize_instance()
   {

      m_pfactory->enable_simple_factory_request();

      if(!::axis::system::initialize_instance())
         return false;

      return true;

   }


   bool system::finalize()
   {



      __wait_threading_count_except(this,::millis((5000) * 77));

      bool bOk = false;



      try
      {

         if(m_spcrypto.is_set())
         {

            m_spcrypto.release();

         }

      }
      catch(...)
      {

         bOk = false;

      }

      try
      {

         bOk = ::axis::application::finalize();

      }
      catch(...)
      {

         bOk = false;

      }


      try
      {

         if(m_spfile.is_set())
         {

            m_spfile.release();

         }

      }
      catch(...)
      {

         bOk = false;

      }


      return bOk;

   }


   int32_t system::exit_instance()
   {

      __wait_threading_count(::millis((5000) * 8));

      try
      {


         /*      try
         {
         if(m_plemonarray != NULL)
         {
         delete m_plemonarray;
         }
         }
         catch(...)
         {
         }
         m_plemonarray = NULL;
         */


         m_pmath.release();

         m_pgeometry.release();

      }
      catch(...)
      {
         m_iReturnCode = -86;
      }






      for(int i = 0; i < m_serviceptra.get_size(); i++)
      {
         try
         {
            m_serviceptra[i]->Stop(0);
         }
         catch(...)
         {
         }
      }


      for(int i = 0; i < m_serviceptra.get_size(); i++)
      {
         try
         {
            m_serviceptra[i]->Stop((5000) * 2);
         }
         catch(...)
         {
         }
      }

      m_serviceptra.remove_all();

      try
      {
         if(m_pfactory != NULL)
         {

            m_pfactory->enable_simple_factory_request(false);

            m_pfactory.release();

         }

      }
      catch(...)
      {
         TRACE("system::exit_instance: Potentially catastrophical error : error disabling simple factory request");
      }


      int32_t iRet = m_iReturnCode;


      try
      {

         iRet = ::axis::application::exit_instance();

      }
      catch(...)
      {

      }






      try
      {
         if(m_spos.is_set())
         {
            m_spos.release();
         }
      }
      catch(...)
      {
      }
      try
      {
         m_spdir.release();
      }
      catch(...)
      {
      }


      try
      {
         m_spos.release();
      }
      catch(...)
      {
      }
      try
      {
         m_spdir.release();
      }
      catch(...)
      {
      }

      try
      {
         if(m_pmachineeventcentral != NULL)
         {
            m_pmachineeventcentral->set_run(false);
         }
      }
      catch(...)
      {
      }



      m_plog.release();



      {

         synch_lock sl(m_spmutexFactory);

         m_typemap.remove_all();

         m_typemap.release();

      }



      //::axis::application::exit_instance();

#ifdef METROWIN
      m_pdevicecontext = nullptr;

      m_pmutexDc.release();
#endif

#ifdef WINDOWSEX



      try
      {

         ::release(m_psystemwindow);

      }
      catch(...)
      {

         m_iReturnCode = -2;

      }
#endif

      if(m_peengine != NULL)
      {

         delete m_peengine;

         m_peengine = NULL;

      }


      return iRet;

   }


   bool system::verb()
   {

      return ::axis::system::verb();

   }


   //UINT system::os_post_to_all_threads(UINT uiMessage,WPARAM wparam,lparam lparam)
   //{

   //   post_to_all_threads(uiMessage,wparam,lparam);

   //   return 0;

   //}

   //sp(element) system::clone()
   //{
   //   // by the time, it is not possible to clone a system
   //   return NULL;
   //}



   //void system::discard_to_factory(sp(element) pca)
   //{

   //   if(m_pfactory == NULL)
   //      return;

   //   m_pfactory->discard(pca);

   //}



   //void system::wait_twf()
   //{

   //}

   bool system::is_system()
   {

      return true;

   }



   class ::crypto::crypto & system::crypto()
   {
      return *m_spcrypto;
   }





   ::fontopus::user_set & system::userset()
   {
      return m_userset;
   }



   //::axis::compress & system::compress()
   //{
   //   return m_compress;
   //}





   ::user::interaction * system::get_active_guie()
   {

      return ::axis::system::get_active_guie();

   }


   ::user::interaction * system::get_focus_guie()
   {

      return ::axis::system::get_focus_guie();

   }


   string system::dir_appmatter_locator(::aura::application * papp)
   {

      return dir().appmatter_locator(papp);

   }


   string system::crypto_md5_text(const string & str)
   {

      return crypto().md5(str);

   }


   uint32_t _thread_proc_start_system(void * p)
   {

      ::axis::system * psystem = (::axis::system *)p;

      return psystem->main();

   }

   CLASS_DECL_AXIS void __start_system(::axis::system * psystem)
   {

      ::create_thread(NULL,0,&_thread_proc_start_system,(LPVOID)psystem,0,0);

   }


   void system::install_progress_add_up(int iAddUp)
   {

      UNREFERENCED_PARAMETER(iAddUp);

      install().m_progressApp()++;

   }



} // namespace axis



















