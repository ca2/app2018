//#include "framework.h"



object::object()
{

   common_construct();

   m_pmutex             = NULL;
   m_ulFlags            = (uint32_t)flag_auto_clean;
   m_pfactoryitembase   = NULL;
   m_countReference     = 1;
   m_pauraapp           = NULL;

}


object::object(const object& objectSrc)
{

	m_psetObject         = NULL;
   m_pmutex             = NULL;
   m_ulFlags            = (uint32_t)flag_auto_clean;
   m_countReference     = 1;

   operator =(objectSrc);

}


void object::common_construct()
{

   m_pmutex             = NULL;
   m_psetObject         = NULL;

}


object::object(::aura::application * papp)
{

   common_construct();

   m_pauraapp           = papp;
   m_ulFlags            = (uint32_t)flag_auto_clean;
   m_pfactoryitembase   = NULL;
   m_countReference     = 1;

}


object::~object()
{

   ::aura::del(m_pmutex);

   ::aura::del(m_psetObject);

}


int64_t object::add_ref()
{

#ifdef WINDOWS

   return InterlockedIncrement64(&m_countReference);

#else

   return __sync_add_and_fetch(&m_countReference,1);

#endif

}


int64_t object::dec_ref()
{

#ifdef WINDOWS

   return InterlockedDecrement64(&m_countReference);

#else

   return  __sync_sub_and_fetch(&m_countReference,1);

#endif

}


int64_t object::release()
{

   int64_t i = dec_ref();

   if(i == 0)
   {

      delete_this();

   }

   return i;

}


object & object::operator=(const object & objectSrc)
{

   if(objectSrc.m_psetObject == NULL)
   {

      if(m_psetObject != NULL)
      {

         ::aura::del(m_psetObject);

      }

   }
   else
   {

      if(m_psetObject == NULL)
      {

         m_psetObject = new property_set(get_app());

      }

      *m_psetObject     = *objectSrc.m_psetObject;

   }

   m_pauraapp        = objectSrc.m_pauraapp;

   m_ulFlags = objectSrc.m_ulFlags;

   m_pfactoryitembase   = objectSrc.m_pfactoryitembase;

   return *this;

}


void object::assert_valid() const
{

   ASSERT(this != NULL);

}


void object::dump(dump_context & dumpcontext) const
{

   dumpcontext << "a " << typeid(*this).name() << " at " << (void *)this << "\n";

   UNUSED(dumpcontext); // unused in release build

}


void assert_valid_object(const object * pOb, const char * lpszFileName, int32_t nLine)
{
   if (pOb == NULL)
   {
//      TRACE(::aura::trace::category_AppMsg, 0, "ASSERT_VALID fails with NULL pointer.\n");
      if (__assert_failed_line(lpszFileName, nLine))
         debug_break();
      return;     // quick escape
   }
   if (!__is_valid_address(pOb, sizeof(object)))
   {
      ///TRACE(::aura::trace::category_AppMsg, 0, "ASSERT_VALID fails with illegal pointer.\n");
      if (__assert_failed_line(lpszFileName, nLine))
         debug_break();
      return;     // quick escape
   }

   // check to make sure the VTable pointer is valid
   //   ASSERT(sizeof(::object) == sizeof(void *));
   //   if (!__is_valid_address(*(void **)pOb, sizeof(void *), FALSE))
   if (!__is_valid_address(*(void **)pOb, sizeof(void *), FALSE))
   {
//      TRACE(::aura::trace::category_AppMsg, 0, "ASSERT_VALID fails with illegal vtable pointer.\n");
      if (__assert_failed_line(lpszFileName, nLine))
         debug_break();
      return;     // quick escape
   }

   /*if (!__is_valid_address(pOb, typeid(pOb->GetRuntimeClass()->m_nObjectSize, FALSE))
   {
   TRACE(::aura::trace::category_AppMsg, 0, "ASSERT_VALID fails with illegal pointer.\n");
   if (__assert_failed_line(lpszFileName, nLine))
   debug_break();
   return;     // quick escape
   }*/
   pOb->assert_valid();
}




void object::keep_alive()
{

   try
   {

      on_keep_alive();

   }
   catch(...)
   {

   }


}


void object::on_keep_alive()
{

}


bool object::is_alive()
{

   return true;

}



//
//object::waitable()
//{
//
//   m_pmutex = NULL;
//
//}
//
//object::waitable(const waitable & objectSrc)
//{
//
//   UNREFERENCED_PARAMETER(objectSrc);
//
//   m_pmutex = NULL;
//
//}
//
//
/////  \brief		destructor
//object::~waitable()
//{
//
//   if(m_pmutex != NULL)
//   {
//
//      delete m_pmutex;
//
//      m_pmutex = NULL;
//
//   }
//
//}

