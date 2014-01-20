#include "framework.h"


namespace simple_ui
{


   label::label(sp(base_application) papp) :
      element(papp),
      interaction(papp)
   {

   }

   label::~label()
   {
   }




   void label::draw_this(::draw2d::graphics * pgraphics)
   {


      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);


      COLORREF c;

      //if(get_style()->m_eschema == ::hotplugin::plugin::schema_darker)
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS
         c = ARGB(255, 184 + 49, 149 + 49, 177 + 49);
#else
         c = ARGB(255, 149 + 49, 184 + 49, 123 + 49);
#endif

      }
      //else
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS
         c = ARGB(255, 84, 49, 77);
#else
         c = ARGB(255, 49, 84, 23);
#endif

      }

      rect rectClient;

      get_client_rect(rectClient);

      ::draw2d::brush_sp b(allocer());

      b->create_solid(c);

      ::draw2d::font_sp f(allocer());

      /*f->create_pixel_font("Geneva", (int32_t)height(rectClient) * 0.7);

      float fMargin = (height(rectClient) * ((1.0f - 0.7f) / 2.0f));*/

      f->create_pixel_font("Geneva", (int32_t)height(rectClient) * 1.0);

      float fMargin = 0.0;

      pgraphics->SelectObject(f);

      pgraphics->SelectObject(b);

      pgraphics->TextOut(rectClient.left, (int32_t)(rectClient.top + fMargin), m_strText);

   }


} // namespace simple_ui
