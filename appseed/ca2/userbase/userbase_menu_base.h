#pragma once

namespace userbase
{

   class menu_item;

   class CLASS_DECL_ca menu_base :
      virtual public ::radix::object
   {
   public:
      
      
      sp(menu_item)  m_pitem;


      menu_base(::ax::application * papp);
      virtual ~menu_base();


      bool create_menu(const stringa & straCommand, const stringa & straCommandTitle);
      bool LoadMenu(xml::node * lpnode);
      bool LoadXmlMenu(const char * pszMatter);
      

      virtual void clear();
      virtual void layout() = 0;
   };

} // namespace userbase