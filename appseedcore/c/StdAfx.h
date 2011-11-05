#pragma once

#include "c.h"


#if defined(MACOS)
#include "macos/c_os_internal.h"
#elif defined(LINUX)
#include "linux/c_os_internal.h"
#elif defined(WINDOWS)
#include "windows/c_os_internal.h"
#endif
