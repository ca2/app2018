#include "framework.h"


bool call_stack::s_bDoStackTrace = false;


#if defined(LINUX) || defined(MACOS)
call_stack::call_stack(sp(base_application) papp, uint32_t uiSkip, void * address) :
   element(papp)
   ,m_caller_address(address)
#else
call_stack::call_stack(sp(base_application) papp, uint32_t uiSkip) :
   element(papp)
#endif
{

   if(s_bDoStackTrace && uiSkip != 0xffffffffu)
   {

      m_strCallStack = call_stack::get(uiSkip);

   }

}

call_stack::call_stack(const ::call_stack & cs) :
   element(cs)
{

   m_strCallStack = cs.m_strCallStack;

}

vsstring call_stack::get(uint32_t uiSkip)
{

   vsstring str;

   UNREFERENCED_PARAMETER(uiSkip);

#ifdef LINUX
   system::eengine().stack_trace(str, uiSkip, m_caller_address);
#else
   system::eengine().stack_trace(str, uiSkip);
#endif

   return str;

}

const char * call_stack::stack_trace() const
{

   return m_strCallStack;

}
