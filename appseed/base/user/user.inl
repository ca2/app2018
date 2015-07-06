#pragma once



namespace user
{


   template < class VIEW >
   inline VIEW * impact::create_view(::user::document * pdoc,const RECT & rect, ::user::interaction * pwndParent,id id, ::user::interaction * pviewLast)
   {
      return dynamic_cast < VIEW * > (create_view(System.type_info < VIEW >(), pdoc, rect, pwndParent, id, pviewLast));
   }


   template < class VIEW >
   inline VIEW * impact::create_view(const RECT & rect,::user::interaction * pwndParent,id id,::user::interaction * pviewLast)
   {

      return dynamic_cast < VIEW * > (create_view < VIEW >(NULL,rect,pwndParent,id,pviewLast));

   }


   template < class VIEW >
   inline VIEW * impact::create_view(::user::view_creator_data * pcreatordata, ::user::interaction * pviewLast)
   {

      VIEW * pview = create_view < VIEW >(pcreatordata->m_rectCreate, pcreatordata->m_pholder, pcreatordata->m_id, pviewLast);

      if (pview != NULL)
      {

         pcreatordata->m_pdoc = get_document();

      }

      return pview;

   }


   template < class DATA >
   DATA * impact::get_typed_data()
   {
      ASSERT(this != NULL);
      return m_pdocument->get_typed_data < DATA >();
   }

   template < class DOCUMENT >
   ::data::data * impact::get_typed_document_data()
   {
      ASSERT(this != NULL);
      return m_pdocument->get_typed_document_data < DOCUMENT >();
   }

   template < class DOCUMENT >
   DOCUMENT * impact::get_typed_document()
   {
      ASSERT(this != NULL);
      return m_pdocument->get_typed_document < DOCUMENT >();
   }


   template < class BASE >
   void scroll_x < BASE >::create_x_scroll_bar(const RECT & rect)
   {

      if(m_pscrollbarHorz != NULL)
         return;

      scroll_bar * pbar = new simple_scroll_bar(this->get_app());

      if(!pbar->create_window(::orientation_horizontal,WS_CHILD | WS_VISIBLE,null_rect(),this,7000 + 1))
      {
         delete pbar;
         return;
      }

      m_pscrollbarHorz = pbar;

   }


   template < class BASE >
   void scroll_y < BASE >::create_y_scroll_bar(const RECT & rect)
   {

      if(m_pscrollbarVert != NULL)
         return;

      scroll_bar * pbar = new simple_scroll_bar(this->get_app());

      if(!pbar->create_window(::orientation_vertical,WS_CHILD | WS_VISIBLE,null_rect(),this,7002))
      {
         delete pbar;
         return;
      }

      m_pscrollbarVert = pbar;

   }



} //   namespace aura













