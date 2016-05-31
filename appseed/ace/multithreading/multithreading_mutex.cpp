#include "framework.h"


#if defined(LINUX) || defined(SOLARIS) || defined(APPLEOS)
#include <sys/ipc.h>
#include <sys/sem.h>
#include <pthread.h>
#elif defined(ANDROID)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#endif


static int g_iMutex = 0;

string str_md5_dup(const char * psz);

mutex::mutex(::ace::application * papp, bool bInitiallyOwn, const char * pstrName, LPSECURITY_ATTRIBUTES lpsaAttribute /* = NULL */) :
   sync_object(pstrName)
{

   m_bOwner = true;

   m_bAlreadyExists = false;

#ifdef _WIN32

    m_object = ::CreateMutexExW(lpsaAttribute,pstrName == NULL ? NULL : (const unichar *) ::str::international::utf8_to_unicode(pstrName),bInitiallyOwn ?  CREATE_MUTEX_INITIAL_OWNER : 0,MUTEX_ALL_ACCESS);

    DWORD dwLastError = ::GetLastError();

    m_bAlreadyExists = dwLastError == ERROR_ALREADY_EXISTS;

   if(m_object == NULL)
   {

      DWORD dwError1 = ::GetLastError();



      if(pstrName != NULL)
      {

         m_object = ::OpenMutexW(SYNCHRONIZE,FALSE,::str::international::utf8_to_unicode(pstrName));

      }

      if(m_object == NULL)
      {

         DWORD dwError2 = ::GetLastError();

         throw resource_exception(papp);

      }

   }

#elif defined(ANDROID)

    if(pstrName != NULL && *pstrName != '\0')
    {

       m_pmutex = NULL;

       m_psem = NULL;

       return;

       //string strName(pstrName);

       ////if(str::begins_ci(pstrName, "Global"))
       ////{

       ////   m_strName = ::file::path(::ace::system::g_p->m_pandroidinitdata->m_pszCacheDir) / "var" / "tmp"/ strName;

       ////   ::dir::mk(::file::path(m_strName).folder());

       ////}
       ////else
       ////{

       ////   m_strName = ::file::path(getenv("HOME")) / strName;

       ////}

       //s

       //m_strName.replace("/", "_");
       //m_strName.replace(":", "_");
       //m_strName.replace("/", "_");

       ////::file_put_contents_dup(m_strName, m_strName);

       ////string strTest = file_as_string_dup(m_strName);

       ////int isCreator = 0;

       //if ((m_psem = sem_open(m_strName, O_CREAT|O_EXCL, 0644, 1)) != SEM_FAILED)
       //{

       //   // We got here first

       //   //isCreator = 1;

       //}
       //else
       //{

       //   int err = errno;

       //   if (err != EEXIST)
       //      throw resource_exception(get_app());

       //   // We're not first.  Try again

       //   m_psem = sem_open(m_strName, 0);

       //   if (m_psem == SEM_FAILED)
       //      throw resource_exception(get_app());;

       //}

    }
    else
    {

      m_psem = SEM_FAILED;

      pthread_mutexattr_t attr;

      pthread_mutexattr_init(&attr);

      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

      pthread_mutex_init(&m_mutex, &attr);

   }
   
#elif defined(APPLEOS)

   if(pstrName != NULL && *pstrName != '\0')
   {
      
      m_pmutex = NULL;
      
      SetLastError(0);
      
      m_pszName = strdup(string("/") + str_md5_dup(pstrName).Left(24-1));
      
      if ((m_psem = sem_open(m_pszName, O_CREAT|O_EXCL, 0644, 1)) != SEM_FAILED)
      {
      
         m_bOwner = true;
      
      }
      else
      {
      
         int err = errno;
      
         if (err != EEXIST)
            throw resource_exception(get_app());
      
         
         SetLastError(ERROR_ALREADY_EXISTS);
         
         m_bOwner = false;

         // We're not first.  Try again
      
         m_psem = sem_open(m_pszName, 0);
      
         if (m_psem == SEM_FAILED)
            throw resource_exception(get_app());;
      
      }
      
   }
   else
   {
      
      m_psem = SEM_FAILED;
      
      pthread_mutexattr_t attr;
      
      pthread_mutexattr_init(&attr);
      
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
      
      pthread_mutex_init(&m_mutex, &attr);
      
   }
   
#else
   
   if(pstrName != NULL && *pstrName != '\0')
   {


      m_pmutex = NULL;

      if(str::begins_ci(pstrName, "Global"))
      {

         m_pszName = strdup(::file::path("/var/tmp") / pstrName);

      }
      else
      {

         m_pszName = strdup(::file::path(getenv("HOME")) / pstrName);

      }

      ::dir::mk(::dir::name(m_pszName));

      ::file_put_contents_dup(m_pszName, m_pszName);

      m_key = ftok(m_pszName, 1); //Generate a unique key or supply a value

      bool bAlreadyExists;

      get_existing:
      
      SetLastError(0);

      m_semid = semget(
                  m_key, // a unique identifier to identify semaphore set
                  1,  // number of semaphore in the semaphore set
                  0// permissions (rwxrwxrwx) on the new
                       //semaphore set and creation flag
                  );

      if(m_semid >= 0)
      {

         m_bOwner = false;

         bAlreadyExists = true;

         if(bAlreadyExists)
         {
                  SetLastError(ERROR_ALREADY_EXISTS);

         }

      }
      else
      {

         bAlreadyExists = false;

         m_semid = semget(
                     m_key, // a unique identifier to identify semaphore set
                     1,  // number of semaphore in the semaphore set
                     0777 | IPC_CREAT | IPC_EXCL// permissions (rwxrwxrwx) on the new
                          //semaphore set and creation flag
                     );

         if(m_semid == -1 && errno == EEXIST)
         {

            goto get_existing;

         }

      }

      if(m_semid < 0)
         throw resource_exception(get_app());

      semun semctl_arg;

      //set Initial value for the resource
      semctl_arg.val = 1; //Setting semval to 1

      semctl(m_semid, 0, SETVAL, semctl_arg);


   }
   else
   {

      m_key = 0;

      m_semid = -1;

      pthread_mutexattr_t attr;

      pthread_mutexattr_init(&attr);

      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

      pthread_mutex_init(&m_mutex, &attr);

   }


#endif

}


