#pragma once


namespace boot
{


   class thread_base;
   class window;


} // namespace boot


namespace user
{


   class interaction_base;
   class interaction;


} // namespace user


typedef comparable_array < oswindow_data * > oswindow_dataptra;

class simple_event;


class simple_mutex;



struct CLASS_DECL_BASE oswindow_data
{


   bool                          m_bMessageOnlyWindow;
   ::user::interaction *         m_pui;
   HTHREAD                       m_hthread;
   COLORREF *                    m_pcolorref;
   RECT                          m_rect;
   int_map < LONG > *            m_plongmap;
   bool                          m_bDestroying;


   static oswindow_dataptra * s_pdataptra;
   static mutex * s_pmutex;


   oswindow_data();
   oswindow_data(::user::interaction_base * puibaseMessageOnlyWindow);
   oswindow_data(const void * p);
   oswindow_data(const LPARAM & lparam);
   oswindow_data(const WPARAM & wparam);


   ~oswindow_data();



   operator void * ()
   {
      return this;
   }

   operator void * () const
   {
      return (void *) this;
   }

   oswindow_data & operator = (const oswindow_data & window);

   bool operator == (const void * p) const
   {
      return this == p;
   }

   bool operator != (const void * p) const
   {
      return this != p;
   }


   int32_t store_name(const char * psz);
   int32_t select_input(int32_t iInput);
   int32_t select_all_input();
   int32_t map_window();

   void set_user_interaction(::user::interaction * pui);
   ::user::interaction * get_user_interaction();
   ::user::interaction * get_user_interaction() const;


   void post_nc_destroy();


   bool is_child(oswindow oswindowCandidateChildOrDescendant); // or descendant
   oswindow get_parent();
   oswindow set_parent(oswindow oswindowNewParent);
   long get_state();
   bool is_iconic();
   bool is_window_visible();
   bool show_window(int32_t nCmdShow);
   LONG get_window_long(int32_t nIndex);
   LONG set_window_long(int32_t nIndex, LONG l);
   bool client_to_screen(LPPOINT lppoint);
   bool screen_to_client(LPPOINT lppoint);



   bool is_null() const
   {
      return this == NULL;
   }


   bool is_destroying();


};


CLASS_DECL_BASE int32_t oswindow_find_message_only_window(::user::interaction_base * puibaseMessageWindow);
CLASS_DECL_BASE int32_t oswindow_find(::user::interaction * pwindow);
CLASS_DECL_BASE oswindow_data * oswindow_get_message_only_window(::user::interaction_base * puibaseMessageWindow);
CLASS_DECL_BASE oswindow_data * oswindow_get(::user::interaction * pwindow);
CLASS_DECL_BASE oswindow oswindow_defer_get(::user::interaction * pwindow);
CLASS_DECL_BASE bool oswindow_remove(::user::interaction * pwindow);
CLASS_DECL_BASE bool oswindow_remove_message_only_window(::user::interaction_base * puibaseMessageOnlyWindow);



