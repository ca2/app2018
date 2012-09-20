#pragma once


namespace filehandler
{


   class CLASS_DECL_ca2 view :
      public form_view
   {
   public:


      class CLASS_DECL_ca2 item
      {
      public:
         

         string      m_strApp;
         int         m_iIndex;


         void parse(const char * pszApp);


      };

      class CLASS_DECL_ca2 list :
         public array_ptr_alloc < item >
      {
      public:


         void parse(handler * phandler, const char * pszTopic);


      };

      class CLASS_DECL_ca2 draw_item
      {
      public:

         rect        m_rectItem;
         rect        m_rectStatusImage;
         rect        m_rectName;

         void draw(view * pview, ::ca::graphics * pdc, list * plist, item * pcontact);

      };

      class CLASS_DECL_ca2 draw_list :
         public array_ptr_alloc < draw_item >
      {
      public:


         int m_iItemHeight;

         draw_list();

         void layout(LPCRECT lpcrect, list * plist);
         void draw(view * pview, ::ca::graphics * pdc, list * plist);

      };

      ::xml::document         m_document;
      list                    m_list;
      draw_list               m_drawlist;


      view(::ca::application * papp);


      virtual void _001OnDraw(::ca::graphics * pdc);
      
      virtual void install_message_handling(::gen::message::dispatch * pdispatch);

      virtual void refresh();

      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnLButtonUp)

      virtual void layout();


      index hit_test(point pt, e_element & eelement);


   };



} // namespace filehandler





