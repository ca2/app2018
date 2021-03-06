#pragma once


#include "core/user/user/user_margin.h"


namespace html
{


   class data;


} // namespace html


namespace user
{


   class menu_item_ptra;
   class menu_item;
   class menu_item_ptra;
   class front_end;
   class menu_item;


   //CLASS_DECL_CORE bool set_wallpaper(string strLocalImagePath);
   //CLASS_DECL_CORE stringa get_wallpaper();



} // namespace user


#define USER_TIMER 50000


class simple_mesh_data;


enum e_position
{
   position_none           = 0,
   position_top            = 1,
   position_left           = 2,
   position_top_left       = 3,
   position_right          = 4,
   position_top_right      = 5,
   position_hcenter        = 6,
   position_top_center     = 7,
   position_bottom         = 8,
   position_vcenter        = 9,
   position_bottom_left    = 10,
   position_left_center    = 11,
   position_bottom_right   = 12,
   position_right_center   = 13,
   position_bottom_center  = 14,
   position_center         = 15,
};


namespace user
{
   enum e_line_hit
   {
      line_hit_none,
      line_hit_normal,
      line_hit_link,
   };
}



#include "user_list_header.h"


#include "user_combo_box.h"
#include "user_combo_list.h"


#include "user_check_box.h"
#include "user_toggle_switch.h"


//#include "user_document_interface.h"
//#include "user_document.h"


#include "user_mesh.h"
#include "user_list.h"
#include "user_list_view.h"
#include "user_mesh_cache_interface.h"
#include "user_mesh_cache.h"
#include "user_list_cache.h"
#include "user_mesh_data.h"
#include "user_list_data.h"
#include "user/simple/simple_mesh_data.h"
#include "user/simple/simple_list_data.h"

#include "user_tree_data.h"

#include "user_tree.h"

//#include "user_form.h"
//#include "user_form_list.h"
//#include "user_form_list_view.h"



//#include "user_place_holder.h"
//#include "user_place_holder_container.h"


#include "user_tab_callback.h"
#include "user_tab.h"




struct CPrintPreviewState;  // forward reference (see afxext.h)
namespace user
{
   class control_bar;          // forward reference (see afxext.h)
}
class CReBar;               // forward reference (see afxext.h)
class simple_frame_window;
class CDockBar;             // forward reference (see afxpriv.h)
class mini_dock_frame_window;    // forward reference (see afxpriv.h)
class CDockState;           // forward reference (see afxpriv.h)


#include "core/user/wndfrm/wndfrm.h"



/*


/////////////////////////////////////////////////////////////////////////////
// CMiniFrameWnd

// MiniFrame window styles
#define MFS_SYNCACTIVE      0x00000100L // syncronize activation w/ parent
#define MFS_4THICKFRAME     0x00000200L // thick frame all around (no tiles)
#define MFS_THICKFRAME      0x00000400L // use instead of WS_THICKFRAME
#define MFS_MOVEFRAME       0x00000800L // no sizing, just moving
#define MFS_BLOCKSYSMENU    0x00001000L // block hit testing on system menu

#pragma warning( disable: 4263 )
#pragma warning( disable: 4264 )
class CLASS_DECL_CORE CMiniFrameWnd : public frame_window
{
public:


   CMiniFrameWnd();
   ~CMiniFrameWnd();


   virtual bool create(const char * lpClassName, const char * lpWindowName,
      uint32_t dwStyle, const RECT& rect,
      ::user::interaction_child* pParentWnd = NULL, UINT nID = 0);
   virtual bool create_window_ex(uint32_t dwExStyle, const char * lpClassName, const char * lpWindowName,
      uint32_t dwStyle, const RECT& rect,
      ::user::interaction_child* pParentWnd = NULL, UINT nID = 0);


   bool OnNcActivate(bool bActive);
   LRESULT OnNcHitTest(point point);
   void OnSysCommand(UINT nID, LPARAM lParam);
   void OnGetMinMaxInfo(MINMAXINFO* pMMI);
   LRESULT OnFloatStatus(WPARAM wParam, LPARAM lParam);
   LRESULT OnQueryCenterWnd(WPARAM wParam, LPARAM lParam);
   bool OnNcCreate(::user::create_struct * lpcs);

public:
   static void CalcBorders(LPRECT lpClientRect, uint32_t dwStyle = 0, uint32_t dwExStyle = 0);

protected:
   virtual bool pre_create_window(::user::create_struct& cs);

protected:
   bool m_bSysTracking;
   bool m_bInSys;
   bool m_bActive;
   string m_strCaption;
};
#pragma warning( default: 4263 )
#pragma warning( default: 4264 )


typedef uint32_t DROPEFFECT;
class COleDataObject;   // forward reference (see afxole.h)

*/






