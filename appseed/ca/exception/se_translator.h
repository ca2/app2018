/*
 Copyright (c) 2001
 Author: Konstantin Boukreev
 E-mail: konstantin@mail.primorye.ru
 Created: 25.12.2001 14:47:20
 Version: 1.0.0

 Permission to use, copy, modify, distribute and sell this software
 and its documentation for any purpose is hereby granted without fee,
 provided that the above copyright notice appear in all copies and
 that both that copyright notice and this permission notice appear
 in supporting documentation.  Konstantin Boukreev makes no representations
 about the suitability of this software for any purpose.
 It is provided "as is" without express or implied warranty.

 wrapper class for _set_se_translator API
 maps Win32 exceptions (C structured exceptions) into C++ typed exceptions.
*/

#pragma once

#ifdef _WINDOWS
#include <eh.h>
#else
#include <signal.h>
#endif


class CLASS_DECL_ca se_translator :
   virtual public ::radix::object
{
 public:
   se_translator();
   virtual ~se_translator();

#ifdef _WINDOWS
   void filter(unsigned int uiCode, _EXCEPTION_POINTERS * p);
   static void __cdecl filter2(unsigned int uiCode, _EXCEPTION_POINTERS * p);
   //void filter( _EXCEPTION_POINTERS * p);
   static vsstring name(unsigned int uiCode);
   static vsstring description(unsigned int uiCode);
#else
   static void filter_sigsegv(int signal, siginfo_t * psiginfo, void * pc);
   static void filter_sigfpe(int signal, siginfo_t * psiginfo, void * pc);
#endif


   bool attach();
   bool detach();



 private:
    bool                      m_bSet;
    //LPTOP_LEVEL_EXCEPTION_FILTER m_pfn;
#ifdef _WINDOWS
    _se_translator_function   m_pfn;
#endif
};

/*
class CLASS_DECL_ca the_se_translator : public se_translator
{
public:
   the_se_translator();
   virtual ~the_se_translator();
   static the_se_translator s_thesetranslator;
};*/
