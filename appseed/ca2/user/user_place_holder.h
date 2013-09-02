#pragma once


namespace user
{

   class place_holder_container;

   class CLASS_DECL_ca2 place_holder :
      virtual public control
   {
   public:


      interaction_ptr_array  m_uiptraHold;


      place_holder(sp(::application) papp);
      virtual ~place_holder();


      using ::user::control::create;
      virtual bool create(sp(::user::place_holder_container) pcontainer, id id);

      virtual void install_message_handling(::ca2::message::dispatch * pdispatch);

      virtual bool can_merge(sp(::user::interaction) pui);
      virtual bool merge(sp(::user::interaction) pui);
      virtual bool hold(sp(::user::interaction) pui);
      virtual bool unhold(sp(::user::interaction) pui);
      virtual bool is_holding(sp(::user::interaction) pui);


      virtual void layout();

      sp(::user::interaction) get_ui();


   };

   class CLASS_DECL_ca2 place_holder_ptra :
      virtual public spa(place_holder)
   {
   public:

      int32_t hold(sp(::user::interaction) pui);
      int32_t unhold(sp(::user::interaction) pui);

   };

} // namespace user
