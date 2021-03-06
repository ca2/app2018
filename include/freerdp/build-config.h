#ifndef FREERDP_BUILD_CONFIG_H
#define FREERDP_BUILD_CONFIG_H

#define FREERDP_DATA_PATH ""
#define FREERDP_KEYMAP_PATH ""
#define FREERDP_PLUGIN_PATH ""

#define FREERDP_INSTALL_PREFIX ""

#define FREERDP_LIBRARY_PATH ""

#define FREERDP_ADDIN_PATH ""

#define FREERDP_SHARED_LIBRARY_SUFFIX ""
#ifdef _WIN32
#define FREERDP_SHARED_LIBRARY_PREFIX  ""
#else
#define FREERDP_SHARED_LIBRARY_PREFIX  "lib"
#endif

#define FREERDP_VENDOR_STRING "ca2"
#define FREERDP_PRODUCT_STRING "freerdp_library"

#endif /* FREERDP_BUILD_CONFIG_H */
