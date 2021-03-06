#pragma once


#include "core/core/core.h"


#if defined(_CA2_PROGRAMMING_DLL)
#define CLASS_DECL_PROGRAMMING  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_PROGRAMMING  CLASS_DECL_IMPORT
#endif


#define VS2017_CURRENT_BUILD "10.0.17134.0"

namespace programming
{


   class application;


} // namespace netnode

#undef App
#define App(papp) (*papp->cast < ::programming::application > ())


#include "app/appseed/programming/dynamic_source/dynamic_source.h"

#include "app/appseed/programming/introjection/introjection.h"



#include "programming_application.h"
