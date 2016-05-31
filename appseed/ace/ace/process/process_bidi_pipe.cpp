#include "framework.h"


namespace process
{


   bidi_pipe::bidi_pipe(::ace::application * papp):
      object(papp),
      m_sppipeIn(allocer()),
      m_sppipeOut(allocer())
   {

   }


   bidi_pipe::~bidi_pipe()
   {

   }



   bool bidi_pipe::create(bool bBlock,bool bInherit)
   {

      if(!m_sppipeIn->create(bBlock,bInherit))
         return false;

      if(!m_sppipeIn->not_inherit_write())
         return false;

      if(!m_sppipeOut->create(bBlock,bInherit))
         return false;

      if(!m_sppipeOut->not_inherit_read())
         return false;

      return true;

   }


} // namespace core




































