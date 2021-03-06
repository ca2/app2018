#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"

#ifdef APPLEIOS
oswindow GetWindow(oswindow window, int iParentHood);
#endif


#ifdef WINDOWSEX


#define MESSAGE_WINDOW_PARENT HWND_MESSAGE


#endif


namespace user
{


   void oswindow_array::SortByZOrder()
   {

      window_util::SortByZOrder(*this);

   }


   // This function sort the interaction_impl array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings
   void oswindow_array::SortSiblingsByZOrder()
   {

      oswindow oswindowSwap;

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         for (int32_t j = i + 1; j < this->get_size(); j++)
         {

            if (window_util::GetZOrder(this->element_at(i)) > window_util::GetZOrder(this->element_at(j)))
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

      int32_t iOrder = 0;
      oswindow oswindowOrder = ::get_desktop_window();
      oswindowOrder = ::GetWindow(oswindowOrder, GW_CHILD);
      while (oswindowOrder != NULL
             && ::is_window(oswindowOrder))
      {
         add(oswindowOrder);
         oswindowOrder = ::GetWindow(oswindowOrder, GW_HWNDNEXT);
         iOrder++;
      }

#else

      ::exception::throw_not_implemented(get_app());

#endif

   }


   window_util::window_util()
   {

   }

   window_util::~window_util()
   {

   }

   // This function sort the interaction_impl array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings,
   // have a permanent interaction_impl associated object
   // and that all interaction_impl array pointers
   // are pointers to permanent objects.
   /*void window_util::SortByZOrder(Carray < ::window_sp, ::window_sp > & wndpa)
   {
   if(wndpa.get_size() <= 0)
   return;

   Carray < ::window_sp, ::window_sp > wndpa2;

   ::window_sp pwnd = wndpa[0];

   ::window_sp pwndChild = pwnd->GetWindow(GW_HWNDFIRST);

   while(pwndChild != NULL
   && ::is_window(pwndChild->GetSafeoswindow_()))
   {
   pwnd = interaction_impl::FromHandlePermanent(pwndChild->GetSafeoswindow_());
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

   Carray < ::window_sp, ::window_sp > wndpa3;
   Carray < ::window_sp, ::window_sp > wndpa4;

   for(int32_t i = 0; i < wndpa2.get_size(); i++)
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

   // This function sort the interaction_impl array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings,
   // have a permanent interaction_impl associated object
   // and that all interaction_impl array pointers
   // are pointers to permanent objects.
   /*void window_util::SortByZOrder(Carray < oswindow, oswindow > & oswindowa)
   {
   if(oswindowa.get_size() <= 0)
   return;

   Carray < oswindow, oswindow > oswindowa2;

   oswindow oswindow = oswindowa[0];

   oswindow oswindowChild = ::GetWindow(oswindow, GW_HWNDFIRST);

   while(oswindowChild != NULL
   && ::is_window(oswindowChild))
   {
   oswindowa2.add(oswindowChild);
   oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);
   }

   Carray < oswindow, oswindow > oswindowa3;
   Carray < oswindow, oswindow > oswindowa4;

   for(int32_t i = 0; i < oswindowa2.get_size(); i++)
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
   if(!::is_window)
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

   for(int32_t i = 0; i < oswindowa.get_size(); i++)
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
   ::CombineRgn(hrgn, hrgn, hrgnChild, ::draw2d::region::combine_exclude);
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

      if (!::is_window(oswindow))
         return;

      ::oswindow oswindowChild = ::GetTopWindow(oswindow);

      while (oswindowChild != NULL)
      {
         m_oswindowtreea.add_new();
         oswindow_tree & oswindowtreeChild = m_oswindowtreea.last();
         oswindowtreeChild.m_oswindow = oswindowChild;
         oswindowtreeChild.m_dwUser = 0;
         oswindowtreeChild.m_pvoidUser = NULL;
         oswindowtreeChild.EnumDescendants();
         oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);
      }

#else

      _throw(not_implemented(get_app()));

#endif

   }


   void oswindow_tree::Array::EnumDescendants()
   {

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         this->element_at(i)->EnumDescendants();

      }

   }


   oswindow_tree::oswindow_tree()
   {

      m_oswindow = NULL;

   }


   oswindow_tree::oswindow_tree(oswindow interaction_impl) :
      m_oswindow(interaction_impl)
   {

   }


   oswindow_tree::oswindow_tree(const oswindow_tree & tree)
   {
      operator =(tree);
   }


   oswindow_tree & oswindow_tree::operator = (const oswindow_tree & tree)
   {
      m_oswindow = tree.m_oswindow;
      m_oswindowtreea.copy(&tree.m_oswindowtreea);
      return *this;
   }

   index oswindow_tree::compare_oswindow(const ::user::oswindow_tree * ptree1, const ::user::oswindow_tree * ptree2)
   {
      return (index)((byte *)(void *)ptree1->m_oswindow - (byte *)(void *)ptree2->m_oswindow);
   }


   int_ptr oswindow_tree::Array::find(oswindow oswindow)
   {

      return comp_find_first(oswindow_tree(oswindow), &::user::oswindow_tree::compare_oswindow);

   }


   bool oswindow_tree::Array::remove(oswindow oswindow)
   {
      if (oswindow == NULL)
         return true;
      int32_t i;
      for (i = 0; i < this->get_size();)
      {
         oswindow_tree & tree = *this->element_at(i);
         if (tree.m_oswindow == oswindow)
         {
            remove_at(i);
            return true;
         }
         else
         {
            i++;
         }
      }
      for (i = 0; i < this->get_size(); i++)
      {
         oswindow_tree & tree = *this->element_at(i);
         if (tree.m_oswindowtreea.remove(oswindow))
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
      for (int32_t i = 0; i < oswindowa.get_size(); i++)
      {
         oswindowtree.m_oswindow = oswindowa[i];
         add(new oswindow_tree(oswindowtree));
      }
      return *this;
   }



   // This function get all child windows of
   // the interaction_impl pointed by pwnd and add it
   // to the interaction_impl array wndpa. The top
   // windows come first in the enumeration.

   /*void window_util::EnumChildren(::window_sp pwnd, interaction_spa & wndpa)
   {
   if(!::is_window(pwnd->GetSafeoswindow_()))
   return;
   ::window_sp pwndChild = pwnd->GetTopWindow();
   while(pwndChild != NULL)
   {
   wndpa.add(pwndChild);
   pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
   }
   }*/

   // This function get all child windows of
   // the interaction_impl pointed by pwnd and add it
   // to the interaction_impl array wndpa. The top
   // windows come first in the enumeration.

   void window_util::EnumChildren(oswindow oswindow, oswindow_array & oswindowa)
   {

#ifdef WINDOWSEX

      if (!::is_window(oswindow))
         return;

      ::oswindow oswindowChild = ::GetTopWindow(oswindow);

      while (oswindowChild != NULL)
      {

         oswindowa.add(oswindowChild);

         oswindowChild = ::GetWindow(oswindowChild, GW_HWNDNEXT);

      }

#else

      _throw(todo(get_app()));

#endif

   }


   // This function sort the interaction_impl array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings,
   // have a permanent interaction_impl associated object
   // and that all interaction_impl array pointers
   // are pointers to permanent objects.
   /*void window_util::SortByZOrder(interaction_spa & wndpa)
   {
   if(wndpa.get_size() <= 0)
   return;

   interaction_spa wndpa2;

   ::window_sp pwnd = wndpa[0];

   ::window_sp pwndChild = pwnd->GetWindow(GW_HWNDFIRST);

   while(pwndChild != NULL)
   {
   pwnd = interaction_impl::FromHandlePermanent(pwndChild->GetSafeoswindow_());
   if(pwnd == NULL)
   {
   wndpa2.add(pwnd);
   }
   pwndChild = pwndChild->GetWindow(GW_HWNDNEXT);
   }

   interaction_spa wndpa3;
   interaction_spa wndpa4;

   int32_t i;
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

   /*void window_util::ExcludeChild(interaction_spa & wndpa)
   {

   for(int32_t i = 0; i < wndpa.get_size();)
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
      //#if !defined(METROWIN) && !defined(APPLE_IOS)
      //      rect rectMajor;
      //      ::oswindow oswindowParent = ::GetParent(oswindow);
      //      if(oswindowParent == NULL)
      //      {
      //
      //#ifdef WINDOWSEX
      //
      //         rectMajor.left = 0;
      //         rectMajor.top = 0;
      //         rectMajor.right = GetSystemMetrics(SM_CXSCREEN);
      //         rectMajor.bottom = GetSystemMetrics(SM_CYSCREEN);
      //
      //#else
      //
      //         _throw(todo(get_app()));
      //
      //#endif
      //
      //      }
      //      else
      //      {
      //         ::GetClientRect(oswindowParent, rectMajor);
      //      }
      //
      //      rect rect;
      //      ::GetClientRect(oswindow, rect);
      //
      //#ifdef WINDOWSEX
      //
      //      ::ClientToScreen(oswindow, &rect.top_left());
      //
      //      ::ClientToScreen(oswindow, &rect.bottom_right());
      //
      //      if(oswindowParent != NULL)
      //      {
      //
      //         ::ScreenToClient(oswindowParent, &rect.top_left());
      //
      //         ::ScreenToClient(oswindowParent, &rect.bottom_right());
      //
      //      }
      //
      //#else
      //
      //      _throw(todo(get_app()));
      //
      //#endif
      //
      //      bool bModified = false;
      //
      //      if(rect.left > rectMajor.right)
      //      {
      //         rect.offset(- rect.width() - (rect.left - rectMajor.right), 0);
      //         bModified = true;
      //      }
      //      if(rect.right < rectMajor.left)
      //      {
      //         rect.offset(rect.width() + (rectMajor.left - rect.right), 0);
      //         bModified = true;
      //      }
      //      if(rect.top > rectMajor.bottom)
      //      {
      //         rect.offset(0, - rect.height() - (rect.top - rectMajor.bottom));
      //         bModified = true;
      //      }
      //
      //      if(rect.bottom < rectMajor.top)
      //      {
      //
      //         rect.offset(0, rect.height() + (rectMajor.top - rect.bottom));
      //
      //         bModified = true;
      //
      //      }
      //
      //
      //#ifdef WINDOWS
      //
      //      if(bModified)
      //      {
      //
      //         ::SetWindowPos(oswindow, HWND_TOP, rect.left, rect.top, rect.width(), rect.height(), 0);
      //
      //      }
      //
      //#else
      //
      //      _throw(todo(get_app()));
      //
      //#endif
      //#endif

   }

   /*void window_util::send_message_to_descendants(oswindow oswindow, UINT message,
   WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm)
   {
   // walk through HWNDs to avoid creating temporary interaction_impl objects
   // unless we need to call this function recursively
   for (oswindow oswindow_Child = ::GetTopWindow; oswindow_Child != NULL;
   oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
   {
   // if bOnlyPerm is TRUE, don't send to non-permanent windows
   if (bOnlyPerm)
   {
   ::window_sp pwindow = interaction_impl::FromHandlePermanent(oswindow_Child);
   if (pwindow != NULL)
   {
   // call interaction_impl proc directly since it is a C++ interaction_impl
   __call_window_procedure(pwindow, pwindow->m_oswindow_, message, wParam, lParam);
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
   send_message_to_descendants(oswindow_Child, message, wParam, lParam,
   bDeep, bOnlyPerm);
   }
   }
   }*/


   void window_util::send_message_to_descendants(oswindow oswindow, UINT message, WPARAM wParam, LPARAM lParam, bool bDeep)
   {

#if defined(WINDOWSEX)

      // walk through HWNDs to avoid creating temporary interaction_impl objects
      // unless we need to call this function recursively
      for (::oswindow oswindow_Child = ::GetTopWindow(oswindow); oswindow_Child != NULL; oswindow_Child = ::GetNextWindow(oswindow_Child, GW_HWNDNEXT))
      {
         // send message with Windows SendMessage API
         try
         {
            ::SendMessage(oswindow_Child, message, wParam, lParam);
         }
         catch (...)
         {
         }

         if (bDeep && ::GetTopWindow(oswindow_Child) != NULL)
         {

            // send to child windows after parent
            try
            {

               send_message_to_descendants(oswindow_Child, message, wParam, lParam, bDeep);

            }
            catch (...)
            {

            }

         }

      }

#else

      _throw(todo(get_app()));

#endif

   }

   // This function sort the interaction_impl array
   // by ascending z order.

   // This implementation relays in the fact
   // that all windows are siblings
   void window_util::SortByZOrder(oswindow_array & oswindowa)
   {
      if (oswindowa.get_size() <= 0)
         return;

      int_array ia1;
      int_array ia2;
      oswindow oswindowSwap;

      for (int32_t i = 0; i < oswindowa.get_size(); i++)
      {
         for (int32_t j = i + 1; j < oswindowa.get_size(); j++)
         {
            try
            {
               GetZOrder(oswindowa[i], ia1);
               GetZOrder(oswindowa[j], ia2);
               if (ia1.Cmp(ia2) > 0)
               {
                  oswindowSwap = oswindowa[i];
                  oswindowa[i] = oswindowa[j];
                  oswindowa[j] = oswindowSwap;
               }
            }
            catch (...)
            {
            }
         }
      }

   }

   int32_t window_util::GetZOrder(oswindow oswindow)
   {

#ifdef METROWIN

      return 0;

#else

      ::oswindow oswindowOrder = NULL;

      if (!::is_window(oswindow))
         return 0x7fffffff;

      if (::GetParent(oswindow) == MESSAGE_WINDOW_PARENT)
         return 0x7fffffff;

      try
      {

         oswindowOrder = ::get_window(oswindow, GW_HWNDFIRST);

      }
      catch (...)
      {

         return 0x7fffffff;

      }

      int32_t iOrder = 0;

      while (oswindowOrder != NULL && ::is_window(oswindowOrder))
      {

         if (oswindow == oswindowOrder)
            return iOrder;

         oswindowOrder = ::get_window(oswindowOrder, GW_HWNDNEXT);

         iOrder++;

      }

      return 0x7fffffff;

#endif

   }


   void window_util::GetZOrder(oswindow oswindow, int_array & ia)
   {

      if (!is_window(oswindow))
      {

         return;

      }

      int32_t iOrder;
      ia.remove_all();
      while (true)
      {

         if (oswindow == NULL || !::is_window(oswindow))
         {

            break;

         }

         iOrder = GetZOrder(oswindow);
         if (iOrder == 0x7fffffff)
            break;
         ia.insert_at(0, iOrder);
         oswindow = ::GetParent(oswindow);
      }


   }

   /*void window_util::EnumChildren(oswindow oswindow, oswindow_array & oswindowa)
   {
   if(!::is_window)
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

      for (int32_t i = 0; i < oswindowa.get_size(); i++)
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

         ::CombineRgn(hrgn, hrgn, hrgnChild, ::draw2d::region::combine_exclude);

         ::DeleteObject(hrgnChild);

      }

#else

      _throw(todo(get_app()));

#endif


   }

   HRGN window_util::GetAClipRgn(oswindow oswindow, POINT ptOffset, bool bExludeChildren)
   {

#ifdef WINDOWSEX

      rect rectWnd;

      ::GetClientRect(oswindow, rectWnd);

      rectWnd.offset(ptOffset);

      HRGN hrgn = ::CreateRectRgnIndirect(rectWnd);

      if (bExludeChildren)
      {

         ExcludeChildren(oswindow, hrgn, ptOffset);

      }

      return hrgn;

#else

      _throw(todo(get_app()));

#endif

   }

   bool window_util::IsAscendant(oswindow oswindowAscendant, oswindow oswindowDescendant)
   {
#ifndef METROWIN
      while (true)
      {
         oswindowDescendant = ::GetParent(oswindowDescendant);
         if (oswindowDescendant == NULL)
            return false;
         if (oswindowDescendant == oswindowAscendant)
            return true;
      }
#else
      return false;
#endif
   }








   /*
      interaction_ptra::interaction_ptra()
      {

      }


      interaction_ptra::interaction_ptra(const interaction_ptra & a)
      {

         this->operator = (a);

      }

      interaction_ptra::interaction_ptra(const ref_array < ::user::interaction > & a)
      {

         this->operator = (a);

      }*/

   //interaction_ptra::interaction_ptra(const interaction_spa & a)
   //{

   //   for(index i = 0; i < a.get_count(); i++)
   //   {

   //      add(a[i]);

   //   }

   //}

   interaction_ptra & interaction_ptra::operator=(const interaction_spa & a)
   {

      for (index i = 0; i < a.get_count(); i++)
      {

         add(a[i]);

      }

      return *this;

   }


   /*   interaction_ptra::interaction_ptra(interaction_ptra && a)
      {

         this->operator = (a);

      }


      interaction_ptra & interaction_ptra::operator = (const interaction_ptra & a)
      {

         ref_array < ::user::interaction >::operator = (a);

         return *this;

      }


      interaction_ptra & interaction_ptra::operator = (const ref_array < ::user::interaction > & a)
      {

         ref_array < ::user::interaction >::operator = (a);

         return *this;

      }


      interaction_ptra & interaction_ptra::operator = (interaction_ptra && a)
      {

         ref_array < ::user::interaction >::operator = (a);

         return *this;

      }*/


   oswindow_array interaction_ptra::get_hwnda()
   {

      oswindow_array oswindowa;

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         oswindowa.add(this->element_at(i)->get_handle());

      }

      return oswindowa;

   }


