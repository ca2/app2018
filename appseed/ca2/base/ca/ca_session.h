#pragma once


namespace ca2
{


   class CLASS_DECL_ca2 session :
      virtual public ::ca2::application
   {
   public:


      session();
      virtual ~session();


      virtual bool is_remote_session();


   };


} // namespace ca2


