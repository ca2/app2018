#include "framework.h" // from "axis/user/user.h"


namespace user
{

   mutex * g_pmutexUi = NULL;
   map < oswindow,oswindow,::user::interaction *,::user::interaction * > * g_pmapUi = NULL;
   map < ::user::interaction *,::user::interaction *, oswindow,oswindow > * g_pmapHandle = NULL;


   CLASS_DECL_AXIS void init_windowing()
   {

      g_pmutexUi = new mutex(NULL);

      g_pmapUi = new map < oswindow,oswindow,::user::interaction *,::user::interaction * >;

      g_pmapHandle = new  map < ::user::interaction *,::user::interaction *,oswindow,oswindow >;


   }

   CLASS_DECL_AXIS void term_windowing()
   {

      delete g_pmapUi;
      g_pmapUi = NULL;

      delete g_pmapHandle;
      g_pmapHandle = NULL;

      delete g_pmutexUi;
      g_pmutexUi = NULL;

   }


} // namespace user

#if defined(WINDOWS) || defined(LINUX)
::user::interaction * window_from_handle(oswindow oswindow)
{

   if(oswindow == NULL)
      return NULL;

   synch_lock slOsWindow(::user::g_pmutexUi);

   return ::user::g_pmapUi->operator[](oswindow);

}
#endif

CLASS_DECL_AXIS bool oswindow_assign(oswindow oswindow,::user::interaction * pui)
{

   synch_lock slOsWindow(::user::g_pmutexUi);

   ::user::g_pmapUi->set_at(oswindow,pui);

   ::user::g_pmapHandle->set_at(pui, oswindow);

   return true;

}


CLASS_DECL_AXIS int_bool oswindow_remove(::user::interaction * pui)
{

   ASSERT(pui != NULL);

   if(pui == NULL)
      return false;

   synch_lock slOsWindow(::user::g_pmutexUi);

   ::user::g_pmapUi->remove_key(::user::g_pmapHandle->operator[](pui));

   ::user::g_pmapUi->remove_key(pui->get_safe_handle());

   ::user::g_pmapHandle->remove_key(pui);

   return true;

}

