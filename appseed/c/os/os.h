#pragma once


#include "os_thread.h"
#include "os_process.h"


#if defined(MACOS)

#include "c/ansios/ansios.h"
#include "os_binreloc.h"
#include "c/macos/c_os.h"

#elif defined(LINUX)

#include "c/ansios/ansios.h"
#include "os_binreloc.h"
#include "c/linux/c_os.h"

#elif defined(MERDE_WINDOWS)

#include "c/merdes_windows/windows_merde.h"

#else

#include "c/windows/c_os.h"

#endif
