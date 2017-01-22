#pragma once




#define SSL_SESSION_TICKET_KEY_NAME_LEN 16
#define SSL_SESSION_TICKET_AES_KEY_LEN  16
#define SSL_SESSION_TICKET_HMAC_KEY_LEN 16
#define SSL_SESSION_TICKET_KEY_SIZE     48


struct ssl_ticket_key
{
   unsigned char   key_name[SSL_SESSION_TICKET_KEY_NAME_LEN];
   unsigned char   hmac_key[SSL_SESSION_TICKET_HMAC_KEY_LEN];
   unsigned char   aes_key[SSL_SESSION_TICKET_AES_KEY_LEN];
};


//#include "basic/sockets_ssl_client_context.h"
//#include "basic/sockets_ssl_client_context_map.h"
//#include "basic/sockets_ssl_server_context.h"
//#include "basic/sockets_ssl_server_context_map.h"
#include "basic/sockets_socket.h"
#include "basic/sockets_stream_socket.h"
#include "basic/sockets_sctp_socket.h"
#include "basic/sockets_tcp_socket.h"
#include "basic/sockets_udp_socket.h"

#include "basic/sockets_tls_socket.h"


#include "axis/net/sockets/base/sockets_base_socket_handler.h"
#include "basic/sockets_socket_handler.h"
#include "basic/sockets_listen_socket.h"






#include "axis/net/sockets/http/sockets_http.h"






#include "webserver/AjpBaseSocket.h"
#include "webserver/Ajp13Socket.h"
#include "webserver/ajp13.h"
#include "webserver/http_base_socket.h"
#include "webserver/httpd_socket.h"

#include "sip/sip_base.h"

#include "sip/sip_transaction.h"
#include "sip/sip_request.h"
#include "sip/sip_response.h"

#include "sip/sip_base_client_socket.h"
#include "sip/sip_tcp_client_socket.h"
#include "sip/sip_udp_client_socket.h"
#include "sip/sip_client.h"
#include "sip/sip_server.h"


#include "smtp/smtp_socket.h"
#include "smtp/smtpd_socket.h"

#include "timer/EventTime.h"
#include "timer/Event.h"

#include "timer/IEventHandler.h"
#include "timer/IEventOwner.h"
#include "timer/EventHandler.h"

#include "asynch_dns/resolv_socket.h"
#include "asynch_dns/resolv_server.h"


#include "basic/sockets_sync_socket_handler.h"


#include "axis/net/sockets/sockets_link_out_socket.h"
#include "axis/net/sockets/sockets_link_in_socket.h"

#include "basic/sockets_net.h"


#include "sockets_sockets.h"




