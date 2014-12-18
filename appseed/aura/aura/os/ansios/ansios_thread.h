#ifndef AXIS_ANSIOS_THREAD_H
#define AXIS_ANSIOS_THREAD_H


#pragma once


/*class hthread :
    virtual public root
{
public:


   event               * m_pevent;
   ::thread          * m_pthread;
   os_thread                  * m_posthread;


   hthread();
   virtual ~hthread();


   void wait();


};
 */




class thread;

class CLASS_DECL_AURA message_array :
public array < MESSAGE > {
};

class CLASS_DECL_AURA mq {
public:


    mutex m_mutex;
    message_array ma;
    manual_reset_event m_eventNewMessage;
    HTHREAD m_hthread;
    uint32_t m_uiId;


    mq();



};

CLASS_DECL_AURA mq * get_mq(HTHREAD h);

CLASS_DECL_AURA int_bool WINAPI GetMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax);

#define GetMessage GetMessageW

CLASS_DECL_AURA int_bool WINAPI PeekMessageW(LPMESSAGE lpMsg, oswindow oswindow, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

#define PeekMessage PeekMessageW

CLASS_DECL_AURA DWORD WINAPI GetThreadId(HTHREAD Thread);

#define PostThreadMessage  PostThreadMessageW

CLASS_DECL_AURA int_bool WINAPI PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);

CLASS_DECL_AURA HTHREAD GetCurrentThread();

CLASS_DECL_AURA UINT GetCurrentThreadId();




// Stored data for CREATE_SUSPENDED and ResumeThread.

struct PendingThreadInfo {
    DWORD(WINAPI * lpStartAddress)(LPVOID);
    LPVOID lpParameter;
    HTHREAD m_hthread;
    event * suspensionEvent;
    int32_t nPriority;
    int32_t cbStack;

    PendingThreadInfo() {
    }

};


#endif // AXIS_ANSIOS_THREAD_H










