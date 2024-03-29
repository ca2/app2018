﻿#include "framework.h"


/***
*int32_t _validdrive( uint32_t drive ) -
*
*Purpose: returns non zero if drive is a valid drive number.
*
*Entry: drive = 0 => default drive, 1 => a:, 2 => b: ...
*
*Exit:  0 => drive does not exist.
*
*Exceptions:
*
*******************************************************************************/

int32_t __cdecl _validdrive (
   uint32_t drive
   )
{
   uint32_t retcode;
   char drvstr[4];

   if ( drive == 0 )
      return 1;

   drvstr[0] = (char) ('A' + drive - 1);
   drvstr[1] = ':';
   drvstr[2] = '\\';
   drvstr[3] = '\0';

   if ( ((retcode = GetDriveType( drvstr )) == DRIVE_UNKNOWN) ||
      (retcode == DRIVE_NO_ROOT_DIR) )
      return 0;

   return 1;
}

/*WCHAR * __cdecl _wgetdcwd (
int32_t drive,
WCHAR *pnbuf,
int32_t maxlen
);

*/
/***
*_TSCHAR *_getcwd(pnbuf, maxlen) - get current working directory of default drive
*
*Purpose:
*       _getcwd gets the current working directory for the ::fontopus::user,
*       placing it in the buffer pointed to by pnbuf.  It returns
*       the length of the string put in the buffer.  If the length
*       of the string exceeds the length of the buffer, maxlen,
*       then NULL is returned.  If pnbuf = NULL, maxlen is ignored.
*       An entry point "_getdcwd()" is defined with takes the above
*       parameters, plus a drive number.  "_getcwd()" is implemented
*       as a call to "_getcwd()" with the default drive (0).
*
*       If pnbuf = NULL, maxlen is ignored, and a buffer is automatically
*       allocated using malloc() -- a pointer to which is returned by
*       _getcwd().
*
*       side effects: no global data is used or affected
*
*Entry:
*       _TSCHAR *pnbuf = pointer to a buffer maintained by the ::fontopus::user;
*       int32_t maxlen = length of the buffer pointed to by pnbuf;
*
*Exit:
*       Returns pointer to the buffer containing the ca.w.d. name
*       (same as pnbuf if non-NULL; otherwise, malloc is
*       used to allocate a buffer)
*
*Exceptions:
*
*******************************************************************************/

/*WCHAR * __cdecl _wgetcwd (
WCHAR *pnbuf,
int32_t maxlen
)
{
WCHAR *retval;


retval = _wgetdcwd(0, pnbuf, maxlen);

return retval;
}*/


/***
*_TSCHAR *_getdcwd(drive, pnbuf, maxlen) - get ca.w.d. for given drive
*
*Purpose:
*       _getdcwd gets the current working directory for the ::fontopus::user,
*       placing it in the buffer pointed to by pnbuf.  It returns
*       the length of the string put in the buffer.  If the length
*       of the string exceeds the length of the buffer, maxlen,
*       then NULL is returned.  If pnbuf = NULL, maxlen is ignored,
*       and a buffer is automatically allocated using malloc() --
*       a pointer to which is returned by _getdcwd().
*
*       side effects: no global data is used or affected
*
*Entry:
*       int32_t drive   - number of the drive being inquired about
*                     0 = default, 1 = 'a:', 2 = 'b:', etc.
*       _TSCHAR *pnbuf - pointer to a buffer maintained by the ::fontopus::user;
*       int32_t maxlen  - length of the buffer pointed to by pnbuf;
*
*Exit:
*       Returns pointer to the buffer containing the ca.w.d. name
*       (same as pnbuf if non-NULL; otherwise, malloc is
*       used to allocate a buffer)
*
*Exceptions:
*
*******************************************************************************/


/*WCHAR * __cdecl _wgetdcwd (
int32_t drive,
WCHAR *pnbuf,
int32_t maxlen
)
{
WCHAR *p;
WCHAR dirbuf[_MAX_PATH];
WCHAR drvstr[4];
int32_t len;
WCHAR *pname; /* only used as argument to GetFullPathName */

