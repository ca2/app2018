#include "StdAfx.h"
#include <winnls.h>

namespace gen
{

   namespace international
   {

      UINT       g_uiACP = GetACP();

      bool UnicodeToMultiByte(UINT uiCodePage, char * lpstrMultiByte, int nCount, const wchar_t * lpcsz)
      {
         return WideCharToMultiByte(uiCodePage, 0, lpcsz, -1, lpstrMultiByte, nCount, NULL, NULL) != FALSE;
      }

      bool UnicodeToMultiByte(UINT uiCodePage, char * lpstrMultiByte, int iMultiByteCount, const wchar_t * lpcsz, int iCount)
      {
         return WideCharToMultiByte(uiCodePage, 0, lpcsz, iCount, lpstrMultiByte, iMultiByteCount, NULL, NULL) != FALSE;
      }

      bool UnicodeToMultiByte(UINT uiCodePage, string &str, const wchar_t * lpcsz)
      {
         int iCount = UnicodeToMultiByteCount(uiCodePage, lpcsz);
         LPTSTR lpsz = str.GetBuffer(iCount);
         if(UnicodeToMultiByte(uiCodePage, lpsz, iCount, lpcsz))
         {
            str.ReleaseBufferSetLength(iCount - 1);
            return true;
         }
         else
         {
            str.ReleaseBufferSetLength(0);
            str.Empty();
            return false;
         }
         return true;
      }

      bool UnicodeToMultiByte(UINT uiCodePage, string &str, const wchar_t * lpcsz, int iCount)
      {
         int iMultiByteCount = UnicodeToMultiByteCount(uiCodePage, lpcsz, iCount);
         LPTSTR lpsz = str.GetBuffer(iCount);
         if(UnicodeToMultiByte(uiCodePage, lpsz, iMultiByteCount, lpcsz, iCount))
         {
            str.ReleaseBufferSetLength(iCount - 1);
            return true;
         }
         else
         {
            str.ReleaseBufferSetLength(0);
            str.Empty();
            return false;
         }
      }

      bool unicode_to_utf8(string & str, const wchar_t * lpcsz, int iCount)
      {
         return UnicodeToMultiByte(CodePageUtf8, str, lpcsz, iCount);
      }

      int UnicodeToMultiByteCount(UINT uiCodePage, const wchar_t * lpcsz)
      {
         return WideCharToMultiByte(uiCodePage, 0, lpcsz, -1, NULL, 0, NULL, NULL);
      }

      int UnicodeToMultiByteCount(UINT uiCodePage, const wchar_t * lpcsz, int iCount)
      {
         return WideCharToMultiByte(uiCodePage, 0, lpcsz, iCount, NULL, 0, NULL, NULL);
      }

      int Utf8ToMultiByteCount(UINT uiCodePage, const char * lpcsz)
      {
         return WideCharToMultiByte(uiCodePage, 0, MultiByteToUnicode(uiCodePage, lpcsz), -1, NULL, 0, NULL, NULL);
      }

      bool MultiByteToUnicode(UINT uiCodePage, wchar_t * lpwsz, int iBuffer, const char * lpcsz, int iCount)
      {
         return MultiByteToWideChar(uiCodePage, 0, lpcsz, iCount, lpwsz, iBuffer) != 0;
      }

      bool MultiByteToUnicode(UINT uiCodePage, wchar_t * lpwsz, int iBuffer, const char * lpcsz)
      {
         return MultiByteToUnicode(uiCodePage, lpwsz, iBuffer, lpcsz, -1);
      }

      int MultiByteToUnicodeCount(UINT uiCodePage, const char * lpcsz, int iCount)
      {
         return MultiByteToWideChar(uiCodePage, 0, lpcsz, iCount, NULL, 0);
      }

      int MultiByteToUnicodeCount(UINT uiCodePage, const char * lpcsz)
      {
         return MultiByteToUnicodeCount(uiCodePage, lpcsz, -1);
      }

      wstring MultiByteToUnicode(UINT uiCodePage, const char * lpcsz)
      {
         return MultiByteToUnicode(uiCodePage, lpcsz, -1);
      }

      wstring MultiByteToUnicode(UINT uiCodePage, const char * lpcsz, int nCount)
      {
         int iBuffer = MultiByteToUnicodeCount(uiCodePage, lpcsz, nCount);
         if(iBuffer == ERROR_NO_UNICODE_TRANSLATION)
         {
            return NULL;
         }
         else if(iBuffer == 0)
         {
            return L"";
         }
         wstring wstr;
         wstr.alloc(iBuffer);
         if(MultiByteToUnicode(uiCodePage, wstr.m_pwsz, iBuffer, lpcsz, nCount))
         {
            return wstr;
         }
         return L"";
      }

