#pragma once

#define WH_SHSTDAPI(type) type


namespace linux
{

class CLASS_DECL_ACE shell
{
public:
   shell();
   virtual ~shell();


   static shell theLinuxShell;

   static bool Initialize();
// xxx   static WINBOOL WINAPI _SHGetPathFromIDList(LPCITEMIDLIST pidl, unichar * pszPath);
// xxx   static HANDLE WINAPI _FindFirstFile(const unichar * lpcsz, WIN32_FIND_DATAW * lpdata);
// xxx   static WINBOOL WINAPI _FindNextFile(HANDLE handle, WIN32_FIND_DATAW * lpdata);
/*   static WINBOOL WINAPI _MoveFile(const unichar * lpExistingFileName, const unichar * lpNewFileName);
   static DWORD WINAPI _GetFullPathName(const unichar * lpFileName, DWORD nBufferLength, unichar * lpBuffer, unichar ** lpFilePart);
   static WCHAR * __cdecl __fullpath(WCHAR *UserBuf, const WCHAR *path, size_t maxlen);
   static WINBOOL WINAPI _GetVolumeInformation(
      const unichar * lpRootPathName,           // root directory
      unichar * lpVolumeNameBuffer,        // volume name buffer
      DWORD nVolumeNameSize,            // length of name buffer
      LPDWORD lpVolumeSerialNumber,     // volume serial number
      LPDWORD lpMaximumComponentLength, // maximum file name length
      LPDWORD lpFileSystemFlags,        // file system options
      unichar * lpFileSystemNameBuffer,    // file system name buffer
      DWORD nFileSystemNameSize);         // length of file system name buffer
// xxx   static DWORD_PTR __stdcall _SHGetFileInfo(
      // xxx const unichar * pszPath,
      // xxx DWORD dwFileAttributes,
      // xxx SHFILEINFOW *psfi,
      // xxx UINT cbFileInfo,
      // xxx UINT uFlags);
   static WINBOOL __stdcall _GetStringTypeEx(
      LCID Locale,
      DWORD dwInfoType,
      const unichar * lpSrcStr,
      int32_t cchSrc,
      LPWORD lpCharType);
   static DWORD WINAPI  _GetTempPath(
      DWORD nBufferLength,
      unichar * lpBuffer);
   static UINT WINAPI _GetTempFileName(
      const unichar * lpPathName,
      const unichar * lpPrefixString,
      UINT uUnique,
      unichar * lpTempFileName);
   static HANDLE WINAPI _CreateFile(
      const unichar * lpFileName,
      DWORD dwDesiredAccess,
      DWORD dwShareMode,
      LPSECURITY_ATTRIBUTES lpSecurityAttributes,
      DWORD dwCreationDisposition,
      DWORD dwFlagsAndAttributes,
      HANDLE hTemplateFile
      );
   static DWORD WINAPI _GetModuleFileName(
       HMODULE hModule,
       unichar * lpFilename,
       DWORD nSize
       );
// xxx    static WINBOOL WINAPI _GetClassInfo(
// xxx        HINSTANCE hInstance ,
// xxx        const unichar * lpClassName,
// xxx        LPWNDCLASSW lpWndClass);
// xxx    static ATOM WINAPI _RegisterClass(
         // xxx CONST WNDCLASSW *lpWndClass);

   static oswindow WINAPI _CreateWindowEx(
      DWORD dwExStyle,
      const unichar * lpClassName,
      const unichar * lpWindowName,
      DWORD dwStyle,
      int32_t x,
      int32_t y,
      int32_t nWidth,
      int32_t nHeight,
      oswindow hWndParent,
      HMENU hMenu,
      HINSTANCE hInstance,
      LPVOID lpParam);





// xxx    WINBOOL (WINAPI * m_pfnSHGetPathFromIDList)(LPCITEMIDLIST pidl, unichar * pszPath);
// xxx    HANDLE (WINAPI * m_pfnFindFirstFile)(const unichar * lpcsz, WIN32_FIND_DATAW * lpdata);
// xxx    WINBOOL (WINAPI * m_pfnFindNextFile)(HANDLE handle, WIN32_FIND_DATAW * lpdata);
   WINBOOL (WINAPI * m_pfnMoveFile)(const unichar * lpExistingFileName, const unichar * lpNewFileName);
   WCHAR * (__cdecl * m_pfn_fullpath)(WCHAR *UserBuf, const WCHAR *path, size_t maxlen);
   DWORD (WINAPI * m_pfnGetFullPathName)(const unichar * lpFileName, DWORD nBufferLength, unichar * lpBuffer, unichar ** lpFilePart);
   WINBOOL (WINAPI * m_pfnGetVolumeInformation)(
      const unichar * lpRootPathName,           // root directory
      unichar * lpVolumeNameBuffer,        // volume name buffer
      DWORD nVolumeNameSize,            // length of name buffer
      LPDWORD lpVolumeSerialNumber,     // volume serial number
      LPDWORD lpMaximumComponentLength, // maximum file name length
      LPDWORD lpFileSystemFlags,        // file system options
      unichar * lpFileSystemNameBuffer,    // file system name buffer
      DWORD nFileSystemNameSize);         // length of file system name buffer

// xxx    DWORD_PTR (__stdcall * m_pfnSHGetFileInfo)(
// xxx       const unichar * pszPath,
// xxx       DWORD dwFileAttributes,
// xxx       SHFILEINFOW *psfi,
// xxx       UINT cbFileInfo,
// xxx       UINT uFlags);
   WINBOOL (__stdcall * m_pfnGetStringTypeEx)(
      LCID Locale,
      DWORD dwInfoType,
      const unichar * lpSrcStr,
      int32_t cchSrc,
      LPWORD lpCharType);
   DWORD (WINAPI * m_pfnGetTempPath)(
      DWORD nBufferLength,
      unichar * lpBuffer);
   UINT (WINAPI * m_pfnGetTempFileName)(
      const unichar * lpPathName,
      const unichar * lpPrefixString,
      UINT uUnique,
      unichar * lpTempFileName);
   HANDLE (WINAPI * m_pfnCreateFile)(
      const unichar * lpFileName,
      DWORD dwDesiredAccess,
      DWORD dwShareMode,
      LPSECURITY_ATTRIBUTES lpSecurityAttributes,
      DWORD dwCreationDisposition,
      DWORD dwFlagsAndAttributes,
      HANDLE hTemplateFile
      );

   DWORD (WINAPI * m_pfnGetModuleFileName)(
       HMODULE hModule,
       unichar * lpFilename,
       DWORD nSize
       );

// xxx    WINBOOL (WINAPI * m_pfnGetClassInfo)(
       // xxx HINSTANCE hInstance ,
// xxx        const unichar * lpClassName,
// xxx        LPWNDCLASSW lpWndClass);

   // xxx ATOM (WINAPI * m_pfnRegisterClass)(
         // xxx CONST WNDCLASSW *lpWndClass);

   oswindow (WINAPI * m_pfnCreateWindowEx)(
      DWORD dwExStyle,
      const unichar * lpClassName,
      const unichar * lpWindowName,
      DWORD dwStyle,
      int32_t x,
      int32_t y,
      int32_t nWidth,
      int32_t nHeight,
      oswindow hWndParent,
      HMENU hMenu,
      HINSTANCE hInstance,
      LPVOID lpParam);








// xxx    static WINBOOL SHGetPathFromIDList(LPCITEMIDLIST pidl, string & str);
// xxx    static WINBOOL SHGetPathFromIDList(LPCITEMIDLIST pidl, unichar * pszPath);
// xxx    static HANDLE FindFirstFile(const unichar * lpcsz, WIN32_FIND_DATAW * lpdata);
// xxx    static WINBOOL WINAPI FindNextFile(HANDLE handle, WIN32_FIND_DATAW * lpdata);
   static WINBOOL MoveFile(const unichar * lpExistingFileName, const unichar * lpNewFileName);
   static WCHAR * __cdecl _fullpath(WCHAR *UserBuf, const WCHAR *path, size_t maxlen);
   static DWORD GetFullPathName(const unichar * lpFileName, DWORD nBufferLength, unichar * lpBuffer, unichar ** lpFilePart);
   static WINBOOL GetVolumeInformation(
      const unichar * lpRootPathName,           // root directory
      unichar * lpVolumeNameBuffer,        // volume name buffer
      DWORD nVolumeNameSize,            // length of name buffer
      LPDWORD lpVolumeSerialNumber,     // volume serial number
      LPDWORD lpMaximumComponentLength, // maximum file name length
      LPDWORD lpFileSystemFlags,        // file system options
      unichar * lpFileSystemNameBuffer,    // file system name buffer
      DWORD nFileSystemNameSize);         // length of file system name buffer

// xxx    static WH_SHSTDAPI(DWORD) SHGetFileInfo(
// xxx       const unichar * pszPath,
// xxx       DWORD dwFileAttributes,
// xxx       SHFILEINFOW *psfi,
// xxx       UINT cbFileInfo,
// xxx       UINT uFlags);

   static WINBOOL  GetStringTypeEx(
      LCID Locale,
      DWORD dwInfoType,
      const char * lpSrcStr,
      int32_t cchSrc,
      LPWORD lpCharType);

   static DWORD GetTempPathW(
      DWORD nBufferLength,
      unichar * lpBuffer);

   static UINT GetTempFileNameW(
      const unichar * lpPathName,
      const unichar * lpPrefixString,
      UINT uUnique,
      unichar * lpTempFileName);

   static HANDLE CreateFile(
      const unichar * lpFileName,
      DWORD dwDesiredAccess,
      DWORD dwShareMode,
      LPSECURITY_ATTRIBUTES lpSecurityAttributes,
      DWORD dwCreationDisposition,
      DWORD dwFlagsAndAttributes,
      HANDLE hTemplateFile);

   static DWORD WINAPI GetModuleFileName(
       HMODULE hModule,
        unichar * lpFilename,
        DWORD nSize
       );

// xxx    static WINBOOL WINAPI GetClassInfo(
// xxx        HINSTANCE hInstance ,
// xxx        const unichar * lpClassName,
// xxx        LPWNDCLASSW lpWndClass);

// xxx    static ATOM WINAPI RegisterClass(
// xxx          CONST WNDCLASSW *lpWndClass);

   static oswindow WINAPI CreateWindowEx(
      DWORD dwExStyle,
      const unichar * lpClassName,
      const unichar * lpWindowName,
      DWORD dwStyle,
      int32_t x,
      int32_t y,
      int32_t nWidth,
      int32_t nHeight,
      oswindow hWndParent,
      HMENU hMenu,
      HINSTANCE hInstance,
      LPVOID lpParam);


// xxx    static WH_SHSTDAPI(DWORD) SHGetFileInfo(
// xxx       const char * pszPath,
// xxx       DWORD dwFileAttributes,
// xxx       SHFILEINFO *psfi,
// xxx       UINT cbFileInfo,
// xxx       UINT uFlags);

   static DWORD GetTempPath(string & str);

   static UINT GetTempFileName(
      const char * lpPathName,
      const char * lpPrefixString,
      UINT uUnique,
      string & str);

   static WINBOOL MoveFile(const char * lpExistingFileName, const char * lpNewFileName);
*/
};



/*inline WINBOOL shell::SHGetPathFromIDList(LPCITEMIDLIST pidl, string & str)
{
   unichar * pwszPath = (unichar *) malloc(sizeof(unichar) * MAX_PATH * 4);
   if(!(*theLinuxShell.m_pfnSHGetPathFromIDList)(pidl, pwszPath))
   {
      free(pwszPath);
      return FALSE;
   }
   ::str::international::unicode_to_utf8(str, pwszPath);
   free(pwszPath);
   return TRUE;
}
*/
/*

inline WINBOOL shell::SHGetPathFromIDList(LPCITEMIDLIST pidl, unichar * pszPath)
{
   return (*theLinuxShell.m_pfnSHGetPathFromIDList)(pidl, pszPath);
}
*/
/*
inline HANDLE shell::FindFirstFile(const unichar * lpcsz, WIN32_FIND_DATAW * lpdata)
{
   return (*theLinuxShell.m_pfnFindFirstFile)(lpcsz, lpdata);
}

inline WINBOOL shell::FindNextFile(HANDLE handle, WIN32_FIND_DATAW * lpdata)
{
   return (*theLinuxShell.m_pfnFindNextFile)(handle, lpdata);
}

*/

/*
inline WCHAR * shell::_fullpath(WCHAR *UserBuf, const WCHAR *path, size_t maxlen)
{
   return (*theLinuxShell.m_pfn_fullpath)(UserBuf, path, maxlen);
}

inline DWORD shell::GetFullPathName(const unichar * lpFileName, DWORD nBufferLength, unichar * lpBuffer, unichar ** lpFilePart)
{
   return (*theLinuxShell.m_pfnGetFullPathName)(lpFileName, nBufferLength, lpBuffer, lpFilePart);
}


inline WINBOOL shell::MoveFile(const unichar * lpExistingFileName, const unichar * lpNewFileName)
{
   return (*theLinuxShell.m_pfnMoveFile)(lpExistingFileName, lpNewFileName);
}

inline WINBOOL shell::GetVolumeInformation(
      const unichar * lpRootPathName,           // root directory
      unichar * lpVolumeNameBuffer,        // volume name buffer
      DWORD nVolumeNameSize,            // length of name buffer
      LPDWORD lpVolumeSerialNumber,     // volume serial number
      LPDWORD lpMaximumComponentLength, // maximum file name length
      LPDWORD lpFileSystemFlags,        // file system options
      unichar * lpFileSystemNameBuffer,    // file system name buffer
      DWORD nFileSystemNameSize)         // length of file system name buffer
{
   return (*theLinuxShell.m_pfnGetVolumeInformation)(
      lpRootPathName,           // root directory
      lpVolumeNameBuffer,        // volume name buffer
      nVolumeNameSize,            // length of name buffer
      lpVolumeSerialNumber,     // volume serial number
      lpMaximumComponentLength, // maximum file name length
      lpFileSystemFlags,        // file system options
      lpFileSystemNameBuffer,    // file system name buffer
      nFileSystemNameSize);         // length of file system name buffer
}

/*
inline DWORD shell::SHGetFileInfo(
   const unichar * pszPath,
   DWORD dwFileAttributes,
   SHFILEINFOW *psfi,
   UINT cbFileInfo,
   UINT uFlags)
{
   return (*theLinuxShell.m_pfnSHGetFileInfo)(
      pszPath,
      dwFileAttributes,
      psfi,
      cbFileInfo,
      uFlags);
}
*/
/*
inline DWORD shell::GetTempPathW(
   DWORD nBufferLength,
   unichar * lpBuffer)
{
     return (*theLinuxShell.m_pfnGetTempPath)(
      nBufferLength,
      lpBuffer);
}

inline UINT shell::GetTempFileNameW(
   const unichar * lpPathName,
   const unichar * lpPrefixString,
   UINT uUnique,
   unichar * lpTempFileName)
{
   return (*theLinuxShell.m_pfnGetTempFileName)(
      lpPathName,
      lpPrefixString,
      uUnique,
      lpTempFileName);
}

inline HANDLE shell::CreateFile(
   const unichar * lpFileName,
   DWORD dwDesiredAccess,
   DWORD dwShareMode,
   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   DWORD dwCreationDisposition,
   DWORD dwFlagsAndAttributes,
   HANDLE hTemplateFile)
{
   return (*theLinuxShell.m_pfnCreateFile)(
      lpFileName,
      dwDesiredAccess,
      dwShareMode,
      lpSecurityAttributes,
      dwCreationDisposition,
      dwFlagsAndAttributes,
      hTemplateFile);
}

inline DWORD shell::GetModuleFileName(
    HMODULE hModule,
    unichar * lpFilename,
    DWORD nSize
    )
{
   return (*theLinuxShell.m_pfnGetModuleFileName)(
      hModule,
      lpFilename,
      nSize);
}

/*inline WINBOOL shell::GetClassInfo(
   HINSTANCE hInstance ,
   const unichar * lpClassName,
   LPWNDCLASSW lpWndClass)
{
   return (*theLinuxShell.m_pfnGetClassInfo)(
      hInstance ,
      lpClassName,
      lpWndClass);
}

inline ATOM shell::RegisterClass(
   CONST WNDCLASSW *lpWndClass)
{
   return (*theLinuxShell.m_pfnRegisterClass)(
      lpWndClass);
}*/
/*
inline oswindow shell::CreateWindowEx(
   DWORD dwExStyle,
   const unichar * lpClassName,
   const unichar * lpWindowName,
   DWORD dwStyle,
   int32_t x,
   int32_t y,
   int32_t nWidth,
   int32_t nHeight,
   oswindow hWndParent,
   HMENU hMenu,
   HINSTANCE hInstance,
   LPVOID lpParam)
{
   return (*theLinuxShell.m_pfnCreateWindowEx)(
      dwExStyle,
      lpClassName,
      lpWindowName,
      dwStyle,
      x,
      y,
      nWidth,
      nHeight,
      hWndParent,
      hMenu,
      hInstance,
      lpParam);

}

inline DWORD shell::GetTempPath(string & str)
{
   unichar * pwsz = (unichar *) malloc(sizeof(unichar) * MAX_PATH * 4);
   DWORD dwResult = (*theLinuxShell.m_pfnGetTempPath)(sizeof(unichar) * MAX_PATH * 4, pwsz);
   ::str::international::unicode_to_utf8(str, pwsz);
   free(pwsz);
   return dwResult;
}

inline UINT shell::GetTempFileName(const char * lpPathName, const char * lpPrefixString, UINT uUnique, string & str)
{
   wstring wstrPathName = ::str::international::utf8_to_unicode(lpPathName);
   wstring wstrPrefixString = ::str::international::utf8_to_unicode(lpPrefixString);
   wstring wstr;
   wstr.alloc(MAX_PATH * 4);
   UINT uiResult = (*theLinuxShell.m_pfnGetTempFileName)(
      wstrPathName,
      wstrPrefixString,
      uUnique,
      wstr);
   ::str::international::unicode_to_utf8(str, wstr);
   return uiResult;
}

inline WINBOOL shell::MoveFile(const char * lpExistingFileName, const char * lpNewFileName)
{
   return MoveFile(::str::international::utf8_to_unicode(lpExistingFileName), ::str::international::utf8_to_unicode(lpNewFileName));
}

*/


} // namespace linux

