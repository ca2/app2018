#include "StdAfx.h"
#include "pane_view.h"
#include "application.h"
#include "html/html_view.h"
#include "html/html_document.h"

namespace verisimplevideo
{

   pane_view::pane_view(::ca::application * papp) :
      ca(papp),
      ::user::tab(papp),
      ::userbase::view(papp),
      ::userbase::tab_view(papp),
      ::userex::pane_tab_view(papp),
      place_holder_container(papp)
   {

      m_pviewdataOld = NULL;

      
      m_etranslucency      = TranslucencyPresent;

      m_pviewdata              = NULL;
      m_pviewdataOld              = NULL;

      /*   ::userbase::single_document_template* pdoctemplate;
      pdoctemplate = new ::userbase::single_document_template(
      IDR_ALBUM,
      ::ca::get_type_info < MediaLibraryDoc > (),
      ::ca::get_type_info < MediaLibraryChildFrame > (),
      ::ca::get_type_info < ::mplite::library::view > ());
      m_pdoctemplateAlbum = pdoctemplate;

      pdoctemplate = new ::userbase::single_document_template(
      IDR_ALBUM,
      ::ca::get_type_info < OptionsDoc > (),
      ::ca::get_type_info < OptionsChildFrame > (),
      ::ca::get_type_info < OptionsView > ());


      m_pdoctemplateOptions = pdoctemplate;

      pdoctemplate = new ::userbase::single_document_template(
      IDR_ALBUM,
      ::ca::get_type_info < GoodMixerDoc > (),
      ::ca::get_type_info < simple_child_frame > (),
      ::ca::get_type_info < MixerMainView > ());

      m_pdoctemplateAudioControl = pdoctemplate;*/

   }

   pane_view::~pane_view()
   {
   }




   #ifdef _DEBUG
   void pane_view::assert_valid() const
   {
      ::userbase::view::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
      ::userbase::view::dump(dumpcontext);
   }
   #endif //_DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // pane_view message handlers

   void pane_view::_001OnCreate(gen::signal_object * pobj) 
   {
//      SCAST_PTR(::user::win::message::create, pcreate, pobj)
      if(pobj->previous())
            return;

      add_tab("menu", PaneViewContextMenu);
      add_tab("video", PaneViewVideo);
      add_tab("file manager", PaneViewFileManager);

      set_cur_tab_by_id(PaneViewVideo);

   }


   void pane_view::on_update(::view* pSender, LPARAM lHint, ::radix::object* pHint) 
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);

      /*   ::ca::application * papp = dynamic_cast < ::ca::application * > (get_app());
      POSITION pos = papp->m_ptemplate_html->get_document_count();
      while(pos != NULL)
      {
      ::userbase::document * pdoc = papp->m_ptemplate_html->get_document(index);
      pdoc->update_all_views(pSender, lHint, pHint);
      }*/


      if(pHint != NULL)
      {
         if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeGetView))
            {
               puh->m_eview = (EPaneView) (int) get_view_id();
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeSetView))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
      if(pHint != NULL)
      {
         if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowKaraoke))
            {
               (dynamic_cast < userbase::frame_window * > (GetTopLevelFrame()))->SetActiveView(this);
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {
               int iTab;
               //            if(puh->m_eview == PaneViewContextMenu)
               //          {
               //           m_tab._001AddSel(0);
               //      }
               //            else
               {
                  switch(puh->m_eview)
                  {
                  case PaneViewContextMenu:
                     iTab = 0;
                     break;
                  case PaneViewVideo:
                     iTab = 1;
                     break;
                  case PaneViewFileManager:
                     iTab = 2;
                     break;
                  }
                  _001SetSel(iTab);
               }

            }
         }
      }

   }



   BOOL pane_view::PreCreateWindow(CREATESTRUCT& cs) 
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;   

      return ::userbase::view::PreCreateWindow(cs);
   }

   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
      application * papp = dynamic_cast < application * > ((dynamic_cast < userbase::frame_window * > (GetParentFrame()))->get_app());
      switch(pcreatordata->m_id)
      {
      case PaneViewContextMenu:
         /*{
         devedgeFrontDoc * pdoc = (devedgeFrontDoc *) (dynamic_cast < ::ca::application * > ((dynamic_cast < userbase::frame_window * > (GetParentFrame()))->m_papp))->m_ptemplateFront->open_document_file("http://localhost:10011/");
         if(pdoc != NULL)
         {
         POSITION pos = pdoc->get_view_count();
         ::view * pview = pdoc->get_view(pos);
         if(pview != NULL)
         {
         userbase::frame_window * pframe = dynamic_cast < userbase::frame_window * > (pview->GetParentFrame());
         if(pframe != NULL)
         {
         pcreatordata->m_pdoc = pdoc;
         pcreatordata->m_pwnd = pframe;


         }
         }
         }
         }*/
         break;

      case PaneViewVideo:
         {
            verisimplevideo::document * pdoc = dynamic_cast < verisimplevideo::document * > (papp->m_ptemplateVideo->open_document_file(NULL, true, pcreatordata->m_pholder));
            if(pdoc != NULL)
            {

               ::view * pview = pdoc->get_view();
               pview->on_update(NULL, 0, NULL);
               if(pview != NULL)
               {
                  userbase::frame_window * pframe = dynamic_cast < userbase::frame_window * > (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                     pcreatordata->m_pdoc = pdoc;
                     pcreatordata->m_eflag.signalize(::user::view_creator_data::flag_hide_all_others_on_show);
                     //pcreatordata->m_iExtendOnParent = -1;
                  }
               }
            }
         }
         break;
      case PaneViewFileManager:
         {
            ::filemanager::document * pdoc = papp->GetStdFileManagerTemplate()->OpenChild(papp, false, true, pcreatordata->m_pholder);
            if(pdoc != NULL)
            {
               ::view * pview = pdoc->get_view();
               if(pview != NULL)
               {
                  userbase::frame_window * pframe = dynamic_cast < userbase::frame_window * > (pview->GetParentFrame());
                  if(pframe != NULL)
                  {
                     pcreatordata->m_pdoc = pdoc;
                     pdoc->Initialize(true);
                  }
               }
            }
         }
         break;
      default:
         ASSERT(FALSE);
         break;
      }
   }

   void pane_view::rotate()
   {
      EPaneView eview = (EPaneView) (int) get_view_id();
      EPaneView eviewNew = PaneViewVideo;
      switch(eview)
      {
      case PaneViewVideo:
         eviewNew = PaneViewFileManager;
         break;
      case PaneViewFileManager:
         eviewNew = PaneViewPlaylist;
         break;
      case PaneViewPlaylist:
         eviewNew = PaneViewMediaLibrary;
         break;
      case PaneViewMediaLibrary:
         eviewNew = PaneViewAudioControl;
         break;
      case PaneViewAudioControl:
         eviewNew = PaneViewVideo;
         break;
      case PaneViewOptions:
         eviewNew = PaneViewVideo;
         break;
      default:
         ASSERT(FALSE);
         break;
      }

      set_cur_tab_by_id(eviewNew);
   }

   void pane_view::_001OnMenuMessage(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      set_cur_tab_by_id(m_pviewdataOld->m_id);
   }

   void pane_view::install_message_handling(::user::win::message::dispatch * pinterface)
   {
      ::userex::pane_tab_view::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_USER + 1122  , pinterface, this, &pane_view::_001OnMenuMessage);
   }

} // namespace verisimplevideo
