#include "framework.h"

//   // BEGIN_MESSAGE_MAP(html_view, ::user::impact)
//{{__MSG_MAP(html_view)
/*
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_PAINT()

ON_WM_CONTEXTMENU()
ON_WM_SETCURSOR()
ON_WM_ERASEBKGND()
//}}__MSG_MAP
// Standard printing commands
ON_MESSAGE(WM_USER + 177, OnTabClick)
ON_COMMAND(ID_FILE_PRINT, ::user::impact::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, ::user::impact::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, ::user::impact::OnFilePrintPreview)
*/
// // END_MESSAGE_MAP()


html_view::html_view(::aura::application * papp) :
   ::object(papp),
   ::user::interaction(papp),
   html_form(papp)
{
}

html_view::~html_view()
{
}

void html_view::install_message_routing(::message::sender * pinterface)
{
   ::html_form::install_message_routing(pinterface);

   IGUI_MSG_LINK(WM_DESTROY, pinterface, this, &html_view::_001OnDestroy);
   //IGUI_MSG_LINK(WM_SIZE, pinterface, this, &html_view::_001OnSize);
   IGUI_MSG_LINK(WM_CREATE, pinterface, this, &html_view::_001OnCreate);
   IGUI_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &html_view::_001OnContextMenu);
   IGUI_MSG_LINK(WM_SETCURSOR, pinterface, this, &html_view::_001OnSetCursor);

   //   IGUI_MSG_LINK(WM_USER + 177     , this, this, &html_view::_001OnTabClick);
   //connect_command(ID_FILE_PRINT, ::user::impact::OnFilePrint)
   //connect_command(ID_FILE_PRINT_DIRECT, ::user::impact::OnFilePrint)
   //connect_command(ID_FILE_PRINT_PREVIEW, ::user::impact::OnFilePrintPreview)
   //   IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &::user::interaction::_001OnLButtonDown);
   //   IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &::user::interaction::_001OnLButtonUp);
   //IGUI_MSG_LINK(WM_KEYDOWN, pinterface, this, &::user::interaction::_001OnKeyDown);
   //IGUI_MSG_LINK(WM_KEYUP, pinterface, this, &::user::interaction::_001OnKeyUp);

   //IGUI_MSG_LINK(WM_LBUTTONDOWN, pinterface, this, &html_view::_001OnLButtonDown);
   //IGUI_MSG_LINK(WM_MOUSEMOVE, pinterface, this, &html_view::_001OnMouseMove);
   //IGUI_MSG_LINK(WM_LBUTTONUP, pinterface, this, &html_view::_001OnLButtonUp);
   //IGUI_MSG_LINK(WM_KEYDOWN, pinterface, this, &html_view::_001OnKeyDown);

}

/////////////////////////////////////////////////////////////////////////////
// html_view diagnostics


void html_view::assert_valid() const
{
   ::user::impact::assert_valid();
}

void html_view::dump(dump_context & dumpcontext) const
{
   ::user::impact::dump(dumpcontext);
}



bool html_view::pre_create_window(::user::create_struct& cs)
{
   cs.style &= ~WS_EX_CLIENTEDGE;
   return ::user::impact::pre_create_window(cs);
}
void html_view::_001OnInitialUpdate()
{

   ::user::impact::_001OnInitialUpdate();

}


void html_view::on_update(::user::impact * pSender, LPARAM lHint, object* phint)
{

   ::html_form::on_update(pSender, lHint, phint);

   if(phint != NULL)
   {

      html_view_update_hint * puh = dynamic_cast < html_view_update_hint * > (phint);

      if(puh != NULL)
      {

         if(puh->m_etype == html_view_update_hint::type_document_complete)
         {

            {

               synch_lock sl(m_pmutex);

               if(get_html_data() == NULL)
               {

                  return;

               }

               get_html_data()->implement(this);

            }

            on_document_complete(puh->m_strUrl);

            set_need_layout();

            set_need_redraw();

         }

      }

   }

}


void html_view::_001OnDestroy(::message::message * pobj)
{
   ::user::impact::_001OnDestroy(pobj);

}





void html_view::_001OnCreate(::message::message * pobj)
{
   if(pobj->previous())
      return;

   get_html_data()->m_pui = this;
   /*get_html_data()->m_papp = get_app();*/
   get_html_data()->m_strPathName = get_document()->get_file_path();


}
void html_view::_001OnContextMenu(::message::message * pobj)
{
   //   SCAST_PTR(::message::context_menu, pcontextmenu, pobj);
   //   point point = pcontextmenu->GetPoint();

}



void html_view::_001OnSetCursor(::message::message * pobj)
{

   SCAST_PTR(::message::mouse, pmouse, pobj);

   pmouse->m_ecursor = ::visual::cursor_arrow;

   pobj->previous();

}


sp(html_document) html_view::get_document()
{
   return  (::user::impact::get_document());
}

string html_view::GetLocationURL()
{
   return m_strLocationURL;
}

bool html_view::Refresh2(int32_t i)
{
   UNREFERENCED_PARAMETER(i);
   get_document()->on_open_document(GetLocationURL());
   return true;
}

bool html_view::Navigate(const char * pszUrl)
{
   if(!get_document()->on_open_document(pszUrl))
      return false;
   m_strLocationURL = pszUrl;
   return true;
}

void html_view::on_document_complete(const char * pszUrl)
{
   UNREFERENCED_PARAMETER(pszUrl);
}




