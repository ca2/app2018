#pragma once


#ifdef SetWindowLongPtrA
#undef SetWindowLongPtrA
inline int_ptr SetWindowLongPtrA( oswindow oswindow, int32_t nIndex, int_ptr dwNewLong )
{
   return( ::SetWindowLongA( oswindow, nIndex, LONG( dwNewLong ) ) );
}
#endif

#ifdef SetWindowLongPtrW
#undef SetWindowLongPtrW
inline int_ptr SetWindowLongPtrW( oswindow oswindow, int32_t nIndex, int_ptr dwNewLong )
{
   return( ::SetWindowLongW( oswindow, nIndex, LONG( dwNewLong ) ) );
}
#endif

#ifdef GetWindowLongPtrA
#undef GetWindowLongPtrA
inline int_ptr GetWindowLongPtrA( oswindow oswindow, int32_t nIndex )
{
   return( ::GetWindowLongA( oswindow, nIndex ) );
}
#endif

#ifdef GetWindowLongPtrW
#undef GetWindowLongPtrW
inline int_ptr GetWindowLongPtrW( oswindow oswindow, int32_t nIndex )
{
   return( ::GetWindowLongW( oswindow, nIndex ) );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Auxiliary System/Screen metrics
/*
struct CLASS_DECL_ca2 AUX_DATA
{
   // system metrics
   int32_t cxVScroll, cyHScroll;
   int32_t cxIcon, cyIcon;

   int32_t cxBorder2, cyBorder2;

   // device metrics for screen
   int32_t cxPixelsPerInch, cyPixelsPerInch;

   // convenient system color
   HBRUSH hbrWindowFrame;
   HBRUSH hbrBtnFace;

   // color values of system colors used for simple_toolbar
   COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
   COLORREF clrBtnText, clrWindowFrame;

   // standard cursors
   HCURSOR hcurWait;
   HCURSOR hcurArrow;
   HCURSOR hcurHelp;       // cursor used in Shift+F1 help

   // special GDI objects allocated on demand
   HFONT   hStatusFont;
   HFONT   hToolTipsFont;
   HBITMAP hbmMenuDot;

   // other system information
   bool    bWin95;         // TRUE if Windows 95 (not NT)
   bool    bWin4;

// Implementation
   AUX_DATA();
   ~AUX_DATA();
   void UpdateSysColors();
   void UpdateSysMetrics();
};

extern CLASS_DECL_ca2 AUX_DATA afxData;*/




// Note: afxData.cxBorder and afxData.cyBorder aren't used anymore
#define CX_BORDER   1
#define CY_BORDER   1

// states for Shift+F1 hep mode
#define HELP_INACTIVE   0   // not in Shift+F1 help mode (must be 0)
#define HELP_ACTIVE     1   // in Shift+F1 help mode (non-zero)
#define HELP_ENTERING   2   // entering Shift+F1 help mode (non-zero)

/////////////////////////////////////////////////////////////////////////////
// Window class names and other ::ca::window creation support


#define __WND_REG                                             0x00001
#define __WNDCONTROLBAR_REG                   0x00002
#define __WNDMDIFRAME_REG                             0x00004
#define __WNDFRAMEORVIEW_REG                  0x00008
#define __WNDCOMMCTLS_REG                             0x00010 // means all original Win95
#define __WNDOLECONTROL_REG                   0x00020
#define __WNDCOMMCTL_UPDOWN_REG       0x00040 // these are original Win95
#define __WNDCOMMCTL_TREEVIEW_REG     0x00080
#define __WNDCOMMCTL_TAB_REG                  0x00100
#define __WNDCOMMCTL_PROGRESS_REG             0x00200
#define __WNDCOMMCTL_LISTVIEW_REG         0x00400
#define __WNDCOMMCTL_HOTKEY_REG           0x00800
#define __WNDCOMMCTL_BAR_REG              0x01000
#define __WNDCOMMCTL_ANIMATE_REG              0x02000
#define __WNDCOMMCTL_INTERNET_REG             0x04000 // these are new in IE4
#define __WNDCOMMCTL_COOL_REG             0x08000
#define __WNDCOMMCTL_USEREX_REG       0x10000
#define __WNDCOMMCTL_DATE_REG             0x20000
#define __WNDCOMMCTL_LINK_REG             0x40000      // new in IE6

#define __WIN95CTLS_MASK                              0x03FC0 // UPDOWN -> ANIMATE
#define __WNDCOMMCTLSALL_REG                  0x7C010 // COMMCTLS|INTERNET|COOL|USEREX|DATE|LINK
#define __WNDCOMMCTLSNEW_REG                  0x7C000 // INTERNET|COOL|USEREX|DATE


// ca API has its own version of the TOOLINFO structure containing the
// the Win95 base version of the structure. Since ca API targets Win95 base,
// we need this structure so calls into that old library don't fail.

typedef struct tag__OLDTOOLINFO {
   UINT cbSize;
   UINT uFlags;
   oswindow oswindow;
   UINT uId;
   RECT rect;
   HINSTANCE hinst;
   LPTSTR lpszText;
} __OLDTOOLINFO;

// special _ ::ca::window class name mangling


#define __WNDCLASS(s)    "ca2" _T(s)
#define __WND             __WNDCLASS("Wnd")
#define __WNDCONTROLBAR   __WNDCLASS("ControlBar")
#define __WNDMDIFRAME     __WNDCLASS("MDIFrame")
#define __WNDFRAMEORVIEW  __WNDCLASS("FrameOrView")
#define __WNDOLECONTROL   __WNDCLASS("OleControl")

// dialog/commdlg hook procs
CLASS_DECL_ca2 int_ptr CALLBACK __dialog_procedure(oswindow, UINT, WPARAM, LPARAM);
CLASS_DECL_ca2 uint_ptr CALLBACK __common_dialog_procedure(oswindow oswindow, UINT, WPARAM, LPARAM);

// support for standard dialogs
extern UINT gen_MsgSETRGB;
typedef uint_ptr (CALLBACK* COMMDLGPROC)(oswindow, UINT, WPARAM, LPARAM);

// conversion helpers
//int32_t c_cdecl _wcstombsz(char* mbstr, const wchar_t* wcstr, size_t count);
//int32_t c_cdecl _mbstowcsz(wchar_t* wcstr, const char* mbstr, size_t count);

/////////////////////////////////////////////////////////////////////////////
// Extended dialog templates (new in Win95)

typedef struct
{
   WORD dlgVer;
   WORD signature;
   uint32_t helpID;
   uint32_t exStyle;
   uint32_t style;
   WORD cDlgItems;
   int16_t x;
   int16_t y;
   int16_t cx;
   int16_t cy;
} DLGTEMPLATEEX;

typedef struct
{
   uint32_t helpID;
   uint32_t exStyle;
   uint32_t style;
   int16_t x;
   int16_t y;
   int16_t cx;
   int16_t cy;
   uint32_t id;
} DLGITEMTEMPLATEEX;

/////////////////////////////////////////////////////////////////////////////
// Special helpers




/*// UNICODE/MBCS abstractions
#ifdef _MBCS
   extern CLASS_DECL_ca2 const bool gen_DBCS;
#else
   #define gen_DBCS FALSE
#endif
   */
// determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifndef ___PORTABLE
int32_t c_cdecl __critical_new_handler(size_t nSize);
#endif


/////////////////////////////////////////////////////////////////////////////
// locale-invariant comparison helpers till CRT gets that support
inline int32_t __invariant_stricmp(const char *pszLeft, const char *pszRight)
{
#ifdef WINDOWSEX
    return ::CompareStringA(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),
                            NORM_IGNORECASE,
                            pszLeft,
                            -1,
                            pszRight,
                            -1)-CSTR_EQUAL;
#else
   return stricmp_dup(pszLeft, pszRight);
#endif
}

