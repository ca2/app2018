#pragma once


class CLASS_DECL_CORE simple_printer_list_view :
   virtual public simple_list_view
{
public:

      
   sp(::user::impact)                   m_pview;


   simple_printer_list_view(::aura::application * papp);
   virtual ~simple_printer_list_view();


   virtual void install_message_routing(::message::sender * psender);

   virtual bool _001OnClick(uint_ptr nFlags, point point);

   DECL_GEN_SIGNAL(_001OnCreate);


};

