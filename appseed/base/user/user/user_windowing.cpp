//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"

namespace user
{


	critical_section * g_pcsUi = NULL;
   map < oswindow,oswindow,::user::interaction *,::user::interaction * > * g_pmapUi = NULL;
   map < ::user::interaction *,::user::interaction *, oswindow,oswindow > * g_pmapHandle = NULL;


   CLASS_DECL_BASE void init_windowing()
   {

      g_pcsUi = new critical_section();

      g_pmapUi = new map < oswindow,oswindow,::user::interaction *,::user::interaction * >;

      g_pmapHandle = new  map < ::user::interaction *,::user::interaction *,oswindow,oswindow >;

   }


   CLASS_DECL_BASE void term_windowing()
   {

      delete g_pmapUi;
      g_pmapUi = NULL;

      delete g_pmapHandle;
      g_pmapHandle = NULL;

      delete g_pcsUi;
      g_pcsUi = NULL;

   }


} // namespace user


CLASS_DECL_BASE ::user::interaction * oswindow_get(oswindow oswindow)
{

#ifdef WINDOWSEX
   
   cslock slOsWindow(::user::g_pcsUi);

   return ::user::g_pmapUi->operator[](oswindow);

#else

   if (oswindow == NULL)
   {

      return NULL;

   }


   return oswindow->m_pui;

#endif

}


CLASS_DECL_BASE bool oswindow_assign(oswindow oswindow,::user::interaction * pui)
{

   cslock slOsWindow(::user::g_pcsUi);

   ::user::g_pmapUi->set_at(oswindow,pui);

   ::user::g_pmapHandle->set_at(pui, oswindow);

   return true;

}


CLASS_DECL_BASE int_bool oswindow_remove(::user::interaction * pui)
{

   ASSERT(pui != NULL);

   if(pui == NULL)
      return false;

   cslock slOsWindow(::user::g_pcsUi);

   ::user::g_pmapUi->remove_key(::user::g_pmapHandle->operator[](pui));

   ::user::g_pmapUi->remove_key(pui->get_safe_handle());

   ::user::g_pmapHandle->remove_key(pui);

   return true;

}

