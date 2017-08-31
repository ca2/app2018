#include "framework.h"


namespace userfs
{


   list_item::list_item(::aura::application * papp) :
      ::object(papp)
   {
      m_iIndex = -1;
      m_iArrange = -1;
//      m_iImage = -1;
   }

   list_item::list_item(const list_item & listitem) :
      ::object(listitem.m_pauraapp),
      ::fs::item(listitem),
      m_strName(listitem.m_strName),
  //    m_iImage(listitem.m_iImage),
      m_iIndex(listitem.m_iIndex),
      m_iArrange(listitem.m_iArrange)
   {
      
   }

   list_item::list_item(const list_item & listitem, index iArrange) :
      ::object(listitem.m_pauraapp),
      ::fs::item(listitem),
      m_strName(listitem.m_strName),
//      m_iImage(listitem.m_iImage),
      m_iIndex(listitem.m_iIndex),
      m_iArrange(iArrange)
   {

   }

   index list_item::CompareArrangeByName(const list_item & item) const
   {
   /*   if(m_iCsidl >= 0)
      {
         if(item.m_iCsidl >= 0)
         {
            return Shell::GetCSIDLSort(m_iCsidl) - Shell::GetCSIDLSort(item.m_iCsidl);
         }
         else
         {
            return -1;
         }
      }
      else if(item.m_iCsidl >= 0)
      {
         return 1;
      }*/
      if(item.IsFolder())
      {
         if(IsFolder())
         {
            return m_strName.compare_ci(item.m_strName);
         }
         else
         {
            return 1;
         }
      }
      else
      {
         if(IsFolder())
         {
            return -1;
         }
         else
         {
            return m_strName.compare_ci(item.m_strName);
         }
      }
   }

   index list_item::CompareArrangeByName(const sp(list_item) & pitema, const sp(list_item) & pitemb)
   {
      return (pitema)->CompareArrangeByName(*pitemb);
   }

   index list_item::GetIndex() const
   {
      return m_iIndex;
   }

   bool list_item::IsFolder() const
   {
      if(m_flags.is_signalized(::fs::FlagFolderEx001Calc))
         return m_flags.is_signalized(::fs::FlagFolderEx001);
      else
      {
         bool bFolderEx001 = m_flags.is_signalized(::fs::FlagFolder) || m_flags.is_signalized(::fs::FlagInZip);
         const_cast < list_item * > (this)->m_flags.signalize(::fs::FlagFolderEx001Calc);
         if(bFolderEx001)
         {
            const_cast < list_item * > (this)->m_flags.signalize(::fs::FlagFolderEx001);
         }
         return m_flags.is_signalized(::fs::FlagFolderEx001);
      }
   }

   list_item & list_item::operator = (const list_item & item)
   {

      if(&item == this)
         return *this;

      m_pauraapp  = item.m_pauraapp;
      m_filepath = item.m_filepath;
      m_strName   = item.m_strName;
//      m_iImage    = item.m_iImage;
      m_iIndex    = item.m_iIndex;
      m_iArrange  = item.m_iArrange;
      m_flags     = item.m_flags;

      return *this;

   }


} // namespace userfs



template <>
::fs::item cast < ::fs::item > (::userfs::list_item & item)
{
   ::fs::item itemT;
   itemT.m_filepath = item.m_filepath;
   itemT.m_flags = item.m_flags;
   return itemT;
}
