#pragma once


namespace user
{

   
   class combo_box;
   namespace wndfrm
   {
      namespace frame
      {
         class WorkSet;
      }
   }


   class CLASS_DECL_CORE combo_list : 
      virtual public ::user::control
   {
   public:


      bool                                m_bComboList;
      combo_box *                         m_pcombo;
      int32_t                             m_iItemHeight;

      ::user::interaction *               m_puiDeactivateTogether;
      ::user::wndfrm::frame::WorkSet *    m_puiDeactivateTogetherSet;


      combo_list();
      virtual ~combo_list();


      virtual void install_message_handling(::message::dispatch * pdispatch);


      virtual void _001OnDraw(::draw2d::graphics * pdc);
      virtual void _001OnDrawVerisimple(::draw2d::graphics * pdc);
      virtual void _001OnDrawSimply(::draw2d::graphics * pdc);


      virtual void query_full_size(LPSIZE lpsize) const;

      virtual int32_t _001GetItemHeight() const;

      bool pre_create_window(::user::create_struct & cs);


      DECL_GEN_SIGNAL(_001OnKillFocus);
      DECL_GEN_SIGNAL(_001OnClose);
      DECL_GEN_SIGNAL(_001OnActivate);
      DECL_GEN_SIGNAL(_001OnMouseActivate);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMButtonDown);
      DECL_GEN_SIGNAL(_001OnRButtonDown);
      DECL_GEN_SIGNAL(_001OnMouseMove);


      virtual index hit_test(point pt, e_element & eelement);
      simple_combo_list(::aura::application * papp);
      virtual ~simple_combo_list();


      virtual void install_message_handling(::message::dispatch * pdispatch);


      virtual void _001OnDraw(::draw2d::graphics * pdc);




   };


} //  namespace user



