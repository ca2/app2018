#pragma once

namespace ca8
{

   class CLASS_DECL_ca network_configuration :
      public ::user::form_callback,
      public ::user::create_view
   {
   public:
      form_document *               m_pdoc;
      form_view *                   m_pview;

      network_configuration(::ca::application * papp);
      virtual ~network_configuration();

      bool initialize(::user::interaction * puiParent);

      void on_show();

      bool BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent);
   };


} // namespace ca8