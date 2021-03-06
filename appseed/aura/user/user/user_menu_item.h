#pragma once


namespace user
{


   class CLASS_DECL_AURA menu_item :
      virtual public ::object
   {
   public:


      id                         m_id;
      sp(menu_item_ptra)         m_spitema;
      int32_t                    m_iFullHeightItemCount;
      int32_t                    m_iSeparatorCount;
      ::user::interaction *      m_pui;
      bool                       m_bPopup;
      bool                       m_bBreak;
      int32_t                    m_iLevel;
      ::user::menu *             m_pmenu;
      string                     m_strTitle;
      rect                       m_rectUi;
      int                        m_iColumn;
      ::draw2d::dib_sp           m_dib;


      menu_item(::aura::application * papp);
      virtual ~menu_item();


      void add_item(menu_item * pitem);
      bool create_menu(const stringa & straCommand, const stringa & straCommandTitle);
      bool load_menu(::xml::node * pxmlnode);
      bool load_menu(::xml::node * pxmlnode, int32_t iLevel);

      virtual void OnAddRef();
      virtual void OnRelease();


      virtual bool create_buttons(menu * pmenu);


      bool IsPopup();
      menu_item * find(id id);


   };


   class CLASS_DECL_AURA menu_item_ptra :
      public spa(menu_item)
   {
   public:


      menu_item *    m_pitemParent;


      menu_item_ptra(menu_item * pitemParent);
      virtual ~menu_item_ptra();


      menu_item * find(id id);

   };



} // namespace user



