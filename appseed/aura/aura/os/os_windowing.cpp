#include "framework.h"


static oswindow g_oswindowSplash = NULL;


void set_splash(oswindow oswindow)
{

   g_oswindowSplash = oswindow;

}


oswindow get_splash()
{

   return g_oswindowSplash;

}



bool g_bWindowingOutputDebugString = false;


void windowing_output_debug_string(const char * pszDebugString)
{

   if(g_bWindowingOutputDebugString)
   {

      output_debug_string(pszDebugString);

      fflush(stdout);

   }

}



CLASS_DECL_AURA void update_application_session_cursor(void * pvoidApp, POINT ptCursor)
{

   ::aura::application * papp = (::aura::application *) pvoidApp;

   Sess(papp).m_ptCursor = ptCursor;

}



