#include "framework.h"


html_form::html_form(::aura::application * papp) :
   ::object(papp),
   ::user::interaction(papp)
{

   m_flagNonClient.unsignalize(non_client_background);
   m_flagNonClient.unsignalize(non_client_focus_rect);

   m_phtmlform = canew(html::form());

   m_phtmlform->m_pelementalHover = NULL;

}

html_form::~html_form()
{

}



void html_form::_001OnDraw(::draw2d::graphics * pgraphics)
{

   ::html::data * sphtmldata = NULL;

   try
   {

      sphtmldata = get_html_data();

   }
   catch(...)
   {
   }


   if(sphtmldata != NULL)
   {

      sphtmldata->_001OnDraw(pgraphics);

   }



}


void html_form::_001DrawChildren(::draw2d::graphics * pgraphics)
{

   sp(::user::interaction) pui = first_child();

   while(pui != NULL)
   {

      try
      {

         if(pui->is_this_visible() && (get_html_data() == NULL || !get_html_data()->contains(pui)))
         {

            pui->_000OnDraw(pgraphics);

         }

         pui = pui->above_sibling();

      }
      catch(...)
      {

      }

   }

}


void html_form::_001OnImageLoaded(signal_details * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   if(get_html_data() != NULL)
   {

      rect rectClient;

      GetClientRect(rectClient);

      if(rectClient.area() > 0)
      {

         get_html_data()->m_box = rectClient;

         synch_lock lock(get_html_data()->m_pmutex);

         ::draw2d::dib_sp pdib(allocer());

         pdib->create(50, 50);

         get_html_data()->delete_implementation();

         get_html_data()->on_layout(pdib->get_graphics());

         RedrawWindow();

      }

   }

}


void html_form::install_message_handling(::message::dispatch * pinterface)
{
   ::user::form_view::install_message_handling(pinterface);

   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &html_form::_001OnCreate);

//   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
//   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_KEYDOWN, pinterface, this, &::html_form::_001OnKeyDown);
   IGUI_WIN_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &html_form::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &html_form::_001OnMouseMove);
   IGUI_WIN_MSG_LINK(WM_MOUSELEAVE, pinterface, this, &html_form::_001OnMouseLeave);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &html_form::_001OnLButtonUp);

   IGUI_WIN_MSG_LINK(html::message_on_image_loaded, pinterface, this, &html_form::_001OnImageLoaded);

   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &html_form::_001OnDestroy);

}

/*
bool html_form::open_document(const char * lpszPathName)
{
   string strPathName(lpszPathName);
   if(strPathName.Mid(3) == "wb:")
      return FALSE;
   string str = Application.file().as_string(lpszPathName);
   if(str.is_empty())
   {
      System.sync_load_url(str, lpszPathName, &ApplicationUser, mm_pcookies);
   }
   m_strPathName = lpszPathName;
   get_html_data()->m_strPathName = m_strPathName;
   get_html_data()->load(str);
   on_layout();
   return TRUE;

}
*/

void html_form::GetClientBox(::html::box & box)
{

   rect rectClient;

   GetClientRect(rectClient);

   box = rectClient;

}


void html_form::on_layout()
{

   synch_lock sl(m_pmutex);

   if(get_html_data() == NULL)
      return;

   get_html_data()->implement_and_layout(this);

}


void html_form::_001OnCreate(signal_details * pobj)
{
   SCAST_PTR(::message::create, pcreate, pobj);
   if(pobj->previous())
      return;
   get_html_data()->m_pform = this;
   ASSERT(get_html_data() != NULL);
   if(get_html_data() == NULL)
   {
      pcreate->set_lresult(0);
      pcreate->m_bRet = true;
      
   }

}


void html_form::on_update(::user::impact * pSender, LPARAM lHint, object* pHint)
{

   ::user::form_view::on_update(pSender, lHint, pHint);

   if (lHint == 123)
   {
    

      if (m_strOpenOnCreate.has_char())
      {
         
         get_document()->on_open_document(m_strOpenOnCreate);

         m_strOpenOnCreate.Empty();

      }

   }

}


void html_form::_001OnLButtonDown(signal_details * pobj)
{
   SCAST_PTR(::message::mouse, pmouse, pobj);
   point pt;
   pt = pmouse->m_pt;
   ScreenToClient(&pt);
   m_phtmlform->m_pelementalLButtonDown = get_html_data()->m_elemental.hit_test(get_html_data(), pt);
   if(m_phtmlform->m_pelementalLButtonDown != NULL)
   {
      html::signal signal(pobj->m_psignal);
      signal.m_pdata = get_html_data();
      signal.m_psignal = pmouse;
      m_phtmlform->m_pelementalLButtonDown->OnLButtonDown(&signal);
   }
   pmouse->m_bRet = true;
   pmouse->set_lresult(1);
}

void html_form::_001OnMouseMove(signal_details * pobj)
{

   SCAST_PTR(::message::mouse, pmouse, pobj);

   track_mouse_hover();

   point pt(pmouse->m_pt);

   ScreenToClient(&pt);
   
   synch_lock sl(m_pmutex);

   html::elemental * pelemental = get_html_data()->m_elemental.hit_test(get_html_data(), pt);

   if(pelemental != NULL)
   {

      if (pelemental != m_phtmlform->m_pelementalHover)
      {

         if (m_phtmlform->m_pelementalHover != NULL)
         {

            if (m_phtmlform->m_pelementalHover->m_pimpl != NULL)
            {

               m_phtmlform->m_pelementalHover->m_pimpl->m_bHover = false;

            }

         }

         m_phtmlform->m_pelementalHover = pelemental;

      }

      html::signal signal(pobj->m_psignal);

      signal.m_pdata = get_html_data();

      signal.m_psignal = pmouse;

      signal.m_pui = this;

      pelemental->OnMouseMove(&signal);
/*      if(signal.m_bRet)
         m_elementalptraMouseMove.add(pelemental);*/
   }

   //for(int32_t i = 0; i < m_elementalptraMouseMove.get_count(); )
   //{
   //   try
   //   {
   //      m_elementalptraMouseMove.element_at(i)->OnMouseMove(&signal);
   //      if(!signal.m_bRet)
   //      {
   //         m_elementalptraMouseMove.remove_at(i);
   //      }
   //      else
   //      {
   //         i++;
   //      }
   //   }
   //   catch(...)
   //   {
   //      m_elementalptraMouseMove.remove_at(i);
   //   }
   //}

}

