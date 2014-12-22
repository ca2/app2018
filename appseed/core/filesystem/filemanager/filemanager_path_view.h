#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE path_view :
      virtual public ::user::edit_plain_text_view,
      virtual public  impact
   {
   public:

      bool m_bVoidSync;



       path_view(::aura::application * papp);


      virtual void _001OnAfterChangeText(::action::context actioncontext);


      void on_update(::aura::impact * pSender, LPARAM lHint, object* phint);

      virtual void _001OnDraw(::draw2d::graphics * pdc);

      virtual void browse_sync(::action::context actioncontext);

   };


} // namespace filemanager




