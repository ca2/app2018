#pragma once

#include "pane_view_update_hint.h"

namespace verisimplevideo
{

   class CLASS_DECL_CA2_TESSERACT pane_view : 
      virtual public ::userex::pane_tab_view
   {
   public:
      pane_view(::ca::application * papp);
      
      void rotate();

      void on_create_view(::user::view_creator_data * pcreatordata);
      virtual void install_message_handling(::user::win::message::dispatch * pinterface);

      virtual void on_update(::view * pSender, LPARAM lHint, ::radix::object* pHint);
      virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

      DECL_GEN_SIGNAL(_001OnMenuMessage)
      virtual ~pane_view();
   #ifdef _DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif
      DECL_GEN_SIGNAL(_001OnCreate)
   };

} // namespace verisimplevideo
