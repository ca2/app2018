#pragma once


namespace user
{


   class CLASS_DECL_CORE static_control :
      virtual public ::user::control
   {
   public:

      enum e_type
      {
         type_text,
         type_icon,
         type_bitmap,
         type_cursor,
      };


      union
      {
         ::draw2d::bitmap *             m_pbitmap;
         ::visual::icon *           m_picon;
         ::visual::cursor *         m_pcursor;
      };


      e_type                              m_etype;

      bool                                m_bLButtonDown;
      bool                                m_bHover;


      static_control(::aura::application * papp);
      virtual ~static_control();


      virtual void install_message_routing(::message::sender * psender);


      virtual bool create_window(const RECT & rect, sp(::user::interaction) puiParent,id id);


      HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile);
      HENHMETAFILE GetEnhMetaFile();
      void static_set_icon(::visual::icon * picon);
      ::visual::icon * static_get_icon();
      void static_set_bitmap(::draw2d::bitmap * pbitmap);
      ::draw2d::bitmap * static_get_bitmap();
      void static_set_cursor(::visual::cursor * pcursor);
      ::visual::cursor * static_get_cursor();


      e_type get_type();


      virtual void _001OnDraw(::draw2d::graphics * pgraphics);


      virtual bool OnChildNotify(::message::base * pbase);

      DECL_GEN_SIGNAL(_001OnCreate);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);


   };


} // namespace user


