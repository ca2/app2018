#pragma once


namespace visual
{


   class CLASS_DECL_BASE cursor :
      virtual public ::object
   {
   public:


      e_cursor                m_ecursor;
      ::draw2d::dib_sp        m_dib;
      //::draw2d::dib_sp        m_dibWork;
      //::draw2d::dib_sp        m_dibWork2;
      size                    m_szHotspotOffset;



#ifdef WINDOWSEX

      HCURSOR                 m_hcursor;

#endif


      cursor(::aura::application * papp);

      bool to(::draw2d::graphics * pgraphics, point pt);

      bool initialize_system_default();

#ifdef WINDOWSEX

      HCURSOR get_HCURSOR();

#endif


   };


   typedef sp(cursor) cursor_sp;


   CLASS_DECL_BASE void cursor_alloc(::aura::application * papp,cursor * & pdib,int xHotspot,int yHotspot);
   CLASS_DECL_BASE ::draw2d::dib * cursor_get_dib(cursor * pcursor);


} // namespace visual



#ifdef WINDOWSEX

// Derived from Microsoft KB318876!!
// on 2014-06-29!! During World Cup Games 2014!! Oitavas de Final, third game between Nederland and Mexico!!
// Thank you God!!
// Thank you Microsoft!!
// Thank you Pappi!! Pelo passeio no Mercado Municipal de Curitiba com a Mummi e pelo almo�o com o-bento de sushi e onigiri delicioso!!
// Obrigado Mummi por tudo!! E por hoje tamb�m e por propiciar isso tudo!! Por falar t�o profundamente do Chico Xavier mesmo com muitas d�vidas minhas!!
// Obrigado Thomas!! Voc� poderia colaborar, n�o �!!
// Meu Deus me ajude!!

CLASS_DECL_BASE HBITMAP CreateAlphaBitmapV5(::draw2d::dib * pdib);
CLASS_DECL_BASE HICON CreateAlphaIcon(::draw2d::dib * pdib,bool bIcon = true,int xHotSpot = 0,int yHotSpot = 0);
CLASS_DECL_BASE HCURSOR CreateAlphaCursor(::draw2d::dib * pdib,int xHotSpot,int yHotSpot);

#endif