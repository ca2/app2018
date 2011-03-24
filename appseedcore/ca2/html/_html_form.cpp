#include "StdAfx.h"

html_form::html_form(::ca::application * papp) :
   ca(papp),
   ::user::interaction(papp),
   ::userbase::view(papp),
   ::user::scroll_view(papp),
   ::userbase::scroll_view(papp),
   ::user::form(papp),
   ::userbase::form_view(papp)
{
}

html_form::~html_form()
{
}



void html_form::_001OnDraw(::ca::graphics * pdc)
{
   if(get_html_data() != NULL)
   {
      get_html_data()->_001OnDraw(pdc);
   }
}


void html_form::_001DrawChildren(::ca::graphics *pdc)
{
      
   if(m_pguie != NULL && m_pguie != this)
   {
      m_pguie->_001DrawChildren(pdc);
   }
   else
   {
      ::user::interaction * pui = get_bottom_child();
      while(pui != NULL)
      {
         try
         {
            if(pui->m_bVisible && (get_html_data() == NULL || !get_html_data()->contains(pui)))
            {
               pui->_000OnDraw(pdc);
            }
            pui = pui->above_sibling();
         }
         catch(...)
         {
         }
      }
   }
}


void html_form::_001OnImageLoaded(gen::signal_object * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   if(get_html_data() != NULL)
   {
      GetClientRect(&get_html_data()->m_rect);
      if(get_html_data()->m_rect.width() > 0 &&
         get_html_data()->m_rect.height() > 0)
      {
         ::ca::graphics * pdc = GetDC();
         get_html_data()->delete_implementation();
         get_html_data()->layout(pdc);
         ReleaseDC(pdc);
         _001RedrawWindow();
      }
   }
}


void html_form::_001InstallMessageHandling(::user::win::message::dispatch * pinterface)
{
   ::userbase::form_view::_001InstallMessageHandling(pinterface);

   IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &html_form::_001OnSize);
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &html_form::_001OnCreate);

//   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
//   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_KEYDOWN, pinterface, this, &::user::interaction::_001OnKeyDown);
   IGUI_WIN_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &html_form::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &html_form::_001OnMouseMove);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pinterface, this, &html_form::_001OnLButtonUp);

   IGUI_WIN_MSG_LINK(html::message_on_image_loaded, pinterface, this, &html_form::_001OnImageLoaded);
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
   layout();
   return TRUE;

}
*/

void html_form::layout()
{
   if(get_html_data() == NULL)
      return;
   GetClientRect(&get_html_data()->m_rect);
   bool bLayoutOk = false;
   if(get_html_data()->m_rect.width() > 0 &&
      get_html_data()->m_rect.height() > 0)
   {
      bLayoutOk = true;
   }
   ::ca::graphics * pdc = GetDC();
   get_html_data()->m_pguie = this;
   get_html_data()->m_pguie = this;
   get_html_data()->implement(pdc);
   if(bLayoutOk)
   {
      get_html_data()->m_pform = this;
      get_html_data()->layout(pdc);
   }
   ReleaseDC(pdc);
   if(bLayoutOk)
   {
      _001RedrawWindow();
   }

}


void html_form::_001OnSize(gen::signal_object * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   layout();
}

void html_form::_001OnCreate(gen::signal_object * pobj)
{
   SCAST_PTR(::user::win::message::create, pcreate, pobj)
   pobj->previous();
   m_spdata = calc_data();
   get_html_data()->m_pform = this;
   ASSERT(get_html_data() != NULL);
   if(get_html_data() == NULL)
   {
      pcreate->set_lresult(0);
      pcreate->m_bRet = true;
      return;
   }
   if(m_strOpenOnCreate.has_char() && !get_html_data()->open_document(m_strOpenOnCreate))
   {
      pcreate->set_lresult(-1);
      pcreate->m_bRet = true;
      return;
   }
   return;
}



  void html_form::_001OnLButtonDown(gen::signal_object * pobj)
{
   SCAST_PTR(::user::win::message::mouse, pmouse, pobj);
   point pt;
   pt = pmouse->m_pt;
   ScreenToClient(&pt);
   m_pelementalLButtonDown = get_html_data()->m_elemental.hit_test(pt);
   if(m_pelementalLButtonDown != NULL)
   {
      html::signal signal(pobj->m_psignal);
      signal.m_pdata = get_html_data();
      signal.m_psignal = pmouse;
      m_pelementalLButtonDown->OnLButtonDown(&signal);
   }
   pmouse->m_bRet = true;
   pmouse->set_lresult(1);
}

