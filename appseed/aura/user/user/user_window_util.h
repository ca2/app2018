#pragma once


namespace user
{


   class interaction_spa;


   class CLASS_DECL_AURA oswindow_array :
      public comparable_array < oswindow, oswindow >
   {
      public:
         void SortByZOrder();
         void SortSiblingsByZOrder();
         void top_windows_by_z_order();
   };

   class CLASS_DECL_AURA interaction_ptra:
      public ref_array < ::user::interaction >
   {
      public:

         //DECLARE_AND_IMPLEMENT_DEFAULT_CONSTRUCTION_AND_ASSIGNMENT(interaction_ptra, ref_array < ::user::interaction >)

         //interaction_ptra(const interaction_spa & a);


         interaction_ptra & operator = (const interaction_spa & a);

         ::user::oswindow_array get_hwnda();

         using ref_array < ::user::interaction >::find_first;
         ::user::interaction * find_first_typed(sp(type) info);
         ::user::interaction * find_first(oswindow oswindow);

         virtual bool get_child(sp(::user::interaction ) & pui);
         virtual bool rget_child(sp(::user::interaction) & pui);

   };


   class CLASS_DECL_AURA interaction_spa :
      virtual public spa(::user::interaction)
   {
      public:


         interaction_spa();
         interaction_spa(::aura::application * papp);
         interaction_spa(const ref_array < ::user::interaction > & a);
         interaction_spa(const spa(::user::interaction) & a)
         {

            copy(a);

         }

         interaction_spa(const ::user::interaction_spa & a)
         {

            copy(a);

         }

#ifdef MOVE_SEMANTICS

         interaction_spa(const ::user::interaction_spa && a)
         {

            copy(a);

         }

#endif


         using spa(::user::interaction)::find_first;
         sp(::user::interaction) find_first_typed(sp(type) info);
         sp(::user::interaction) find_first(oswindow oswindow);

         ::user::oswindow_array get_hwnda();
         void send_message(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0);
         void send_message_to_descendants(UINT uiMessage, WPARAM wparam = 0, LPARAM lparam = 0, bool bRecursive = true);

         interaction_spa & operator = (const spa(::user::interaction) & a);
         interaction_spa & operator = (const interaction_spa & a);

         virtual bool get_child(sp(::user::interaction) & pui);
         virtual bool rget_child(sp(::user::interaction) & pui);

   };


   class CLASS_DECL_AURA oswindow_tree :
      virtual public object
   {
      public:

         class CLASS_DECL_AURA Array :
            public spa(oswindow_tree)
         {
            public:

               bool remove(oswindow oswindow);
               int_ptr find(oswindow oswindow);
               void EnumDescendants();
               Array & operator = (oswindow_array & oswindowa);

         };

         oswindow       m_oswindow;
         uint32_t       m_dwUser;
         oswindow       m_pvoidUser;
         Array          m_oswindowtreea;


         oswindow_tree();
         oswindow_tree(oswindow interaction_impl);
         oswindow_tree(const oswindow_tree & tree);

         void EnumDescendants();

         oswindow_tree & operator = (const oswindow_tree & tree);

         static_function index compare_oswindow(const oswindow_tree * ptree1, const oswindow_tree * ptree2);


   };


   class CLASS_DECL_AURA window_util
   {
      public:
         static_function void ContraintPosToParent(oswindow oswindow);
         //static_function void EnumChildren(::window_sp pwnd, interaction_spa & wndpa);
         static_function void EnumChildren(oswindow oswindow, oswindow_array & oswindowa);
         /*static_function void ExcludeChild(interaction_spa & wndpa);*/
         /*static_function void SortByZOrder(interaction_spa & wndpa);*/
         static_function void SortByZOrder(oswindow_array & oswindowa);
         static_function HRGN GetAClipRgn(oswindow oswindow, POINT ptOffset, bool bExludeChilren);
         static_function void ExcludeChildren(oswindow oswindow, HRGN hrgn, POINT ptOffset);
         window_util();
         virtual ~window_util();
         /*static_function void send_message_to_descendants(oswindow oswindow, UINT message,
         WPARAM wParam, LPARAM lParam, bool bDeep, bool bOnlyPerm);*/
         static_function void send_message_to_descendants(oswindow oswindow, UINT message,
               WPARAM wParam, LPARAM lParam, bool bDeep);
         static_function bool IsAscendant(oswindow oswindowAscendant, oswindow oswindowDescendant);

         static_function int32_t GetZOrder(oswindow oswindow);
         static_function void GetZOrder(oswindow oswindow, int_array & ia);

         //      static_function HRGN GetAClipRgn(oswindow oswindow, POINT ptOffset, bool bExludeChilren);
         //      static_function void ExcludeChildren(oswindow oswindow, HRGN hrgn, POINT ptOffset);

         //static_function void SortByZOrder(comparable_array < ::window_sp, ::window_sp > & wndpa);
         //static_function void SortByZOrder(comparable_array < oswindow, oswindow > & oswindowa);
         //static_function void EnumChildren(oswindow oswindow, comparable_array < oswindow, oswindow > & oswindowa);

   };




} // namespace user