/*
* GetCurrentDirectory only works for the default drive in Win32
*/
/*      if ( drive != 0 ) {
/*
* Not the default drive - make sure it's valid.
*/
/*        if ( !_validdrive(drive) ) {
_doserrno = ERROR_INVALID_DRIVE;
//                errno = EACCES;
return NULL;
}

/*
* get the current directory string on that drive and its length
*/
/*      drvstr[0] = (char) ('A' - 1 + drive);
drvstr[1] = ':';
drvstr[2] = '.';
drvstr[3] = '\0';
len = shell::GetFullPathName( drvstr,
sizeof(dirbuf) / sizeof(_TSCHAR),
dirbuf,
&pname );

} else {

/*
* get the current directory string and its length
*/
/*            len = GetCurrentDirectory( sizeof(dirbuf) / sizeof(_TSCHAR),
(LPTSTR)dirbuf );
}

/* API call failed, or buffer not large enough */
/*      if ( len == 0 || ++len > sizeof(dirbuf)/sizeof(_TSCHAR) )
return NULL;

/*
* set up the buffer.
*/
/*    if ( (p = pnbuf) == NULL ) {
/*
* allocate a buffer for the ::fontopus::user.
*/
/*      if ( (p = (WCHAR *)malloc(__max(len, maxlen) * sizeof(WCHAR)))
== NULL )
{
//                errno = ENOMEM;
return NULL;
}
}
else if ( len > maxlen ) {
/*
* Won't fit in the ::fontopus::user-supplied buffer!
*/
//            errno = ERANGE; /* Won't fit in ::fontopus::user buffer */
/*    return NULL;
}

/*
* Place the current directory string into the ::fontopus::user buffer
*/
/*
return wcscpy(p, dirbuf);
}

/*#ifndef WPRFLAG

/***
*int32_t _validdrive( uint32_t drive ) -
*
*Purpose: returns non zero if drive is a valid drive number.
*
*Entry: drive = 0 => default drive, 1 => a:, 2 => b: ...
*
*Exit:  0 => drive does not exist.
*
*Exceptions:
*
*******************************************************************************/

/*int32_t __cdecl _validdrive (
uint32_t drive
)
{
uint32_t retcode;
char drvstr[4];

if ( drive == 0 )
return 1;

drvstr[0] = 'A' + drive - 1;
drvstr[1] = ':';
drvstr[2] = '\\';
drvstr[3] = '\0';

if ( ((retcode = GetDriveType( drvstr )) == DRIVE_UNKNOWN) ||
(retcode == DRIVE_NO_ROOT_DIR) )
return 0;

return 1;
}*/

namespace windows
{

   shell shell::theWindowsShell;


   shell::shell()
   {

   }

   shell::~shell()
   {

   }