#include "user_network_configuration.h"
#include "user_shell.h"
#include "user_keyboard_layout_cfg.h"



#include "user_document_manager.h"


#include "user_data_exchange.h"


//#include "user_view_container.h"
//#include "user_view_creator.h"

//#include "user_job.h"




#include "user_xfplayer_view_line.h"
#include "user_xfplayer_view_lines.h"

#include "user_tool_tip_tool.h"
#include "user_tool_tip_window.h"













struct __SIZEPARENTPARAMS;


namespace user
{


   class user;


} // namespace user

#include "app/appseed/core/user/fontopus/fontopus_view.h"

//#include "user_button.h"
//#include "user_meta_button.h"
//#include "user_bitmap_button.h"
#include "user_combo_box.h"
#include "user_combo_list.h"
#include "user_static_control.h"

//#include "user_button_control.h"

//#include "user_mdi_frame_window.h"
//#include "user_mdi_child_window.h"


//#include "user_tool_::user::command.h"


#include "user_toolbar_control.h"

//#include "user_control_bar.h"
#include "user_toolbar.h"
#include "user_statusbar.h"

//#include "user_view.h"
//#include "user_document.h"
//#include "user_document_template.h"

#include "user_split_bar.h"
#include "user_split_layout.h"

#include "user_form_window.h"
#include "user_form_mesh.h"
#include "user_form_list.h"
#include "user_form_view.h"
#include "user_form_list_view.h"
#include "user_split_bar.h"
#include "user_split_window.h"
#include "user_split_view.h"


#include "user_edit_plain_text_view.h"


//#include "user_form.h"
#include "user_tab_view.h"

#include "user_tab_view_update_hint.h"

#include "user_document_manager.h"

#ifdef LINUX
typedef struct _AppIndicator AppIndicator;
#endif


#include "user_notification_area.h"
#include "user_notify_icon.h"
#include "user_notify_icon_listener.h"







#include "user_slider.h"
#include "user_step_slider.h"
#include "user_elastic_slider.h"





#include "user_image_manager.h"

//#include "user_core.h"












//#include "core/filesystem/filemanager/filemanager.h"




#include "core/user/netuser/netuser.h"





#include "app/appseed/core/user/user/user_inline.h"


#define new AURA_NEW

//#include "core/user/user/user_form.h"
#include "core/user/user/user_form_mesh.h"


#include "core/user/user/user_font_list.h"

#include "math/calculator/calculator_plain_edit_view.h"

namespace core
{

   class CLASS_DECL_CORE session_docs :
      virtual public ::object
   {
   public:


      ::user::single_document_template *                 m_ptemplate_bergedge;
      ::user::single_document_template *                 m_ptemplate_platform;
      ::user::single_document_template *                 m_ptemplate_nature;
      ::user::single_document_template *                 m_ptemplate_html;
      sp(::user::document)                               m_pbergedgedocument;
      sp(::user::document)                               m_pplatformdocument;
      sp(::user::document)                               m_pnaturedocument;


      session_docs();

      virtual ~session_docs();

      void close_all_documents();

   };


   session_docs * create_session_docs();
   void destroy_session_docs(session_docs * pdocs);


} // namespace core

#include "core/user/user/user.inl"


