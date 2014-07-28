#include "framework.h"

namespace data
{

   simple_item::simple_item(::data::tree * pdatatree) :
      m_pdatatree(pdatatree)
   {
      m_iImage          = -1;
      m_iImageSelected  = -1;

   }

   simple_item::~simple_item()
   {
   }

   string simple_item::data_item_get_text(object * pobject) const
   {
      UNREFERENCED_PARAMETER(pobject);
      return m_str;
   }
   
   index simple_item::data_item_get_image(object * pobject) const
   {

      if(m_pdatatree->is_selected(this))
      {

         return m_iImageSelected;

      }
      else
      {

         return m_iImage;

      }

   }

   sp(image_list) simple_item::data_item_get_image_list(object * pobject) const
   {

      return m_pdatatree->get_image_list();

   }


/*#undef new
   void * simple_item::operator new(size_t, const char *, int32_t)
   {
      return _alloc001SimpleTreeItem.Alloc();
   }

   void * simple_item::operator new(size_t)
   {
      return _alloc001SimpleTreeItem.Alloc();
   }
#define new BASE_NEW*/

/*   void simple_item::operator delete(void * p)
   {
      return _alloc001SimpleTreeItem.Free(p);
   }

   void simple_item::operator delete(void * p, const char *, int32_t)
   {
      return _alloc001SimpleTreeItem.Free(p);
   }*/

} // namespace data