   bool shell::Initialize()
   {
      
         theWindowsShell.m_pfnSHGetPathFromIDList = ::SHGetPathFromIDListW;
         theWindowsShell.m_pfnFindFirstFile = ::FindFirstFileW;
         theWindowsShell.m_pfnFindNextFile = ::FindNextFileW;
         theWindowsShell.m_pfnMoveFile = ::MoveFileW;
         theWindowsShell.m_pfn_fullpath = ::_wfullpath;
         theWindowsShell.m_pfnGetFullPathName = ::GetFullPathNameW;
         theWindowsShell.m_pfnGetVolumeInformation = ::GetVolumeInformationW;
         theWindowsShell.m_pfnSHGetFileInfo = ::SHGetFileInfoW;
         theWindowsShell.m_pfnGetStringTypeEx = ::GetStringTypeExW;
         theWindowsShell.m_pfnGetTempPath = ::GetTempPathW;
         theWindowsShell.m_pfnGetTempFileName = ::GetTempFileNameW;
         theWindowsShell.m_pfnCreateFile = ::CreateFileW;
         theWindowsShell.m_pfnGetModuleFileName = ::GetModuleFileNameW;
         theWindowsShell.m_pfnGetClassInfo = ::GetClassInfoW;
         theWindowsShell.m_pfnRegisterClass = ::RegisterClassW;
         theWindowsShell.m_pfnCreateWindowEx = ::CreateWindowExW;
      return true;
   }

/*   BOOL shell::_SHGetPathFromIDList(LPCITEMIDLIST pidl, unichar * pszPath)
   {
      CHAR pszPathA[MAX_PATH * 2];
      if(!::SHGetPathFromIDListA(pidl, pszPathA))
         return FALSE;
      return ::str::international::utf8_to_unicode(pszPath, MAX_PATH * 2, pszPathA) ? TRUE : FALSE;
   }

   BOOL shell::_MoveFile(const unichar * lpExistingFileName, const unichar * lpNewFileName)
   {
      string str1, str2;
      ::str::international::unicode_to_utf8(str1, lpExistingFileName);
      ::str::international::unicode_to_utf8(str2, lpNewFileName);
      return ::MoveFileA(str1, str2);
   }

   HANDLE shell::_FindFirstFile(const unichar * lpcsz, WIN32_FIND_DATAW * lpdata)
   {
      string 
      ::str::international::unicode_to_utf8(pszPathA, MAX_PATH * 2, lpcsz);
      WIN32_FIND_DATAA data;
      HANDLE handle = ::FindFirstFileA(pszPathA, &data);
      if(handle == INVALID_HANDLE_VALUE)
         return INVALID_HANDLE_VALUE;

      lpdata->dwFileAttributes = data.dwFileAttributes;
      lpdata->ftCreationTime = data.ftCreationTime;
      lpdata->ftLastAccessTime = data.ftLastAccessTime;
      lpdata->ftLastWriteTime = data.ftLastWriteTime;
      lpdata->nFileSizeHigh = data.nFileSizeHigh;
      lpdata->nFileSizeLow = data.nFileSizeLow;
      lpdata->dwReserved0 = data.dwReserved0;
      lpdata->dwReserved1 = data.dwReserved1;
      ::str::international::ACPToUnicode(lpdata->cFileName, MAX_PATH, data.cFileName);
      ::str::international::ACPToUnicode(lpdata->cAlternateFileName, MAX_PATH, data.cAlternateFileName);

      return handle;
   }


   BOOL shell::_FindNextFile(HANDLE handle, WIN32_FIND_DATAW * lpdata)
   {
      WIN32_FIND_DATAA data;
      bool b = ::FindNextFileA(handle, &data) != FALSE;
      if(b == FALSE)
         return FALSE;

      lpdata->dwFileAttributes = data.dwFileAttributes;
      lpdata->ftCreationTime = data.ftCreationTime;
      lpdata->ftLastAccessTime = data.ftLastAccessTime;
      lpdata->ftLastWriteTime = data.ftLastWriteTime;
      lpdata->nFileSizeHigh = data.nFileSizeHigh;
      lpdata->nFileSizeLow = data.nFileSizeLow;
      lpdata->dwReserved0 = data.dwReserved0;
      lpdata->dwReserved1 = data.dwReserved1;
      ::str::international::ACPToUnicode(lpdata->cFileName, MAX_PATH, data.cFileName);
      ::str::international::ACPToUnicode(lpdata->cAlternateFileName, MAX_PATH, data.cAlternateFileName);

      return b;
   }


   WCHAR * __cdecl shell::__fullpath (
      WCHAR *UserBuf,
      const WCHAR *path,
      size_t maxlen
      )
   {
      WCHAR *buf;
      WCHAR *pfname;
      unsigned long count;


      if ( !path || !*path )  /* no work to do */
/*         return( _wgetcwd( UserBuf, (int32_t) maxlen ) );

      /* allocate buffer if necessary */

   //   if ( !UserBuf )
   //      if ( !(buf = (WCHAR *) malloc(_MAX_PATH * sizeof(WCHAR))) ) {
   //         //                errno = ENOMEM;
   //         return( NULL );
   //      }
   //      else
   //         maxlen = _MAX_PATH;
   //   else
   //      buf = UserBuf;

