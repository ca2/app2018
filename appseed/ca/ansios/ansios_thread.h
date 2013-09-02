#pragma once


class CLASS_DECL_c message_array :
   public array < MESSAGE >
{
};

class CLASS_DECL_c mq
{
public:


   mutex               m_mutex;
   message_array              ma;
   simple_event               m_eventNewMessage;
   HTHREAD                    m_hthread;
   uint32_t                   m_uiId;


   mq() : m_eventNewMessage(false, true) {}



};

CLASS_DECL_c mq * get_mq(HTHREAD h);

CLASS_DECL_c int_bool WINAPI GetMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax);

#define GetMessage GetMessageW

CLASS_DECL_c int_bool WINAPI PeekMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax,UINT wRemoveMsg);

#define PeekMessage PeekMessageW

CLASS_DECL_c DWORD WINAPI GetThreadId(HTHREAD Thread);

#define PostThreadMessage  PostThreadMessageW

CLASS_DECL_c int_bool WINAPI PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);

#define PostMessage  PostMessageW

CLASS_DECL_c int_bool WINAPI PostMessageW(oswindow oswindow, UINT Msg, WPARAM wParam, LPARAM lParam);


CLASS_DECL_c HTHREAD GetCurrentThread();

CLASS_DECL_c UINT GetCurrentThreadId();

CLASS_DECL_ca void thread_get_os_priority(int32_t * piOsPolicy, sched_param * pparam, int32_t iCa2Priority);

CLASS_DECL_ca int32_t thread_get_scheduling_priority(int iOsPolicy, const sched_param * pparam);

CLASS_DECL_ca void process_get_os_priority(int32_t * piOsPolicy, sched_param * pparam, int32_t iCa2Priority);

CLASS_DECL_ca int32_t process_get_scheduling_priority(int iOsPolicy, const sched_param * pparam);


