#pragma once


namespace userbase
{


   class CLASS_DECL_ca2 edit_plain_text_view :
      virtual public view,
      public edit_plain_text
   {
   public:

      
      
      edit_plain_text_view(::ca::application * papp);

      virtual void install_message_handling(::ca::message::dispatch * pinterface);

      virtual void _001OnSetText();
      virtual void _001OnUpdate();

      virtual void layout();


   };


} // namespace userbase