void html_form::_001OnMouseLeave(signal_details * pobj)
{

   if(m_phtmlform->m_pelementalHover != NULL)
   {

      if (m_phtmlform->m_pelementalHover->m_pimpl != NULL)
      {

         m_phtmlform->m_pelementalHover->m_pimpl->m_bHover = false;

      }

      m_phtmlform->m_pelementalHover = NULL;

   }

}

void html_form::_001OnLButtonUp(signal_details * pobj)
{
   SCAST_PTR(::message::mouse, pmouse, pobj);
   point pt(pmouse->m_pt);
   ScreenToClient(&pt);
   html::elemental * pelemental = get_html_data()->m_elemental.hit_test(get_html_data(), pt);
   if(m_phtmlform->m_pelementalLButtonDown != NULL
      && pelemental == m_phtmlform->m_pelementalLButtonDown)
   {
      html::signal signal(pobj->m_psignal);
      signal.m_pdata = get_html_data();
      signal.m_psignal = pmouse;
      m_phtmlform->m_pelementalLButtonDown->OnLButtonUp(&signal);
   }
}


void html_form::_001OnDestroy(signal_details * pobj)
{
 
   if (get_document() != NULL)
   {

      ::html::data * pdata = get_document()->m_spadata.get < ::html::data >();
      
      if (pdata != NULL)
      {

         if (pdata->m_pform == this)
         {

            pdata->m_pform = NULL;

         }

         if (pdata->m_pui == this)
         {

            pdata->m_pui = NULL;

         }

      }

   }

}


string html_form::get_path()
{
   return m_strPath;
}


bool html_form::open_document(var varFile)
{
   if(!::user::form::open_document(varFile))
      return false;
   string strPathName;
   if(varFile.get_type() == var::type_propset && varFile.propset()["url"].get_string().has_char())
   {

      strPathName = varFile.propset()["url"];

   }
   else if(varFile.cast < ::file::binary_file > () != NULL)
   {

      strPathName = System.datetime().international().get_gmt_date_time() + "." + get_document()->get_document_template()->m_set["default_extension"];

   }
   else
   {

      strPathName = varFile;

   }

   m_strPath = strPathName;

   return true;

}


void html_form::_001GetText(string & str) const
{

   ((html_form *) this)->get_html_data()->m_elemental.get_html((const_cast < html_form * > (this)->get_html_data()), str);

}


void html_form::_001SetText(const string & str, ::action::context actioncontext)
{

   bool bFocus = Session.get_keyboard_focus() == this || is_descendant(dynamic_cast < ::user::interaction * > (Session.get_keyboard_focus()));

   sp(::html::data) sphtmldata;

   sphtmldata = canew(::html::data(get_app()));

   sphtmldata->m_pform = this;

   sphtmldata->load(str);

   sphtmldata->implement_and_layout(this);

   m_phtmlform->m_sphtmldata = sphtmldata;

   if(bFocus)
   {
	   sp(::user::elemental) pfocus = get_focusable_descendant();
      if(pfocus != NULL)
      {
         Session.set_keyboard_focus(pfocus);
      }
   }
}


html_document * html_form::get_document()
{

   return dynamic_cast < html_document *> (::user::impact::get_document());

}


::html::data * html_form::get_html_data()
{

   if (m_phtmlform->m_sphtmldata == NULL)
   {

      html_document * pdocument = get_document();

      ASSERT(pdocument != NULL);

      if (pdocument != NULL)
      {

         m_phtmlform->m_sphtmldata = pdocument->get_html_data();

         m_phtmlform->m_sphtmldata->m_pui = this;

      }

   }

   return m_phtmlform->m_sphtmldata;

}


void html_form::_001OnKeyDown(signal_details * pobj)
{
   SCAST_PTR(::message::key, pkey, pobj);
   if(pkey->m_ekey == ::user::key_tab)
   {
      pkey->m_bRet = true;
      return;
   }
}


void html_form::defer_implement()
{

   if(get_html_data() == NULL)
      return;

   GetClientBox(get_html_data()->m_box);

   if(get_html_data()->m_box.area() <= 0.f)
      return;

   ::draw2d::dib_sp pdib(allocer());

   pdib->create(50, 50);

   get_html_data()->m_pui = this;

   get_html_data()->m_pform = this;

   get_html_data()->implement(pdib->get_graphics());


}


void html_form::defer_layout()
{

   if(get_html_data() == NULL)
      return;

   GetClientBox(get_html_data()->m_box);

   if(get_html_data()->m_box.area() <= 0.f)
      return;

   ::draw2d::dib_sp pdib(allocer());

   pdib->create(50, 50);

   get_html_data()->m_pui = this;

   get_html_data()->m_pform = this;

   get_html_data()->on_layout(pdib->get_graphics());

   RedrawWindow();

}


bool html_form::get_edit_mode()
{

   return get_html_data()->m_bEdit;

}


void html_form::set_edit_mode(bool bSet)
{

   get_html_data()->m_bEdit = bSet;

}


bool html_form::load_html(const string & str)
{

   get_html_data()->load(str);

   return true;

}
