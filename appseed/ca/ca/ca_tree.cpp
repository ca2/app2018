#include "framework.h"


namespace ca
{


   tree::tree(sp(::ca::application) papp) :
      ca(papp),
      data_container(papp)
   {

      m_pitem = allocate_item(::null());

      if(m_pitem == ::null())
         throw memory_exception(get_app());

      m_pitem->m_dwState |= ::ca::tree_item_state_expandable;

      m_pitem->m_dwState |= ::ca::tree_item_state_expanded;

      ::ca::simple_tree_item_data * pitemdata = new ::ca::simple_tree_item_data();

      m_pitem->m_pitemdata = pitemdata;

      pitemdata->m_str = "tree";


   }


   tree::~tree()
   {

   }


   sp(tree_data) tree::get_tree_data()
   {

      return ::ca::data_container::get_data();

   }


   sp(tree_item) tree::find(sp(tree_item_data) pitemdata, index i)
   {
      if(i < 0)
         return ::null();
      if(pitemdata == ::null())
         return ::null();
      if(pitemdata->get_tree_data() != get_data())
         return ::null();
      sp(::ca::tree_item) pitem = m_pitem;
      for(; pitem != ::null(); pitem = pitem->get_item(TreeNavigationExpandedForward))
      {
         if(pitem->m_pitemdata == pitemdata)
         {
            i--;
            if(i < 0)
               return pitem;
         }
      }
      return ::null();
   }

   bool tree::contains(sp(tree_item_data) pitemdata)
   {
      return find(pitemdata) != ::null();
   }

   bool tree::contains(sp(tree_item) pitemParam)
   {
      sp(::ca::tree_item) pitem = m_pitem;
      for(; pitem != ::null(); pitem = pitem->get_item(TreeNavigationExpandedForward))
      {
         if(pitem == pitemParam)
            return true;
      }
      return false;
   }


   void tree::_001OnTreeDataChange()
   {
   }

   bool tree::add_tree_item_data_selection(sp(tree_item_data) pitemdata, index i)
   {
      sp(tree_item) pitem = find(pitemdata, i);
      if(pitem == ::null())
         return false;
      return add_tree_item_selection(pitem);
   }

   ::count tree::add_tree_item_array_selection(tree_item_ptr_array & itemptra)
   {
      ::count count = 0;
      for(int32_t i = 0; i < itemptra.get_count(); i++)
      {
         if(add_tree_item_selection(itemptra(i)))
            count++;
      }
      return count;
   }

   bool tree::add_tree_item_selection(sp(tree_item) pitem)
   {
      if(!contains(pitem))
         return false;
      return m_itemptraSelected.add_unique(pitem);
   }

   bool tree::set_tree_item_data_selection(sp(tree_item_data) pitemdata, index i)
   {
      sp(tree_item) pitem = find(pitemdata, i);
      if(pitem == ::null())
         return false;
      return set_tree_item_selection(pitem);
   }

   ::count tree::set_tree_item_array_selection(tree_item_ptr_array & itemptra)
   {
      ::count count = 0;
      for(int32_t i = 0; i < itemptra.get_count(); i++)
      {
         if(contains(itemptra(i)) && m_itemptraSelected.add_unique(itemptra(i)))
            count++;
      }
      return count;
   }

   bool tree::set_tree_item_selection(sp(tree_item) pitem)
   {
      if(!contains(pitem))
         return false;
      bool bContains = m_itemptraSelected.contains(pitem);
      m_itemptraSelected.remove_all();
      m_itemptraSelected.add(pitem);
      return bContains;
   }

   bool tree::remove_tree_item_data_selection(sp(tree_item_data) pitemdata, index i)
   {
      sp(tree_item) pitem = find(pitemdata, i);
      if(pitem == ::null())
         return false;
      return remove_tree_item_selection(pitem);
   }

   ::count tree::remove_tree_item_array_selection(tree_item_ptr_array & itemptra)
   {
      ::count count = 0;
      for(int32_t i = 0; i < itemptra.get_count(); i++)
      {
         if(m_itemptraSelected.remove(itemptra(i)))
            count++;
      }
      return count;
   }

   bool tree::remove_tree_item_selection(sp(tree_item) pitem)
   {
      return m_itemptraSelected.remove(pitem) >= 0;
   }

   ::count tree::remove_tree_item_data(sp(tree_item_data) pitemdata, index i)
   {
      sp(tree_item) pitem = find(pitemdata, i);
      if(pitem == ::null())
         return false;
      return remove_tree_item(pitem);
   }

   ::count tree::remove_tree_item_array(tree_item_ptr_array & itemptra)
   {
      
      ::count c = 0;

      for(int32_t i = 0; i < itemptra.get_count(); i++)
      {

         c += remove_tree_item(itemptra(i));

      }

      return c;

   }

