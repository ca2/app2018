#pragma once


namespace core
{


   class CLASS_DECL_ca2 session :
      virtual public base_session,
      virtual public ::application
   {
   public:


      session();
      virtual ~session();


      virtual bool is_remote_session();


   };


} // namespace core