   ::user::interaction * interaction_ptra::find_first_typed(sp(type) info)
   {

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         ::user::interaction * pui = this->element_at(i);

         if (typeid(*pui).name() == info->name())
         {

            return pui;

         }

      }

      return NULL;

   }


   ::user::interaction * interaction_ptra::find_first(oswindow oswindow)
   {

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         if (this->element_at(i)->get_safe_handle() == oswindow)
         {

            return this->element_at(i);
         }
      }

      return NULL;

   }






















































   interaction_spa::interaction_spa()
   {

   }



   interaction_spa::interaction_spa(::aura::application * papp) :
      ::object(papp)
   {

   }

   interaction_spa::interaction_spa(const pointer_array < ::user::interaction * > & ptra)
   {

      for (index i = 0; i < ptra.get_count(); i++)
      {

         try
         {

            add(ptra[i]);

         }
         catch (...)
         {

         }

      }

   }


   sp(::user::interaction) interaction_spa::find_first_typed(sp(type) info)
   {

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         ::user::interaction * pui = this->element_at(i);

         if (typeid(*pui).name() == info->name())
         {

            return this->element_at(i);

         }

      }

      return NULL;

   }

   sp(::user::interaction) interaction_spa::find_first(oswindow oswindow)
   {

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         if (this->element_at(i)->get_safe_handle() == oswindow)
         {

            return this->element_at(i);
         }
      }

      return NULL;

   }


   interaction_spa & interaction_spa::operator = (const spa(::user::interaction) & a)
   {

      copy(a);

      return *this;

   }


   interaction_spa & interaction_spa::operator = (const interaction_spa & a)
   {

      copy(a);

      return *this;

   }



   oswindow_array interaction_spa::get_hwnda()
   {

      oswindow_array oswindowa;

      for (int32_t i = 0; i < this->get_size(); i++)
      {

         oswindowa.add(this->element_at(i)->get_handle());

      }

      return oswindowa;

   }


   void interaction_spa::send_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
      for (int32_t i = 0; i < this->get_size(); i++)
      {
         try
         {
            this->element_at(i)->send_message(uiMessage, wparam, lparam);
         }
         catch (...)
         {
         }
      }
   }

   void interaction_spa::send_message_to_descendants(UINT uiMessage, WPARAM wparam, LPARAM lparam, bool bRecursive)
   {
      for (int32_t i = 0; i < this->get_size(); i++)
      {
         try
         {
            this->element_at(i)->send_message_to_descendants(uiMessage, wparam, lparam, bRecursive);
         }
         catch (...)
         {
         }
      }
   }

   bool interaction_spa::get_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      if (get_count() <= 0)
      {

         return false;

      }

      if (pui.is_null())
      {

         pui = element_at(0);

         return true;

      }
      else
      {

         for (index i = get_upper_bound(); i >= 0; i--)
         {

            if (element_at(i) == pui)
            {

               i++;

               if (i < get_count())
               {

                  pui = element_at(i);

                  return true;

               }
               else
               {

                  return false;

               }

            }

         }

      }

      return false;

   }


   bool interaction_spa::rget_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      if (get_count() <= 0)
      {

         return false;

      }

      if (pui == NULL)
      {

         pui = last_sp();

         return true;

      }
      else
      {

         for (index i = 0; i < get_size(); i++)
         {

            if (element_at(i) == pui)
            {

               i--;

               if (i >= 0)
               {

                  pui = element_at(i);

                  return true;

               }
               else
               {

                  return false;

               }

            }

         }

      }

      return false;

   }


   bool interaction_ptra::get_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      if (get_count() <= 0)
      {

         return false;

      }

      if (pui == NULL)
      {

         pui = element_at(0);

         return true;

      }
      else
      {

         for (index i = get_upper_bound(); i >= 0; i--)
         {

            if (element_at(i) == pui)
            {

               i++;

               if (i < get_count())
               {

                  pui = element_at(i);

                  return true;

               }
               else
               {

                  return false;

               }

            }

         }

      }

      return false;

   }

   bool interaction_ptra::rget_child(sp(::user::interaction) & pui)
   {

      synch_lock sl(m_pmutex);

      if (get_count() <= 0)
      {

         return false;

      }

      if (pui == NULL)
      {

         pui = last();

         return true;

      }
      else
      {

         for (index i = 0; i < get_size(); i++)
         {

            if (element_at(i) == pui)
            {

               i--;

               if (i >= 0)
               {

                  pui = element_at(i);

                  return true;

               }
               else
               {

                  return false;

               }

            }

         }

      }

      return false;

   }
   CLASS_DECL_AURA bool is_docking_appearance(::user::e_appearance eappearance)
   {
      return eappearance == ::user::appearance_left
             || eappearance == ::user::appearance_top
             || eappearance == ::user::appearance_right
             || eappearance == ::user::appearance_bottom
             || eappearance == ::user::appearance_top_left
             || eappearance == ::user::appearance_top_right
             || eappearance == ::user::appearance_bottom_right
             || eappearance == ::user::appearance_bottom_left;
   }
} // namespace user






