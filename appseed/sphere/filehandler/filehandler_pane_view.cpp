#include "framework.h"




namespace filehandler
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
      System.type_info < MediaLibraryDoc > (),
      System.type_info < MediaLibraryChildFrame > (),
      System.type_info < ::mplite::library::view > ());
      m_pdoctemplateAlbum = pdoctemplate;

      pdoctemplate = new ::userbase::single_document_template(
      IDR_ALBUM,
      System.type_info < OptionsDoc > (),
      System.type_info < OptionsChildFrame > (),
      System.type_info < OptionsView > ());


      m_pdoctemplateOptions = pdoctemplate;

      pdoctemplate = new ::userbase::single_document_template(
      IDR_ALBUM,
      System.type_info < GoodMixerDoc > (),
      System.type_info < simple_child_frame > (),
      System.type_info < MixerMainView > ());

      m_pdoctemplateAudioControl = pdoctemplate;*/

   }

   pane_view::~pane_view()
   {
   }




#ifdef DEBUG
   void pane_view::assert_valid() const
   {
      ::userbase::view::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
      ::userbase::view::dump(dumpcontext);
   }
#endif //DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // pane_view message handlers

   void pane_view::_001OnCreate(gen::signal_object * pobj)
   {
      if(pobj->previous())
         return;


      application * papp = dynamic_cast < application * > (get_app());

      papp->m_ppaneview = this;

      add_tab("New", pane_view_new);
   }


   void pane_view::on_update(::view* pSender, LPARAM lHint, ::gen::object* pHint)
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
               puh->m_eview = (EPaneView) (int32_t) get_view_id();
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
            }
         }
      }

   }



   bool pane_view::pre_create_window(CREATESTRUCT& cs)
   {
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

      return ::userbase::view::pre_create_window(cs);
   }

   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {

      //         application * papp = dynamic_cast < application * > ((dynamic_cast < userbase::frame_window * > (GetParentFrame()))->get_app());

      if(pcreatordata->m_id.is_text())
      {

         string strFile = pcreatordata->m_id;

         if(gen::str::begins_eat_ci(strFile, "default_file_handler://"))
         {

            ::filehandler::view * pview = create_view < ::filehandler::view > (get_document(), pcreatordata->m_pholder);

            pcreatordata->m_pwnd = pview;

            pcreatordata->m_pdoc = get_document();

            pview->m_strName = strFile;
            //pview->layout();

         }

         return;
      }
      switch(pcreatordata->m_id)
      {
      case pane_view_new:
         {
            /*               ::ca::create_context_sp createcontext(get_app());
            createcontext->m_bMakeVisible = true;
            createcontext->m_puiParent = this;
            rtprx::document * pdoc = dynamic_cast < rtprx::document * > (papp->m_ptemplateVideo->open_document_file(createcontext));
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
            pcreatordata->m_pwnd = pframe;

            //                  pdoc->Initialize(true);
            }
            }
            }*/
            /*         create_context cc;
            cc.m_pCurrentDoc = get_document();
            cc.m_typeinfoNewView =  System.type_info < rtprx::view > ();

            ::userbase::view * pview = dynamic_cast < ::userbase::view * > (CreateView(&cc, 101, this));
            if(pview != NULL)
            {
            pcreatordata = new ViewData();
            pcreatordata->m_eview = eview;
            pcreatordata->m_pdoc = get_document();
            pcreatordata->m_pwnd = pview;
            }*/
         }
         break;
      default:
         ASSERT(FALSE);
         break;
      }
   }

   void pane_view::rotate()
   {
   }

   void pane_view::_001OnMenuMessage(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      set_cur_tab_by_id(m_pviewdataOld->m_id);
   }

   void pane_view::install_message_handling(::gen::message::dispatch * pinterface)
   {
      ::userex::pane_tab_view::install_message_handling(pinterface);

      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &pane_view::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_USER + 1122  , pinterface, this, &pane_view::_001OnMenuMessage);
   }


} // namespace filehandler




