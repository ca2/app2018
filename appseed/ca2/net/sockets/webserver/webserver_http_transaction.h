#pragma once


namespace http
{


   class CLASS_DECL_ca2 transaction :
      virtual public ::ca2::object
   {
   public:


      ::ca2::property_set m_propertysetHeader;
      ::ca2::property_set m_propertysetAttribute;
      http::cookies m_cookies;
      string m_null;


      transaction(sp(::ca2::application) papp);
      transaction(const transaction & src);
      virtual ~transaction();

      

      void SetAccept(const string & value);
      string Accept();

      void SetAcceptCharset(const string & value);
      string AcceptCharset();

      void SetAcceptEncoding(const string & value);
      string AcceptEncoding();

      void SetAcceptLanguage(const string & value);
      string AcceptLanguage();

      void SetConnection(const string & value);
      string Connection();

      void SetContentType(const string & value);
      string ContentType();

      void SetContentLength(primitive::memory_size value);
      primitive::memory_size ContentLength();

      void SetHost(const string & value);
      string Host();

      void SetPragma(const string & value);
      string Pragma();

      void SetReferer(const string & value);
      string Referer();

      void SetUserAgent(const string & value);
      string UserAgent();

      ::ca2::property_set & headers();
      ::ca2::property & header(id idKey);
      
      ::ca2::property_set & attrs();
      ::ca2::property & attr(id idKey);


      http::cookies & cookies();

      virtual void clear();


   }; // end of class

   inline ::ca2::property_set & transaction::attrs()
   {
      return m_propertysetAttribute;
   }

   inline ::ca2::property & transaction::attr(id idKey)
   {
      return m_propertysetAttribute[idKey];
   }

   inline ::ca2::property_set & transaction::headers()
   {
      return m_propertysetHeader;
   }

   inline ::ca2::property & transaction::header(id idKey)
   {
      return m_propertysetHeader[idKey];
   }

   inline http::cookies & transaction::cookies()
   {
      return m_cookies;
   }


} // namespace http