///  \brief		abstract function to initialize a waiting action without a timeout
wait_result sync_object::wait()
{

   return wait(duration::infinite());

}

///  \brief		abstract function to initialize a waiting action with a timeout
///  \param		duration time period to wait for item
///  \return	waiting action result as wait_result
//wait_result object::wait(const duration & duration)
//{
//
//
//   if(m_pmutex == NULL)
//   {
//
//      ((object *)this)->m_pmutex = new mutex();
//
//   }
//
//   try
//   {
//      return m_pmutex->wait(duration);
//   }
//   catch(...)
//   {
//   }
//
//   return wait_result(wait_result::Failure);
//
//}




// forward declaration
//class event_base;

/// This class represents a virtual interface for a callback method for WaitableItems in a
/// event_collection.

waitable_callback::~waitable_callback()
{
}

/// called on signalization of a event_base
/// \param signaling event_base
//	virtual void callback(const event_base& waitItem) = 0;
//};


///  \brief		pauses waitable for specified time
///  \param		duration sleeping time of waitable
/*CLASS_DECL_AURA void sleep(const duration & duration)
{
Sleep((uint32_t)duration.total_milliseconds());
}*/

CLASS_DECL_AURA void sleep(const duration & duration)
{
   ::Sleep(static_cast<uint32_t>(duration.total_milliseconds()));
}





//void * object::get_os_data() const
//{
//
//   //if(m_pmutex == NULL)
//   //{
//
//     // ((object *)this)->m_pmutex = new mutex();
//
//   //}
//
//   //return m_pmutex;
//
//   return NULL;
//
//}

bool sync_object::lock()
{

   return lock(duration::infinite());

}

//bool object::lock(const duration & duration)
//{
//
//   if(m_pmutex == NULL)
//   {
//
//      ((object *)this)->m_pmutex = new mutex();
//
//   }
//
//   bool bLocked = false;
//
//   try
//   {
//
//      bLocked = m_pmutex->lock(duration);
//
//   }
//   catch(...)
//   {
//
//      bLocked = false;
//
//   }
//
//   if(!bLocked)
//      return false;
//
//   return true;
//
//}


//bool sync_object::unlock()
//{
//
//   if(m_pmutex == NULL)
//      return false;
//
//   bool bUnlocked = false;
//
//   try
//   {
//
//      bUnlocked = m_pmutex->unlock();
//
//   }
//   catch(...)
//   {
//
//      bUnlocked = false;
//
//   }
//
//   if(!bUnlocked)
//      return false;
//
//   return true;
//
//}

//bool object::unlock(LONG lCount,LPLONG lpPrevCount)
//{
//   UNREFERENCED_PARAMETER(lCount);
//   UNREFERENCED_PARAMETER(lpPrevCount);
//   return true;
//}


bool sync_object::is_locked() const
{

   // CRITICAL SECTIONS does *NOT* support is locked and timed locks
   ASSERT(dynamic_cast < critical_section * > (const_cast < sync_object * > (this)) == NULL);

   single_lock sl(const_cast < sync_object * > (this));

   bool bWasLocked = !sl.lock(duration::zero());

   if(!bWasLocked)
      sl.unlock();

   return bWasLocked;

}





void object::create(sp(::create) pcreatecontext)
{
   if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("client_only"))
   {
      pcreatecontext->m_bClientOnly = true;
   }

   request_create(pcreatecontext);
}

void object::add_line(const char * pszCommandLine,application_bias * pbiasCreate)
{
   ::command_thread * commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));
   createcontext->m_spApplicationBias = pbiasCreate;

   createcontext->m_spCommandLine->_001ParseCommandLine(pszCommandLine);

   if (createcontext->m_spCommandLine->m_strApp.find_ci("app._.exe") >= 0)
      return;

   if(get_app()->command_central()->m_varTopicQuery.has_property("appid"))
   {

      if(createcontext->m_spCommandLine->m_varQuery["app"].is_empty())
      {

         createcontext->m_spCommandLine->m_varQuery["app"] = get_app()->command_central()->m_varTopicQuery["appid"];

      }
      else
      {

         createcontext->m_spCommandLine->m_varQuery["app"].stra().insert_at(0,get_app()->command_central()->m_varTopicQuery["appid"].get_string());

      }

      createcontext->m_spCommandLine->m_strApp = createcontext->m_spCommandLine->m_varQuery["app"];

   }

   if(get_app()->command_central()->m_varTopicQuery["build_number"].has_char())
   {

      createcontext->m_spCommandLine->m_varQuery["build_number"] = get_app()->command_central()->m_varTopicQuery["build_number"];

   }
   else if(createcontext->m_spCommandLine->m_varQuery["build_number"].is_empty())
   {

      if(createcontext->m_spCommandLine->m_strApp.CompareNoCase("app-core/netnodelite") == 0)
      {

         createcontext->m_spCommandLine->m_varQuery["build_number"] = "static";

      }
      else
      {

         createcontext->m_spCommandLine->m_varQuery["build_number"] = "installed";

      }

   }

   commandcentral->consolidate(createcontext);
   if (commandcentral != System.command())
   {
      System.command()->consolidate(createcontext);
   }
   create(createcontext);
}

