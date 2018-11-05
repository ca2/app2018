#pragma once


namespace user
{

   class CLASS_DECL_AURA place_holder_container :
      virtual public interaction
   {
      public:


         spa(place_holder) m_holdera;

         place_holder_container();
         place_holder_container(::aura::application * papp);
         virtual ~place_holder_container();


         void install_message_routing(::message::sender * psender);


         virtual bool create_window(const RECT & rect, ::user::interaction * puiParent,id id);


         virtual ::user::place_holder * get_new_place_holder(const RECT & lpcrectCreate);
         virtual bool remove_place_holder(::user::place_holder * pholder);


         virtual ::user::place_holder * place(::user::interaction * pui,const RECT & lpcrectCreate);
         virtual bool unplace(::user::interaction * pui);

         virtual bool on_hold(::user::interaction * pui, ::user::place_holder * pholder);
         virtual bool on_unhold(::user::interaction * pui, ::user::place_holder * pholder);

   };


   class CLASS_DECL_AURA place_holder_container_ptra :
      virtual public spa(place_holder_container)
   {
      public:

         virtual place_holder_ptra place(::user::interaction * pui, const RECT & lpcrectCreate);
         virtual int32_t unplace(::user::interaction * pui);

   };

} // namespace user
