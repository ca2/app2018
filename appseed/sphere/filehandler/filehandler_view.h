#pragma once


namespace filehandler
{


   class CLASS_DECL_SPHERE view :
      public html_view
   {
   public:


      class list;


      class CLASS_DECL_SPHERE item :
         virtual public ::object
      {
      public:
         

         string         m_strApp;
         int32_t        m_iIndex;

         rect           m_rectItem;
         rect           m_rectStatusImage;
         rect           m_rectName;

         item(::aura::application * papp);

         void parse(const char * pszApp);

         void draw(sp(view) pview, ::draw2d::graphics * pgraphics, list * plist);

      };

      class CLASS_DECL_SPHERE list :
         virtual public spa(item)
      {
      public:

         int32_t m_iItemHeight;

         list(::aura::application * papp);


         void parse(::filehandler::handler * phandler, const char * pszTopic);

         void on_layout(LPCRECT lpcrect);
         void draw(sp(view) pview, ::draw2d::graphics * pgraphics);

      };



      ::xml::document         m_document;
      sp(list)                m_plistWorking;
      sp(list)                m_plist;

      ::draw2d::font_sp       m_spfont;


      view(::aura::application * papp);


      virtual void _001OnDraw(::draw2d::graphics * pgraphics);
      
      virtual void install_message_handling(::message::dispatch * pdispatch);

      virtual void refresh();

      DECL_GEN_SIGNAL(_001OnCreate);
      void _001OnTimer(::timer * ptimer);
      DECL_GEN_SIGNAL(_001OnLButtonUp);

      virtual void on_layout();


      index hit_test(point pt, ::user::e_element & eelement);


      void layout_list(list * plist);

      virtual bool get_font(::draw2d::font_sp & font);

   };



} // namespace filehandler





