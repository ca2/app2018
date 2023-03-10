#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_core
         {


            class CLASS_DECL_APP_CORE_WNDFRM_CORE FrameSchemaHardCoded001 :
               public frame_schema
            {
            public:

               
               FrameSchemaHardCoded001(::aura::application * papp);
               virtual ~FrameSchemaHardCoded001();

               virtual void on_style_change();

               void on_draw_frame(::draw2d::graphics * pgraphics);
               void DrawBorder(::draw2d::graphics * pgraphics, const RECT & lpcrectClient);
               void DrawBorderSide(::draw2d::graphics * pgraphics, const RECT & lpcrectClient, EBorder eside);
               void GetBorderRect(const RECT & lpcrectClient, LPRECT lprect, EBorder eside);
               void DrawGripSet(::draw2d::graphics * pgraphics, const RECT & lpcrectClient);
               void DrawGrip(::draw2d::graphics * pgraphics, const RECT & lpcrectClient, EGrip egrip);
               void DrawRectGrip(::draw2d::graphics * pgraphics, const RECT & rect);

               virtual EHitTest _000HitTest(point pt);


            };


         } // namespace wndfrm_core


      } // namespace frame


   } // namespace wndfrm


} // namespace user