      wstring MultiByteToUnicode(UINT uiCodePage, const string & str)
      {
         int iBuffer = MultiByteToUnicodeCount(uiCodePage, str, str.get_length());
         if(iBuffer == ERROR_NO_UNICODE_TRANSLATION)
         {
            return NULL;
         }
         else if(iBuffer == 0)
         {
            return L"";
         }
         wstring wstr;
         wstr.alloc(iBuffer);
         if(MultiByteToUnicode(uiCodePage, wstr.m_pwsz, iBuffer, str, str.get_length()))
         {
            return wstr;
         }
         return L"";
      }

      bool MultiByteToMultiByte(UINT uiCodePageDst, string & str, UINT uiCodePageSrc, const char * lpcsz)
      {
         if(uiCodePageDst == uiCodePageSrc)
         {
            str = lpcsz;
            return true;
         }
         else
            return UnicodeToMultiByte(uiCodePageDst, str, MultiByteToUnicode(uiCodePageSrc, lpcsz));
      }

      bool MultiByteToMultiByte(UINT uiCodePageDst, char * lpsz, int nCount, UINT uiCodePageSrc, const char * lpcsz)
      {
         return UnicodeToMultiByte(uiCodePageDst, lpsz, nCount, MultiByteToUnicode(uiCodePageSrc, lpcsz));
      }

      bool MultiByteToMultiByte(UINT uiCodePageDst, string & str, UINT uiCodePageSrc, const char * lpcsz, int nCount)
      {
         return UnicodeToMultiByte(uiCodePageDst, str, MultiByteToUnicode(uiCodePageSrc, lpcsz, nCount));
      }

      bool MultiByteToOEM(UINT uiCodePage, string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(::GetOEMCP(), str, uiCodePage, lpcsz);
      }

      bool MultiByteToOEM(UINT uiCodePage, string & str, const char * lpcsz, int nCount)
      {
         return MultiByteToMultiByte(::GetOEMCP(), str, uiCodePage, lpcsz, nCount);
      }

      bool OEMToMultiByte(UINT uiCodePage, string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(uiCodePage, str, ::GetOEMCP(), lpcsz);
      }

      bool OEMToMultiByte(UINT uiCodePage, string & str, const char * lpcsz, int nCount)
      {
         return MultiByteToMultiByte(uiCodePage, str, ::GetOEMCP(), lpcsz, nCount);
      }

      string unicode_to_utf8(const wchar_t * lpcsz)
      {
         string str;
         unicode_to_utf8(str, lpcsz);
         return str;
      }

      bool MultiByteToMultiByte(UINT uiCodePageDst, primitive::memory & str, UINT uiCodePageSrc, const char * lpcsz)
      {
         return UnicodeToMultiByte(uiCodePageDst, str, MultiByteToUnicode(uiCodePageSrc, lpcsz));
      }

      bool MultiByteToMultiByte(UINT uiCodePageDst, primitive::memory & str, UINT uiCodePageSrc, const char * lpcsz, int nCount)
      {
         return UnicodeToMultiByte(uiCodePageDst, str, MultiByteToUnicode(uiCodePageSrc, lpcsz, nCount));
      }

      bool UnicodeToMultiByte(UINT uiCodePage, primitive::memory & memstorage, const wchar_t * lpcsz)
      {
         memstorage.allocate(UnicodeToMultiByteCount(uiCodePage, lpcsz));
         return UnicodeToMultiByte(uiCodePage, (char *) memstorage.get_data(), memstorage.get_size(), lpcsz);
      }

      bool OEMToMultiByte(UINT uiCodePage, primitive::memory & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(uiCodePage, str, ::GetOEMCP(), lpcsz);
      }

      bool OEMToMultiByte(UINT uiCodePage, primitive::memory & str, const char * lpcsz, int nCount)
      {
         return MultiByteToMultiByte(uiCodePage, str, ::GetOEMCP(), lpcsz, nCount);
      }

      ///////////////////////////////////////////////////////////////
      // ACP ( GetACP() function) conversion
      ///////////////////////////////////////////////////////////////

      string UnicodeToMultiByte(UINT uiCodePage, const wchar_t * lpcsz)
      {
         string str;
         UnicodeToMultiByte(uiCodePage, str, lpcsz);
         return str;
      }

      bool UnicodeToACP(string & str, const wchar_t * lpcsz)
      {
         return UnicodeToMultiByte(g_uiACP, str, lpcsz);
      }

      bool UnicodeToACP(char * lpstrUnicode, int nCount, const wchar_t * lpcsz)
      {
         return UnicodeToMultiByte(g_uiACP, lpstrUnicode, nCount, lpcsz);
      }


      string UnicodeToACP(const wchar_t * lpcsz)
      {
         return UnicodeToMultiByte(CP_ACP, lpcsz);
      }


      bool ACPToUnicode(wchar_t * lpstrUnicode, int nCount, const char * lpcsz)
      {
         return MultiByteToUnicode(g_uiACP, lpstrUnicode, nCount, lpcsz);
      }