#ifdef WINDOWS

mutex::mutex(::ace::application * papp, const char * pstrName, void * h, bool bOwner) :
   object(papp),
   sync_object(pstrName)
{
   m_bOwner = bOwner;
   m_object = h;

}

mutex::mutex(const mutex & m):
object(m.get_app()),
sync_object(m.m_pszName)
{
   m_bOwner = false;
   m_object = m.m_object;

}

#elif defined(ANDROID) || defined(APPLEOS)

mutex::mutex(::ace::application * papp, const char * pstrName, sem_t * psem, bool bOwner) :
   object(papp),
   sync_object(pstrName)
{

   m_bOwner       = bOwner;
   m_pszName      = strdup(pstrName);
   m_psem         = psem;

}

mutex::mutex(const mutex & m):
   object(m.get_app()),
   sync_object(m.m_pszName)
{

   m_bOwner       = false;
   m_psem         = m.m_psem;
   m_mutex        = m.m_mutex;

}


#else

mutex::mutex(::ace::application * papp,const char * pstrName,key_t key,int32_t semid,bool bOwner):
   object(papp),
   sync_object(pstrName)
{

   m_bOwner       = bOwner;
   m_key          = key;
   m_semid        = semid;

}


mutex::mutex(const mutex & m):
   object(m.get_app()),
   sync_object(m.m_pszName)
{

   m_bOwner       = false;
   m_semid        = m.m_semid;
   m_mutex        = m.m_mutex;

}

#endif





mutex::~mutex()
{

#if defined(ANDROID) || defined(APPLEOS)

   if(m_psem != SEM_FAILED)
   {
      
      //if(m_bOwner)
      {
         sem_close(m_psem);
         sem_unlink(m_pszName);
         
      }

   }
   else
   {

      pthread_mutex_destroy(&m_mutex);

      if(m_pmutex != NULL)
      {
         delete (pthread_mutex_t *)m_pmutex;
      }

   }

#elif defined(LINUX) 
   
   if(m_semid >= 0)
   {

      if(m_bOwner)
      {

         semun ignored_argument;

         semctl(m_semid, 1, IPC_RMID , ignored_argument);

      }

   }
   else
   {

      pthread_mutex_destroy(&m_mutex);

      if(m_pmutex != NULL)
      {
         delete (pthread_mutex_t *)m_pmutex;
      }

   }

#endif

}


bool mutex::already_exists()
{

   return m_bAlreadyExists;

}


#ifdef ANDROID

