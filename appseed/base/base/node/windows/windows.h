#pragma once




#include "axis/node/windows/windows.h"

#include "windows_interaction_impl.h"

#include "windows_factory_exchange.h"

#include "windows_implementation.h"

#include "windows_print_job.h"
#include "windows_copydesk.h"




CLASS_DECL_BASE const unichar * __register_window_class(::aura::application * papp,UINT nClassStyle,HCURSOR hCursor = 0,HBRUSH hbrBackground = 0,HICON hIcon = 0);

CLASS_DECL_BASE bool __register_class(WNDCLASSW* lpWndClass);

CLASS_DECL_BASE LRESULT CALLBACK __window_procedure(oswindow oswindow,UINT nMsg,WPARAM wParam,LPARAM lParam);
CLASS_DECL_BASE WNDPROC __get_window_procedure();
#define __window_procedure (*__get_window_procedure())
extern CLASS_DECL_BASE const unichar gen_Wnd[];           // simple child windows/controls
extern CLASS_DECL_BASE const unichar gen_WndControlBar[]; // controls with gray backgrounds
extern CLASS_DECL_BASE const unichar gen_WndMDIFrame[];
extern CLASS_DECL_BASE const unichar gen_WndFrameOrView[];
extern CLASS_DECL_BASE const unichar gen_WndOleControl[];
CLASS_DECL_BASE bool __is_combo_box_control(oswindow oswindow,UINT nStyle);
CLASS_DECL_BASE bool __check_center_dialog(const char * lpszResource);
CLASS_DECL_BASE bool __compare_class_name(oswindow oswindow,const char * lpszClassName);
CLASS_DECL_BASE oswindow __child_window_from_point(oswindow,POINT);
CLASS_DECL_BASE WNDPROC __get_window_procedure();
// interaction_impl creation hooking
CLASS_DECL_BASE bool hook_window_create(::windows::interaction_impl * pwindow);
CLASS_DECL_BASE bool unhook_window_create();
CLASS_DECL_BASE void reset_message_cache();
CLASS_DECL_BASE wstring get_user_interaction_window_class(::user::interaction * pui);