   ::count tree::remove_tree_item(sp(tree_item) pitem)
   {

      if(pitem->m_ptree == this)
         return pitem->remove_tree_item();

      return 0;

   }

   ::count tree::clear_selection()
   {
      return m_itemptraSelected.remove_all();
   }

   bool tree::hover_tree_item_data(sp(tree_item_data) pitemdata, index i)
   {
      sp(tree_item) pitem = find(pitemdata, i);
      return hover_tree_item(pitem);
   }

   bool tree::hover_tree_item(sp(tree_item) pitem)
   {
      if(pitem == ::null())
      {
         m_pitemHover = ::null();
         return true;
      }
      if(!contains(pitem))
         return false;
      return m_pitemHover == pitem;
   }

   bool tree::is_tree_item_selected(sp(tree_item) pitem)
   {
      return m_itemptraSelected.contains(pitem);
   }

   bool tree::is_tree_item_data_selected(sp(tree_item_data) pitemdata)
   {
      if(pitemdata == ::null())
         return false;
      if(pitemdata->get_tree_data() != get_data())
         return false;
      for(int32_t i = 0; i < m_itemptraSelected.get_count(); i++)
      {
         if(m_itemptraSelected[i].m_pitemdata == pitemdata)
            return true;
      }
      return false;
   }

   bool tree::is_tree_item_hover(sp(tree_item) pitem)
   {
      return pitem != ::null() && m_pitemHover == pitem;
   }

   bool tree::is_tree_item_data_hover(sp(tree_item_data) pitemdata)
   {
      if(pitemdata == ::null())
         return false;
      if(pitemdata->get_tree_data() != get_data())
         return false;
      if(m_pitemHover == ::null())
         return false;
      if(m_pitemHover->m_pitemdata != pitemdata)
         return false;
      return true;
   }

   sp(::ca::tree_item) tree::get_proper_item(index iIndex, index * piLevel)
   {
      if(*piLevel) *piLevel = 0;
      sp(::ca::tree_item) pitem = get_base_item();
      while(pitem != ::null() && iIndex >= 0)
      {
         pitem = pitem->get_item(TreeNavigationProperForward, piLevel);
         iIndex--;
      }
      return pitem;

   }

   index tree::get_proper_item_index(sp(::ca::tree_item) pitemParam, index * piLevel)
   {
      int32_t iIndex = 0;
      if(piLevel != ::null()) *piLevel = 0;
      sp(::ca::tree_item) pitem = get_base_item();
      while(pitem != ::null())
      {
         pitem = pitem->get_item(TreeNavigationProperForward, piLevel);
         if(pitem == pitemParam)
            return iIndex;
         iIndex++;
      }
      return -1;
   }

   ::count tree::get_proper_item_count()
   {
      return get_base_item()->get_proper_item_count();
   }


   sp(::ca::tree_item) tree::get_base_item()
   {
      return m_pitem;
   }

   sp(::ca::tree_item) tree::insert_item_data(sp(::ca::tree_data) pdata, sp(::ca::tree_item_data) pitemdataNew, ERelative erelativeNewItem, sp(::ca::tree_item) pitemRelative)
   {
      if(erelativeNewItem == RelativeReplace)
      {

         if(!contains(pitemRelative))
            return ::null();

         pitemRelative->m_pitemdata = pitemdataNew;

         return pitemRelative;

      }
      sp(::ca::tree_item) pitemNew = allocate_item(pdata);
      if(pitemNew == ::null())
         return ::null();
      if(!insert_item(pdata, pitemNew, erelativeNewItem, pitemRelative))
      {
         return ::null();
      }
      pitemNew->m_pitemdata = pitemdataNew;
      return pitemNew;
   }

   sp(::ca::tree_item) tree::create_item(sp(::ca::tree_data) pdata, sp(::ca::tree_item) pitemRelative, ERelative erelativeNewItem)
   {
      if(erelativeNewItem == RelativeReplace)
         return ::null();
      sp(::ca::tree_item) pitemNew = allocate_item(pdata);
      if(pitemNew == ::null())
         return ::null();
      if(!insert_item(pdata, pitemNew, erelativeNewItem, pitemRelative))
      {
         return ::null();
      }
      if(get_data() != ::null())
      {
         pitemNew->m_pitemdata = get_tree_data()->_allocate_item();
      }
      return pitemNew;
   }


