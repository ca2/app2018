//#include "framework.h"


namespace user
{


   single_document_template::single_document_template(::aura::application * papp, const char * pszMatter, sp(type) pDocClass, sp(type) pFrameClass, sp(type) pViewClass) :
      object(papp),
      ::aura::impact_system(papp, pszMatter, pDocClass, pFrameClass, pViewClass)
   {
      m_pdocument = NULL;
   }

   single_document_template::~single_document_template()
   {
#ifdef DEBUG
      if (m_pdocument != NULL)
         TRACE(::aura::trace::category_AppMsg, 0, "Warning: destroying single_document_template with live ::aura::document.\n");
#endif
   }

   ::count single_document_template::get_document_count() const
   {
      return (m_pdocument == NULL) ? 0 : 1;
   }

   ::aura::document * single_document_template::get_document(index index) const
   {
      if(index == 0)
         return m_pdocument;
      else
         return NULL;
   }

   void single_document_template::add_document(::aura::document * pdocument)
   {
      if(m_pdocument == NULL)
      {
         m_pdocument = pdocument;
         impact_system::add_document(pdocument);
      }
   }

   void single_document_template::remove_document(::aura::document * pdocument)
   {
      if(m_pdocument == pdocument)
      {
         impact_system::remove_document(pdocument);
         m_pdocument = NULL;
      }
   }

   /////////////////////////////////////////////////////////////////////////////
   // single_document_template commands

   void single_document_template::request_create(sp(::create) pcreatecontext)
      // if lpszPathName == NULL => create new file of this type
   {
      pcreatecontext->m_spCommandLine->m_varQuery["document"] = (sp(object)) NULL;
      bool bMakeVisible = pcreatecontext->m_spCommandLine->m_varQuery["make_visible_boolean"] || pcreatecontext->m_bMakeVisible;
      //   sp(::user::interaction) pwndParent = pcreatecontext->m_spCommandLine->m_varQuery["parent_user_interaction"].cast < ::user::interaction > ();
      //   sp(::aura::impact) pviewAlloc = pcreatecontext->m_spCommandLine->m_varQuery["allocation_view"].cast < ::aura::impact > ();

      sp(::aura::document) pdocument = NULL;
      sp(::user::frame_window) pFrame = NULL;
      bool bCreated = FALSE;      // => doc and frame created
      bool bWasModified = FALSE;

      if (m_pdocument != NULL)
      {
         // already have a ::aura::document - reinit it
         pdocument = m_pdocument;
         if (!pdocument->save_modified())
            return;        // leave the original one

         pFrame = pdocument->get_view()->GetParentFrame();
         ASSERT(pFrame != NULL);
         ASSERT_KINDOF(frame_window, pFrame);
         ASSERT_VALID(pFrame);
      }
      else
      {
         // create a new ::aura::document
         pdocument = create_new_document(pcreatecontext);
         ASSERT(pFrame == NULL);     // will be created below
         bCreated = TRUE;
      }

      if (pdocument == NULL)
      {
         // linux System.simple_message_box(__IDP_FAILED_TO_CREATE_DOC);
         System.simple_message_box(NULL, "Failed to create document");
         return;
      }
      ASSERT(pdocument == m_pdocument);

      if (pFrame == NULL)
      {
         ASSERT(bCreated);

         // create frame - set as main ::aura::document frame
         bool bAutoDelete = pdocument->m_bAutoDelete;
         pdocument->m_bAutoDelete = FALSE;
         // don't destroy if something goes wrong
         pFrame = create_new_frame(pdocument, NULL, pcreatecontext);
         pdocument->m_bAutoDelete = bAutoDelete;
         if (pFrame == NULL)
         {
            // linux System.simple_message_box(__IDP_FAILED_TO_CREATE_DOC);
            // System.simple_message_box(NULL, "Failed to create ::aura::document");
            pdocument.release();       // explicit delete on error
            return;
         }
      }

      if (pcreatecontext->m_spCommandLine->m_varFile.is_empty())
      {
         // create a new ::aura::document
         set_default_title(pdocument);

         // avoid creating temporary compound file when starting up invisible
         if (!bMakeVisible)
            pdocument->m_bEmbedded = TRUE;

         if (!pdocument->on_new_document())
         {
            // user has been alerted to what failed in on_new_document
            TRACE(::aura::trace::category_AppMsg, 0, "::aura::document::on_new_document returned FALSE.\n");
            if (bCreated)
               pFrame->DestroyWindow();    // will destroy ::aura::document
            return;
         }
      }
      else
      {
         wait_cursor wait(get_app());

         // open an existing ::aura::document
         bWasModified = pdocument->is_modified();
         pdocument->set_modified_flag(FALSE);  // not dirty for open

         if (!on_open_document(pdocument, pcreatecontext->m_spCommandLine->m_varFile))
         {
            // user has been alerted to what failed in on_open_document
            TRACE(::aura::trace::category_AppMsg, 0, "::aura::document::on_open_document returned FALSE.\n");
            if (bCreated)
            {
               pFrame->DestroyWindow();    // will destroy ::aura::document
            }
            else if (!pdocument->is_modified())
            {
               // original ::aura::document is untouched
               pdocument->set_modified_flag(bWasModified);
            }
            else
            {
               // we corrupted the original ::aura::document
               set_default_title(pdocument);

               if (!pdocument->on_new_document())
               {
                  TRACE(::aura::trace::category_AppMsg, 0, "Error: on_new_document failed after trying "
                     "to open a ::aura::document - trying to continue.\n");
                  // assume we can continue
               }
            }
            return;        // open failed
         }
         pdocument->set_path_name(pcreatecontext->m_spCommandLine->m_varFile);
         pdocument->update_title();
      }

//      thread* pThread = ::get_thread();
      
if(!pcreatecontext->m_bHold)
      {
         pFrame->oprop("should_not_be_automatically_holded_on_initial_update_frame") = true;
      }

if(bCreated)
{

   InitialUpdateFrame(pFrame,pdocument,bMakeVisible);

}

      ::user::view_update_hint uh(get_app());
      uh.m_ehint = ::user::view_update_hint::hint_open_document;
      pdocument->update_all_views(NULL, 0, &uh);

      pcreatecontext->m_spCommandLine->m_varQuery["document"] = pdocument;

   }

   void single_document_template::set_default_title(::aura::document * pdocument)
   {
      string strDocName;
      if (!GetDocString(strDocName, impact_system::docName) ||
         strDocName.is_empty())
      {
         strDocName = System.load_string("untitled");
      }
      pdocument->set_title(strDocName);
   }

   /////////////////////////////////////////////////////////////////////////////
   // single_document_template diagnostics


   void single_document_template::dump(dump_context & dumpcontext) const
   {
      impact_system::dump(dumpcontext);

      if (m_pdocument)
         dumpcontext << "with ::aura::document: " << (void *)m_pdocument;
      else
         dumpcontext << "with no ::aura::document";

      dumpcontext << "\n";
   }

   void single_document_template::assert_valid() const
   {
      impact_system::assert_valid();
   }


} // namespace user






