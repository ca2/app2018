//*****************************************************************************
//
//      Class:          thread_pool
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Provides the ability to queue simple member functions
//                      to the Windows thread pool.
//
//*****************************************************************************

#pragma once

template < typename T >
class simple_signal :
   virtual public ::radix::object
{
public:

   typedef void (T::*method)();
   
   
   T *      m_pobject;
   method   m_pmethod;


   simple_signal(T * pobject, method pmethod);

   void emit();

};

template < typename T >
simple_signal < T > ::simple_signal(T * pobject, method pmethod) :
   m_pobject(pobject), 
   m_pmethod(pmethod) 
{
}

template < typename T >
void simple_signal < T > ::emit()
{ 
   (m_pobject->*m_pmethod)();
}

class thread_pool
{
public:

   template <typename T>
   static void QueueUserWorkItem(T * pobject, void (T::*pmethod)(), ULONG flags = 
#ifdef VC6
   0
#else
   WT_EXECUTELONGFUNCTION
#endif
   )
   {
      simple_signal < T > * psignal = new simple_signal < T >(pobject, pmethod);

      if(!::QueueUserWorkItem(thread_proc < T >, psignal, flags))
      {
         delete psignal;
         _template::AtlThrowLastWin32();
      }                
   }

private:

   template <typename T>
   static DWORD WINAPI thread_proc(PVOID pcontext)
   {
      simple_signal < T > * psignal =  static_cast < simple_signal < T > * >(pcontext);

      psignal->emit();

      delete psignal;
      return 0;
   }

   thread_pool();
};
