#include "framework.h"
//#include <math.h>






namespace prompt
{


   primary_view::primary_view(::aura::application * papp) :
      ::object(papp),
      ::user::plain_edit(papp)
   {

      m_iCompromised = 0;

      connect_command_probe("edit_copy", &primary_view::_001OnUpdateEditCopy);
      connect_command("edit_copy", &primary_view::_001OnEditCopy);
      connect_command_probe("edit_paste", &primary_view::_001OnUpdateEditPaste);
      connect_command("edit_paste", &primary_view::_001OnEditPaste);

   }


   void primary_view::on_update(::user::impact * pSender, LPARAM lHint, ::object* phint)
   {

      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(phint);

   }


   void primary_view::install_message_routing(::message::sender * pinterface)
   {

      BASE::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &primary_view::_001OnContextMenu);

   }


   void primary_view::_001OnAfterChangeText(::action::context actioncontext)
   {
      string str;

      _001GetText(str);
      strsize iFind;
      strsize iFind2;

      if((iFind = str.reverse_find('\n')) >= 0)
      {
         iFind2 = str.reverse_find('\n', iFind - 1);
         iFind2++;
         if(iFind > iFind2 && iFind2 >= m_iCompromised)
         {
            string strLine = str.Mid(iFind2, iFind - iFind2 - 1);
            if(strLine.has_char())
            {
               bool bOk = false;
               string strNewLine;
               try
               {

                  ::calculator::parser parser(get_app());

                  ::calculator::element * pelement = parser.parse(strLine);

                  string strValue;

                  strValue.Format("%f", pelement->get_value());

                  string strNewText = str + pelement->get_expression() + " = " + strValue  + "\n";

                  strNewText.replace("\r\n", "\n");

                  _001SetText(strNewText, ::action::source::sync());

                  str = strNewText;

                  m_iCompromised = m_ptree->m_iSelBeg = m_ptree->m_iSelEnd = strNewText.get_length();

                  bOk = true;

               }
               catch(const char * psz)
               {

                  TRACE("error %s", psz);

                  bOk = false;

               }

               try
               {
                  if(!bOk)
                  {

                     stringa stra;

                     if(cregexp_util::match(stra, strLine, "(.+)\\s*segundos", true, 2) == 1)
                     {

                        calculator::parser parser(get_app());

                        calculator::element * pelement = parser.parse(stra[1]);

                        string strValue;

                        strValue.Format("%f", pelement->get_value());

                        string strMinFrac;

                        strMinFrac.Format("%f", pelement->get_value().mod() / 60.0);

                        string strMin;

                        strMin.Format("%d", ((int32_t)pelement->get_value().mod() / 60));

                        string strMinSec;

                        strMinSec.Format("%f", fmod(pelement->get_value().mod(), 60.0));

                        string strNewText = str + pelement->get_expression() + " segundos = " + strValue  + " segundos = " + strMinFrac + " minutos = " + strMin + " minutos e " + strMinSec + " segundos\n";

                        strNewText.replace("\r\n", "\n");

                        _001SetText(strNewText, ::action::source::sync());

                        str = strNewText;

                        m_iCompromised = m_ptree->m_iSelBeg = m_ptree->m_iSelEnd = strNewText.get_length();

                        bOk = true;

                     }
                     else if(cregexp_util::match(stra, strLine, "(.+)\\s*dias", true, 2) == 1)
                     {

                        calculator::parser parser(get_app());

                        calculator::element * pelement = parser.parse(stra[1]);

                        string strValue;

                        strValue.Format("%f", pelement->get_value());

                        string strMinFrac;

                        strMinFrac.Format("%f", pelement->get_value().mod() / 60.0);

                        string strMin;

                        strMin.Format("%d", ((int32_t)pelement->get_value().mod() / 60));

                        string strMinSec;

                        strMinSec.Format("%f", fmod(pelement->get_value().mod(), 60.0));

                        string strNewText = str + pelement->get_expression() + " segundos = " + strValue  + " segundos = " + strMinFrac + " minutos = " + strMin + " minutos e " + strMinSec + " segundos\n";

                        strNewText.replace("\r\n", "\n");

                        _001SetText(strNewText, ::action::source::sync());

                        str = strNewText;

                        m_iCompromised = m_ptree->m_iSelBeg = m_ptree->m_iSelEnd = strNewText.get_length();

                        Application.send_simple_command("winactionareaview::show_calendar(\""+ ::str::from((int32_t) pelement->get_value().mod()) +"\")", (void *) get_wnd()->get_os_data());

                        bOk = true;

                     }

                  }

               }
               catch(const char * psz)
               {

                  TRACE("error %s", psz);

                  bOk = false;

               }

               if(!bOk)
               {

#ifdef WINDOWSEX

                  if(::ShellExecuteW(
                        NULL,
                        NULL,
                        ::str::international::utf8_to_unicode(strLine),
                        NULL,
                        NULL,
                        SW_SHOW))
                  {
                     string strNewText = str + "executing " + strLine  + "...";
                     strNewText.replace("\r\n", "\n");
                     _001SetText(strNewText, ::action::source::system());
                     m_iCompromised = m_ptree->m_iSelBeg = m_ptree->m_iSelEnd = strNewText.get_length();
                  }

#else

                  _throw(todo(get_app()));

#endif

               }
            }
         }


      }
   }

   void primary_view::_001OnUpdateEditCopy(::message::message * pobj)
   {
      SCAST_PTR(::user::command, pcommand, pobj);
      pcommand->Enable(TRUE);
   }

   void primary_view::_001OnEditCopy(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      clipboard_copy();
   }

   void primary_view::_001OnUpdateEditPaste(::message::message * pobj)
   {
      SCAST_PTR(::user::command, pcommand, pobj);
      pcommand->Enable(TRUE);
   }


   void primary_view::_001OnEditPaste(::message::message * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      clipboard_paste();
   }


   void primary_view::_001OnContextMenu(::message::message * pobj)
   {

      track_popup_xml_matter_menu("command/popup_primary_verbing.xml", 0, pobj);

   }


} // namespace prompt


