#pragma once


namespace userex
{


   class CLASS_DECL_ca2 second_list_view :
      virtual public ::simple_list_view
   {
   public:


      second_list_view(::ca::applicationsp papp);   
      virtual ~second_list_view();


      virtual void install_message_handling(::ca::message::dispatch * pinterface);

      virtual void _001GetItemText(::user::list_item * pitem);
      virtual count _001GetItemCount();
      virtual void _001InsertColumns();

      virtual void _001OnSelectionChange();

      int32_t get_second();
      int32_t set_second(int32_t iSecond);

      DECL_GEN_SIGNAL(_001OnCreate)


      
   
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif

   };


} // namespace userex