wait_result mutex::wait(const duration & duration)
{

   uint32_t dwTimeout = duration.lock_duration();

   timespec delay;

   delay.tv_sec = 0;
   delay.tv_nsec = 1000000;  // 1 milli sec delay

   int32_t irc;

   bool bFirst = true;

   uint32_t start = ::get_tick_count();

   if(m_psem != SEM_FAILED)
   {
      //Wait for Zero

      while((dwTimeout == (uint32_t) INFINITE || ::get_tick_count() - start < dwTimeout) || bFirst)
      {

         bFirst = false;

         int32_t ret = sem_timedwait(m_psem, &delay);

         if(ret < 0)
         {
            /* check whether somebody else has the mutex */
            if (errno == ETIMEDOUT)
            {
               /* sleep for delay time */
               //nanosleep(&delay, NULL);
            }
            else
            {
               return wait_result(wait_result::Failure);
            }
         }
         else
         {
            return wait_result(wait_result::Event0);
         }
      }

   }
   else
   {

      if(duration.is_pos_infinity())
      {

         irc = pthread_mutex_lock(&m_mutex);
         if (!irc)
         {
            return wait_result(wait_result::Event0);
         }
         else
         {
            return wait_result(wait_result::Failure);

         }

      }
      else
      {
         while((::get_tick_count() - start < dwTimeout) || bFirst)
         {

            bFirst = false;

            // Tries to acquire the mutex and access the shared resource,
            // if success, access the shared resource,
            // if the shared reosurce already in use, it tries every 1 milli sec
            // to acquire the resource
            // if it does not acquire the mutex within 2 secs delay,
            // then it is considered to be failed

            irc = pthread_mutex_trylock(&m_mutex);
            if (!irc)
            {
               return wait_result(wait_result::Event0);
            }
            else
            {
               // check whether somebody else has the mutex
               if (irc == EPERM )
               {
                  // Yes, Resource already in use so sleep
                  nanosleep(&delay, NULL);
               }
               else
               {
                  return wait_result(wait_result::Failure);
               }
            }
         }
      }
   }

   return wait_result(wait_result::Timeout);

}


bool mutex::lock(const duration & duration)
{

   wait_result result = wait(duration);

   if(!result.signaled())
      return false;

   return true;

}


#elif !defined(WINDOWS)

wait_result mutex::wait(const duration & duration)
{

   uint32_t dwTimeout = duration.lock_duration();

   timespec delay;

   delay.tv_sec = 0;
   delay.tv_nsec = 1000000;  // 1 milli sec delay

   int32_t irc;

   bool bFirst = true;

   uint32_t start = ::get_tick_count();

   if(dwTimeout == (uint32_t) INFINITE)
   {
      //dwTimeout = 30000 + 1977;
      //((::duration&)  duration) = millis(dwTimeout);
   }

#if !defined(ANDROID) && !defined(APPLEOS)
   if(m_semid >= 0)
   {
      //Wait for Zero

      struct sembuf operation[1] ;


      while((dwTimeout == (uint32_t) INFINITE || ::get_tick_count() - start < dwTimeout) || bFirst)
      {

         bFirst = false;

         //Call Wait for Zero with IPC_NOWAIT option,so it will be
         // non blocking

         operation[0].sem_op = -1; // Wait
         operation[0].sem_num = 0;
         operation[0].sem_flg = IPC_NOWAIT;

         int32_t ret = semop(m_semid, operation,1);

         if(ret < 0)
         {
            /* check whether somebody else has the mutex */
            if (errno == EPERM || errno == EAGAIN)
            {
               /* sleep for delay time */
               nanosleep(&delay, NULL);
            }
            else
            {
               return wait_result(wait_result::Failure);
            }
         }
         else
         {
            return wait_result(wait_result::Event0);
         }
      }

   }
   else
#endif
   {

       if(duration.is_pos_infinity())
       {

          irc = pthread_mutex_lock(&m_mutex);
          if (!irc)
          {
               return wait_result(wait_result::Event0);
          }
          else
          {
               return wait_result(wait_result::Failure);

          }

       }
        else
    {
      while((::get_tick_count() - start < dwTimeout) || bFirst)
      {

         bFirst = false;

         // Tries to acquire the mutex and access the shared resource,
         // if success, access the shared resource,
         // if the shared reosurce already in use, it tries every 1 milli sec
         // to acquire the resource
         // if it does not acquire the mutex within 2 secs delay,
         // then it is considered to be failed

          irc = pthread_mutex_trylock(&m_mutex);
          if (!irc)
          {
               return wait_result(wait_result::Event0);
          }
          else
          {
            // check whether somebody else has the mutex
            if(irc == EPERM ) // owned by own thread !! OK !!
            {
               return wait_result(wait_result::Failure);
            }
            else if (irc == EBUSY)
            {
               // Yes, Resource already in use so sleep
               nanosleep(&delay, NULL);
            }
            else
            {
               return wait_result(wait_result::Failure);
            }
          }
       }
     }
   }

   if(dwTimeout == 30000 + 1977 && g_iMutex == 0)
   {
      g_iMutex++;
      debug_print("HAHAHA");
   }

   return wait_result(wait_result::Timeout);

}


bool mutex::lock(const duration & duration)
{

    wait_result result = wait(duration);

    if(!result.signaled())
        return false;

    return true;

}

#endif











