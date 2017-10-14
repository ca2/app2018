#pragma once


#include "core/core/core.h"
#include "core/filesystem/filemanager/filemanager.h"
#include "core/user/userex/userex.h"
#include "core/user/html/html/html.h"


#ifdef _APP_CORE_HELLOMULTIVERSE_LIBRARY
#define CLASS_DECL_APP_CORE_HELLOMULTIVERSE  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CORE_HELLOMULTIVERSE  CLASS_DECL_IMPORT
#endif


namespace user
{

   typedef ::user::show < ::user::button > button_view;

} // namespace user


namespace helloworld
{

   class application;
   class main_view;
   class document;
   class view;

} // namespace flag


#undef App
#define App(pbaseapp) (pbaseapp->m_pcoreapp->cast_app < ::helloworld::application > ())


#include "helloworld_render.h"

#include "helloworld_document.h"
#include "helloworld_view_base.h"
#include "helloworld_view.h"
//#include "helloworld_lite_view.h"
//#include "helloworld_full_view.h"
#include "helloworld_main_view.h"
#include "helloworld_switcher_view.h"
#include "helloworld_frame.h"
#include "helloworld_main_frame.h"
#include "helloworld_pane_view_update_hint.h"
#include "helloworld_pane_view.h"

#include "helloworld_application.h"

