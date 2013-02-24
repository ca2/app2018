#pragma once

class CLASS_DECL_ca2 file_manager_operation_info_view :
   virtual public ::userbase::view
   
{
public:

   double m_dAnime;

   file_manager_operation_info_view(::ca::application * papp);

   virtual void OnDraw(::ca::graphics * pdc);

   virtual void _001OnDraw(::ca::graphics * pdc);

   file_manager_operation_document * get_document();

   void OnFileOperationStep(int32_t iOperation, bool bFinal);

   DECL_GEN_SIGNAL(_001OnCreate)
   DECL_GEN_SIGNAL(_001OnTimer)
   virtual void install_message_handling(::ca::message::dispatch * pdispatch);

   void assert_valid() const;
   void dump(dump_context & context) const;
};