   //   count = GetFullPathName ( path,
   //      (int32_t) maxlen,
   //      buf,
   //      &pfname );

   //   if ( count >= maxlen ) {
   //      if ( !UserBuf )
   //         free(buf);
   //      //errno = ERANGE;
   //      return( NULL );
   //   }
   //   else if ( count == 0 ) {
   //      if ( !UserBuf )
   //         free(buf);
   //      //_dosmaperr( get_last_error() );
   //      return( NULL );
   //   }

   //   return( buf );

   //}


   //DWORD WINAPI shell::_GetFullPathName(
   //   const unichar * lpFileName, 
   //   DWORD nBufferLength, 
   //   unichar * lpBuffer, 
   //   unichar ** lpFilePart)
   //{
   //   CHAR pszPathA[MAX_PATH * 2];
   //   ::str::international::UnicodeToACP(pszPathA, MAX_PATH * 2, lpFileName);
   //   string str;
   //   LPTSTR lpsz = str.GetBuffer(nBufferLength * 2);
   //   LPTSTR lpszFilePart;
   //   DWORD dw = ::GetFullPathName(pszPathA, nBufferLength, lpsz, &lpszFilePart);
   //   str.ReleaseBuffer();
   //   ::str::international::ACPToUnicode(lpBuffer, nBufferLength, str);
   //   *lpFilePart = lpBuffer + ((int32_t) (lpszFilePart - lpsz));
   //   return dw;
   //}

   //BOOL WINAPI shell::_GetVolumeInformation(
   //   const unichar * lpRootPathName,           // root directory
   //   unichar * lpVolumeNameBuffer,        // volume name buffer
   //   DWORD nVolumeNameSize,            // length of name buffer
   //   LPDWORD lpVolumeSerialNumber,     // volume serial number
   //   LPDWORD lpMaximumComponentLength, // maximum file name length
   //   LPDWORD lpFileSystemFlags,        // file system options
   //   unichar * lpFileSystemNameBuffer,    // file system name buffer
   //   DWORD nFileSystemNameSize)         // length of file system name buffer
   //{
   //   string strRootPathName;
   //   string strVolumeNameBuffer;
   //   string strFileSystemNameBuffer;
   //   ::str::international::UnicodeToACP(strRootPathName, lpRootPathName);
   //   bool b = ::GetVolumeInformation(
   //      strRootPathName,
   //      strVolumeNameBuffer.GetBuffer(nVolumeNameSize),
   //      nVolumeNameSize,
   //      lpVolumeSerialNumber,
   //      lpMaximumComponentLength,
   //      lpFileSystemFlags,
   //      strFileSystemNameBuffer.GetBuffer(nFileSystemNameSize),
   //      nFileSystemNameSize) != FALSE;

   //   strVolumeNameBuffer.ReleaseBuffer();
   //   strFileSystemNameBuffer.ReleaseBuffer();
   //   ::str::international::ACPToUnicode(
   //      lpVolumeNameBuffer, 
   //      nVolumeNameSize, 
   //      strVolumeNameBuffer);
   //   ::str::international::ACPToUnicode(
   //      lpFileSystemNameBuffer, 
   //      nFileSystemNameSize, 
   //      strFileSystemNameBuffer);
   //   return b;
   //}

   //uint_ptr shell::_SHGetFileInfo(      
   //   const unichar * pszPath,
   //   DWORD dwFileAttributes,
   //   SHFILEINFOW *psfi,
   //   UINT cbFileInfo,
   //   UINT uFlags)
   //{
   //   UNREFERENCED_PARAMETER(cbFileInfo);
   //   string strPath;
   //   ::str::international::UnicodeToACP(strPath, pszPath);
   //   SHFILEINFOA shia;
   //   if(!::SHGetFileInfoA(strPath, dwFileAttributes, 
   //      &shia,
   //      sizeof(shia),
   //      uFlags))
   //      return FALSE;
   //   ::str::international::ACPToUnicode(
   //      psfi->szDisplayName, 
   //      sizeof(psfi->szDisplayName) / sizeof(WCHAR),
   //      shia.szDisplayName);
   //   ::str::international::ACPToUnicode(
   //      psfi->szTypeName, 
   //      sizeof(psfi->szTypeName) / sizeof(WCHAR),
   //      shia.szTypeName);
   //   return TRUE;
   //}


