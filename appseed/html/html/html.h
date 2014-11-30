#pragma once


#include "core/core/core.h"


#if defined(_CA2_HTML_DLL)
   #define CLASS_DECL_HTML  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_HTML  CLASS_DECL_IMPORT
#endif


#include "html/graphics/html.h"


#include "html/userhtml/userhtml.h"


#include "html/hi5/hi5_twitter_authorization.h"