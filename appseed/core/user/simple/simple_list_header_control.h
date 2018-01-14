#pragma once


class CLASS_DECL_CORE simple_list_header_control :
   virtual public ::user::list_header
{
public:


   static const int32_t         s_iDividerWidth;



   simple_list_header_control(::aura::application * papp);
   virtual ~simple_list_header_control();



   using ::user::list_header::create_window;
   virtual bool create_window(UINT nStyle, const RECT & rect, sp(::user::interaction) pwndParent, UINT nID);

   virtual void install_message_routing(::message::sender * pinterface);



   DECL_GEN_SIGNAL(_001OnEndTrack);
   DECL_GEN_SIGNAL(_001OnTrack);
   DECL_GEN_SIGNAL(_001OnEndDrag);
   DECL_GEN_SIGNAL(_001OnCreate);



};