   //BOOL shell::_GetStringTypeEx(      
   //   LCID uiCodePage,
   //   DWORD dwInfoType,
   //   const unichar * lpSrcStr,
   //   int32_t cchSrc,
   //   LPWORD lpCharType)
   //{
   //   strsize iCount = cchSrc;
   //   if(iCount < 0)
   //      iCount = ::str::international::UnicodeToMultiByteCount(uiCodePage, lpSrcStr);
   //   string str;
   //   LPTSTR lpsz = str.GetBuffer(iCount);
   //   if(::str::international::UnicodeToMultiByte(uiCodePage, lpsz, iCount, lpSrcStr))
   //   {
   //      //str.ReleaseBuffer();
   //      //return true;
   //   }
   //   else
   //   {
   //      return false;
   //   }
   //   if(!GetStringTypeA(uiCodePage, dwInfoType, lpsz, (int32_t) iCount, lpCharType))
   //   {
   //      return FALSE;
   //   }
   //   return TRUE;
   //}


   //DWORD shell::_GetTempPath(
   //   DWORD nBufferLength,
   //   unichar * lpBuffer)
   //{
   //   string str;
   //   DWORD dw = ::GetTempPathA(nBufferLength, str.GetBuffer(nBufferLength * 2));
   //   str.ReleaseBuffer();
   //   ::str::international::ACPToUnicode(lpBuffer, nBufferLength, str);
   //   return dw;
   //}

   //UINT shell::_GetTempFileName(
   //   const unichar * lpPathName,
   //   const unichar * lpPrefixString,
   //   UINT uUnique,
   //   unichar * lpTempFileName)
   //{
   //   string strPathName;
   //   string strPrefixString;
   //   string strTempFileName;
   //   ::str::international::UnicodeToACP(strPathName, lpPathName);
   //   ::str::international::UnicodeToACP(strPrefixString, lpPrefixString);
   //   UINT user = ::GetTempFileNameA(
   //      strPathName,
   //      strPrefixString,
   //      uUnique,
   //      strTempFileName.GetBuffer(MAX_PATH));
   //   if(user == 0)
   //   {
   //      return 0;
   //   }
   //   strTempFileName.ReleaseBuffer();
   //   ::str::international::ACPToUnicode(
   //      lpTempFileName,
   //      MAX_PATH,
   //      strTempFileName);
   //   return user;
   //}


   //HANDLE shell::_CreateFile(
   //   const unichar * lpFileName,
   //   DWORD dwDesiredAccess,
   //   DWORD dwShareMode,
   //   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   //   DWORD dwCreationDisposition,
   //   DWORD dwFlagsAndAttributes,
   //   HANDLE hTemplateFile
   //   )
   //{
   //   string strFileName;
   //   ::str::international::UnicodeToACP(strFileName, lpFileName);
   //   HANDLE handle = ::CreateFileA(
   //      strFileName,
   //      dwDesiredAccess,
   //      dwShareMode,
   //      lpSecurityAttributes,
   //      dwCreationDisposition,
   //      dwFlagsAndAttributes,
   //      hTemplateFile);
   //   return handle;
   //}


   //DWORD shell::_GetModuleFileName(
   //   HMODULE hModule,
   //   unichar * lpFilename,
   //   DWORD nSize
   //   )
   //{
   //   string str;
   //   DWORD dw = ::GetModuleFileNameA(hModule, str.GetBuffer(nSize * 2), nSize * 2);
   //   str.ReleaseBuffer();
   //   ::str::international::ACPToUnicode(lpFilename, nSize, str);
   //   return dw;
   //}

   //BOOL shell::_GetClassInfo(HINSTANCE hInstance , const unichar * lpClassName, LPWNDCLASSW lpWndClass)
   //{
   //   WNDCLASS wndclass;

