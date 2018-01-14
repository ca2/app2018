﻿#pragma once




#ifdef __cplusplus


//
//class CLASS_DECL_AURA thread_data
//{
//public:
//
//
//   DWORD g_dwTlsIndex;
//
//   thread_data();
//   ~thread_data();
//
//   void * get();
//   void set(void * p);
//
//
//};


#endif

#ifndef METROWIN

typedef struct tag_MSG
{
   oswindow    hwnd;
   UINT        message;
   WPARAM      wParam;
   LPARAM      lParam;
   POINT       pt;
} MESSAGE, * LPMESSAGE;
#endif



CLASS_DECL_AURA WINBOOL WINAPI GetMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax);

#define GetMessage GetMessageW

CLASS_DECL_AURA WINBOOL WINAPI PeekMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax,UINT wRemoveMsg);

#define PeekMessage PeekMessageW

#define PostThreadMessage  PostThreadMessageW

CLASS_DECL_AURA WINBOOL WINAPI PostThreadMessageW(IDTHREAD idthread, UINT Msg, WPARAM wParam, LPARAM lParam);

CLASS_DECL_AURA void WINAPI TlsShutdown();


#ifdef __cplusplus

class mq;

//CLASS_DECL_AURA mq * __get_mq();
CLASS_DECL_AURA mq * __get_mq(IDTHREAD iThreadId, bool bCreate);


#endif

#ifndef METROWIN

CLASS_DECL_AURA BOOL WINAPI SetThreadPriority(HTHREAD hThread,int iPriority);

CLASS_DECL_AURA int WINAPI GetThreadPriority(HTHREAD hThread);

#endif



DWORD WinMsgWaitForMultipleObjects(DWORD dwSize,const HANDLE * lphandles,DWORD dwTimeout,DWORD dwWakeMask,DWORD dwFlags);
