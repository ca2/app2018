#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE save_as_view:
      virtual public ::user::edit_plain_text_view,
      virtual public ::filemanager::impact
   {
   public:

      
      bool m_bVoidSync;


      save_as_view(sp(::base::application) papp);


      virtual void _001OnAfterChangeText(::action::context actioncontext);


      void _017Synchronize(::action::context actioncontext);
      void on_update(sp(::user::impact) pSender,LPARAM lHint,object* phint);


   };


} // namespace filemanager





