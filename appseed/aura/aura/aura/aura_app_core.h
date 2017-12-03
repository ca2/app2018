﻿#pragma once


struct aura_main_data
{
public:

   ::command::command *          m_pmaininitdata;
   bool                          m_bConsole;
   int                           m_argc;
   char **                       m_argv;
   int                           m_iExitCode;

#ifdef WINDOWS

   HINSTANCE                     m_hinstance;
   HINSTANCE                     m_hPrevInstance;
   LPTSTR                        m_lpCmdLine;
   int32_t                       m_nCmdShow;

#endif

   aura_main_data(int argc, char ** argv)
   {

      m_bConsole = true;

      m_argc = argc;

      m_argv = argv;

#ifdef WINDOWSEX

      m_hinstance = ::GetModuleHandle(NULL);

      m_hPrevInstance = NULL;

      m_lpCmdLine = NULL;

      m_nCmdShow = SW_SHOWDEFAULT;

#endif

   }

#ifdef WINDOWSEX

   aura_main_data(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
   {

      m_bConsole = false;

      m_hinstance = hinstance;

      m_hPrevInstance = hPrevInstance;

      m_lpCmdLine = lpCmdLine;

      m_nCmdShow = nCmdShow;

   }

#endif

   void * operator new(size_t sz)
   {

      return malloc(sz * 2);

   }

   void operator delete(void * p)
   {

      free(p);

   }

};


#define APP_CORE_MAXIMUM_ERROR_COUNT 100


// ATTENTION
// This class should (if it uses) member functions with care:
// It is used after all application app_core (library) finalization,
// when even new/delete/strings/ids can be used anymore for example.

class CLASS_DECL_AURA app_core
{
public:


   bool                          m_bAcidApp = false;
   ::aura::PFN_GET_NEW_LIBRARY   m_pfnNewLibrary = NULL;
   ::aura::PFN_GET_NEW_APP       m_pfnNewApp = NULL;
   DWORD                         m_dwStartTime;
   DWORD                         m_dwAfterApplicationFirstRequest;
   aura_main_data *              m_pmaindata = NULL;
   ::aura::system *              m_psystem = NULL;

   int                           m_iaError[APP_CORE_MAXIMUM_ERROR_COUNT];
   int                           m_iErrorCount = 0;
   int                           m_iTotalErrorCount = 0;


   app_core(aura_main_data * pdata);


   bool on_result(int iResultCode);

   bool beg();

   bool ini();

   void run();

   void end();

   void * operator new(size_t sz)
   {

      return malloc(sz * 2);

   }

   void operator delete(void * p)
   {

      free(p);

   }

};

typedef bool FN_AURA_APP_CORE(app_core * pappcore);

typedef FN_AURA_APP_CORE * PFN_AURA_APP_CORE;

CLASS_DECL_AURA long aura_aura(aura_main_data * pmaindata);

CLASS_DECL_AURA void aura_boot(app_core * pappcore);

CLASS_DECL_AURA void aura_main(app_core * pappcore);

CLASS_DECL_AURA bool node_fill(app_core * pappcore);


class CLASS_DECL_AURA aura_prelude
{
public:


   static aura_prelude *         s_pprelude;

   ::aura::PFN_GET_NEW_APP       m_pfnNewApp;


   aura_prelude();


   aura_prelude(::aura::PFN_GET_NEW_APP pgetnewapp);


   virtual ~aura_prelude();


   static bool defer_call_construct(app_core * pappcore);


   virtual bool construct(app_core * pappcore);


   static bool defer_call_prelude(app_core * pappcore);


   virtual bool prelude(app_core * pappcore);


};