bool mutex::unlock()
{

#ifdef _WIN32

   return ::ReleaseMutex(m_object) != FALSE;

#elif defined(ANDROID) || defined(APPLEOS)


   if(m_psem != SEM_FAILED)
   {

      return sem_post(m_psem) == 0;

   }
   else
   {

      return pthread_mutex_unlock(&m_mutex) != 0;

   }


#else

   if(m_semid >= 0)
   {

      struct sembuf operation[1] ;

      operation[0].sem_op = 1; //Release the resource so Wait in process
                               // 1 will be triggered
      operation[0].sem_num = 0;
      operation[0].sem_flg = SEM_UNDO;

      //Release semaphore
      semop(m_semid, operation, 1);
       union semun sem_union;
        sem_union.val = 0;
        return semctl(m_semid, 0, GETVAL, sem_union);

   }
   else
   {

      return pthread_mutex_unlock(&m_mutex) != 0;

   }

#endif

}



mutex * mutex::open_mutex(::ace::application * papp,  const char * pstrName)
{

#ifdef WINDOWS

   HANDLE h = ::OpenMutexW(SYNCHRONIZE, FALSE, ::str::international::utf8_to_unicode(pstrName));

   if(h == NULL || h == INVALID_HANDLE_VALUE)
      return NULL;

   mutex * pmutex = canew(mutex(papp, pstrName, h));

   return pmutex;

#elif defined(ANDROID) || defined(APPLEOS)

   string strName = pstrName;

   sem_t * psem;

   int isCreator = 0;

   if((psem = sem_open(strName,O_CREAT | O_EXCL,0666,1)) != SEM_FAILED)
   {

      isCreator = 1;

   }
   else
   {

      psem = sem_open(strName,0);

      if(psem == SEM_FAILED)
      {

         //throw resource_exception(papp,"failed to create named mutex");
         throw resource_exception(papp);

      }

   }


   mutex * pmutex = new mutex(papp,strName, psem, isCreator);

   return pmutex;

#else

   if(pstrName == NULL || *pstrName == '\0')
      return NULL;

   string strName = pstrName;

   key_t key = ftok(strName, 0); //Generate a unique key or supply a value

   int32_t semid = semget(
               key, // a unique identifier to identify semaphore set
               1,  // number of semaphore in the semaphore set
               0666 // permissions (rwxrwxrwx) on the new
                    //semaphore set and creation flag
                  );
   if(semid < 0)
   {
      return NULL;
   }

   mutex * pmutex = new mutex(papp, strName, key, semid);

   return pmutex;

#endif

}


void wait_until_mutex_does_not_exist(const char * pszName)
{
   mutex * pmutex = new mutex(get_thread_app(), false, "Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      while(::GetLastError() == ERROR_ALREADY_EXISTS)
      {
         delete pmutex;
         Sleep(184);
         pmutex = new mutex(get_thread_app(), false, "Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
      }
   }
   delete pmutex;
}


mutex * g_pmutexUiDestroyed = NULL;


CLASS_DECL_ACE mutex * get_ui_destroyed_mutex()
{

  return g_pmutexUiDestroyed;

}


#ifdef WINDOWSEX
null_dacl_security_attributes::null_dacl_security_attributes()
{


   ZERO(m_securityattributes);

   m_securityattributes.nLength = sizeof(m_securityattributes);

   m_securityattributes.bInheritHandle = FALSE; // object uninheritable

   // declare and initialize a security descriptor
   ZERO(m_securitydescriptor);

   bool bInitOk = InitializeSecurityDescriptor(&m_securitydescriptor,SECURITY_DESCRIPTOR_REVISION) != FALSE;

   if(bInitOk)
   {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      bool bSetOk = SetSecurityDescriptorDacl(&m_securitydescriptor,TRUE,(PACL)NULL,FALSE) != FALSE;

      if(bSetOk)
      {

         m_securityattributes.lpSecurityDescriptor = &m_securitydescriptor;

      }

   }

}
#endif


spaadmin_mutex::spaadmin_mutex() :
#ifdef WINDOWSEX
mutex(NULL,false,"Global\\::ca2::fontopus::ccvotagus::" + process_platform_dir_name() + "::198411151951042219770204-11dd-ae16-0800200c7784",&m_securityattributes)
, sync_object("Global\\::ca2::fontopus::ccvotagus::" + process_platform_dir_name() + "::198411151951042219770204-11dd-ae16-0800200c7784")
#else
mutex(NULL,false,"Global\\::ca2::fontopus::ccvotagus::" + process_platform_dir_name() + "::198411151951042219770204-11dd-ae16-0800200c7784")
, sync_object("Global\\::ca2::fontopus::ccvotagus::" + process_platform_dir_name() + "::198411151951042219770204-11dd-ae16-0800200c7784")
#endif

{
}