#ifndef AURA_ANSIOS_MULTITHREADING_H
#define AURA_ANSIOS_MULTITHREADING_H


#pragma once


#if defined(LINUX) // || defined(ANDROID)

void set_defer_process_x_message(bool (* pfn)(HTHREAD hthread,LPMESSAGE lpMsg,oswindow oswindow,bool bPeek));

#endif




#ifdef __cplusplus


class object;


DWORD WaitForMultipleObjectsEx(DWORD dwSize, object ** pwaitableptra, WINBOOL bWaitForAll, DWORD dwTimeout, WINBOOL bAlertable);

DWORD WaitForMultipleObjects(DWORD dwSize, object ** pwaitableptra, WINBOOL bWaitForAll, DWORD dwTimeout);

DWORD MsgWaitForMultipleObjects(DWORD dwSize, object ** pwaitableptra, WINBOOL bWaitForAll, DWORD dwTimeout, DWORD dwWakeMask);

DWORD MsgWaitForMultipleObjectsEx(DWORD dwSize, object ** pwaitableptra, DWORD dwTimeout, DWORD dwWakeMask, DWORD dwFlags);

DWORD WaitForSingleObject(object * pwaitable, DWORD dwMilliseconds);

DWORD WaitForSingleObjectEx(object * pwaitable, DWORD dwMilliseconds, WINBOOL bAlertable);


#endif


/*
#define STATUS_WAIT_0            ((DWORD   ) 0x00000000L)
#define STATUS_ABANDONED_WAIT_0  ((DWORD   ) 0x00000080L)
#define STATUS_USER_APC          ((DWORD   ) 0x000000C0L)
#define STATUS_TIMEOUT           ((DWORD   ) 0x00000102L)
 */

#define WAIT_FAILED              ((DWORD   ) 0xFFFFFFFFL)
#define WAIT_OBJECT_0            ((DWORD   ) STATUS_WAIT_0 + 0)

#define WAIT_ABANDONED           ((DWORD   ) STATUS_ABANDONED_WAIT_0 + 0)
#define WAIT_ABANDONED_0         ((DWORD   ) STATUS_ABANDONED_WAIT_0 + 0)

#define WAIT_IO_COMPLETION                   STATUS_USER_APC

// based on Windows implementation and also a good limit for iterations with nanosleep and multiple locks in objects up to MAXIMUM_WAIT_OBJECTS in a single call
#define MAXIMUM_WAIT_OBJECTS      64


#if !defined(APPLEOS)

union semun {
    int val; /* Value for SETVAL */
    struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* Array for GETALL, SETALL */
    struct seminfo *__buf; /* Buffer for IPC_INFO
                                (Linux-specific) */
};

#endif

#define MWMO_WAITALL        0x0001
#define MWMO_ALERTABLE      0x0002
#define MWMO_INPUTAVAILABLE 0x0004


#ifdef cplusplus


class thread_data
{
public:


    void *    m_pkey; // pthread_key_t           key;


    thread_data();


    void * get();
    void set(void * p);


};



#endif






















































#endif // AURA_ANSIOS_MULTITHREADING_H










