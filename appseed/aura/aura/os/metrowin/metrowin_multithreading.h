#pragma once




#ifdef __cplusplus



class CLASS_DECL_AURA thread_data
{
public:


   DWORD g_dwTlsIndex;

   thread_data();
   ~thread_data();

   void * get();
   void set(void * p);


};


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

//CLASS_DECL_AURA DWORD WINAPI get_thread_id(HTHREAD Thread);

#define PostThreadMessage  PostThreadMessageW

CLASS_DECL_AURA WINBOOL WINAPI PostThreadMessageW(IDTHREAD idthread, UINT Msg, WPARAM wParam, LPARAM lParam);

CLASS_DECL_AURA void WINAPI TlsShutdown();

//CLASS_DECL_AURA DWORD WINAPI GetThreadId(HTHREAD Thread);


#ifdef __cplusplus

class mq;

CLASS_DECL_AURA mq * get_mq();
CLASS_DECL_AURA mq * get_mq(int iThreadId);


#endif

#ifndef METROWIN

CLASS_DECL_AURA BOOL WINAPI SetThreadPriority(HTHREAD hThread,int iPriority);

CLASS_DECL_AURA int WINAPI GetThreadPriority(_In_ HTHREAD hThread);

#endif



DWORD MsgWaitForMultipleObjects(DWORD dwSize,const HANDLE * lphandles,DWORD dwTimeout,DWORD dwWakeMask,DWORD dwFlags);
