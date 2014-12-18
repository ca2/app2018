#include "framework.h"


//#include "freeimage/Source/FreeImage.h"



int g_iBaseRefCount = 0;


CLASS_DECL_BASE int get_base_init()
{

   return g_iBaseRefCount;

}


CLASS_DECL_BASE int_bool defer_base_init()
{

   g_iBaseRefCount++;

   if(g_iBaseRefCount > 1)
      return TRUE;

   if(!base_init())
      return FALSE;

   return TRUE;

}


CLASS_DECL_BASE int_bool defer_base_term()
{

   g_iBaseRefCount--;

   if(g_iBaseRefCount >= 1)
      return TRUE;

   base_term();

   return TRUE;

}



bool base_init()
{

   if(!defer_axis_init())
      return false;

   ::base::static_start::init();

   /*if(!__node_base_pre_init())
      return false;

   ::base::static_start::init();

   __init_threading_count();

   ::multithreading::init_multithreading();

   ::user::init_windowing();

   ::os_thread::s_pmutex = new mutex();

   ::os_thread::s_pptra = new ptr_array <  os_thread >();


   if(!__node_base_pos_init())
      return false;*/

#ifdef BASE_FREEIMAGE
   try
   {

      FreeImage_Initialise(FALSE);

   }
   catch(...)
   {

      ::simple_message_box(NULL,"Failure to initialize FreeImage (::core::init_core)","FreeImage_Initialise failure",MB_ICONEXCLAMATION);

      return false;

   }

#endif

   return true;

}


bool base_term()
{
#ifdef BASE_FREEIMAGE
   try
   {

      FreeImage_DeInitialise();

   }
   catch(...)
   {

   }
#endif

   /*__wait_threading_count(::millis((5000) * 8));

   __node_base_pre_term();

   ::user::term_windowing();

   ::multithreading::term_multithreading();

   __term_threading_count();

   __node_base_pos_term();*/

   ::base::static_start::term();

   defer_axis_term();

   return true;

}

