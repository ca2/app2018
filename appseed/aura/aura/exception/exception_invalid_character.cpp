//#include "framework.h"


invalid_character::invalid_character(::aura::application * papp, const char * pszMessage) :
   object(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   simple_exception(papp, pszMessage)
{
      if(pszMessage == NULL)
      {
         printf(":invalid_character(NULL)");
      }
      else
      {
         printf(":invalid_character(\"%s\")",pszMessage);
      }
}


invalid_character::~invalid_character()
{

}