   //   string strClassName;
   //   const char * lpszClassName;
   //   if(((DWORD) lpClassName) >> 16 == 0)
   //   {
   //      lpszClassName = (const char *) lpClassName;
   //   }
   //   else
   //   {
   //      strClassName = lpClassName;
   //      lpszClassName = strClassName;
   //   }
   //   if(!::GetClassInfoA(hInstance, lpszClassName, &wndclass))
   //      return FALSE;

   //   lpWndClass->style = wndclass.style;
   //   lpWndClass->lpfnWndProc = wndclass.lpfnWndProc;
   //   lpWndClass->lpfnWndProc = wndclass.lpfnWndProc;
   //   lpWndClass->cbClsExtra = wndclass.cbClsExtra;
   //   lpWndClass->cbWndExtra = wndclass.cbWndExtra;
   //   lpWndClass->hInstance = wndclass.hInstance;
   //   lpWndClass->hIcon = wndclass.hIcon;
   //   lpWndClass->hCursor = wndclass.hCursor;
   //   lpWndClass->hbrBackground = wndclass.hbrBackground;

   //   /*   if(((DWORD) wndclass) >> 16 == 0)
   //   {
   //   lpszClassName = (const char *) lpClassName;
   //   }
   //   else
   //   {
   //   strClassName = lpClassName;
   //   lpszClassName = strClassName
   //   }
   //   string strMenuName;*/
   //   //strMenuName = wndclass->lpszMenuName;
   //   lpWndClass->lpszMenuName = (const unichar *) wndclass.lpszMenuName;

   //   lpWndClass->lpszClassName = (const unichar *) wndclass.lpszClassName;

   //   return TRUE;

   //}
   //ATOM shell::_RegisterClass(
   //   CONST WNDCLASSW *lpWndClass)
   //{
   //   WNDCLASS wndclass;

   //   wndclass.style = lpWndClass->style;
   //   wndclass.lpfnWndProc = lpWndClass->lpfnWndProc;
   //   wndclass.lpfnWndProc = lpWndClass->lpfnWndProc;
   //   wndclass.cbClsExtra = lpWndClass->cbClsExtra;
   //   wndclass.cbWndExtra = lpWndClass->cbWndExtra;
   //   wndclass.hInstance = lpWndClass->hInstance;
   //   wndclass.hIcon = lpWndClass->hIcon;
   //   wndclass.hCursor = lpWndClass->hCursor;
   //   wndclass.hbrBackground = lpWndClass->hbrBackground;

   //   string strMenuName;
   //   strMenuName = lpWndClass->lpszMenuName;
   //   wndclass.lpszMenuName = strMenuName;

   //   string strClassName;
   //   strClassName = lpWndClass->lpszClassName;
   //   wndclass.lpszClassName = strClassName;

   //   return ::RegisterClassA(&wndclass);
   //}


   //oswindow shell::_CreateWindowEx(
   //   DWORD dwExStyle,
   //   const unichar * lpClassName,
   //   const unichar * lpWindowName,
   //   DWORD dwStyle,
   //   int32_t x,
   //   int32_t y,
   //   int32_t nWidth,
   //   int32_t nHeight,
   //   oswindow oswindow_Parent,
   //   HMENU hMenu,
   //   HINSTANCE hInstance,
   //   LPVOID lpParam)
   //{
   //   string strClassName;
   //   const char * lpszClassName;
   //   if(((DWORD) lpClassName) >> 16 == 0)
   //   {
   //      lpszClassName = (const char *) lpClassName;
   //   }
   //   else
   //   {
   //      strClassName = lpClassName;
   //      lpszClassName = strClassName;
   //   }

   //   string strWindowName;
   //   strWindowName = lpWindowName;

   //   return ::CreateWindowExA(
   //      dwExStyle,
   //      lpszClassName,
   //      strWindowName,
   //      dwStyle,
   //      x,
   //      y,
   //      nWidth,
   //      nHeight,
   //      oswindow_Parent,
   //      hMenu,
   //      hInstance,
   //      lpParam);

   //}
   //*/

} // namespace windows


