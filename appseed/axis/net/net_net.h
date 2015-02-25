#pragma once

#if defined(LINUX)

#include <netinet/in.h>

#endif

CLASS_DECL_AXIS int_bool from_string(in6_addr & addr,const string & str);
template < >
CLASS_DECL_AXIS string & to_string(string & str, in6_addr & addr);
CLASS_DECL_AXIS int_bool from_string(in_addr & addr,const string & str);
template < >
CLASS_DECL_AXIS string & to_string(string & str, in_addr & addr);
#ifdef BSD_STYLE_SOCKETS
template < >
CLASS_DECL_AXIS string & to_string(string & str, sockaddr & addr);
#endif





CLASS_DECL_AXIS uint32_t c_inet_addr(const char *src);
CLASS_DECL_AXIS int32_t c_inet_pton(int32_t af,const char *src,void *dst);
CLASS_DECL_AXIS const char * c_inet_ntop(int32_t af,const void *src,char *dst,int32_t cnt);
CLASS_DECL_AXIS string c_inet_ntop(int32_t af,const void *src);



CLASS_DECL_AXIS string c_gethostbyname(const char * hostname);

CLASS_DECL_AXIS string get_file_extension_mime_type(const string & strExtension);


#include "net_byte_order.h"


