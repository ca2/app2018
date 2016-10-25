#pragma once

#ifdef APPLEOS
#include <semaphore.h>
#endif



class CLASS_DECL_AURA mutex :
   virtual public sync_object
{
public:


#if !defined(WINDOWS)


   pthread_mutex_t         m_mutex;
   
#ifdef APPLEOS
   
   pthread_t               m_thread;
   pthread_cond_t          m_cond;
   int                     m_count;
   
#endif

#if defined(ANDROID) || defined(APPLEOS)

   sem_t *                 m_psem;

#else

   key_t                   m_key;
   int32_t                 m_semid;

#endif


#endif


   bool                    m_bAlreadyExists;


   mutex(const mutex & m);
#ifdef WINDOWS
   mutex(::aura::application * papp,const char * pstrName,void * posdata, bool bOwner = true);
#elif defined(ANDROID) || defined(APPLEOS)
   mutex(::aura::application * papp,const char * pstrName,sem_t * psem,bool bOwner = true);
#else
   mutex(::aura::application * papp,const char * pstrName,key_t key, int32_t semid, bool bOwner = true);
#endif
   mutex(::aura::application * papp = NULL,bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);



   virtual ~mutex();


#ifndef WINDOWS

   virtual bool lock();

   virtual bool lock(const duration & durationTimeout);

   virtual wait_result wait();

   virtual wait_result wait(const duration & durationTimeout);

#endif

   using sync_object::unlock;
   virtual bool unlock();


   bool already_exists();


   static mutex * open_mutex(::aura::application * papp, const char * pstrName);


};


CLASS_DECL_AURA void wait_until_mutex_does_not_exist(const char * pszName);



CLASS_DECL_AURA mutex * get_ui_destroyed_mutex();




#ifdef WINDOWSEX

class null_dacl_security_attributes
{
public:

   SECURITY_ATTRIBUTES m_securityattributes;

   SECURITY_DESCRIPTOR m_securitydescriptor;

   null_dacl_security_attributes();

};

#endif

class CLASS_DECL_AURA spaadmin_mutex :
#ifdef WINDOWSEX
   public null_dacl_security_attributes,
#endif
   public mutex
{
public:

   spaadmin_mutex(string strSuffix = "");

};


class CLASS_DECL_AURA spa_mutex:
#ifdef WINDOWSEX
   public null_dacl_security_attributes,
#endif
   public mutex
{
public:

   spa_mutex(string strSuffix = "");

};

