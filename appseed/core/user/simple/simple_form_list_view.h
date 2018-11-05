#pragma once


class CLASS_DECL_CORE simple_form_list_view :
   virtual public ::user::show < ::user::margin < ::user::form_list > >
{
public:


   typedef ::user::show < ::user::margin < ::user::form_list > > BASE;

   simple_form_list_view(::aura::application * papp);
   virtual ~simple_form_list_view();


   virtual bool pre_create_window(::user::create_struct& cs);


   virtual void install_message_routing(::message::sender * pinterface);


   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;

   
   virtual void _001OnInitialUpdate();

};


