/*
Copyright (c) 2001 - 2002
Author: Konstantin Boukreev
E-mail: konstantin@mail.primorye.ru
Created: 25.12.2001 19:41:07
Version: 1.0.2

Permission to use, copy, modify, distribute and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear
in supporting documentation.  Konstantin Boukreev makes no representations
about the suitability of this software for any purpose.
It is provided "as is" without express or implied warranty.

*/

#include "framework.h"

#include <malloc.h>

#ifdef WINDOWS

#include <tlhelp32.h>

#pragma comment (lib, "dbghelp")

#ifdef VERIFY
#undef VERIFY
#endif // VERIFY

#ifdef DEBUG
#define VERIFY(x) _ASSERTE(x)
#else
#define VERIFY(x) (x)
#endif //DEBUG

#define WORK_AROUND_SRCLINE_BUG

#ifdef DEBUG
//#if 1
// #define SYM_ENGINE_TRACE_SPIN_COUNT
#endif //DEBUG

///////////////////////////////////////////////////////////////////////

bool IsNT()
{
#if 1
   OSVERSIONINFO vi = { sizeof(vi)};
   ::GetVersionEx(&vi);
   return vi.dwPlatformId == VER_PLATFORM_WIN32_NT;
#else
   return false;
#endif
}

HANDLE SymGetProcessHandle()
{
   if (IsNT())
      //   if (0)
         return GetCurrentProcess();
   else
      return (HANDLE)GetCurrentProcessId();
}

