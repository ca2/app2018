#include "framework.h"


simple_status_bar::simple_status_bar(::aura::application * papp) :
   object(papp),
   ::user::status_bar(papp)
{

}

simple_status_bar::~simple_status_bar()
{
}

/*
 // BEGIN_MESSAGE_MAP(simple_status_bar, ::user::status_bar)
   //{{__MSG_MAP(simple_status_bar)
   ON_WM_ERASEBKGND()
   //}}__MSG_MAP
 // END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// simple_status_bar message handlers


/*
bool simple_status_bar::OnEraseBkgnd(::draw2d::graphics * pgraphics)
{
   return TRUE;
}
*/

void simple_status_bar::_001OnDraw(::draw2d::graphics * pgraphics)
{
   return;
//   rect rectClient;
//
//   GetClientRect(rectClient);
//
//   
//
//   class imaging & imaging = Application.imaging();
//#ifdef WINDOWSEX
//   imaging.color_blend(
//      pgraphics,
//      rectClient.left,
//      rectClient.top,
//      rectClient.width(),
//      rectClient.height(),
//      Session.get_default_color(COLOR_3DFACE),
//      208);
//#else
//   imaging.color_blend(
//      pgraphics,
//      rectClient.left,
//      rectClient.top,
//      rectClient.width(),
//      rectClient.height(),
//      ARGB(255, 184, 184, 177),
//      208);
//#endif
//
//

}

