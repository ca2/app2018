#include "framework.h"
//#include "android1.h"

/////////////////////////////////////////////////////////////////////////////
// __EXCEPTION_CONTEXT (thread global state)

//inline __EXCEPTION_CONTEXT* __get_exception_context()
//{
//   DWORD lError = get_last_error();
//   __EXCEPTION_CONTEXT* pContext = &gen_ThreadState->m_exceptionContext;
//   set_last_error(lError);
//   return pContext;
//}

/////////////////////////////////////////////////////////////////////////////
// __exception_link linked 'jmpbuf' and out-of-line helpers

__exception_link::__exception_link()
{
   // setup initial link state
   //m_pException = NULL;    // no current exception yet

   //// wire into top of exception link stack
   //__EXCEPTION_CONTEXT* pContext = __get_exception_context();
   //m_pLinkPrev = pContext->m_pLinkTop;
   //pContext->m_pLinkTop = this;
}


// out-of-line cleanup called from inline __exception_link destructor
CLASS_DECL_AURA void __try_cleanup()
{
   //__EXCEPTION_CONTEXT* pContext = __get_exception_context();
   //__exception_link* pLinkTop = pContext->m_pLinkTop;

   //// delete current exception
   //ASSERT(pLinkTop != NULL);
   //if (pLinkTop == NULL)
   //   return;
   //if (pLinkTop->m_pException != NULL)
   //   pLinkTop->m_pException->Delete();

   //// remove ourself from the top of the chain
   //pContext->m_pLinkTop = pLinkTop->m_pLinkPrev;
}

// special out-of-line implementation of THROW_LAST (for auto-delete behavior)
void CLASS_DECL_AURA __throw_last_cleanup()
{
   //__EXCEPTION_CONTEXT* pContext = __get_exception_context();
   //__exception_link* pLinkTop = pContext->m_pLinkTop;

   //// check for THROW_LAST inside of auto-delete block
   //if (pLinkTop != NULL)
   //{
   //   // make sure current exception does not get auto-deleted
   //   pLinkTop->m_pException = NULL;
   //}

   //// THROW_LAST macro will do actual '_throw('
}






namespace gen
{

#if defined( _CUSTOM_THROW )  // You can define your own _throw( hresult_exception to _throw( a custom exception.

CLASS_DECL_AURA void WINAPI atl_throw_impl( HRESULT hr )
{
   TRACE(atlTraceException, 0, "_throw(hresult_exception: hr = 0x%x\n", hr ));
#ifdef _AFX
   if( hr == E_OUTOFMEMORY )
   {
      _throw(memory_exception());
   }
   else
   {
//      ::ca2::ThrowOleException( hr );
   }
#else
   _throw(atl_exception( hr ));
#endif
};

#endif


// Throw a atl_exception with th given HRESULT
#if !defined( _CUSTOM_THROW )  // You can define your own _throw( hresult_exception

//CLASS_DECL_AURA void WINAPI atl_throw_impl(HRESULT hr)
//{
//   TRACE("_throw(hresult_exception: hr = 0x%x\n", hr));
 //  _throw(hresult_exception(hr));
/*   ::output_debug_string("_throw(hresult_exception"));
   char sz[200];
   sprintf(sz, "0x%s", hr);
   ::output_debug_string(sz);
   ::output_debug_string("\n");
   //TRACE(trace::category_Exception, 0, "_throw(hresult_exception: hr = 0x%x\n", hr ));
   ASSERT( false );
   DWORD dwExceptionCode;
   switch(hr)
   {
   case E_OUTOFMEMORY:
      dwExceptionCode = STATUS_NO_MEMORY;
      break;
   default:
      dwExceptionCode = EXCEPTION_ILLEGAL_INSTRUCTION;
   }
   atl_raise_exception((DWORD)dwExceptionCode);*/
//}
#endif


} // namespace gen
