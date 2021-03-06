#pragma once


namespace user
{

   extern CLASS_DECL_AURA critical_section * g_pcsImpl;

   extern CLASS_DECL_AURA map < oswindow,oswindow,::user::interaction_impl *,::user::interaction_impl * > * g_pmapImpl;

   CLASS_DECL_AURA void init_windowing();

   CLASS_DECL_AURA void term_windowing();


} // namespace user





CLASS_DECL_AURA bool oswindow_assign(oswindow oswindow, ::user::interaction_impl * pimpl);
CLASS_DECL_AURA oswindow oswindow_remove(::user::interaction_impl * pimpl);
CLASS_DECL_AURA ::user::interaction_impl * oswindow_get(oswindow oswindow);


//#if defined(WINDOWS) || defined(LINUX)
//
//inline ::user::interaction * window_from_handle(oswindow oswindow);
//
//inline ::user::interaction * window_from_handle(oswindow oswindow)
//{
//
//   if(oswindow == NULL)
//   {
//
//      return NULL;
//
//   }
//
//   cslock slOsWindow(::user::g_pcsUi);
//
//   return ::user::g_pmapUi->operator[](oswindow);
//
//}
//
//#else
//
//CLASS_DECL_AURA ::user::interaction * window_from_handle(oswindow oswindow);
//
//#endif
