#pragma once

CLASS_DECL_AURA string get_user_interaction_window_class(::user::interaction * pui);





// from wincore.cpp
extern CLASS_DECL_AURA const char gen_Wnd[];           // simple child windows/controls
extern CLASS_DECL_AURA const char gen_WndControlBar[]; // controls with gray backgrounds
extern CLASS_DECL_AURA const char gen_WndMDIFrame[];
extern CLASS_DECL_AURA const char gen_WndFrameOrView[];
extern CLASS_DECL_AURA const char gen_WndOleControl[];


/////////////////////////////////////////////////////////////////////////////
// Special helpers

CLASS_DECL_AURA void __cancel_modes(oswindow oswindow_Rcvr);
CLASS_DECL_AURA bool __help_enabled();  // determine if ID_HELP handler exists
CLASS_DECL_AURA bool __custom_log_font(UINT nIDS, LOGFONTW* pLogFont);
CLASS_DECL_AURA bool __get_prop_sheet_font(string & strFace, WORD& wSize, bool bWizard);

CLASS_DECL_AURA bool __is_combo_box_control(oswindow oswindow, UINT nStyle);
CLASS_DECL_AURA bool __check_center_dialog(const char * lpszResource);
CLASS_DECL_AURA bool __compare_class_name(oswindow oswindow, const char * lpszClassName);
CLASS_DECL_AURA oswindow __child_window_from_point(oswindow, POINT);

// for determining version of COMCTL32.DLL
#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3             MAKELONG(70, 4)
#define VERSION_IE4             MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)
#define VERSION_6      MAKELONG(0, 6)
extern int32_t gen_ComCtlVersion;
DWORD __get_common_controls_version();
