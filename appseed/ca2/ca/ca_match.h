#pragma once


namespace ca
{
   

   namespace match
   {


      class CLASS_DECL_ca2 item :
         virtual public ::ca::ca
      {
      public:
         item() {}
         virtual ~item() {}
         virtual bool matches(const var & var) const = 0;
      };


      class CLASS_DECL_ca2 _and :
         virtual public item
      {
      public:


         item *   m_pitem1;
         item *   m_pitem2;


         virtual bool matches(const var & var) const;

      };


      class CLASS_DECL_ca2 string :
         virtual public item
      {
      public:


         ::string      m_str;


         string(const char * psz = ::null());


         virtual bool matches(const var & var) const;


      };


      class CLASS_DECL_ca2 ci_string :
         virtual public string
      {
      public:


         ci_string(const char * psz = ::null());


         virtual bool matches(const var & var) const;


      };


      class CLASS_DECL_ca2 prefix :
         virtual public string
      {
      public:


         prefix(const char * psz = ::null());


         virtual bool matches(const var & var) const;


      };


      class CLASS_DECL_ca2 ci_prefix :
         virtual public string
      {
      public:


         ci_prefix(const char * psz = ::null());


         virtual bool matches(const var & var) const;


      };


      class CLASS_DECL_ca2 suffix :
         virtual public string
      {
      public:


         suffix(const char * psz = ::null());


         virtual bool matches(const var & var) const;


      };


      class CLASS_DECL_ca2 ci_suffix :
         virtual public string
      {
      public:


         ci_suffix(const char * psz = ::null());


         virtual bool matches(const var & var) const;

      };


      class CLASS_DECL_ca2 any :
         virtual public spa(item),
         virtual public item
      {
      public:


         virtual bool matches(const var & var) const;


      };


   } // namespace match


} // namespace ca