inline int32_t __invariant_stricmp(const wchar_t *pwszLeft, const wchar_t *pwszRight)
{
#ifdef WINDOWSEX
    return ::CompareStringW(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US),SORT_DEFAULT),
                            NORM_IGNORECASE,
                            pwszLeft,
                            -1,
                            pwszRight,
                            -1)-CSTR_EQUAL;
#else
   return wcsicmp_dup(pwszLeft, pwszRight);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// static exceptions

extern not_supported_exception   _simpleNotSupportedException;
extern memory_exception          _simpleMemoryException;
extern user_exception            _simpleUserException;
extern resource_exception _simpleResourceException;

/////////////////////////////////////////////////////////////////////////////
// useful message ranges

#define WM_SYSKEYFIRST  WM_SYSKEYDOWN
#define WM_SYSKEYLAST   WM_SYSDEADCHAR

#define WM_NCMOUSEFIRST WM_NCMOUSEMOVE
#define WM_NCMOUSELAST  WM_NCMBUTTONDBLCLK


/////////////////////////////////////////////////////////////////////////////
// Portability abstractions

#define __set_dialog_control_id(oswindow, nID)     SetWindowLong(oswindow, GWL_ID, nID)
#define __set_dialog_control_id_(oswindow, nID)     oswindow->SetWindowLong(GWL_ID, nID)
#define __get_dialog_control_id(oswindow)         ((UINT)(WORD)::GetDlgCtrlID(oswindow))
#define __get_dialog_control_id_(oswindow)         ((UINT)(WORD)oswindow->GetDlgCtrlId())

// misc helpers
CLASS_DECL_ca2 void __get_root_path(const char * lpszPath, string & strRoot);

#ifndef ___NO_OLE_SUPPORT
class CLASS_DECL_ca2 __COM
{
public:
   HRESULT CreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter,
      REFIID riid, LPVOID* ppv);
   HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
};

CLASS_DECL_ca2 string __string_from_clsid(REFCLSID rclsid);
CLASS_DECL_ca2 bool __get_in_proc_server(const char * lpszCLSID, string & str);
#endif // ___NO_OLE_SUPPORT

#define NULL_TLS ((uint32_t)-1)

/////////////////////////////////////////////////////////////////////////////
// Message map and message dispatch



/////////////////////////////////////////////////////////////////////////////
// Debugging/Tracing helpers

#ifdef DEBUG
   CLASS_DECL_ca2 bool __check_dialog_template(const char * lpszResource, bool bInvisibleChild);
#endif





