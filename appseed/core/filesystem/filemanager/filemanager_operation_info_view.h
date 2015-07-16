#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE operation_info_view:
      virtual public ::user::impact

   {
   public:

      double m_dAnime;

      operation_info_view(::aura::application * papp);

      void assert_valid() const;
      void dump(dump_context & context) const;

      virtual void _001OnDraw(::draw2d::graphics * pdc);

      sp(operation_document) get_document();

      void OnFileOperationStep(int32_t iOperation,bool bFinal);

      DECL_GEN_SIGNAL(_001OnCreate);
      void _001OnTimer(::timer * ptimer);
      virtual void install_message_handling(::message::dispatch * pdispatch);

   };


} // namespace filemanager













