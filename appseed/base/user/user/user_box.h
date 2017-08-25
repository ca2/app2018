#pragma once


#include "user_interaction_impl_base.h"
#include "user_scroll.h"
#include "base/user/simple_ui/simple_ui_interaction.h"


namespace user
{


   class CLASS_DECL_BASE box :
      virtual public scroll,
      virtual public ::simple_ui::interaction,
      virtual public ::database::client
   {
   public:


      string         m_strDisplay;
      string         m_strWindowRectDataAddUp;

      box();
      virtual ~box();

      void install_message_routing(::message::sender * pinterface);

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnSize);
      DECL_GEN_SIGNAL(_001OnShowWindow);

      virtual void _001WindowRestore();

      virtual string calc_display();
      virtual bool does_display_match();
      virtual void defer_update_display();

      virtual bool IsFullScreen();
      void WindowDataEnableSaveWindowRect(bool bEnable);
      bool WindowDataSaveWindowRect();
      bool WindowDataLoadWindowRect(bool bForceRestore = false, bool bInitialFramePosition = false);

      virtual bool LoadWindowRect_(class ::database::id id, sp(::user::box) pwindow, bool bForceRestore = false, bool bInitialFramePosition = false);
      virtual bool SaveWindowRect_(class ::database::id id, sp(::user::box) pwindow);

      
      using ::simple_ui::interaction::on_simple_command;
      virtual bool on_simple_command(e_simple_command ecommand, lparam lparam, LRESULT & lresult);


      virtual void on_set_parent(::user::interaction * puiParent) override;
      virtual bool on_before_set_parent(::user::interaction * pinterface) override;


      virtual string calc_data_id();


   };


} // namespace user





