#pragma once


namespace user
{


   class combo_list;


   class CLASS_DECL_ca2 combo_box : 
      virtual public ::user::control
   {
   public:


      index                m_iSel;
      string               m_strText;
      bool                 m_bCaseSensitiveMatch;

      combo_list *         m_plist;

      ::ca::type_info      m_typeComboList;


      combo_box();
      virtual ~combo_box();


      virtual void install_message_handling(::gen::message::dispatch * pdispatch);


      virtual void _001OnDraw(::ca::graphics * pdc);


      virtual void _001GetText(string & str) const;
      virtual void _001SetText(const char * psz);


      virtual void _001GetListText(index iSel, string & str) const;
      virtual index _001FindListText(const string & str) const;
      virtual count _001GetListCount() const;


      virtual bool get_element_rect(LPRECT lprect, e_element eelement) const;
      virtual e_element hit_test(point pt) const;

      virtual void get_simple_drop_down_open_arrow_path(point_array & pointa) const;


      DECL_GEN_SIGNAL(_001OnLButtonDown)
      DECL_GEN_SIGNAL(_001OnLButtonUp)

      virtual void _001ShowDropDown(bool bShow = true);


      combo_list * create_combo_list();


   };


} //  namespace user



