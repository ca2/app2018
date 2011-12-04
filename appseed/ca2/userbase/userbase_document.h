#pragma once

namespace userbase
{
   class document_template;
   class frame_window;
   class view;

   class CLASS_DECL_ca2 document : 
      virtual public ::document,
      virtual public ::database::client
   {
   public:

      document(::ca::application * papp);
      virtual ~document();

      virtual void on_alloc(::ca::application * papp);

   #ifdef _DEBUG
      virtual void dump(dump_context&) const;
      virtual void assert_valid() const;
   #endif //_DEBUG

      virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);
         
      friend class ::userbase::document_template;

   };

} // namepsace userbase
