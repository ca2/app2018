#include "framework.h"
#include <WinInet.h>

HINTERNET  g_hSession = ::null();
HINTERNET  g_hConnect = ::null();
HINTERNET  g_hPreviousRequest = ::null();
uint32_t g_MsDownloadSize = 1024 * 16;
char * g_MsDownloadBuffer = ::null();
vsstring * g_pstrHost = ::null();


void reset_http()
{

   g_hSession           = ::null();

   g_hConnect           = ::null();
   
   g_hPreviousRequest   = ::null();

   g_MsDownloadSize     = 1024 * 16;

   g_MsDownloadBuffer   = ::null();

   g_pstrHost           = ::null();

   ::LoadLibrary("Wininet.dll");

}


void prepare_http()
{
   
   if(g_MsDownloadBuffer == ::null())
   {

      g_MsDownloadBuffer = new char[g_MsDownloadSize];

   }

   if(g_pstrHost == ::null())
   {

      g_pstrHost = new vsstring();

   }

}


bool ms_download_dup(const char * pszUrl, const char * pszFile, bool bProgress, bool bUrlEncode, int32_t * piStatus, void (*callback)(void *, int32_t, uint_ptr), void * callback_param )
{

   if(piStatus != ::null())
      *piStatus = 0;

   vsstring strUrl;

   char * szBuf = (char *) _ca_alloc(4096);

   prepare_http();

   if(file_exists_dup(pszFile) && !::DeleteFile(pszFile))
   {
      //trace("download failed: could not delete file prior to download.");
      vsstring str;
      str = "ms_download_dup: error url=\"";
      str += pszUrl;
      str += "\"";
      str = "file path=\"";
      str += pszFile;
      str += "\"";
      trace(str);
      return false;
   }

   if(callback != ::null())
   {
      callback(callback_param, -1, 0);
   }

   int32_t iCol = 3;


   strUrl = pszUrl;
   if(bUrlEncode)
   {
      strUrl = url_encode_dup(strUrl);
      strUrl.replace_ci("%5C", "\\");
      strUrl.replace_ci("\\", "/");
      strUrl.replace_ci("%3A", ":");
      strUrl.replace_ci("%2F", "/");
   }
   vsstring strHost;
   vsstring strReq;
   if(strUrl.substr(0, 7) == "http://")
   {
      size_t iPos = strUrl.find("/", 8);
      strHost = strUrl.substr(7, iPos - 7);
      strReq = strUrl.substr(iPos);
   }
   uint32_t dwSize = 0;
   LPSTR pszOutBuffer;
   bool  bResults = FALSE;

   WCHAR * pwzHost = utf8_to_16(strHost);

   if(g_hSession == ::null())
   {
      g_hSession = InternetOpen(
         "ca2",
         INTERNET_OPEN_TYPE_PRECONFIG,
         ::null(),
         ::null(),
         INTERNET_FLAG_EXISTING_CONNECT
         | INTERNET_FLAG_KEEP_CONNECTION);
      HTTP_VERSION_INFO vi;
      vi.dwMajorVersion = 1;
      vi.dwMinorVersion = 1;
      InternetSetOption(
         g_hSession,
         INTERNET_OPTION_HTTP_VERSION,
         &vi,
         sizeof(vi));
   }


   if(g_hSession != ::null())
   {
      if(strHost != *g_pstrHost || g_hConnect == ::null())
      {
         if(g_hConnect != ::null())
         {
            ::InternetCloseHandle(g_hConnect);
         }
         g_hConnect = InternetConnect( g_hSession, strHost,
            80, ::null(), ::null(), INTERNET_SERVICE_HTTP,
            INTERNET_FLAG_EXISTING_CONNECT
            | INTERNET_FLAG_KEEP_CONNECTION,
            1);
         *g_pstrHost = strHost;
      }
   }


   HINTERNET hRequest = ::null();
   if (g_hConnect != ::null())
      hRequest = HttpOpenRequest( g_hConnect, "GET", strReq,
      ::null(), "ca2",
      ::null(),
      INTERNET_FLAG_EXISTING_CONNECT
      | INTERNET_FLAG_KEEP_CONNECTION,
      1);

   if (hRequest)
      bResults = HttpSendRequest(hRequest, ::null(), 0, ::null(), 0) != FALSE;

   if(hRequest != g_hPreviousRequest)
   {
      InternetCloseHandle(g_hPreviousRequest);
   }


   DWORD dwStatusCode = 0;
   DWORD dwBufferLen = sizeof(dwStatusCode);
   DWORD dwIndex = 0;
   if(bResults)
      bResults = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwBufferLen, &dwIndex) != FALSE;
   if(piStatus != ::null())
      *piStatus = dwStatusCode;
   dwBufferLen = 1024;
   char * pszStatus = (char *) _ca_alloc(dwBufferLen);
   if(pszStatus != ::null())
   {
      memset_dup(pszStatus, 0, dwBufferLen);
      dwIndex = 0;
      HttpQueryInfo(
         hRequest,
         HTTP_QUERY_STATUS_TEXT,
         pszStatus,
         &dwBufferLen,
         &dwIndex);
   }

   if(bResults && dwStatusCode != HTTP_STATUS_OK)
   {
      if(piStatus == ::null())
      {
         /*dwBufferLen = 1024 + 256;
         char * pszMessage = (char *) _ca_alloc(dwBufferLen);
         if(pszMessage != ::null())
         {
         //sprintf_s(pszMessage, dwBufferLen, "download error : status %d - %s", dwStatusCode, pszStatus);
         //trace(pszMessage);
         free(pszMessage);
         }
         else
         {
         char sz[1024];
         sprintf_s(sz, sizeof(sz), "download error : status %d", dwStatusCode);
         trace(sz);
         }*/
      }
      bResults = FALSE;
   }
   if(pszStatus != ::null())
   {
      _ca_free(pszStatus, 0);
   }

   if (bResults)
   {

      DWORD dwWritten;
      uint32_t dwError;
      DWORD dwDownloaded;
      vsstring strPath;
      strPath = pszFile;
      uint32_t dwLen = 0;
      dir::mk(dir::name(strPath));
      HANDLE hfile = ::create_file(strPath, GENERIC_WRITE, 0, ::null(), CREATE_ALWAYS, 0, ::null());
      if(hfile == INVALID_HANDLE_VALUE)
      {
         dwError = ::GetLastError();
         return false;
      }
      uint32_t dwSize = g_MsDownloadSize;
      pszOutBuffer = g_MsDownloadBuffer;
      if (!pszOutBuffer)
      {
         ::CloseHandle(hfile);
         ::OutputDebugStringA("Out of memory\n");
         vsstring str;
         str = "ms_download_dup: out of memory error url=\"";
         str += pszUrl;
         str += "\"";
         trace(str);
         return false;
      }
      if(callback != ::null())
      {

         callback(callback_param, 0, 0);
      }
      while(true)
      {
         memset_dup(pszOutBuffer, 0, dwSize);
         dwDownloaded = 0;
         if (!InternetReadFile( hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
         {
            //sprintf(szBuf, "Error %u in WinHttpReadData.\n", GetLastError());
            //trace(szBuf);
            if(callback != ::null())
            {
               callback(callback_param, -2, 0);
            }
         }
         else
         {
            ::WriteFile(hfile, pszOutBuffer, dwDownloaded, &dwWritten, ::null());
            dwLen += dwWritten;
            if(callback != ::null())
            {
               callback(callback_param, 1, dwLen);
            }
         }
         if(dwDownloaded == 0)
            break;
      }
      ::CloseHandle(hfile);
   }
   if(bResults)
   {
      if(callback != ::null())
      {
         callback(callback_param, 2,  0);
      }
   }
   else
   {
      //sprintf(szBuf, "Error %d has occurred.\n",dw);
      //trace(szBuf);
      if(callback != ::null())
      {
         callback(callback_param, -2, 0);
      }
   }
   delete [] pwzHost;
   g_hPreviousRequest = hRequest;

   if(!bResults)
   {
      vsstring str;
      str = "ms_download_dup: error url=\"";
      str += pszUrl;
      str += "\"";
      str = "file path=\"";
      str += pszFile;
      str += "\"";
      trace(str);
   }

   return bResults != FALSE;
}



vsstring ms_get_dup(const char * pszUrl, bool bCache, void (*callback)(void *, int32_t, uint_ptr), void * callback_param, bool bProgress)
{

   prepare_http();
   vsstring strRet;
   vsstring strUrl(pszUrl);
   vsstring strHost;
   vsstring strReq;
   int32_t iPort = 80;
   if(strUrl.substr(0, 7) == "http://")
   {
      size_t iPos = strUrl.find("/", 8);
      strHost = strUrl.substr(7, iPos - 7);
      strReq = strUrl.substr(iPos);
   }
   else if(strUrl.substr(0, 8) == "https://")
   {
      size_t iPos = strUrl.find("/", 9);
      strHost = strUrl.substr(8, iPos - 8);
      strReq = strUrl.substr(iPos);
      iPort = 443;
   }
   strsize iFind = strReq.find("?");
   if(iFind >= 0)
   {
      vsstring strQ = strReq.substr(iFind);
      strQ.replace("/", "%2F");
      strReq = strReq.substr(0, iFind) + strQ;
   }
   uint32_t dwSize = 0;
   DWORD dwDownloaded = 0;
   LPSTR pszOutBuffer;
   bool  bResults = FALSE;
   HINTERNET  hSession = ::null(),
      hConnect = ::null(),
      hRequest = ::null();

   WCHAR * pwzHost = utf8_to_16(strHost);

   /*WCHAR * pwzAutoUrl = ::null();
   if(WinHttpDetectAutoProxyConfigUrl(
   WINHTTP_AUTO_DETECT_TYPE_DHCP|
   WINHTTP_AUTO_DETECT_TYPE_DNS_A, pwzAutoUrl))
   {
   WinHttpGetProxyForUrl
   GlobalFree((HGLOBAL) pwzAutoUrl);
   }*/



   // Use WinHttpOpen to obtain a session handle.
   hSession = InternetOpen("ca2", INTERNET_OPEN_TYPE_PRECONFIG, ::null(), ::null(), 0);

   if(hSession)
      hConnect = InternetConnect(hSession, strHost, iPort, ::null(), ::null(), INTERNET_SERVICE_HTTP, iPort == 443 ? INTERNET_FLAG_SECURE : 0, 1);

   uint32_t dwFlags = 0;

   if(!bCache)
   {
      dwFlags |= INTERNET_FLAG_NO_CACHE_WRITE;
      dwFlags |= INTERNET_FLAG_PRAGMA_NOCACHE;
      dwFlags |= INTERNET_FLAG_RELOAD;
   }

   if(iPort == 443)
   {
      dwFlags |= INTERNET_FLAG_SECURE;
   }

   if(hConnect)
      hRequest = HttpOpenRequest(hConnect, "GET", strReq, ::null(), "ca2", ::null(),  dwFlags, 1); 

   if(hRequest)
      bResults =
      HttpSendRequest(hRequest, ::null(), 0, ::null(), 0) != FALSE;

   if(!bResults)
   {
      if(callback != ::null())
      {
         callback(callback_param, -2, 0);
      }
   }

   DWORD dwStatusCode = 0;
   DWORD dwBufferLen = sizeof(DWORD);
   DWORD dwIndex = 0;
   if(bResults)
      bResults = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwBufferLen, &dwIndex) != FALSE;
   dwBufferLen = 1024;
   char * pszStatus = (char *) _ca_alloc(dwBufferLen);
   if(pszStatus != ::null())
   {
      memset_dup(pszStatus, 0, dwBufferLen);
      dwIndex = 0;
      HttpQueryInfo(
         hRequest,
         HTTP_QUERY_STATUS_TEXT,
         pszStatus,
         &dwBufferLen,
         &dwIndex);
   }

   if(bResults && dwStatusCode != HTTP_STATUS_OK)
   {
      if(callback != ::null())
      {
         simple_http_status status;
         status.m_dwStatusCode = dwStatusCode;
         status.m_pszStatus = pszStatus;
         callback(callback_param, -3, (uint_ptr) (void *) &status);
         bResults = FALSE;
      }
   }
   if(pszStatus != ::null())
   {
      _ca_free(pszStatus, 0);
   }

   uint32_t dwLen = 0;
   uint32_t dwLast100k = 0;
   dwSize = 1024 * 64;
   // Allocate space for the buffer.
   pszOutBuffer = new char[dwSize+1];
   if (bResults)
   {
      if(callback != ::null())
      {
         callback(callback_param, 0, 0);
      }

      do
      {
         if (!pszOutBuffer)
         {
            ::OutputDebugStringA("Out of primitive::memory\n");
            dwSize=0;
         }
         else
         {
            // Read the Data.
            memset_dup(pszOutBuffer, 0, dwSize + 1);

            if(!InternetReadFile( hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
            {
               //printf( "Error %u in WinHttpReadData.\n",
               //      GetLastError());
               if(callback != ::null())
               {
                  callback(callback_param, -2, 0);
               }
            }
            else
            {
               strRet += vsstring(pszOutBuffer, dwDownloaded);
               dwLen += dwDownloaded;
               if(bProgress && callback != ::null())
               {
                  callback(callback_param, 1, dwSize);
               }
            }
         }
         // Free the primitive::memory allocated to the buffer.
      }
      while (dwDownloaded>0);
      if(callback != ::null())
      {

         callback(callback_param, 2, 0);
      }

   }
   delete [] pszOutBuffer;

   if (hRequest) InternetCloseHandle(hRequest);
   if (hConnect) InternetCloseHandle(hConnect);
   if (hSession) InternetCloseHandle(hSession);
   delete [] pwzHost;

   if(!bResults)
   {
      vsstring str;
      str = "ms_download_dup: error url=\"";
      str += pszUrl;
      str += "\"";
      trace(str);
   }

   return strRet;

}


vsstring url_encode_dup(const char * psz)
{
   vsstring str;
   char sz[256];
   while(*psz != '\0')
   {
      uchar uch = *psz;
      if(isdigit_dup(uch)
         || isalpha_dup(uch)
         || uch == '.'
         || uch == '-'
         || uch== '_')
      {
         str += uch;
      }
      else if(uch == ' ')
      {
         str += "+";
      }
      else
      {
         itoa_dup(sz, uch, 16);
         if(strlen_dup(sz) == 0)
         {
            str += "%00";
         }
         else if(strlen_dup(sz) == 1)
         {
            str += "%0";
            str += sz;
         }
         else if(strlen_dup(sz) == 2)
         {
            str += "%";
            str += sz;
         }

      }

      psz++;

   }

   return str;

}





/*
vsstring ms_post(const char * pszUrl, const char * pszPost)
{
vsstring strUrl(pszUrl);
vsstring strHost;
vsstring strReq;
int32_t iPort;
if(strUrl.substr(0, 7) == "http://")
{
int32_t iPos = strUrl.find("/", 8);
strHost = strUrl.substr(7, iPos - 7);
strReq = strUrl.substr(iPos);
iPort = 80;
}
else if(strUrl.substr(0, 8) == "https://")
{
int32_t iPos = strUrl.find("/", 9);
strHost = strUrl.substr(8, iPos - 8);
strReq = strUrl.substr(iPos);
iPort = 443;
}
uint32_t dwSize = 0;
uint32_t dwDownloaded = 0;
LPSTR pszOutBuffer;
bool  bResults = FALSE;
HINTERNET  hSession = ::null(),
hConnect = ::null(),
hRequest = ::null();

WCHAR * pwzHost = utf8_to_16(strHost);

/*WCHAR * pwzAutoUrl = ::null();
if(WinHttpDetectAutoProxyConfigUrl(
WINHTTP_AUTO_DETECT_TYPE_DHCP|
WINHTTP_AUTO_DETECT_TYPE_DNS_A, pwzAutoUrl))
{
WinHttpGetProxyForUrl
GlobalFree((HGLOBAL) pwzAutoUrl);
}*/



// Use WinHttpOpen to obtain a session handle.
/*  hSession = WinHttpOpen( L"ca2",
WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
WINHTTP_NO_PROXY_NAME,
WINHTTP_NO_PROXY_BYPASS, 0);

WCHAR * pwzReq = utf8_to_16(strReq);
WCHAR * pwzUrl = utf8_to_16(strUrl);
config_session_proxy(hSession, pwzUrl);





// Specify an HTTP server.
if (hSession)
hConnect = WinHttpConnect( hSession, pwzHost,
iPort, 0);


// Create an HTTP request handle.
if (hConnect)
hRequest = WinHttpOpenRequest( hConnect, L"POST",
pwzReq,
::null(), WINHTTP_NO_REFERER,
WINHTTP_DEFAULT_ACCEPT_TYPES,
iPort == 443 ? WINHTTP_FLAG_SECURE : 0);
//                                       WINHTTP_FLAG_SECURE);

// add a request header.
WCHAR * hd1 = utf8_to_16(("Host: " + strHost));
if( hRequest )
bResults = WinHttpAddRequestHeaders( hRequest,
hd1,
-1,
WINHTTP_ADDREQ_FLAG_ADD );
if( hRequest )
bResults = WinHttpAddRequestHeaders( hRequest,
L"Content-Type: application/x-www-form-urlencoded",
-1,
WINHTTP_ADDREQ_FLAG_ADD );

// Send a request.
if (hRequest)
bResults = WinHttpSendRequest( hRequest,
WINHTTP_NO_ADDITIONAL_HEADERS,
0, WINHTTP_NO_REQUEST_DATA, 0,
strlen(pszPost), 0);

uint32_t dwUploaded;
if (bResults)
if (! (bResults = WinHttpWriteData( hRequest, (LPVOID)pszPost,
strlen(pszPost), &dwUploaded)))
printf( "Error %u in WinHttpReadData.\n",
GetLastError());

// End the request.
if (bResults)
bResults = WinHttpReceiveResponse( hRequest, ::null());

vsstring strResult;

// Keep checking for data until there is nothing left.
if (bResults)
{
do
{

// Check for available data.
dwSize = 0;
if (!WinHttpQueryDataAvailable( hRequest, &dwSize))
printf( "Error %u in WinHttpQueryDataAvailable.\n",
GetLastError());

// Allocate space for the buffer.
pszOutBuffer = new char[dwSize+1];
if (!pszOutBuffer)
{
printf("Out of primitive::memory\n");
dwSize=0;
}
else
{
// Read the Data.
ZeroMemory(pszOutBuffer, dwSize+1);

if (!WinHttpReadData( hRequest, (LPVOID)pszOutBuffer,
dwSize, &dwDownloaded))
printf( "Error %u in WinHttpReadData.\n",
GetLastError());
else
{
g_dwDownloadLen += dwSize;
strResult += pszOutBuffer;
}

// Free the primitive::memory allocated to the buffer.
delete [] pszOutBuffer;
}

} while (dwSize>0);
}

uint32_t dw = GetLastError();
// Report any errors.
if (!bResults)
printf("Error %d has occurred.\n",dw);

delete [] hd1;
delete [] pwzUrl;
delete [] pwzReq;
delete [] pwzHost;

// Close any open handles.
if (hRequest) WinHttpCloseHandle(hRequest);
if (hConnect) WinHttpCloseHandle(hConnect);
if (hSession) WinHttpCloseHandle(hSession);
return strResult;
}
*/
/*
void config_session_proxy(HINTERNET hSession, WCHAR * pwzUrl)
{
WINHTTP_AUTOPROXY_OPTIONS apop;

char szPac[4096];
memset_dup(szPac, 0, sizeof(szPac));
uint32_t lcbPac;
HKEY hkey;
memset_dup(&apop, 0, sizeof(apop));
bool bGot = false;
lcbPac = sizeof(szPac);
if(RegOpenKey(HKEY_CURRENT_USER,
"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections",
&hkey) == ERROR_SUCCESS)
{
LONG l;
uint32_t dw;
if((l = RegQueryValueEx(hkey, "DefaultConnectionSettings", ::null(), ::null(), (LPBYTE) &szPac, &lcbPac)) == ERROR_SUCCESS
&& (szPac[8] & 8))
{
apop.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
apop.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
apop.fAutoLogonIfChallenged = TRUE;
apop.lpszAutoConfigUrl = ::null();
bGot = true;
}
else
{
FormatMessage(0, 0, l, 0, szPac, sizeof(szPac), ::null());
}
}
lcbPac = sizeof(szPac);
if(!bGot)
{
if(RegOpenKey(HKEY_CURRENT_USER,
"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
&hkey) == ERROR_SUCCESS)
{
LONG l;
if((l = RegQueryValueEx(hkey, "AutoConfigURL", ::null(), ::null(), (LPBYTE) szPac, &lcbPac)) == ERROR_SUCCESS)
{

apop.dwAutoDetectFlags = 0;
apop.dwFlags = WINHTTP_AUTOPROXY_CONFIG_URL;
apop.lpszAutoConfigUrl = utf8_to_16(szPac);
apop.fAutoLogonIfChallenged = TRUE;
bGot = true;
}
else
{
FormatMessage(0, 0, l, 0, szPac, sizeof(szPac), ::null());
}
}
}
if(!bGot)
{
return;
}

WINHTTP_PROXY_INFO pinfo;


if(WinHttpGetProxyForUrl(
hSession,
pwzUrl,
&apop,
&pinfo))
{
WinHttpSetOption(
hSession,
WINHTTP_OPTION_PROXY,
&pinfo,
sizeof(pinfo));

}


}
*/

