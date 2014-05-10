#pragma once


namespace simpledb
{

   class server;

   class CLASS_DECL_CORE session :
      virtual public ::object
   {
   public:


      server *       m_pserver;

      session(sp(::base::application) papp);
      virtual ~session();

      
      virtual bool open(const char * pszDatabase);
      virtual bool close();


      virtual bool query(const char * pszQuery, var & var);
      

   };


} // namespace simpledb


