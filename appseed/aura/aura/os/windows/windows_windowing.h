#pragma once


CLASS_DECL_AURA const unichar * windows_register_window_class(::aura::application * papp, UINT nClassStyle, HCURSOR hCursor = 0, HBRUSH hbrBackground = 0, HICON hIcon = 0);
CLASS_DECL_AURA bool windows_register_class(WNDCLASSW* lpWndClass);
CLASS_DECL_AURA wstring windows_calc_icon_window_class(::user::interaction * pui, uint32_t dwDefaultStyle, const char * pszMatter);
CLASS_DECL_AURA wstring windows_get_user_interaction_window_class(::user::interaction * pui);
CLASS_DECL_AURA bool windows_register_with_icon(WNDCLASSW* pWndCls, const unichar * lpszClassName, UINT nIDIcon);




