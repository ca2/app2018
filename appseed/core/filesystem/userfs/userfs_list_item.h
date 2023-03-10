#pragma once


namespace userfs
{


   class CLASS_DECL_CORE list_item :
      virtual public ::fs::item
   {
   public:


      string                           m_strName;
      //index                            m_iImage;
      index                            m_iIndex;
      index                            m_iArrange;


      list_item(::aura::application * papp);
      list_item(const list_item & item);
      list_item(const list_item & item, index iArrange);

      static index CompareArrangeByName(const sp(list_item) & pitema, const sp(list_item) & pitemb);
      index CompareArrangeByName(const list_item & item) const;
      index GetIndex() const;
      bool IsFolder() const;

      list_item & operator = (const list_item & item);


   };


} // namespace userfs


template < class T >
T cast (::userfs::list_item & item)
{
   return T(item);
}


template <>
CLASS_DECL_CORE ::fs::item cast < ::fs::item > (::userfs::list_item & item);
