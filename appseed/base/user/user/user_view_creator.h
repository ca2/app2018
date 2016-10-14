#pragma once


namespace user
{




   class CLASS_DECL_BASE view_creator :
      virtual public view_container
   {
   public:


      class CLASS_DECL_BASE create_exception
      {
      public:


         id       m_id;


         create_exception(id id) : m_id(id) {}


      };


      class CLASS_DECL_BASE view_map :
         public id_map < ::user::view_creator_data * >
      {
      public:
      };


      view_map                      m_viewmap;
      class view_container *        m_pviewcontainer;


      view_creator();
      virtual ~view_creator();


      virtual ::count get_view_count();

      virtual bool on_new_view_creator_data(::user::view_creator_data * pcreatordata);
      virtual void on_create_view(::user::view_creator_data * pcreatordata);
      virtual void on_show_view();

      virtual void on_update(::user::document * pdocument, ::user::impact * pSender, LPARAM lHint, object* pHint);

      virtual ::user::interaction * get_view();

      virtual id get_view_id();

      virtual ::user::view_creator_data * get(id id);

      virtual ::user::view_creator_data * allocate(id id);

      using ::object::create;
      virtual ::user::view_creator_data * create(id id, LPCRECT lpcrectCreate);

      virtual ::user::view_creator_data * create_impact(id id, LPCRECT lpcrectCreate, ::user::frame_window * pframewindow);
      virtual ::user::view_creator_data * create_impact(id id, LPCRECT lpcrectCreate);

      virtual ::user::view_creator_data * get_impact(id id, LPCRECT lpcrectCreate);

      virtual ::user::view_creator_data * on_after_create_impact(::user::view_creator_data * pcreatordata);

      virtual void hide_all_except(const id_array & id);

      template < class T > T *
      get_typed_document()
      {
         T * pdoc;
         ::user::view_creator_data * pcreatordata;
         id key;
         POSITION pos = m_viewmap.get_start_position();
         while(pos != NULL)
         {
            m_viewmap.get_next_assoc(pos, key, pcreatordata);
            if(pcreatordata->m_pdoc != NULL)
            {
               pdoc = dynamic_cast < T * > (pcreatordata->m_pdoc);
               if(pdoc != NULL)
               {
                  return pdoc;
               }
            }
         }
         return NULL;
      }


   };


} // namespace user


