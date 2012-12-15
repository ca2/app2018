#include "framework.h"

namespace user
{

   void oswindow_array::SortByZOrder()
   {
      window_util::SortByZOrder(*this);
   }

   // This function sort the ::ca::window base_array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings
   void oswindow_array::SortSiblingsByZOrder()
   {
      oswindow oswindowSwap;
      for(int i = 0; i < this->get_size(); i++)
      {
         for(int j = i + 1; j < this->get_size(); j++)
         {
            if(window_util::GetZOrder(this->element_at(i)) > window_util::GetZOrder(this->element_at(j)))
            {
               oswindowSwap = this->element_at(i);
               this->element_at(i) = this->element_at(j);
               this->element_at(j) = oswindowSwap;
            }
         }
      }

   }
   void oswindow_array::top_windows_by_z_order()
   {

#ifdef WINDOWSEX

      int iOrder = 0;
      oswindow oswindowOrder = ::GetDesktopWindow();
      oswindowOrder = ::GetWindow(oswindowOrder, GW_CHILD);
      while(oswindowOrder != NULL
         && ::IsWindow(oswindowOrder))
      {
         add(oswindowOrder);
         oswindowOrder = ::GetWindow(oswindowOrder, GW_HWNDNEXT);
         iOrder++;
      }

#else

      throw not_implemented(get_app());

#endif

   }


   window_util::window_util()
   {

   }

   window_util::~window_util()
   {

   }

   // This function sort the ::ca::window base_array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings,
   // have a permanent ::ca::window associated object
   // and that all ::ca::window base_array pointers
   // are pointers to permanent objects.
   /*void window_util::SortByZOrder(Carray < ::ca::window *, ::ca::window * > & wndpa)
   {
   if(wndpa.get_size() <= 0)
   return;

   Carray < ::ca::window *, ::ca::window * > wndpa2;

   ::ca::window * pwnd = wndpa[0];

   ::ca::window * pwndChild = pwnd->GetWindow(GW_HWNDFIRST);

   while(pwndChild != NULL
   && ::IsWindow(pwndChild->GetSafeoswindow_()))
   {
   pwnd = ::ca::window::FromHandlePermanent(pwndChild->GetSafeoswindow_());
   if(pwnd == NULL)
   {
   CTransparentWndInterface * ptwi = NULL;
   CTransparentWndInterface::CGetProperty getp;
   getp.m_eproperty = CTransparentWndInterface::PropertyInterface;
   pwndChild->SendMessage(CTransparentWndInterface::MessageGetProperty, 0, (LPARAM) &getp);
   ptwi = getp.m_pinterface;
   if(ptwi != NULL)
   {
   pwnd = ptwi->TwiGetWnd();
   if(pwnd != NULL)
   {
   wndpa2.add(pwnd);
   }
   }

   }
   else
   {
   wndpa2.add(pwnd);
   }
   pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
   }

   Carray < ::ca::window *, ::ca::window * > wndpa3;
   Carray < ::ca::window *, ::ca::window * > wndpa4;

   for(int i = 0; i < wndpa2.get_size(); i++)
   {
   if(wndpa.find_first(wndpa2[i]) >= 0)
   {
   wndpa3.add(wndpa2[i]);
   }
   }

   for(i = 0; i < wndpa.get_size(); i++)
   {
   if(wndpa3.find_first(wndpa[i]) < 0)
   {
   wndpa3.add(wndpa[i]);
   }
   }

   wndpa.copy(wndpa3);

   }*/

   // This function sort the ::ca::window base_array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings,
   // have a permanent ::ca::window associated object
   // and that all ::ca::window base_array pointers
   // are pointers to permanent objects.
   /*void window_util::SortByZOrder(Carray < oswindow, oswindow > & oswindowa)
   {
   if(oswindowa.get_size() <= 0)
   return;

   Carray < oswindow, oswindow > oswindowa2;

   oswindow oswindow = oswindowa[0];

   oswindow oswindowChild = ::GetWindow(oswindow, GW_HWNDFIRST);

   while(oswindowChild != NULL
   && ::IsWindow(oswindowChild))
   {
   oswindowa2.add(oswindowChild);
   oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);
   }

   Carray < oswindow, oswindow > oswindowa3;
   Carray < oswindow, oswindow > oswindowa4;

   for(int i = 0; i < oswindowa2.get_size(); i++)
   {
   if(oswindowa.find_first(oswindowa2[i]) >= 0)
   {
   oswindowa3.add(oswindowa2[i]);
   }
   }

   for(i = 0; i < oswindowa.get_size(); i++)
   {
   if(oswindowa3.find_first(oswindowa[i]) < 0)
   {
   oswindowa3.add(oswindowa[i]);
   }
   }

   oswindowa.copy(oswindowa3);

   }*/


