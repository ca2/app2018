#pragma once


namespace process
{


   class CLASS_DECL_ACE bidi_pipe:
      virtual public ::object
   {
   public:


      sp(pipe)    m_sppipeIn;
      sp(pipe)    m_sppipeOut;


      bidi_pipe(::ace::application * papp);
      virtual ~bidi_pipe();

      virtual bool create(bool bBlock = true,bool bInherit = false);


   };


} // namespace process


































