#include "StdAfx.h"

simple_list_control::simple_list_control(::ca::application * papp) :
   ca(papp),
   m_headerctrl(papp),
   m_scrollbarVert(papp),
   m_scrollbarHorz(papp),
   ::user::scroll_view(papp),
   ::user::list(papp),
   m_listctrldata(papp)
{
   m_pheaderctrl     = &m_headerctrl;
   m_pheaderctrl->SetBaseListCtrlInterface(this);
   m_pscrollbarVert  = &m_scrollbarVert;
   m_pscrollbarHorz  = &m_scrollbarHorz;
   SetDataInterface(&m_listctrldata);
}

simple_list_control::~simple_list_control()
{
}

::user::interaction* simple_list_control::OnDrawInterfaceGetWnd()
{
   return this;
}

void simple_list_control::OnDrawInterfaceDraw(::ca::graphics * pdc)
{
   _001OnDraw(pdc);
}


BOOL simple_list_control::PreCreateWindow(CREATESTRUCT& cs) 
{

   cs.style |= WS_CLIPCHILDREN;
   
   return ::user::interaction::PreCreateWindow(cs);
}

void simple_list_control::set_data(stringa &stra)
{
   m_listctrldata.set_data(this, stra);
}

void simple_list_control::get_data(stringa &stra)
{
   m_listctrldata.get_data(stra);
}

void simple_list_control::pre_subclass_window() 
{
   PreSubClassWindow();

   if(!CreateHeaderCtrl())
   {
      return;
   }
   
   class rect rect;
   rect.null();

   if(!m_pscrollbarHorz->create(
      ::user::scroll_bar::orientation_horizontal,
      WS_CHILD 
      | WS_VISIBLE,
      rect,
      this,
      1024))
   {
      return;
   }

   rect.null();

   if(!m_pscrollbarVert->create(
      ::user::scroll_bar::orientation_vertical,
      WS_CHILD 
      | WS_VISIBLE,
      rect,
      this,
      1025))
   {
      return;
   }


   layout();

   if(m_pbuffer != NULL)
   {
      class rect rect;
      GetClientRect(rect);
      m_pbuffer->UpdateBuffer(rect.size());
   }

   
   ::user::interaction::pre_subclass_window();

   
}


bool simple_list_control::RemoveItem(int iItem)
{
   if(!m_listctrldata.RemoveItem(iItem))
      return false;
   _001OnUpdateItemCount();
   return true;
}

bool simple_list_control::RemoveItem(ItemRange & range)
{
   bool bOk = true;
   for(int iItem = range.GetUBound(); iItem >= range.GetLBound(); iItem++)
   {
      if(!m_listctrldata.RemoveItem(iItem))
         bOk = false;
   }
   _001OnUpdateItemCount();
   return true;
}

bool simple_list_control::RemoveItem(Range & range)
{
   bool bOk = true;
   int_array iaRemove;
   for(int iRange = 0; iRange < range.get_item_count(); iRange++)
   {
      ItemRange & itemrange = range.ItemAt(iRange);
      for(int iItem = itemrange.GetLBound(); iItem <= itemrange.GetUBound(); iItem++)
      {
         iaRemove.add_unique(iItem);
      }
   }
   iaRemove.QuickSort(false);
   for(int i = 0; i < iaRemove.get_size(); i++)
   {
      if(!m_listctrldata.RemoveItem(iaRemove[i]))
         bOk = false;
   }
   _001OnUpdateItemCount();
   return bOk;
}


void simple_list_control::_001InstallMessageHandling(::user::win::message::dispatch * pinterface)
{
   UNREFERENCED_PARAMETER(pinterface);
}