   /*void window_util::EnumChildren(oswindow oswindow, Carray < oswindow, oswindow > & oswindowa)
   {
   if(!::IsWindow)
   return;
   oswindow oswindowChild = ::GetTopWindow;
   while(oswindowChild != NULL)
   {
   oswindowa.add(oswindowChild);
   oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);
   }
   }

   void window_util::ExcludeChildren(oswindow oswindow, HRGN hrgn, POINT ptOffset)
   {


   Carray < oswindow, oswindow > oswindowa;

   EnumChildren(oswindow, oswindowa);

   for(int i = 0; i < oswindowa.get_size(); i++)
   {
   oswindow oswindowChild = oswindowa[i];
   rect rectChild;
   ::GetClientRect(oswindowChild, rectChild);
   ::ClientToScreen(oswindowChild, &rectChild.top_left());
   ::ClientToScreen(oswindowChild, &rectChild.bottom_right());
   ::ScreenToClient(oswindow, &rectChild.top_left());
   ::ScreenToClient(oswindow, &rectChild.bottom_right());
   rectChild.offset(ptOffset);
   HRGN hrgnChild = ::create_rect(rectChild);
   ::CombineRgn(hrgn, hrgn, hrgnChild, ::ca::region::combine_exclude);
   ::DeleteObject(hrgnChild);
   }




   }*/

   /*HRGN window_util::GetAClipRgn(oswindow oswindow, POINT ptOffset, bool bExludeChildren)
   {
   rect rectWnd;
   ::GetClientRect(oswindow, rectWnd);
   rectWnd.offset(ptOffset);
   HRGN hrgn = ::create_rect(rectWnd);

   if(bExludeChildren)
   {
   ExcludeChildren(oswindow, hrgn, ptOffset);
   }

   return hrgn;
   }*/




   void oswindow_tree::EnumDescendants()
   {

#ifdef WINDOWSEX

      ::oswindow oswindow = m_oswindow;

      if(!::IsWindow(oswindow))
         return;

      ::oswindow oswindowChild = ::GetTopWindow(oswindow);

      while(oswindowChild != NULL)
      {
         m_oswindowtreea.add_new();
         oswindow_tree & oswindowtreeChild = m_oswindowtreea.last_element();
         oswindowtreeChild.m_oswindow = oswindowChild;
         oswindowtreeChild.m_dwUser = 0;
         oswindowtreeChild.m_pvoidUser = NULL;
         oswindowtreeChild.EnumDescendants();
         oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);
      }

#else

      throw not_implemented(::ca::get_thread_app());

#endif

   }

   void oswindow_tree::Array::EnumDescendants()
   {
      for(int i = 0; i < this->get_size(); i++)
      {
         this->element_at(i).EnumDescendants();
      }
   }

   oswindow_tree::oswindow_tree()
   {
      m_oswindow = ::ca::null();
   }
   oswindow_tree::oswindow_tree(const oswindow_tree & tree)
   {
      operator =(tree);
   }


   oswindow_tree & oswindow_tree::operator = (const oswindow_tree & tree)
   {
      m_oswindow = tree.m_oswindow;
      m_oswindowtreea.copy(tree.m_oswindowtreea);
      return * this;
   }

   int oswindow_tree::compare_oswindow(oswindow_tree &tree1, oswindow_tree &tree2)
   {
      return (int) ((byte *) (void *) tree1.m_oswindow - (byte *) (void *) tree2.m_oswindow);
   }

   int_ptr oswindow_tree::Array::find(oswindow oswindow)
   {
      oswindow_tree tree;
      tree.m_oswindow = oswindow;
      return array_ptr_alloc < oswindow_tree, const oswindow_tree & >::find_first(tree, oswindow_tree::compare_oswindow);
   }


   bool oswindow_tree::Array::remove(oswindow oswindow)
   {
      if(oswindow == NULL)
         return true;
      int i;
      for(i = 0; i < this->get_size();)
      {
         oswindow_tree & tree = this->element_at(i);
         if(tree.m_oswindow == oswindow)
         {
            remove_at(i);
            return true;
         }
         else
         {
            i++;
         }
      }
      for(i = 0; i < this->get_size(); i++)
      {
         oswindow_tree & tree = this->element_at(i);
         if(tree.m_oswindowtreea.remove(oswindow))
         {
            return true;
         }
      }
      return false;
   }

   oswindow_tree::Array &
      oswindow_tree::Array::
      operator = (oswindow_array & oswindowa)
   {
      remove_all();
      oswindow_tree oswindowtree;
      for(int i = 0; i < oswindowa.get_size(); i++)
      {
         oswindowtree.m_oswindow = oswindowa[i];
         add(oswindowtree);
      }
      return * this;
   }



   // This function get all child windows of
   // the ::ca::window pointed by pwnd and add it
   // to the ::ca::window base_array wndpa. The top
   // windows come first in the enumeration.

   /*void window_util::EnumChildren(::ca::window * pwnd, interaction_ptr_array & wndpa)
   {
   if(!::IsWindow(pwnd->GetSafeoswindow_()))
   return;
   ::ca::window * pwndChild = pwnd->GetTopWindow();
   while(pwndChild != NULL)
   {
   wndpa.add(pwndChild);
   pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
   }
   }*/

   // This function get all child windows of
   // the ::ca::window pointed by pwnd and add it
   // to the ::ca::window base_array wndpa. The top
   // windows come first in the enumeration.

   void window_util::EnumChildren(oswindow oswindow, oswindow_array & oswindowa)
   {

#ifdef WINDOWSEX

      if(!::IsWindow(oswindow))
         return;

      ::oswindow oswindowChild = ::GetTopWindow(oswindow);

      while(oswindowChild != NULL)
      {

         oswindowa.add(oswindowChild);

         oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);

      }

