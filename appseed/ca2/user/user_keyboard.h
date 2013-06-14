#pragma once


namespace user
{

   class keyboard_layout;

   class CLASS_DECL_ca2 keyboard :
      virtual public ::ca2::object
   {
   public:

      int_map < ::user::e_key >         m_mapKey;
      sp(keyboard_layout)                             m_playout;

      keyboard(sp(::ca2::application) papp);

      virtual class keyboard_layout & layout();

      virtual bool initialize();

      bool load_layout(const char * pszPath, bool bUser);

      string process_key(::user::e_key ekey);

      string process_char(const char * pszChar);

      string process_escape(const char * pszChar);

      void process_escape(sp(::xml::node) pnode, ::ca2::property_set & set);

      string get_current_system_layout();

      bool initialize(::user::keyboard_layout_id * playoutid, const char * pszPath);

      void translate_os_key_message(::ca2::message::key * pkey);

      virtual bool load_os_layout(const char * pszPath);

   };

} // namespace user