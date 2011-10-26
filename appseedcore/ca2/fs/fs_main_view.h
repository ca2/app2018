#pragma once

namespace fs
{


   class document;

   class CLASS_DECL_ca main_view :
      public ::userbase::split_view
   {
   public:
         
      main_view(::ca::application * papp);
      void install_message_handling(::user::win::message::dispatch * pinterface);
      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)


      document * get_document();

   };


} // namespace fs


