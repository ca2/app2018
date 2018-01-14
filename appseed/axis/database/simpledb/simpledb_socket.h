﻿#pragma once


#include "aura/net/sockets/bsd/webserver/httpd_socket.h"

namespace simpledb
{


   class manager;
   class session;
   class socket_thread;

   class socket :
      public ::sockets::httpd_socket
   {
   public:


      string                        m_strLocale;
      ::simpledb::session *         m_psession;
      socket_thread *               m_pthreadParent;


      socket(::sockets::base_socket_handler & h);
      virtual ~socket();

      virtual void OnExecute();
      virtual void OnResponseComplete();

      virtual void OnSSLAccept();

      void send_response();

      class manager & manager();

      virtual void simple_file_server(const ::file::path & psz,const char * pszRelative = NULL);
      virtual bool read_file(const ::file::path & lpcsz,smart_pointer_array < int_array > * prangea = NULL,const char * pszContentType = NULL);

      //virtual bool http_filter_response_header(id key, string & strValue);
   };



} // namespace simpledb