#else

      throw todo(::ca::get_thread_app());

#endif

   }


   // This function sort the ::ca::window base_array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings,
   // have a permanent ::ca::window associated object
   // and that all ::ca::window base_array pointers
   // are pointers to permanent objects.
   /*void window_util::SortByZOrder(interaction_ptr_array & wndpa)
   {
   if(wndpa.get_size() <= 0)
   return;

   interaction_ptr_array wndpa2;

   ::ca::window * pwnd = wndpa[0];

   ::ca::window * pwndChild = pwnd->GetWindow(GW_HWNDFIRST);

   while(pwndChild != NULL)
   {
   pwnd = ::ca::window::FromHandlePermanent(pwndChild->GetSafeoswindow_());
   if(pwnd == NULL)
   {
   wndpa2.add(pwnd);
   }
   pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
   }

   interaction_ptr_array wndpa3;
   interaction_ptr_array wndpa4;

   int i;
   for( i = 0; i < wndpa2.get_size(); i++)
   {
   if(wndpa.find_first(wndpa2[i]) >= 0)
   {
   wndpa3.add(wndpa2[i]);
   }
   }

   for(i = 0; i < wndpa.get_size(); i++)
   {
   if(wndpa3.find_first(wndpa[i]) < 0)
   {
   wndpa3.add(wndpa[i]);
   }
   }

   wndpa.copy(wndpa3);

   }*/

   /*void window_util::ExcludeChild(interaction_ptr_array & wndpa)
   {

   for(int i = 0; i < wndpa.get_size();)
   {
   if(wndpa[i]->get_parent() != NULL)
   {
   wndpa.remove_at(i);
   }
   else
   {
   i++;
   }
   }

   }
   */



   void window_util::ContraintPosToParent(oswindow oswindow)
   {
#ifndef METROWIN
      rect rectMajor;
      ::oswindow oswindowParent = ::GetParent(oswindow);
      if(oswindowParent == NULL)
      {

#ifdef WINDOWSEX

         rectMajor.left = 0;
         rectMajor.top = 0;
         rectMajor.right = GetSystemMetrics(SM_CXSCREEN);
         rectMajor.bottom = GetSystemMetrics(SM_CYSCREEN);

#else

         throw todo(::ca::get_thread_app());

#endif

      }
      else
      {
         ::GetClientRect(oswindowParent, rectMajor);
      }

      rect rect;
      ::GetClientRect(oswindow, rect);

#ifdef WINDOWSEX

      ::ClientToScreen(oswindow, &rect.top_left());

      ::ClientToScreen(oswindow, &rect.bottom_right());

      if(oswindowParent != NULL)
      {

         ::ScreenToClient(oswindowParent, &rect.top_left());

         ::ScreenToClient(oswindowParent, &rect.bottom_right());

      }

#else

      throw todo(::ca::get_thread_app());

#endif

      bool bModified = false;

      if(rect.left > rectMajor.right)
      {
         rect.offset(- rect.width() - (rect.left - rectMajor.right), 0);
         bModified = true;
      }
      if(rect.right < rectMajor.left)
      {
         rect.offset(rect.width() + (rectMajor.left - rect.right), 0);
         bModified = true;
      }
      if(rect.top > rectMajor.bottom)
      {
         rect.offset(0, - rect.height() - (rect.top - rectMajor.bottom));
         bModified = true;
      }

      if(rect.bottom < rectMajor.top)
      {

         rect.offset(0, rect.height() + (rectMajor.top - rect.bottom));

         bModified = true;

      }


#ifdef WINDOWS

      if(bModified)
      {

         ::SetWindowPos(oswindow, HWND_TOP, rect.left, rect.top, rect.width(), rect.height(), 0);

      }

#else

      throw todo(::ca::get_thread_app());

#endif
#endif

   }

   /*void window_util::SendMessageToDescendants(oswindow oswindow, UINT message,
   WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm)
   {
   // walk through HWNDs to avoid creating temporary ::ca::window objects
   // unless we need to call this function recursively
   for (oswindow oswindow_Child = ::GetTopWindow; oswindow_Child != NULL;
   oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
   {
   // if bOnlyPerm is TRUE, don't send to non-permanent windows
   if (bOnlyPerm)
   {
   ::ca::window* pWnd = ::ca::window::FromHandlePermanent(oswindow_Child);
   if (pWnd != NULL)
   {
   // call ::ca::window proc directly since it is a C++ ::ca::window
   __call_window_procedure(pWnd, pWnd->m_oswindow_, message, wParam, lParam);
   }
   }
   else
   {
   // send message with Windows SendMessage API
   ::SendMessage(oswindow_Child, message, wParam, lParam);
   }
   if (bDeep && ::GetTopWindow(oswindow_Child) != NULL)
   {
   // send to child windows after parent
   SendMessageToDescendants(oswindow_Child, message, wParam, lParam,
   bDeep, bOnlyPerm);
   }
   }
   }*/


   void window_util::SendMessageToDescendants(oswindow oswindow, UINT message, WPARAM wParam, LPARAM lParam, bool bDeep)
   {

#if defined(WINDOWSEX)

      // walk through HWNDs to avoid creating temporary ::ca::window objects
      // unless we need to call this function recursively
      for(::oswindow oswindow_Child = ::GetTopWindow(oswindow); oswindow_Child != NULL; oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
      {
         // send message with Windows SendMessage API
         try
         {
            ::SendMessage(oswindow_Child, message, wParam, lParam);
         }
         catch(...)
         {
         }

         if (bDeep && ::GetTopWindow(oswindow_Child) != NULL)
         {

            // send to child windows after parent
            try
            {

               SendMessageToDescendants(oswindow_Child, message, wParam, lParam, bDeep);

            }
            catch(...)
            {

            }

         }

      }

#else

      throw todo(::ca::get_thread_app());

#endif

   }

   // This function sort the ::ca::window base_array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings
   void window_util::SortByZOrder(oswindow_array & oswindowa)
   {
      if(oswindowa.get_size() <= 0)
         return;

      int_array ia1;
      int_array ia2;
      oswindow oswindowSwap;

      for(int i = 0; i < oswindowa.get_size(); i++)
      {
         for(int j = i + 1; j < oswindowa.get_size(); j++)
         {
            try
            {
               GetZOrder(oswindowa[i], ia1);
               GetZOrder(oswindowa[j], ia2);
               if(ia1.Cmp(ia2) > 0)
               {
                  oswindowSwap = oswindowa[i];
                  oswindowa[i] = oswindowa[j];
                  oswindowa[j] = oswindowSwap;
               }
            }
            catch(...)
            {
            }
         }
      }

   }

   int window_util::GetZOrder(oswindow oswindow)
   {
#if !defined(METROWIN) && !defined(LINUX)
//      int iOrder = 0;
      ::oswindow oswindowOrder = ::ca::null();
      try
      {
         oswindowOrder = ::GetWindow(oswindow, GW_HWNDFIRST);
      }
      catch(...)
      {
         return 0x7fffffff;
      }

#ifdef WINDOWS

      int iOrder = 0;

      while(oswindowOrder != NULL && ::IsWindow(oswindowOrder))
      {

         if(oswindow == oswindowOrder)
            return iOrder;

         oswindowOrder = ::GetWindow(oswindowOrder, GW_HWNDNEXT);

         iOrder++;

      }

#else

      throw todo(::ca::get_thread_app());

#endif
#endif

      return 0x7fffffff;


   }


   void window_util::GetZOrder(oswindow oswindow, int_array & ia)
   {

#ifdef WINDOWSEX

      int iOrder;
      ia.remove_all();
      while(true)
      {
         if(oswindow == NULL || !::IsWindow(oswindow))
            break;
         iOrder = GetZOrder(oswindow);
         if(iOrder == 0x7fffffff)
            break;
         ia.insert_at(0, iOrder);
         oswindow = ::GetParent(oswindow);
      }

#else

      throw todo(::ca::get_thread_app());

#endif

   }

   /*void window_util::EnumChildren(oswindow oswindow, oswindow_array & oswindowa)
   {
   if(!::IsWindow)
   return;
   oswindow oswindowChild = ::GetTopWindow;
   while(oswindowChild != NULL)
   {
   oswindowa.add(oswindowChild);
   oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);
   }
   }*/

   void window_util::ExcludeChildren(oswindow oswindow, HRGN hrgn, POINT ptOffset)
   {


#ifdef WINDOWSEX

      oswindow_array oswindowa;

      EnumChildren(oswindow, oswindowa);

      for(int i = 0; i < oswindowa.get_size(); i++)
      {

         ::oswindow oswindowChild = oswindowa[i];

         rect rectChild;

         ::GetClientRect(oswindowChild, rectChild);

         ::ClientToScreen(oswindowChild, &rectChild.top_left());

         ::ClientToScreen(oswindowChild, &rectChild.bottom_right());

         ::ScreenToClient(oswindow, &rectChild.top_left());

         ::ScreenToClient(oswindow, &rectChild.bottom_right());

         rectChild.offset(ptOffset);

         HRGN hrgnChild = ::CreateRectRgnIndirect(rectChild);

         ::CombineRgn(hrgn, hrgn, hrgnChild, ::ca::region::combine_exclude);

         ::DeleteObject(hrgnChild);

      }

#else

      throw todo(::ca::get_thread_app());

#endif


   }

   HRGN window_util::GetAClipRgn(oswindow oswindow, POINT ptOffset, bool bExludeChildren)
   {

#ifdef WINDOWSEX

      rect rectWnd;

      ::GetClientRect(oswindow, rectWnd);

      rectWnd.offset(ptOffset);

      HRGN hrgn = ::CreateRectRgnIndirect(rectWnd);

      if(bExludeChildren)
      {

         ExcludeChildren(oswindow, hrgn, ptOffset);

      }

      return hrgn;

#else

      throw todo(::ca::get_thread_app());

#endif

   }

   bool window_util::IsAscendant(oswindow oswindowAscendant, oswindow oswindowDescendant)
   {
#ifndef METROWIN
      while(true)
      {
         oswindowDescendant = ::GetParent(oswindowDescendant);
         if(oswindowDescendant == NULL)
            return false;
         if(oswindowDescendant == oswindowAscendant)
            return true;
      }
#else
      return false;
#endif
   }

   ::user::interaction * interaction_ptr_array::find_first(::ca::type_info info)
   {
      for(int i = 0; i < this->get_size(); i++)
      {
         if(typeid(*this->element_at(i)).name() == info.name())
         {
            return this->element_at(i);
         }
      }
      return NULL;
   }

   ::user::interaction * interaction_ptr_array::find_first(oswindow oswindow)
   {

      for(int i = 0; i < this->get_size(); i++)
      {

         if(this->element_at(i)->get_safe_handle() == oswindow)
         {

            return this->element_at(i);
         }
      }

      return NULL;

   }

   void interaction_ptr_array::get_wnda(oswindow_array & oswindowa)
   {
      oswindowa.remove_all();
      for(int i = 0; i < this->get_size(); i++)
      {
         oswindowa.add(this->element_at(i)->get_handle());
      }
   }

   void interaction_ptr_array::send_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
      for(int i = 0; i < this->get_size(); i++)
      {
         try
         {
            this->element_at(i)->send_message(uiMessage, wparam, lparam);
         }
         catch(...)
         {
         }
      }
   }

   void interaction_ptr_array::send_message_to_descendants(UINT uiMessage, WPARAM wparam, LPARAM lparam, bool bRecursive)
   {
      for(int i = 0; i < this->get_size(); i++)
      {
         try
         {
            this->element_at(i)->SendMessageToDescendants(uiMessage, wparam, lparam, bRecursive);
         }
         catch(...)
         {
         }
      }
   }

} // namespace user
