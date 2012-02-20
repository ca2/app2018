#include "StdAfx.h"


CLASS_DECL_ca void * base_ca2_alloc(size_t size);
CLASS_DECL_ca void * base_ca2_alloc_dbg(size_t nSize, int nBlockUse, const char * szFileName, int nLine);
CLASS_DECL_ca void * base_ca2_realloc(void * pvoid, size_t nSize, int nBlockUse, const char * szFileName, int nLine);
CLASS_DECL_ca void   base_ca2_free(void * pvoid, int iBlockType);
CLASS_DECL_ca size_t base_ca2_msize(void * pvoid, int iBlockType);


void use_base_ca2_allocator()
{

   g_pfnca2_alloc       = &base_ca2_alloc;
   g_pfnca2_alloc_dbg   = &base_ca2_alloc_dbg;
   g_pfnca2_realloc     = &base_ca2_realloc;
   g_pfnca2_free        = &base_ca2_free;
   g_pfnca2_msize       = &base_ca2_msize;

}


void * base_ca2_alloc(size_t size)
{
   byte * p = (byte *) ca2_heap_alloc(size + 4 + 32);
   if(p == NULL)
   {
      AfxThrowMemoryException();
   }
   p[0] = 0;
   *((size_t *) &p[1]) = size;
   return p + 4 + 16;
}

void * base_ca2_alloc_dbg(size_t nSize, int nBlockUse, const char * szFileName, int nLine)
{
   UNREFERENCED_PARAMETER(nBlockUse);
   UNREFERENCED_PARAMETER(szFileName);
   UNREFERENCED_PARAMETER(nLine);
   //25/07/2011 - Bug DoubleUniqueID for single Call no xpressions no sector 8J analyzed with Cle - Cleber Jaizer - with Caller-Called-StartTime-tuple
   //TODO: to do the dbg version
   //byte * p = (byte *) _malloc_dbg(nSize + 4 + 32, nBlockUse, szFileName, nLine);
   byte * p = (byte *) ca2_heap_alloc_dbg(nSize + 4 + 32, nBlockUse, szFileName, nLine);
   if(p == NULL)
   {
      AfxThrowMemoryException();
   }
   p[0] = 1;
   *((size_t *) &p[1]) = nSize;
   return p + 4 + 16;
}

void * base_ca2_realloc(void * pvoid, size_t nSize, int nBlockUse, const char * szFileName, int nLine)
{
   byte * p = (byte *) pvoid;
   if(p == NULL)
      return ca2_alloc_dbg(nSize, nBlockUse, szFileName, nLine);
   p -= (4 + 16);
   try
   {
      if(p == NULL)
      {
         return ca2_alloc_dbg(nSize, nBlockUse, szFileName, nLine);
      }
      else if(p[0] == 0)
      {
         p = (byte *) ca2_heap_realloc(p, nSize + 4 + 32);
      }
      else if(p[0] == 1)
      {
         //   //25/07/2011 - Bug DoubleUniqueID for single Call no xpressions no sector 8J analyzed with Cle - Cleber Jaizer - with Caller-Called-StartTime-tuple
   //TODO: to do the dbg version
         p = (byte *) ca2_heap_realloc_dbg(p, nSize + 4 + 32, nBlockUse, szFileName, nLine);
         //p = (byte *) _realloc_dbg(p, nSize + 4 + 32, nBlockUse, szFileName, nLine);
      }
      else
      {
         return _ca_realloc(pvoid, nSize, nBlockUse, szFileName, nLine);
      }
   }
   catch(...)
   {
      // todo: rethrow free exception
      {
         AfxThrowMemoryException();
      }
   }
   if(p == NULL)
   {
      AfxThrowMemoryException();
   }
   *((size_t *) &p[1]) = nSize;
   return p + 4 + 16;
}

void base_ca2_free(void * pvoid, int iBlockType)
{
   byte * p = (byte *) pvoid;
   if(p == NULL)
      return;
   p -= (4 + 16);
   try
   {
      if(p[0] == 0)
      {
         ca2_heap_free(p);
      }
      else if(p[0] == 1)
      {
         //   //25/07/2011 - Bug DoubleUniqueID for single Call no xpressions no sector 8J analyzed with Cle - Cleber Jaizer - with Caller-Called-StartTime-tuple
   //TODO: to do the dbg version

         ca2_heap_free_dbg(p);
         //_free_dbg(p, iBlockType);
      }
      else
      {
         _ca_free(pvoid, iBlockType);
      }
   }
   catch(...)
   {
      // todo: rethrow free exception
   }
}

