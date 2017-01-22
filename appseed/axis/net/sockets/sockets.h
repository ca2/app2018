#pragma once




#if defined(BSD_STYLE_SOCKETS)
#include "bsd/sockets_config.h"



#elif defined(METROWIN)
#include "winrt/sockets_config.h"
#else
#error "Unknown Sockets Type?"
#endif



//#include "http_file.h"
//#include "http_memory_file.h"


#include "axis/net/http/webserver/webserver_http_form.h"
#include "axis/net/http/webserver/webserver_http_transaction.h"
#include "axis/net/http/webserver/webserver_http_request.h"
#include "axis/net/http/webserver/webserver_http_response.h"


#include "sockets_logger.h"
#include "sockets_trace_logger.h"

#if defined(BSD_STYLE_SOCKETS)
#include "bsd/basic/sockets_ssl_client_context.h"
#include "bsd/basic/sockets_ssl_client_context_map.h"
#endif

#include "base/sockets_base_socket.h"


#if defined(BSD_STYLE_SOCKETS)
#include "bsd/sockets_bsd.h"
#if defined(METROWIN) || defined(LINUX) 

extern "C"
CLASS_DECL_AXIS const SSL_METHOD * TLS_client_method();
extern "C"
CLASS_DECL_AXIS const SSL_METHOD * TLS_server_method();

#endif

#elif defined(METROWIN)
#include "winrt/sockets.h"
#else
#error "Unknown Sockets Type?"
#endif



//#include "axis/filesystem/file/file_timeout_buffer.h"
//#include "axis/filesystem/file/file_transfer_buffer.h"
//#include "sockets_http_buffer.h"
#include "sockets_http_batch_buffer.h"


#include "sockets_transfer_socket.h"




#include "axis/net/mail/mail.h"



#include "axis/net/net_ip_enum.h"
#include "axis/net/net_port_forward.h"



#include "sockets_websocket_client.h"