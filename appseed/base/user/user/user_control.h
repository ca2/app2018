#pragma once


#include "base/user/user/user_control_event.h"
#include "base/user/user/user_box.h"


namespace user
{


   class CLASS_DECL_BASE control :
      virtual public ::user::box
   {
   public:


      enum EMessageParam
      {
         MessageParamGetBaseControlExPtr = 1
      };


      enum efunction
      {
         // Text got from control m_uiId member
         function_static,
         // Text got from control m_uiText member
         function_static2,
         function_data_selection,
         function_check_box,
         function_duplicate_on_check_box,
         function_disable_on_check_box,
         function_save_on_change,
         function_vms_data_edit,
         function_action,
         function_edit_multi_line,
      };
      enum edatatype
      {
         DataTypeString,
         DataTypeNatural,
      };
      enum eddx
      {
         ddx_none,
         ddx_flags,
         ddx_dbflags,
      };

   // sort_array < INT_PR, int_ptr >
      class CLASS_DECL_BASE ddx_dbflags
      {
      public:


         ddx_dbflags(::database::key key, int_ptr value);


         ::database::key                  m_key;
         int_ptr                          m_value;


      };

      class CLASS_DECL_BASE descriptor :
         virtual public object
      {
      public:

         union
         {
            void *                     m_pvoid;
            class ddx_dbflags *        m_pdbflags;
         } m_ddx;


         ::user::interaction *         m_pui;
         form_window *                 m_pform;
         index_map < sp(control) >     m_controlmap;
         id                            m_id;
         id                            m_uiText;
         sp(type)                      m_typeinfo;
         index                         m_iSubItem;
         id                            m_idPrivateDataSection;
         bool                          m_bTransparent;
         e_control_type                m_etype;
         bool                          m_bCreated;
         bool                          m_bSubclassed;
         eddx                          m_eddx;
         ::database::id                m_dataid;
         flags < efunction >           m_flagsfunction;
         edatatype                     m_edatatype;
         rect                          m_rect;
         property_set                  m_setValue;
         int                           m_iSubItemDisableCheckBox;
         int                           m_iSubItemDuplicateCheckBox;
         int_array                     m_iaSubItemDuplicate;
         int_array                     m_iaSubItemDisable;

         descriptor();
         descriptor(const descriptor & descriptor);
         virtual ~descriptor();


         void clear();
         bool operator == (const descriptor & descriptor) const;
         descriptor & operator = (const descriptor & descriptor);
         e_control_type get_type();
         void set_type(e_control_type e_type);
         void add_function(efunction efunction);
         void remove_function(efunction efunction);
         bool has_function(efunction efunction);
         edatatype get_data_type();
         void set_data_type(edatatype edatatype);
         void set_ddx_dbflags(::database::id id, int_ptr value);
         control * get_control(::user::form_window * pform, index iItem = 0);

         index find_control(::user::interaction * pui);

      };

      class CLASS_DECL_BASE descriptor_set :
         public smart_pointer_array < descriptor >
      {
      public:


         descriptor_set();
         virtual ~descriptor_set();


         sp(control) get_control(::user::form_window * pform,id id, int iItem = 0);
         descriptor * get_by_sub_item(int32_t iSubItem);

         bool find_control(::user::interaction * pui,index & iItem,index & iSubItem);


      };

//#ifdef WINDOWSEX
//      ___static const uint32_t      g_uiMessage;
//#endif
      index                      m_iHover;
      e_element                  m_eelementHover;
      descriptor *               m_pdescriptor;
      sp(::user::interaction)    m_pwndCustomWindowProc;
      bool                       m_bCustomWindowProc;
      index                      m_iEditItem;
      form_window *              m_pform;
      form_callback *            m_pformcallback;
      bool                       m_bControlExCommandEnabled;


      control();
      virtual ~control();


      virtual void install_message_handling(::message::dispatch * pdispatch);

      virtual bool create_control(class control::descriptor * pdescriptor, index iItem);


      bool _003IsCustomMessage();
      ::user::interaction_base * _003GetCustomMessageWnd();


      virtual void _001OnDraw(::draw2d::graphics * pgraphics);



      virtual index get_hover();

      class descriptor & descriptor();

      virtual void _003CallCustomDraw(::draw2d::graphics * pgraphics, ::aura::draw_context * pitem);
      virtual bool _003CallCustomWindowProc(sp(::user::interaction) pwnd, UINT message, WPARAM wparam, LPARAM lparam, LRESULT & lresult);
      virtual void _003OnCustomDraw(::draw2d::graphics * pgraphics, ::aura::draw_context * pitem);
      virtual void _003CustomWindowProc(signal_details * pobj);

      virtual form_window * get_form();

      //virtual form_list * get_form_list();
      virtual bool _001IsPointInside(point64 pt);

      control null() { return control(); }
      bool Validate(string & str);
      bool get_data(sp(::user::interaction) pwnd, var & var);
      void SetEditItem(index iItem);
      void SetEditSubItem(index iItem);
      index GetEditSubItem();
      index GetEditItem();

      virtual ::user::interaction * ControlExGetWnd();

      using ::user::interaction::GetClientRect;
      using ::user::interaction::GetWindowRect;
      virtual bool GetClientRect(LPRECT lprect);
      virtual bool GetWindowRect(LPRECT lprect);


      bool operator == (const class ::user::control::descriptor & descriptor) const;
      bool operator == (const class control & control) const;


      virtual bool IsControlCommandEnabled();
      virtual void EnableControlCommand(bool bEnable);


      virtual void BaseControlExOnMouseMove(UINT nFlags, point point);

      virtual index hit_test(point point, e_element & eelement);

      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnKeyDown);


      virtual bool BaseOnControlEvent(::user::control_event * pevent);

      virtual bool simple_process_system_message(signal_details * pobj, ::user::e_event eevent);

      //virtual void walk_pre_translate_tree(signal_details * pobj,sp(::user::interaction) puiStop);

      virtual bool keyboard_focus_OnSetFocus();
      virtual bool keyboard_focus_OnKillFocus();

   };


   class control_cmd_ui : public ::command_ui
   {
   public:
      
      
      id_to_id      m_mapControlCommand;


      control_cmd_ui(class ::signal * psignal);


      virtual void Enable(bool bOn);
      virtual void SetCheck(int32_t nCheck);
      virtual void SetText(const char * lpszText);
      id GetControlCommand(id id);

   };

   class control_view_impl :
      virtual public ::object
   {
   public:

      control_view_impl();

      virtual LRESULT BaseControlExOnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
      virtual void BaseControlExOnUpdateCmdUI(sp(::user::frame_window) pTarget, bool bDisableIfNoHndler);
      virtual bool BaseControlExOnCommand(WPARAM wParam, LPARAM lParam) ;
      virtual sp(::user::interaction) get_window();

      control_cmd_ui  m_commandui; // command_ui derived class used to update
                                    // base_controlex derived controls
      list<oswindow, oswindow> m_listwndptrBlackList;
   };



} // namespace user
