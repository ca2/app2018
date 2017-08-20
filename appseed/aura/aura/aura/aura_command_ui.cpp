#include "framework.h"


command_ui::command_ui(::aura::application * papp) :
   ::message::base(papp)
{
   
      m_iIndex                      = 0;
      m_iCount                      = 0;
      m_pMenu                       = NULL;
      m_pSubMenu                    = NULL;
      m_pParentMenu                 = NULL;
      m_pOther                      = NULL;
      m_bEnableChanged              = FALSE;
      m_bContinueRouting            = FALSE;
      m_bEnableIfHasCommandHandler  = true;
   
}

command_ui::command_ui(class ::signal * psignal):
   ::message::base(psignal)
{

   
   m_iIndex                      = 0;
   m_iCount                      = 0;
   m_pMenu                       = NULL;
   m_pSubMenu                    = NULL;
   m_pParentMenu                 = NULL;
   m_pOther                      = NULL;
   m_bEnableChanged              = FALSE;
   m_bContinueRouting            = FALSE;
   m_bEnableIfHasCommandHandler  = true;
   
}



void command_ui::reset(class ::signal * psignal)
{

   ::signal_details::reset(psignal);

}


// default command_ui implementation only works for Menu Items
void command_ui::Enable(bool bOn,::action::context actioncontext)
{

   if(m_pMenu != NULL)
   {

      if(m_pSubMenu != NULL)
         return; // don't change popup menus indirectly

      ENSURE(m_iIndex < m_iCount);

   }
   else
   {

      // enable/disable a control (i.e. child interaction_impl)
      ENSURE(m_pOther != NULL);

      Application.enable_window(m_pOther,bOn);

   }

   m_bEnableChanged = TRUE;

}

void command_ui::_001SetCheck(bool bCheck,::action::context actioncontext)
{

   _001SetCheck((check::e_check) (bCheck ? check::checked : check::unchecked),actioncontext);

}


void command_ui::_001SetCheck(check::e_check nCheck,::action::context actioncontext)
{

   if(m_pMenu != NULL)
   {
      if(m_pSubMenu != NULL)
         return; // don't change popup menus indirectly

      // place checkmark next to menu item
      ENSURE(m_iIndex < m_iCount);
   }
   else if(m_pOther != NULL)
   {

#ifdef WINDOWS

      // we can only check buttons or controls acting like buttons
      ENSURE(m_pOther != NULL);
      if(Application.send_message(m_pOther,WM_GETDLGCODE) & DLGC_BUTTON)
         Application.send_message(m_pOther,BM_SETCHECK,nCheck);
      // otherwise ignore it

#endif

   }
}


__STATIC void __load_dot_bitmap(); // for swap tuning

void command_ui::SetRadio(bool bOn,::action::context actioncontext)
{
   _001SetCheck(bOn != FALSE,actioncontext); // this default works for most things as well
   if(m_pMenu != NULL)
   {
      if(m_pSubMenu != NULL)
         return; // don't change popup menus indirectly

      // for menu item - use dot instead of checkmark
      ENSURE(m_iIndex < m_iCount);

      /*      if (afxData.hbmMenuDot == NULL)
      __load_dot_bitmap();    // in INIT segment
      */
   }
}

void command_ui::SetText(const char * lpszText,::action::context actioncontext)
{
   ENSURE_ARG(lpszText != NULL);
   ASSERT(__is_valid_string(lpszText));

   {
      ENSURE(m_pOther != NULL);
      Application.set_window_text(m_pOther,lpszText);
   }
}

bool command_ui::DoUpdate(command_target* pTarget,bool bDisableIfNoHndler)
{
   if(m_id.is_empty())
      return TRUE;     // ignore invalid IDs

   ENSURE_VALID(pTarget);

   m_bEnableChanged = FALSE;
   bool bResult = pTarget->_001SendUpdateCmdUi(this);

   if(!bResult)
      Enable(FALSE);

   return bResult;
}



