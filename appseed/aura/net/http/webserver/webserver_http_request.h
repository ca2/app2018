#pragma once

#include "webserver_http_form.h"
#include "webserver_http_transaction.h"

namespace http
{

   class form;

   class CLASS_DECL_AURA request : public transaction
   {
   public:



      string                           m_strRequestUri;
      string                           m_strQueryString;
      string                           m_strHttpHost;
      id                               m_idHttpMethod;
      memory_file              m_memfileBody;
      ::http::stream            m_ostream;
      string m_null;
      http::form m_form;




      request(::aura::application * papp);
      request(const request& src);
      ~request();


      /** Cookies */
      void add_cookie(const char * pszSetCookie);

      /** open file for body data */
      void InitBody( size_t sz );

      /** write body data */
      void write( const char *buf, memory_size_t sz );

      /** No more writing */
      void CloseBody();

      void ParseBody();

      http::form & form();

      ::http::stream & ostream()   { return m_ostream; }
      ::file::file & file()      { return m_memfileBody; }

      virtual void clear();



   }; // end of class



   // --------------------------------------------------------------------------------------
   inline http::form & request::form()
   {
      return m_form;
   }


} // namespace http