void object::add_line_uri(const char * pszCommandLine,application_bias * pbiasCreate)
{
   sp(::command_thread) commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));
   createcontext->m_spApplicationBias = pbiasCreate;
   createcontext->m_spCommandLine->_001ParseCommandLineUri(pszCommandLine);
   commandcentral->consolidate(createcontext);
   System.command()->consolidate(createcontext);
   create(createcontext);
}

void object::add_fork(const char * pszCommandFork,application_bias * pbiasCreate)
{
   sp(::command_thread) commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));
   createcontext->m_spApplicationBias = pbiasCreate;
   createcontext->m_spCommandLine->_001ParseCommandFork(pszCommandFork);
   commandcentral->consolidate(createcontext);
   System.command()->consolidate(createcontext);
   create(createcontext);
}

void object::add_fork_uri(const char * pszCommandFork,application_bias * pbiasCreate)
{
   sp(::command_thread) commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));
   createcontext->m_spApplicationBias = pbiasCreate;
   createcontext->m_spCommandLine->_001ParseCommandForkUri(pszCommandFork);
   commandcentral->consolidate(createcontext);
   System.command()->consolidate(createcontext);
   create(createcontext);
}

void object::request_file(var & varFile)
{

   sp(::command_thread) commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));

   createcontext->m_spCommandLine->m_varFile              = varFile;

   request_create(createcontext);

   varFile = createcontext->m_spCommandLine->m_varFile;

}

void object::request_file_query(var & varFile,var & varQuery)
{

   sp(::command_thread) commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));

   createcontext->m_spCommandLine->m_varFile              = varFile;
   createcontext->m_spCommandLine->m_varQuery             = varQuery;
   if(!varFile.is_empty())
   {
      createcontext->m_spCommandLine->m_ecommand = command_line::command_file_open;
   }

   request_create(createcontext);

   varFile                       = createcontext->m_spCommandLine->m_varFile;
   varQuery                      = createcontext->m_spCommandLine->m_varQuery;

}

void object::request_command(sp(command_line) pcommandline)
{

   sp(::command_thread) commandcentral = get_app()->command_central();
   sp(::create) createcontext(canew(::create(commandcentral)));

   createcontext->m_spCommandLine = pcommandline;

   request_create(createcontext);

}

void object::request_create(sp(::create) pcreatecontext)
{
   on_request(pcreatecontext);
}

void object::on_request(sp(::create) pcreatecontext)
{

   UNREFERENCED_PARAMETER(pcreatecontext);

}






void object::set_app(::aura::application * papp)
{

   m_pauraapp = papp;

}


void object::system(const char * pszProjectName)
{

   UNREFERENCED_PARAMETER(pszProjectName);

}



void object::delete_this()
{

   if(m_pfactoryitembase != NULL && m_pfactoryitembase->m_pallocator)
   {

      m_pfactoryitembase->m_pallocator->discard(this);

   }
   else if(m_ulFlags & flag_discard_to_factory)
   {

      m_pauraapp->m_paurasystem->discard_to_factory(this);

   }
   else if(is_heap())
   {

      delete this;

   }

}


object * object::clone()
{

   if(m_pfactoryitembase != NULL)
      return m_pfactoryitembase->clone(this);

   return NULL;

}


string object::lstr(id id,const string & strDefault)
{

   return m_pauraapp->lstr(id,strDefault);

}



namespace aura
{


   allocatorsp::allocatorsp(::aura::application * papp)
   {

      allocator * pallocator = canew(allocator());

      pallocator->m_pauraapp = papp;

      smart_pointer < allocator >::operator = (pallocator);

   }


} // namespace aura











