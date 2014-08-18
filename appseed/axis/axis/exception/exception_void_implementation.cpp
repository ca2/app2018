#include "framework.h"


void_implementation_exception::void_implementation_exception(sp(::axis::application) papp, const char * pszTip) : 
   element(papp),
   ::call_stack(papp),
   ::exception::axis(papp),
   not_implemented(papp, pszTip)
{
      if(pszTip == NULL)
      {
         printf(":void_implementation(NULL)");
      }
      else
      {
         printf(":void_implementation(\"%s\")",pszTip);
      }

}

void_implementation_exception::void_implementation_exception(const void_implementation_exception & e) : 
   element(e),
   ::call_stack(e),
   ::exception::axis(e),
   not_implemented(e)
{
         printf(":void_implementation(copy)");

}

void_implementation_exception::~void_implementation_exception()
{
}
