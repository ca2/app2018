//#include "framework.h"


#if defined(APPLE_IOS)

bool call_stack::s_bDoStackTrace = true;

#elif defined(VSNORD)

bool call_stack::s_bDoStackTrace = true;

#else

bool call_stack::s_bDoStackTrace = true;

#endif

// 0 - most deep call stack logging
// 4 - most lite call stack logging
int g_iCallStackLevel = 4; 



#if defined(LINUX) || defined(APPLEOS) || defined(SOLARIS) || defined(VSNORD)
call_stack::call_stack(::aura::application * papp, uint32_t uiSkip, void * address) :
   object(papp)
   ,m_caller_address(address)
#else
call_stack::call_stack(::aura::application * papp, uint32_t uiSkip) :
   object(papp)
#endif
{

   if(s_bDoStackTrace && uiSkip != SKIP_CALL_STACK)
   {

      if (uiSkip == CALL_STACK_DEFAULT_SKIP_TRIGGER)
      {

         uiSkip = CALL_STACK_DEFAULT_SKIP;

      }

      m_strCallStack = call_stack::get(uiSkip);

   }

}

call_stack::call_stack(const ::call_stack & cs) :
object(cs)
{

   m_strCallStack = cs.m_strCallStack;

}

string call_stack::get(uint32_t uiSkip)
{

   string str;

#if defined(LINUX)

   System.eengine().stack_trace(str, uiSkip, m_caller_address);

#else

   System.eengine().stack_trace(uiSkip);

   str = System.eengine()._strS;

#endif

   return str;

}

const char * call_stack::stack_trace() const
{

   return m_strCallStack;

}
