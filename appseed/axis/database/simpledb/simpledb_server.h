#pragma once


namespace simpledb
{

   class CLASS_DECL_CORE server :
      virtual public ::object
   {
   public:


      base *     m_pbase;


      server(::aura::application * papp);
      virtual ~server();

      
      virtual bool open(const char * pszDatabase);
      virtual bool close();


      virtual bool sql(const char * pszQuery, var & var);
      

   };


} // namespace simpledb