      wstring  ACPToUnicode(const char * lpcsz)
      {
         return MultiByteToUnicode(g_uiACP, lpcsz);
      }

      wstring  ACPToUnicode(const char * lpcsz, int iSize)
      {
         return MultiByteToUnicode(g_uiACP, lpcsz, iSize);
      }

      ///////////////////////////////////////////////////////////////
      // OEM ( GetOEM() function) conversion
      ///////////////////////////////////////////////////////////////

      bool UnicodeToOEM(string & str, const wchar_t * lpcsz)
      {
         return UnicodeToMultiByte(CodePageOem, str, lpcsz);
      }

      bool UnicodeToOEM(char * lpstrUnicode, int nCount, const wchar_t * lpcsz)
      {
         return UnicodeToMultiByte(CodePageOem, lpstrUnicode, nCount, lpcsz);
      }



      bool OEMToUnicode(wchar_t * lpstrUnicode, int nCount, const char * lpcsz)
      {
         return MultiByteToUnicode(CodePageOem, lpstrUnicode, nCount, lpcsz);
      }

      wstring OEMToUnicode(const char * lpcsz)
      {
         return MultiByteToUnicode(CodePageOem, lpcsz);
      }

      bool unicode_to_utf8(string & str, const wchar_t * lpcsz)
      {
         return UnicodeToMultiByte(CodePageUtf8, str, lpcsz);
      }

      wstring utf8_to_unicode(const char * lpcsz)
      {
         return MultiByteToUnicode(CodePageUtf8, lpcsz);
      }

      wstring utf8_to_unicode(const string & str)
      {
         return MultiByteToUnicode(CodePageUtf8, str);
      }

      wstring utf8_to_unicode(const var & var)
      {
         return MultiByteToUnicode(CodePageUtf8, (const string &) var);
      }

      int utf8_to_unicode_count(const char * lpcsz)
      {
         return MultiByteToUnicodeCount(CodePageUtf8, lpcsz);
      }

      /*string UnicodeToACP(const wchar_t * lpcsz)
      {
         string str;
         UnicodeToMultiByte(g_uiACP, str, lpcsz);
         return str;
      }

      string ACPToUnicode(const char * lpcsz)
      {
         string str;
         MultiByteToUnicode(g_uiACP, str, lpcsz);
         return str;
      }
      */


      bool AcpToUtf8(string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CodePageUtf8, str, CodePageAnsi, lpcsz);
      }

      bool Utf8ToAcp(string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CodePageAnsi, str, CodePageUtf8, lpcsz);
      }


      bool multibyte_to_utf8(UINT uiCodePageSrc, string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CP_UTF8, str, uiCodePageSrc, lpcsz);
      }

      bool multibyte_to_utf8(UINT uiCodePageSrc, string & str, const char * lpcsz, int nCount)
      {
         return MultiByteToMultiByte(CP_UTF8, str, uiCodePageSrc, lpcsz, nCount);
      }


      bool OemToUtf8(string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CP_UTF8, str, CP_OEMCP, lpcsz);
      }

      bool OemToUtf8(string & str, const char * lpcsz, int nCount)
      {
         return MultiByteToMultiByte(CP_UTF8, str, CP_OEMCP, lpcsz, nCount);
      }

      wstring utf8_to_unicode(const char * lpcsz, int iCount)
      {
         return MultiByteToUnicode(CP_UTF8, lpcsz, iCount);
      }

      bool utf8_to_unicode(wchar_t * lpwsz, int iBuffer, const char * lpcsz, int iCount)
      {
         return MultiByteToUnicode(CP_UTF8, lpwsz, iBuffer, lpcsz, iCount);
      }

      bool utf8_to_unicode(wchar_t * lpwsz, int iBuffer, const char * lpcsz)
      {
         return utf8_to_unicode(lpwsz, iBuffer, lpcsz, -1);
      }

      bool utf8_to_multibyte(UINT uiCodePage, string & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CodePageUtf8, str, uiCodePage, lpcsz);
      }

      bool utf8_to_multibyte(UINT uiCodePage, string & str, const char * lpcsz, int nCount)
      {
         return MultiByteToMultiByte(CodePageUtf8, str, uiCodePage, lpcsz, nCount);
      }

      bool utf8_to_multibyte(UINT uiCodePage, char * lpsz, int nCount, const char * lpcsz)
      {
         return MultiByteToMultiByte(CodePageUtf8, lpsz, nCount, uiCodePage, lpcsz);
      }


      bool AcpToUtf8(LPTSTR lpstrUnicode, int nCount, const char * lpcsz)
      {
         return MultiByteToMultiByte(CP_UTF8, lpstrUnicode, nCount, g_uiACP, lpcsz);
      }

      bool AcpToUtf8(string & str, const char * lpcsz, int iSize)
      {
         return MultiByteToMultiByte(CP_UTF8, str, g_uiACP, lpcsz, iSize);
      }

   } // namespace international

} // namespace gen
