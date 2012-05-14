//
// Al� Telmo Friesen Son God
//
/////////////////////////////////////////////////////


#include "framework.h"


namespace userex
{


   second_list_view::second_list_view(::ca::application * papp) :
      ca(papp),
      userbase::view(papp),
      user::scroll_view(papp),
      user::form(papp),
      ::ca2::user::form(papp),
      user::list(papp),
      user::form_list(papp),
      userbase::form_list(papp),
      simple_list_view(papp)
   {
      
      m_eview = ViewList;
      
      m_scrollinfo.m_rectMargin.null();

      m_etranslucency = TranslucencyPresent;

   }
   
   second_list_view::~second_list_view()
   {
   }


#ifdef _DEBUG
   void second_list_view::assert_valid() const
   {
      simple_list_view::assert_valid();
   }

   void second_list_view::dump(dump_context & dumpcontext) const
   {
      simple_list_view::dump(dumpcontext);
   }
#endif //_DEBUG


   void second_list_view::install_message_handling(::gen::message::dispatch * pinterface)
   {
      simple_list_view::install_message_handling(pinterface);
      USER_MESSAGE_LINK(message_create, pinterface, this, &second_list_view::_001OnCreate);
   }

   void second_list_view::_001GetItemText(::user::list_item * pitem)
   {
      pitem->m_strText.Format("%02d", pitem->m_iItem);
      pitem->m_bOk = true;
   }
   count second_list_view::_001GetItemCount()
   {
      return 60;
   }

   void second_list_view::_001InsertColumns()
   {

      ::user::list_column column;

      column.m_iWidth               = 33;
      column.m_iSubItem             = 0;
      _001AddColumn(column);

   }

   void second_list_view::_001OnCreate(gen::signal_object * pobj)
   {
      pobj->previous();
      if(pobj->m_bRet)
         return;
      _001UpdateColumns();
      _001OnUpdateItemCount();
   }

   void second_list_view::_001OnSelectionChange()
   {

      ::user::control_event ev;
      ev.m_puie         = this;
      ev.m_eevent       = ::user::event_after_change_text;
      ev.m_bUser        = true;
      GetParent()->BaseOnControlEvent(&ev);
      BaseOnControlEvent(&ev);

   }

   int second_list_view::set_second(int isecond)
   {
      return (int) set_cur_sel(isecond);
   }

   int second_list_view::get_second()
   {
      return (int) get_cur_sel();
   }

} // namespace userex


