#pragma once


#include "primitive_command.h"


template < class TYPE, class BASE_ARRAY = array < TYPE > >
class stack :
   virtual public BASE_ARRAY
{
public:


   TYPE * operator ->()
   {
      return &BASE_ARRAY::last_element();
   }

   const TYPE * operator ->() const
   {
      return &BASE_ARRAY::last_element();
   }

};

template < class TYPE, class STACK = stack < TYPE > >
class stacker
{
public:

   STACK & m_stack;

   stacker(STACK & stack) :
      m_stack(stack)
   {
      m_stack.add_new();
   }

   ~stacker()
   {
      m_stack.pop();
   }

   TYPE * operator ->()
   {
      return &m_stack.last_element();
   }

   const TYPE * operator ->() const
   {
      return &m_stack.last_element();
   }

};



class command_line;

class CLASS_DECL_c command_line_sp :
   public smart_pointer < command_line >
{
public:


   command_line_sp();
   command_line_sp(allocatorsp allocer);


   using smart_pointer < command_line >::operator =;
   inline command_line_sp & operator = (command_line * p)
   {
      smart_pointer < command_line >::operator =(p);
      return *this;
   }

};




class CLASS_DECL_c create_context :
   virtual public command
{
public:


   bool                                m_bMakeVisible;
   bool                                m_bTransparentBackground;
   bool                                m_bClientOnly;
   bool                                m_bOuterPopupAlertLike;
   bool                                m_bHold;
   sp(::user::interaction)             m_puiParent;
   sp(::user::view)                    m_pviewAlloc;
   sp(application_bias)                m_spApplicationBias;
   command_line_sp                     m_spCommandLine;
   stack < ::user::create_context * >  m_user;
   sp(command_thread)                  m_pthreadParent;



   create_context(sp(::application) papp);
   create_context(sp(command_thread) pthreadParent);
   create_context(sp(command_thread) pthreadParent, var varFile, bool bMakeVisible = true, sp(::user::interaction) puiParent = NULL);
   create_context(const create_context & createcontext);
   virtual ~create_context();


   create_context & operator = (const create_context & createcontext);

};