   bool tree::insert_item(sp(::ca::tree_data) pdata, sp(::ca::tree_item) pitemNew, ERelative erelativeNewItem, sp(::ca::tree_item) pitemRelative)
   {
      if(pitemNew == ::null())
         return false;
      if(!contains(pitemRelative))
         return false;
      if(erelativeNewItem == RelativeParent)
         return false;

      if(erelativeNewItem == RelativeLastSibling)
      {
         if(pitemRelative == m_pitem)
         {
            erelativeNewItem = RelativeFirstChild;
         }
      }

      switch(erelativeNewItem)
      {
      case RelativeFirstChild:
         {
            if(pitemRelative->m_pchild == ::null())
            {
               pitemRelative->m_pchild      = pitemNew;

               pitemNew->m_pparent  = pitemRelative;
            }
            else
            {
               sp(::ca::tree_item) pitemOldFirstChild = pitemRelative->m_pchild;

               pitemRelative->m_pchild                  = pitemNew;

               pitemNew->m_pparent              = pitemRelative;
               pitemNew->m_pnext                = pitemOldFirstChild;

               pitemOldFirstChild->m_pprevious  = pitemNew;
            }
         }
         break;
      case RelativeLastChild:
         {
            if(pitemRelative->m_pchild == ::null())
            {
               pitemRelative->m_pchild         = pitemNew;

               pitemNew->m_pparent     = pitemRelative;
            }
            else
            {
               sp(::ca::tree_item) pitemOldLastChild = pitemRelative->get_item(RelativeLastChild);

               pitemNew->m_pparent     = pitemRelative;
               pitemNew->m_pprevious   = pitemOldLastChild;

               pitemOldLastChild->m_pnext = pitemNew;
            }
         }
         break;
      case RelativePreviousSibling:
         {
            // all tree items that have siblings have a parent (at least the base item)
            ASSERT(pitemRelative->m_pparent != ::null());
            // Is pitemRelative a first child ?
            if(pitemRelative->get_item(RelativeFirstSibling) == pitemRelative)
            {
               return insert_item(pdata, pitemNew, RelativeFirstChild, pitemRelative->m_pparent);
            }
            else
            {
               pitemNew->m_pprevious   = pitemRelative->m_pprevious;
               pitemNew->m_pnext       = pitemRelative;

               pitemRelative->m_pprevious   = pitemNew;
            }
         }
         break;
      case RelativeNextSibling:
         {
            // all tree items that have siblings have a parent (at least the base item)
            ASSERT(pitemRelative->m_pparent != ::null());
            // Is pitemRelative a last child ?
            if(pitemRelative->get_item(RelativeLastSibling) == pitemRelative)
            {
               return insert_item(pdata, pitemNew, RelativeLastChild, pitemRelative->m_pparent);
            }
            else
            {
               pitemNew->m_pprevious   = pitemRelative;
               pitemNew->m_pnext       = pitemRelative->m_pnext;

               pitemRelative->m_pnext   = pitemNew;
            }
         }
         break;
      case RelativeLastSibling:
         {
            while(pitemRelative->m_pnext != ::null())
               pitemRelative = pitemRelative->m_pnext;
            if(pitemRelative->m_pnext == ::null())
            {
               pitemRelative->m_pnext          = pitemNew;

               pitemNew->m_pparent     = pitemRelative->m_pparent;
               pitemNew->m_pprevious   = pitemRelative;
               pitemNew->m_pnext       = ::null();
            }
         }
         break;
      case RelativeReplace:
         {
            pitemNew->m_dwUser      = pitemRelative->m_dwUser;
            pitemNew->m_dwMetaData  = pitemRelative->m_dwMetaData;
            pitemNew->m_pparent     = pitemRelative->m_pparent;
            pitemNew->m_pchild      = pitemRelative->m_pchild;
            pitemNew->m_pnext       = pitemRelative->m_pnext;
            pitemNew->m_pprevious   = pitemRelative->m_pprevious;
            pitemNew->m_dwState     = pitemRelative->m_dwState;
            pitemNew->m_pitemdata   = pitemRelative->m_pitemdata;
            if(pitemRelative->m_pparent->m_pchild == pitemRelative)
            {
               pitemNew->m_pparent->m_pchild = pitemNew;
            }
        }
         break;
         default:
            throw not_supported_exception(get_app());
      }

      _001OnTreeDataChange();
      return true;
   }

   sp(::ca::tree_item) tree::allocate_item(sp(::ca::tree_data) pdata)
   {
      sp(::ca::tree_item) pitemNew = new tree_item;
      pitemNew->m_ptreedata = pdata;
      pitemNew->m_ptree = this;
      return pitemNew;
   }

   void tree::remove_all()
   {

      get_base_item()->m_pchild.release();

   }

   void tree::sort(int32_t ( * lpfnCompare )(sp(tree_item) &, sp(tree_item) &))
   {
      sp(tree_item) pitem = get_base_item();
      while(pitem != ::null())
      {
         pitem->sort_children(lpfnCompare);
         pitem = (sp(tree_item)) pitem->get_next();
      }
   }



   sp(image_list) tree::get_image_list()
   {
      return ::null();
   }

} // namespace ca