bool __stdcall My_ReadProcessMemory (HANDLE, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, SIZE_T * lpNumberOfBytesRead)
{
   return ReadProcessMemory(GetCurrentProcess(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

namespace exception
{

   engine::engine (unsigned int uiAddress) :
      m_uiAddress(uiAddress),
      m_bOk(false),
      m_pstackframe(NULL)
   {
   }

   engine::~engine()
   {
      //   if (m_bOk) guard::instance().clear();
      delete m_pstackframe;
   }

   size_t engine::module(vsstring & str)
   {
      if (!check())
         return 0;

      HANDLE hprocess = SymGetProcessHandle();
      HMODULE hmodule = (HMODULE)SymGetModuleBase (hprocess, m_uiAddress);
      if (!hmodule) return 0;
      return get_module_basename(hmodule, str);
   }

   size_t engine::symbol(vsstring & str, DWORD_PTR * pdisplacement)
   {
      if (!check())
         return 0;

      BYTE symbol [ 512 ] ;
      PIMAGEHLP_SYMBOL pSym = (PIMAGEHLP_SYMBOL)&symbol;
      memset(pSym, 0, sizeof(symbol)) ;
      pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) ;
      pSym->MaxNameLength = sizeof(symbol) - sizeof(IMAGEHLP_SYMBOL);

      HANDLE hprocess = SymGetProcessHandle();
      DWORD_PTR displacement = 0;
      int r = SymGetSymFromAddr(hprocess, m_uiAddress, &displacement, pSym);
      if (!r) return 0;
      if (pdisplacement)
         *pdisplacement = displacement;

      str = pSym->Name;
      str += "()";

      return str.get_length();
   }

   index engine::fileline (vsstring & str, DWORD_PTR * pline, DWORD_PTR * pdisplacement)
   {

      if (!check())
         return 0;

      IMAGEHLP_LINE img_line;
      memset(&img_line, 0, sizeof(IMAGEHLP_LINE));
      img_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

      HANDLE hprocess = SymGetProcessHandle();
      DWORD_PTR displacement = 0;
      if (!get_line_from_address(hprocess, m_uiAddress, &displacement, &img_line))
         return 0;
      if (pdisplacement)
         *pdisplacement = displacement;
      if (pline)
         *pline = img_line.LineNumber;
      str = img_line.FileName;
      return str.get_length();
   }

   bool engine::stack_first (CONTEXT* pcontext)
   {
      if(!pcontext || IsBadReadPtr(pcontext, sizeof(CONTEXT)))
         return false;

      if(!check())
         return false;

      if(!m_pstackframe)
      {
         m_pstackframe = new STACKFRAME;
         if (!m_pstackframe) return false;
      }

      memset(m_pstackframe, 0, sizeof(STACKFRAME));

#ifdef AMD64
      m_pstackframe->AddrPC.Offset       = pcontext->Rip;
      m_pstackframe->AddrPC.Mode         = AddrModeFlat;
      m_pstackframe->AddrStack.Offset    = pcontext->Rsp;
      m_pstackframe->AddrStack.Mode      = AddrModeFlat;
      m_pstackframe->AddrFrame.Offset    = pcontext->Rbp;
      m_pstackframe->AddrFrame.Mode      = AddrModeFlat;
#elif defined(X86)
      m_pstackframe->AddrPC.Offset       = pcontext->Eip;
      m_pstackframe->AddrPC.Mode         = AddrModeFlat;
      m_pstackframe->AddrStack.Offset    = pcontext->Esp;
      m_pstackframe->AddrStack.Mode      = AddrModeFlat;
      m_pstackframe->AddrFrame.Offset    = pcontext->Ebp;
      m_pstackframe->AddrFrame.Mode      = AddrModeFlat;
#else
      m_pstackframe->AddrPC.Offset       = (DWORD)pcontext->Fir;
      m_pstackframe->AddrPC.Mode         = AddrModeFlat;
      m_pstackframe->AddrReturn.Offset   = (DWORD)pcontext->IntRa;
      m_pstackframe->AddrReturn.Mode     = AddrModeFlat;
      m_pstackframe->AddrStack.Offset    = (DWORD)pcontext->IntSp;
      m_pstackframe->AddrStack.Mode      = AddrModeFlat;
      m_pstackframe->AddrFrame.Offset    = (DWORD)pcontext->IntFp;
      m_pstackframe->AddrFrame.Mode      = AddrModeFlat;
#endif

      m_pcontext = pcontext;
      return stack_next();
   }

   bool engine::stack_next()
   {
      if (!m_pstackframe || !m_pcontext)
      {
         _ASSERTE(0);
         return false;
      }

      if (!m_bOk)
      {
         _ASSERTE(0);
         return false;
      }

      SetLastError(0);
      HANDLE hprocess = SymGetProcessHandle();
      bool r = StackWalk (IMAGE_FILE_MACHINE_I386,
         hprocess,
         GetCurrentThread(),
         m_pstackframe,
         m_pcontext,
         (PREAD_PROCESS_MEMORY_ROUTINE)My_ReadProcessMemory,
         SymFunctionTableAccess,
         SymGetModuleBase,
         0);

      if (!r || !m_pstackframe->AddrFrame.Offset)
      {
         return false;
      }

      // "Debugging Applications" John Robbins
      // Before I get too carried away and start calculating
      // everything, I need to double-check that the address returned
      // by StackWalk really exists. I've seen cases in which
      // StackWalk returns TRUE but the address doesn't belong to
      // a module in the process.
      DWORD_PTR dwModBase = SymGetModuleBase (hprocess, m_pstackframe->AddrPC.Offset);
      if (!dwModBase)
      {
         ::OutputDebugString("engine::stack_next :: StackWalk returned TRUE but the address doesn't belong to a module in the process.");
         return false;
      }

      address(m_pstackframe->AddrPC.Offset);
      return true;
   }

   bool engine::get_line_from_address (HANDLE hprocess, DWORD_PTR uiAddress, DWORD_PTR * puiDisplacement, IMAGEHLP_LINE * pline)
   {
#ifdef WORK_AROUND_SRCLINE_BUG
      // "Debugging Applications" John Robbins
      // The problem is that the symbol engine finds only those source
      // line addresses (after the first lookup) that fall exactly on
      // a zero displacement. I'll walk backward 100 bytes to
      // find the line and return the proper displacement.
      DWORD dwDisplacement = 0 ;
      while (!SymGetLineFromAddr (hprocess, uiAddress - dwDisplacement, (DWORD*)puiDisplacement, pline))
      {
         if (100 == ++dwDisplacement)
            return false;
      }

      // "Debugging Applications" John Robbins
      // I found the line, and the source line information is correct, so
      // change the displacement if I had to search backward to find the source line.
      if(dwDisplacement)
         *puiDisplacement = dwDisplacement;
      return true;
#else
      return 0 != SymGetLineFromAddr (hprocess, uiAddress, (DWORD *) puiDisplacement, pline);
#endif
   }

   size_t engine::get_module_basename (HMODULE hmodule, vsstring & str)
   {

      char filename[MAX_PATH];

      DWORD r = GetModuleFileNameA(hmodule, filename, MAX_PATH);

      if(!r)
         return 0;

      str = filename;

      // find the last '\' mark.
      size_t iPos = str.rfind('\\');

      if(iPos >= 0)
      {

         str = str.substr(iPos + 1);

      }

      return str.get_length();

   }

   bool engine::check()
   {
      if (!m_bOk)
         m_bOk = guard::instance().init();
      return m_bOk;
   }

   engine::guard::guard()
      : m_iRef(0)
   {}

   engine::guard::~guard()
   {
      clear();
   }

   bool engine::guard::init()
   {
      if (!m_iRef)
      {
         m_iRef = -1;

         HANDLE hprocess = SymGetProcessHandle();
         DWORD  dwPid = GetCurrentProcessId();

         // initializes
         SymSetOptions (SymGetOptions()|SYMOPT_DEFERRED_LOADS|SYMOPT_LOAD_LINES);
         //   SymSetOptions (SYMOPT_UNDNAME|SYMOPT_LOAD_LINES);
         if (::SymInitialize(hprocess, 0, TRUE))
         {
            // enumerate modules
            if (IsNT())
            {
               typedef bool (WINAPI *ENUMPROCESSMODULES)(HANDLE, HMODULE*, DWORD, LPDWORD);

               HINSTANCE hInst = LoadLibrary("psapi.dll");
               if (hInst)
               {
                  ENUMPROCESSMODULES fnEnumProcessModules =
                     (ENUMPROCESSMODULES)GetProcAddress(hInst, "EnumProcessModules");
                  DWORD cbNeeded = 0;
                  if (fnEnumProcessModules &&
                     fnEnumProcessModules(GetCurrentProcess(), 0, 0, &cbNeeded) &&
                     cbNeeded)
                  {
                     HMODULE * pmod = (HMODULE *)alloca(cbNeeded);
                     DWORD cb = cbNeeded;
                     if (fnEnumProcessModules(GetCurrentProcess(), pmod, cb, &cbNeeded))
                     {
                        m_iRef = 0;
                        for (unsigned i = 0; i < cb / sizeof (HMODULE); ++i)
                        {
                           if (!load_module(hprocess, pmod[i]))
                           {
                              //   m_iRef = -1;
                              //   break;
                              _ASSERTE(0);
                           }
                        }
                     }
                  }
                  else
                  {
                     _ASSERTE(0);
                  }
                  VERIFY(FreeLibrary(hInst));
               }
               else
               {
                  _ASSERTE(0);
               }
            }
            else
            {
               typedef HANDLE (WINAPI *CREATESNAPSHOT)(DWORD, DWORD);
               typedef bool (WINAPI *MODULEWALK)(HANDLE, LPMODULEENTRY32);

               HMODULE hMod = GetModuleHandle("kernel32");
               CREATESNAPSHOT fnCreateToolhelp32Snapshot = (CREATESNAPSHOT)GetProcAddress(hMod, "CreateToolhelp32Snapshot");
               MODULEWALK fnModule32First = (MODULEWALK)GetProcAddress(hMod, "Module32First");
               MODULEWALK fnModule32Next  = (MODULEWALK)GetProcAddress(hMod, "Module32Next");

               if (fnCreateToolhelp32Snapshot &&
                  fnModule32First &&
                  fnModule32Next)
               {
                  HANDLE hModSnap = fnCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
                  if (hModSnap)
                  {
                     MODULEENTRY32 me32 = {0};
                     me32.dwSize = sizeof(MODULEENTRY32);
                     if (fnModule32First(hModSnap, &me32))
                     {
                        m_iRef = 0;
                        do
                        {
                           if (!load_module(hprocess, me32.hModule))
                           {
                              //   m_iRef = -1;
                              //   break;
                           }
                        }
                        while(fnModule32Next(hModSnap, &me32));
                     }
                     VERIFY(CloseHandle(hModSnap));
                  }
               }
            }

            if (m_iRef == -1)
            {
               VERIFY(SymCleanup(SymGetProcessHandle()));
            }
         }
         else
         {
            _ASSERTE(0);
         }
      }
      if (m_iRef == -1)
         return false;
      if (0 == m_iRef)
         ++m_iRef; // lock it once
      //   ++m_iRef;
      return true;
   }

   void engine::guard::clear()
   {
      if (m_iRef ==  0) return;
      if (m_iRef == -1) return;
      if (--m_iRef == 0)
      {
         VERIFY(SymCleanup(SymGetProcessHandle()));
      }
   }

   bool engine::guard::load_module(HANDLE hProcess, HMODULE hMod)
   {
      char filename[MAX_PATH];
      if (!GetModuleFileNameA(hMod, filename, MAX_PATH))
         return false;

      HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
      if (hFile == INVALID_HANDLE_VALUE) return false;

      // "Debugging Applications" John Robbins
      // For whatever reason, SymLoadModule can return zero, but it still loads the modules. Sheez.
      SetLastError(ERROR_SUCCESS);
      if (!SymLoadModule(hProcess, hFile, filename, 0, (DWORD)hMod, 0) &&
         ERROR_SUCCESS != GetLastError())
      {
         return false;
      }

      return true;
   }

   bool engine::stack_trace(vsstring & str, CONTEXT * pcontext, DWORD_PTR uiSkip, const char * pszFormat)
   {
      if(!pszFormat) return false;
      engine engine(0);
      return stack_trace(str, engine, pcontext, uiSkip, pszFormat);
   }

   /////////////////////////////////////////////
   // prints a current thread's stack

   struct current_context : CONTEXT
   {
      HANDLE   thread;
      volatile int signal;
   };

   DWORD WINAPI engine::stack_trace_ThreadProc(void * lpvoidParam)
   {

#ifdef AMD64

      return -1;

#endif

      current_context * pcontext = reinterpret_cast<current_context*>(lpvoidParam);

      __try
      {
         // Konstantin, 14.01.2002 17:21:32
         // must wait in spin lock until main thread will leave a ResumeThread (must return back to ::fontopus::user context)
         int iInverseAgility = 26 + 33; // former iPatienceQuota
         int iPatience = iInverseAgility;
         while(pcontext->signal && iPatience > 0)
         {
            if(!SwitchToThread())
               Sleep(10); // forces switch to another thread
            iPatience--;
         }

         char sz[200];
         sprintf(sz, "engine::stack_trace patience near down %u%%\n", iPatience * 100 / iInverseAgility);
         ::OutputDebugString(sz);

         if (-1 == SuspendThread(pcontext->thread))
         {
            pcontext->signal  = -1;
            __leave;
         }

         __try
         {
            pcontext->signal = GetThreadContext(pcontext->thread, pcontext) ? 1 : -1;
         }
         __finally
         {
            VERIFY(-1 != ResumeThread(pcontext->thread));
         }
      }
      __except(EXCEPTION_EXECUTE_HANDLER)
      {
         pcontext->signal  = -1;
      }
      return 0;
   }

   bool engine::stack_trace(vsstring & str, DWORD_PTR uiSkip, const char * pszFormat)
   {
      if(!pszFormat) return false;

      // attempts to get current thread's context

      current_context context;
      memset(&context, 0, sizeof(current_context));

      bool bOk = DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(),
         &context.thread, 0, 0, DUPLICATE_SAME_ACCESS);

      _ASSERTE(bOk);
      _ASSERTE(context.thread);

      if (!bOk || !context.thread)
         return false;

      context.ContextFlags = CONTEXT_CONTROL; // CONTEXT_FULL;
      context.signal = -1;

      DWORD dwDummy;
      HANDLE hthreadWorker = CreateThread(0, 0, &engine::stack_trace_ThreadProc, &context, CREATE_SUSPENDED, &dwDummy);
      _ASSERTE(hthreadWorker);

      if (hthreadWorker)
      {
         //VERIFY(SetThreadPriority(hthreadWorker, THREAD_PRIORITY_TIME_CRITICAL)); //  THREAD_PRIORITY_HIGHEST
         if (-1 != ResumeThread(hthreadWorker))
         {
            int iInverseAgility = 26 + 33; // former iPatienceQuota
            int iPatience = iInverseAgility;
            // Konstantin, 14.01.2002 17:21:32
            context.signal = 0;            // only now the worker thread can get this thread context
            while (!context.signal && iPatience > 0)
            {
               if(!SwitchToThread())
                  Sleep(10); // forces switch to another thread
               iPatience--; // wait in spin
            }
            char sz[200];
            sprintf(sz, "engine::stack_trace patience near down %u%%\n", iPatience * 100 / iInverseAgility);
            ::OutputDebugString(sz);
         }
         else
         {
            VERIFY(TerminateThread(hthreadWorker, 0));
         }
         VERIFY(CloseHandle(hthreadWorker));
      }
      VERIFY(CloseHandle(context.thread));

      if (context.signal == -1)
      {
         _ASSERTE(0);
         return false;
      }
      // now it can print stack
      engine symengine(0);
      stack_trace(str, symengine, &context, uiSkip, pszFormat);
      return true;
   }

   bool engine::stack_trace(vsstring & str, engine& symengine,
      CONTEXT * pcontext, DWORD_PTR uiSkip, const char * pszFormat)
   {
      if (!symengine.stack_first(pcontext))
         return false;

      //   char buf [512] = {0};
      //   char fbuf[512] = {0};
      //   char sbuf[512] = {0};

      vsstring strBuf;
      vsstring strFile;
      vsstring strSymbol;

      do
      {
         if (!uiSkip)
         {
            DWORD_PTR uiLineNumber = 0;
            DWORD_PTR uiLineDisplacement = 0;
            DWORD_PTR uiSymbolDisplacement = 0;
            strFile.clear();
            strSymbol.clear();

            for (char * p = (char *)pszFormat; *p; ++p)
            {
               if (*p == '%')
               {
                  ++p; // skips '%'
                  char c = *p;
                  switch (c)
                  {
                  case 'm':
                     if(symengine.module(strBuf))
                     {
                        str += strBuf;
                     }
                     else
                     {
                        str += "?.?";
                     }
                     break;
                  case 'f':
                     if(strFile.is_empty())
                     {
                        if(!symengine.fileline(strFile, &uiLineNumber, &uiLineDisplacement))
                        {
                           strFile = " ";
                        }
                     }
                     str += strFile;
                     break;
                  case 'l':
                     if(strFile.is_empty())
                     {
                        if(!symengine.fileline(strFile, &uiLineNumber, &uiLineDisplacement))
                        {
                           strFile = " ";
                        }
                     }
                     if (*(p + 1) == 'd')
                     {
                        strBuf = itoa_dup(uiLineDisplacement);
                        str += strBuf;
                        ++p;
                     }
                     else
                     {
                        strBuf = itoa_dup(uiLineNumber);
                        str += strBuf;
                     }
                     break;
                  case 's':
                     if(strSymbol.is_empty())
                     {
                        if(!symengine.symbol(strSymbol, &uiSymbolDisplacement))
                        {
                           strSymbol = "?()";
                        }
                     }
                     if (*(p + 1) == 'd')
                     {
                        strBuf = itoa_dup(uiSymbolDisplacement);
                        str += strBuf;
                        ++p;
                     }
                     else
                     {
                        str += strSymbol;
                     }
                     break;
                  case '%':
                     str += '%';
                     break;
                  default:
                     str += '%';
                     str += c;
                     break;
                  }
               }
               else
               {
                  str += *p;
               }
            }
         }
         else
         {
            --uiSkip;
         }
      }
      while (symengine.stack_next());
      return true;
   }


} // namespace exception


#endif