size_t base_ca2_msize(void * pvoid, int iBlockType)
{
   
   byte * p = (byte *) pvoid;
   
   if(p == NULL)
      return 0;

   p -= (4 + 16);

   try
   {
      if(p[0] == 0)
      {
         return *((size_t *) &p[1]);
      }
      else if(p[0] == 1)
      {
         //   //25/07/2011 - Bug DoubleUniqueID for single Call no xpressions no sector 8J analyzed with Cle - Cleber Jaizer - with Caller-Called-StartTime-tuple
   //TODO: to do the dbg version

         return *((size_t *) &p[1]);
         //_free_dbg(p, iBlockType);
      }
      else
      {
         return _ca_msize(pvoid, iBlockType);
      }
   }
   catch(...)
   {
      // todo: rethrow free exception
   }

   return 0;

}

/*
/////////////////////////////////////////////////////////////////////////////
// Debug primitive::memory globals and implementation helpers

#ifdef _DEBUG       // most of this file is for debugging

#undef new

void * __cdecl operator new(size_t nSize, int nType, const char * lpszFileName, int nLine);
void * __cdecl operator new[](size_t nSize, int nType, const char * lpszFileName, int nLine);

/////////////////////////////////////////////////////////////////////////////
// test allocation routines


#define new DEBUG_NEW

void * AfxAllocMemoryDebug(size_t nSize, BOOL bIsObject,  const char * lpszFileName, int nLine)
{
   return ca2_alloc_dbg(nSize, bIsObject ? _AFX_CLIENT_BLOCK : _NORMAL_BLOCK, lpszFileName, nLine);
}

void AfxFreeMemoryDebug(void * pbData, BOOL bIsObject)
{
   ca2_free(pbData, bIsObject ? _AFX_CLIENT_BLOCK : _NORMAL_BLOCK);
}

/////////////////////////////////////////////////////////////////////////////
// allocation failure hook, tracking turn on

BOOL _AfxDefaultAllocHook(size_t, BOOL, LONG)
   { return TRUE; }

AFX_STATIC_DATA AFX_ALLOC_HOOK pfnAllocHook = _AfxDefaultAllocHook;

AFX_STATIC_DATA _CRT_ALLOC_HOOK pfnCrtAllocHook = NULL;
#if _MSC_VER >= 1200
int __cdecl _AfxAllocHookProxy(int nAllocType, void * pvData, size_t nSize,
   int nBlockUse, long lRequest, const unsigned char * szFilename, int nLine)
#else
int __cdecl _AfxAllocHookProxy(int nAllocType, void * pvData, size_t nSize,
   int nBlockUse, long lRequest, const char * szFilename, int nLine)
#endif
{
#if _MSC_VER >= 1200
   if (nAllocType != _HOOK_ALLOC)
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, (const unsigned char*) szFilename, nLine);
   if ((pfnAllocHook)(nSize, _BLOCK_TYPE(nBlockUse) == _AFX_CLIENT_BLOCK, lRequest))
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, (const unsigned char*) szFilename, nLine);
#else
   if (nAllocType != _HOOK_ALLOC)
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, szFilename, nLine);
   if ((pfnAllocHook)(nSize, _BLOCK_TYPE(nBlockUse) == _AFX_CLIENT_BLOCK, lRequest))
      return (pfnCrtAllocHook)(nAllocType, pvData, nSize,
         nBlockUse, lRequest, szFilename, nLine);
#endif
   return FALSE;
}

AFX_ALLOC_HOOK AfxSetAllocHook(AFX_ALLOC_HOOK pfnNewHook)
{
   if (pfnCrtAllocHook == NULL)
      pfnCrtAllocHook = _CrtSetAllocHook(_AfxAllocHookProxy);

   AFX_ALLOC_HOOK pfnOldHook = pfnAllocHook;
   pfnAllocHook = pfnNewHook;
   return pfnOldHook;
}

// This can be set to TRUE to override all AfxEnableMemoryTracking calls,
// allowing all allocations, even ca2 API internal allocations to be tracked.
BOOL _afxMemoryLeakOverride = FALSE;

BOOL AfxEnableMemoryLeakOverride(BOOL bEnable)
{
   BOOL bOldState = _afxMemoryLeakOverride;
   _afxMemoryLeakOverride = bEnable;

   return bOldState;
}

BOOL AfxEnableMemoryTracking(BOOL bTrack)
{
   if (_afxMemoryLeakOverride)
      return TRUE;

   int nOldState = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   if (bTrack)
      _CrtSetDbgFlag(nOldState | _CRTDBG_ALLOC_MEM_DF);
   else
      _CrtSetDbgFlag(nOldState & ~_CRTDBG_ALLOC_MEM_DF);
   return nOldState & _CRTDBG_ALLOC_MEM_DF;
}



/////////////////////////////////////////////////////////////////////////////
// memory_state

memory_state::memory_state()
{
   memset(this, 0, sizeof(*this));
}

void memory_state::UpdateData()
{
   for(int i = 0; i < nBlockUseMax; i++)
   {
      m_lCounts[i] = m_memState.lCounts[i];
      m_lSizes[i] = m_memState.lSizes[i];
   }
   m_lHighWaterCount = m_memState.lHighWaterCount;
   m_lTotalCount = m_memState.lTotalCount;
}

// fills 'this' with the difference, returns TRUE if significant
BOOL memory_state::Difference(const memory_state& oldState,
      const memory_state& newState)
{
   int nResult = _CrtMemDifference(&m_memState, &oldState.m_memState, &newState.m_memState);
   UpdateData();
   return nResult != 0;
}

void memory_state::dumpStatistics() const
{
   _CrtMemDumpStatistics(&m_memState);
}

// -- fill with current primitive::memory state
void memory_state::Checkpoint()
{
   _CrtMemCheckpoint(&m_memState);
   UpdateData();
}

// dump objects created after this primitive::memory state was checkpointed
// Will dump all objects if this primitive::memory state wasn't checkpointed
// dump all objects, report about non-objects also
// List request number in {}
void memory_state::dumpAllObjectsSince() const
{
   _CrtMemDumpAllObjectsSince(&m_memState);
}

/////////////////////////////////////////////////////////////////////////////
// Enumerate all objects allocated in the diagnostic primitive::memory heap

AFX_STATIC void _AfxDoForAllObjectsProxy(void * pObject, void * pContext)
{
   _AFX_ENUM_CONTEXT* p = (_AFX_ENUM_CONTEXT*)pContext;
   (*p->m_pfn)((::radix::object*)pObject, p->m_pContext);
}

void AFXAPI
AfxDoForAllObjects(void (AFX_CDECL *pfn)(::radix::object*, void *), void * pContext)
{
   if (pfn == NULL)
   {
      AfxThrowInvalidArgException();
   }
   _AFX_ENUM_CONTEXT context;
   context.m_pfn = pfn;
   context.m_pContext = pContext;
   _CrtDoForAllClientObjects(_AfxDoForAllObjectsProxy, &context);
}

/////////////////////////////////////////////////////////////////////////////
// Automatic debug primitive::memory diagnostics

BOOL AfxDumpMemoryLeaks()
{
   return _CrtDumpMemoryLeaks();
}

#endif // _AFX_NO_DEBUG_CRT

/////////////////////////////////////////////////////////////////////////////
// Non-diagnostic primitive::memory routines

int AFX_CDECL AfxNewHandler(size_t /* nSize */ /*)
{
   AfxThrowMemoryException();
}

#pragma warning(disable: 4273)

#ifndef _ApplicationFrameworkDLL
_PNH _afxNewHandler = &AfxNewHandler;
#endif

/*
_PNH AfxGetNewHandler(void)
{
#ifdef _ApplicationFrameworkDLL
   AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
   return pState->m_pfnNewHandler;
#else
   return _afxNewHandler;
#endif
}*/
/*

AFX_STATIC_DATA const _PNH _pfnUninitialized = (_PNH)-1;

#undef new

*/