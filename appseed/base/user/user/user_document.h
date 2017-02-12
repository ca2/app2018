#pragma once


namespace user
{


   class CLASS_DECL_BASE document :
      virtual public command_target,
      virtual public ::data::data_container_base,
      virtual public ::file::serializable,
      virtual public ::user::server,
      virtual public ::database::client,
      virtual public ::user::form_callback
   {
   public:


      string                           m_strTitle;
      ::file::path                     m_filepath;
      ::user::impact_system *          m_pimpactsystem;
      spa(::user::impact)              m_viewspa;
      bool                             m_bModified;
      bool                             m_bNew;
      bool                             m_bAutoDelete;     // TRUE => delete document when no more views
      bool                             m_bEmbedded;       // TRUE => document is being created by OLE
      ::user::document_data_map        m_datamap;
      property_set                     m_set;
      ::file::buffer_sp                m_file;




      document(::aura::application * papp);
      virtual ~document();




      virtual void update_title();

      virtual void on_create(sp(::create) pcreatecontext);

      const string & get_title() const;
      virtual void set_title(const char * lpszTitle);
      const ::file::path & get_file_path() const;
      virtual void set_path_name(var varFile, bool bAddToMRU = TRUE);

      ::user::impact_system * get_document_template() const;
      virtual bool is_modified();
      virtual void set_modified_flag(bool bModified = TRUE);
      virtual void set_new(bool bNew = true);

      virtual bool is_new_document();

      // Operations
      void add_view(::user::impact * pview);
      void remove_view(::user::impact * pview);
      virtual ::count get_view_count() const;
      virtual sp(::user::impact) get_view(index index = 0) const;



      template < class T >
      ::count get_typed_view_count() const
      {
         synch_lock sl(((document *)this)->m_pmutex);
         ::count count = 0;
         for (index index = 0; index < m_viewspa.get_count(); index++)
         {
            if (m_viewspa[index].is_null())
            {
               continue;
            }
            sp(T) pt = m_viewspa[index].cast < T > ();
            if (pt.is_set())
               count++;
         }
         return count;
      }

      template < class T >
      sp(T) get_typed_view(index indexFind = 0) const
      {
         synch_lock sl(((document *)this)->m_pmutex);
         if (indexFind < 0 || indexFind >= m_viewspa.get_count())
            return NULL;
         ::count count = 0;
         for (index index = 0; index < m_viewspa.get_count(); index++)
         {
            if(m_viewspa[index].is_null())
            {
               continue;
            }
            sp(T) pt = m_viewspa[index].cast < T > ();
            if (pt.is_set())
            {
               if (indexFind == count)
                  return pt;
               else
                  count++;
            }
         }
         return NULL;
      }

      template < class T >
      sp(T) get_typed_view_with_id(id id) const
      {

         synch_lock sl(((document *)this)->m_pmutex);

         ::count count = 0;

         for(index index = 0; index < m_viewspa.get_count(); index++)
         {

            if(m_viewspa[index].is_null())
            {
               continue;
            }

            sp(T) pt = m_viewspa[index].cast < T >();

            if(pt.is_set())
            {

               if(id == pt->m_id)
                  return pt;
               else
                  count++;

            }

         }

         return NULL;

      }

      virtual sp(::user::impact) get_typed_view(sp(type) info, index indexFind = 0);

      virtual sp(::user::impact) get_typed_view_with_id(sp(type) info,id id);

      virtual void show_all_frames(UINT nCmdShow);


      class update
      {
      public:
         sp(::user::impact)         m_pSender;
         LPARAM         m_lHint;
         ::object *  m_pHint;
      };

      // Update Views (simple update - DAG only)
      void update_all_views(sp(::user::impact) pSender, LPARAM lHint = 0L,
         ::object* pHint = NULL);

      void send_update(sp(::user::impact) pSender, LPARAM lHint = 0L,
         ::object* pHint = NULL);

      // Overridables
      // Special notifications
      virtual void on_changed_view_list(single_lock * psl = NULL); // after add or remove ::user::impact
      virtual void delete_contents(); // delete doc items etc


      // File helpers
      virtual bool on_new_document();
      virtual bool on_open_document(var varFile);
      virtual bool on_save_document(var varFile);
      virtual void on_close_document(single_lock * psl = NULL);
      virtual void pre_close_document();
      virtual void close_document();
      virtual void report_save_load_exception(const char * lpszPathName, ::exception::base* e, bool bSaving, const char * nIDPDefault);

      // advanced overridables, closing down frame/doc, etc.
      virtual bool can_close_frame(sp(::user::frame_window) pFrame);
      virtual bool save_modified(); // return TRUE if ok to continue
      virtual void pre_close_frame(sp(::user::frame_window) pFrame);


      virtual void dump(dump_context &) const;
      virtual void assert_valid() const;

      // implementation helpers
      virtual bool do_save(var varFile, bool bReplace = true);
      virtual bool do_file_save();
      virtual void update_frame_counts(single_lock * psl = NULL);
      virtual void disconnect_views();
      virtual void call_initial_update();

      // overridables for implementation
      virtual HMENU GetDefaultMenu(); // get menu depending on state
      virtual HACCEL GetDefaultAccelerator();
      virtual void on_idle();
      virtual void on_final_release();

      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      friend class impact_system;

      /*

      virtual void write(::file::ostream & ostream) const;
      virtual void read(::file::istream & istream);

      */

      void on_file_close();
      void on_file_save();
      void on_file_save_as();
      void on_file_send_mail();
      void on_update_file_send_mail(cmd_ui* pCmdUI);



      virtual void on_request(sp(::create) pcreatecontext);





//      virtual void on_alloc(::aura::application * papp);

//      virtual void dump(dump_context &) const;
  //    virtual void assert_valid() const;

//      virtual bool _001OnCmdMsg(::aura::cmd_msg * pcmdmsg);

      template < class DOCUMENT >
      DOCUMENT * get_typed_document()
      {
         ASSERT(this != NULL);
         return dynamic_cast < DOCUMENT * >(this);
      }

      template < class DATA >
      DATA * get_typed_data()
      {
         ASSERT(this != NULL);
         return m_spadata.get < DATA >();
      }

      template < class DOCUMENT >
      ::data::data * get_typed_document_data()
      {
         ASSERT(this != NULL);
         return get_data(get_typed_document < DOCUMENT > ());
      }

      template < class DOCUMENT >
      ::data::data * get_data(DOCUMENT * pthis)
      {
         ASSERT(this == pthis);
         if (this != pthis)
            return NULL;
         return m_datamap[pthis];
      }

      template < class DOCUMENT >
      void set_typed_document_data(::data::data * pdata)
      {
         ASSERT(this != NULL);
         set_data(get_typed_document < DOCUMENT >(), pdata);
      }

      template < class DOCUMENT >
      void set_data(DOCUMENT * pthis, ::data::data * pdata)
      {
         ASSERT(this == pthis);
         if (this != pthis)
            return;
         m_spadata.remove_type(pdata);
         m_datamap[pthis] = pdata;
         m_spadata.add(pdata);
      }


      virtual void OnBeforeNavigate2(::html::data * pdata,var & varFile,uint32_t nFlags,const char * lpszTargetFrameName,byte_array& baPostedData,const char * lpszHeaders,bool* pbCancel);

      virtual void form_document_set_property_set(const property_set & set);
      virtual property_set * form_document_get_property_set();
      virtual property_set & form_document_property_set();

      //virtual void form_document_set_view(::user::form_window * pview);
      //virtual void form_document_set_callback(form_callback * pcallback);


      virtual bool open_document(var varFile);


   };


} // namespace user




