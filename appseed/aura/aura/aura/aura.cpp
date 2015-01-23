//#include "framework.h"



BEGIN_EXTERN_C

CLASS_DECL_AURA uint32_t g_dwStartTime = 0;


END_EXTERN_C



//namespace std
//{
//
//   const nothrow_t nothrow;
//
//} // namespace std

int32_t ___()
{

   simple_message_box(NULL,"___ library of ca","___ library of ca",MB_ICONINFORMATION | MB_OK);

   return 0;

}


void __post_quit_message(int32_t nExitCode)
{

#ifdef WINDOWSEX

   ::PostQuitMessage(nExitCode);

#else

   throw not_implemented(get_thread_app());

#endif

}

string_map < INT_PTR,INT_PTR > * g_pmapLibrary = NULL;

string_map < INT_PTR, INT_PTR > & __library()
{
   return *g_pmapLibrary;
}





int g_iAuraRefCount = 0;


CLASS_DECL_AURA int get_aura_init()
{

   return g_iAuraRefCount;

}


CLASS_DECL_AURA int_bool defer_aura_init()
{

   g_iAuraRefCount++;

   if(g_iAuraRefCount > 1)
      return TRUE;

   if(!aura_init())
      return FALSE;

   return TRUE;

}


CLASS_DECL_AURA int_bool defer_aura_term()
{

   g_iAuraRefCount--;

   if(g_iAuraRefCount >= 1)
      return TRUE;

   aura_term();

   return TRUE;

}



bool aura_init()
{

   ::aura::static_start::init();

   if(!__node_aura_pre_init())
      return false;

   //::aura::static_start::init();

   __init_threading_count();

   ::multithreading::init_multithreading();

//   ::hthread::s_pmutex = new mutex();

  // ::hthread::s_pptra = new ptr_array <  hthread >();


   if(!__node_aura_pos_init())
      return false;

   return true;

}


bool aura_term()
{

   __wait_threading_count(::millis((5000) * 8));

   __node_aura_pre_term();

   ::multithreading::term_multithreading();

   __term_threading_count();

   __node_aura_pos_term();

   ::aura::static_start::term();

   processor_cache_oriented_destroy_all_memory_pools();

   return true;

}




void dappy(const char * psz)
{

   //printf("app._ : %s : %s\n",_argv[2],psz);
   //printf("hello!!    : %s\n",psz);
   //::OutputDebugString("hello!!    : " + string(psz) + "\n");

}




::map < void *,void *,::aura::application *,::aura::application * > * g_pmapAura = NULL;

void set_aura(void * p,::aura::application * papp)
{

   g_pmapAura->set_at(p,papp);

}

::aura::application * get_aura(void * p)
{

   return g_pmapAura->operator [](p);

}
