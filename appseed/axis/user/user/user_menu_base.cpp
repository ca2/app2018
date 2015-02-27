//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"



namespace aura
{


   menu_base::menu_base(::aura::application * papp):
      ::object(papp)
   {

      m_pitem = Application.alloc(System.type_info < menu_base_item > ());

      m_pmenubaseThis = NULL;

   }


   menu_base::~menu_base()
   {

   }


   void menu_base::install_message_handling(::message::dispatch * pdispatch)
   {

      ::user::interaction::install_message_handling(pdispatch);

      IGUI_WIN_MSG_LINK(WM_SHOWWINDOW, pdispatch, this, &menu_base::_001OnShowWindow);
      IGUI_WIN_MSG_LINK(WM_DESTROY, pdispatch, this, &menu_base::_001OnDestroy);


   }


   void menu_base::clear()
   {

      m_pitem = Application.alloc(System.type_info < menu_base_item >());

   }

   bool menu_base::create_menu(const stringa & straCommand, const stringa & straCommandTitle)
   {

      return m_pitem->create_menu(straCommand, straCommandTitle);

   }

   bool menu_base::LoadMenu(::xml::node node)
   {

      return m_pitem->load_menu(node);

   }

   bool menu_base::LoadXmlMenu(const char * pszMatter)
   {


      xml::document doc;

      doc.load(Application.file_as_string(Application.dir_matter(pszMatter)));

      return LoadMenu(doc.root());


   }


   bool menu_base::TrackPopupMenu(int32_t iFlags, int32_t x, int32_t y, sp(::user::interaction) oswindowParent, sp(menu_base) * pthis)
   {

      m_pmenubaseThis = pthis;

      return true;

   }

   void menu_base::_001OnShowWindow(signal_details * pobj)
   {

       UNREFERENCED_PARAMETER(pobj);
//      SCAST_PTR(::message::show_window, pshow, pobj);

      //if (!pshow->m_bShow)
      //{

      //   if (m_pmenubaseThis != NULL)
      //   {

      //      sp(menu_base) * pthis = m_pmenubaseThis;

      //      m_pmenubaseThis = NULL;

      //      pthis->release();

      //   }

      //}


   }

   void menu_base::_001OnDestroy(signal_details * pobj)
   {
       
       UNREFERENCED_PARAMETER(pobj);
       

//      SCAST_PTR(::message::show_window, pshow, pobj);

      //if (!pshow->m_bShow)
      //{

      if (m_pmenubaseThis != NULL)
         {

            sp(menu_base) * pthis = m_pmenubaseThis;

            m_pmenubaseThis = NULL;

            pthis->release();

         }

      //}


   }


} // namespace user



