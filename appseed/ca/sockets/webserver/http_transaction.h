#pragma once


namespace http
{


   class CLASS_DECL_ca transaction :
      virtual public ::radix::object
   {
   public:
      transaction(::ca::application * papp);
      transaction(const transaction & src);
      virtual ~transaction();

      gen::property & header(const char * pszKey);

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

      void SetContentLength(long value);
      long ContentLength();

      void SetHost(const string & value);
      string Host();

      void SetPragma(const string & value);
      string Pragma();

      void SetReferer(const string & value);
      string Referer();

      void SetUserAgent(const string & value);
      string UserAgent();

      gen::property_set & headers();

      gen::property_set & attrs();

      gen::property & attr(const char * psz);
      http::cookies & cookies();

      virtual void clear();

      gen::property_set m_propertysetHeader;
      gen::property_set m_propertysetAttribute;
      http::cookies m_cookies;
      string m_null;

   }; // end of class


   inline gen::property_set & transaction::headers()
   {
      return m_propertysetHeader;
   }

   inline gen::property_set & transaction::attrs()
   {
      return m_propertysetAttribute;
   }

   inline http::cookies & transaction::cookies()
   {
      return m_cookies;
   }

   inline gen::property & transaction::attr(const char * psz)
   {
      return m_propertysetAttribute[psz];
   }

} // namespace http