void html_form::_001OnMouseMove(gen::signal_object * pobj)
{
   /*SCAST_PTR(::user::win::message::mouse, pmouse, pobj);
   point pt;
   pt = pmouse->m_pt;
   ScreenToClient(&pt);
   html::elemental * pelemental = get_html_data()->m_elemental.hit_test(pt);
   if(pelemental != NULL)
   {
      html::signal signal(pobj->m_psignal);
      signal.m_pdoccontainer = m_pcontainer;
      signal.m_psignal = pmouse;
      pelemental->OnMouseMove(&signal);
   }*/

   SCAST_PTR(::user::win::message::mouse, pmouse, pobj);
   point pt;
   pt = pmouse->m_pt;
   ScreenToClient(&pt);
   html::elemental * pelemental = get_html_data()->m_elemental.hit_test(pt);
   html::signal signal(pobj->m_psignal);
   signal.m_pdata = get_html_data();
   signal.m_psignal = pmouse;
   signal.m_pui = this;
   if(pelemental != NULL)
   {
      pelemental->OnMouseMove(&signal);
      if(signal.m_bRet)
         m_elementalptraMouseMove.add(pelemental);
   }
   for(int i = 0; i < m_elementalptraMouseMove.get_count(); )
   {
      try
      {
         m_elementalptraMouseMove.element_at(i)->OnMouseMove(&signal);
         if(!signal.m_bRet)
         {
            m_elementalptraMouseMove.remove_at(i);
         }
         else
         {
            i++;
         }
      }
      catch(...)
      {
         m_elementalptraMouseMove.remove_at(i);
      }
   }

}

void html_form::_001OnLButtonUp(gen::signal_object * pobj)
{
   SCAST_PTR(::user::win::message::mouse, pmouse, pobj);
   point pt;
   pt = pmouse->m_pt;
   ScreenToClient(&pt);
   html::elemental * pelemental = get_html_data()->m_elemental.hit_test(pt);
   if(m_pelementalLButtonDown != NULL
      && pelemental == m_pelementalLButtonDown)
   {
      html::signal signal(pobj->m_psignal);
      signal.m_pdata = get_html_data();
      signal.m_psignal = pmouse;
      m_pelementalLButtonDown->OnLButtonUp(&signal);
   }
}

html::data * html_form::calc_data()
{
   if(get_html_data() != NULL)
      return get_html_data();
   else
      return dynamic_cast < html::data * > (System.alloc(typeid(html::data)));
}

/*
bool html_form::open_document(const char * pszPath)
{
   return open_document(pszPath);
}
*/

string html_form::get_path()
{
   return m_strPath;
}


bool html_form::open_document(var varFile)
{
   if(!get_html_data()->open_document(varFile))
      return false;
   string strPathName;
   if(varFile.get_type() == var::type_propset && varFile.propset()["url"].get_string().has_char())
   {
      strPathName = varFile.propset()["url"];
   }
   else if(varFile.ca2 < ::ex1::file > () != NULL)
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
   get_html_data()->m_elemental.get_html(str);
}

void html_form::_001SetText(const char * psz)
{
   get_html_data()->load(psz);
   layout();
}


::html::data * html_form::get_html_data()
{
   return dynamic_cast < ::html::data * > (::view::get_data());
}

const ::html::data * html_form::get_html_data() const
{
   return dynamic_cast < ::html::data * > (::view::get_data());
}