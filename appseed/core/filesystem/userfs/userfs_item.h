#pragma once


namespace userfs
{


   class CLASS_DECL_CORE item:
      virtual public ::data::item,
      virtual public ::fs::item

   {
   public:


      string                                 m_strName;
      int32_t                                m_iIdentation;
      string                                 m_strFolder;
      int32_t                                m_iImage;
      int32_t                                m_iIndex;
      int32_t                                m_iImageSelected;


      item();

      string data_item_get_text(object * pobject);
      index data_item_get_image(::user::interaction * pui);

      static index CompareArrangeByName(sp(::data::tree_item) * pitema, sp(::data::tree_item) * pitemb);
      int32_t get_index() const;
      bool is_folder() const;


   };


} // namespace userfs





