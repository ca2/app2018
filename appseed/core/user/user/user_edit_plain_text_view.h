#pragma once


namespace user
{


   class CLASS_DECL_CORE edit_plain_text_view :
      virtual public impact,
      public edit_plain_text
   {
   public:

      
      
      edit_plain_text_view(sp(base_application) papp);

      virtual void install_message_handling(::message::dispatch * pinterface);

      virtual void _001OnSetText(::action::context actioncontext);
      virtual void _001OnUpdate(::action::context actioncontext);

      virtual void layout();


   };


} // namespace user


