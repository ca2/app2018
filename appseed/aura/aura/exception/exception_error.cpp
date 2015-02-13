//#include "framework.h"


error_exception::error_exception(::aura::application * papp, const char * pszError) :
   object(papp),
   ::call_stack(papp),
   ::exception::base(papp)
{
      if(pszError == NULL)
      {
         printf(":error(NULL)");
      }
      else
      {
         printf(":error(\"%s\")",pszError);
      }
      
   m_strError = pszError;
}

error_exception::~error_exception()
{
}

bool error_exception::get_error_message(string & str, PUINT pnHelpContext)
{

   UNREFERENCED_PARAMETER(pnHelpContext);

   str = m_strError;

   return TRUE;
}

