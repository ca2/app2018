#pragma once


class CLASS_DECL_ca2 XfplayerViewLines :
   virtual public array_app_alloc<XfplayerViewLine, XfplayerViewLine &>,
   virtual public gen::signalizable
{
public:


   XfplayerViewLineSelection   m_selection;
   ::user::interaction *      m_pinteraction;
   index                        m_iFirstVisible;
   index                        m_iLastVisible;


   XfplayerViewLines(::ca::application * papp);


   void SetBlend(double dBlend);
   index FindLine(XfplayerViewLine * pline);
   void SetRenderWindow(::ca::window * pWnd);
   void SetEffect(int iEffect);
   void set_user_interaction(::user::interaction * pinteraction);
   void Prepare(XfplayerViewLine * lpViewLine);
   void Prepare();
    
   index GetFirstVisibleLineIndex();
   index GetLastVisibleLineIndex();
   void OnChildSetVisible(XfplayerViewLine * lpViewLine, bool bVisible);
   
   user::e_line_hit hit_test(POINT &ptCursor, strsize &iLine, strsize &iChar);

   void InstallMessageHandling(::gen::message::dispatch *pinterface);

   virtual ~XfplayerViewLines();
    
   DECL_GEN_SIGNAL(OnMouseMove)
   DECL_GEN_SIGNAL(OnLButtonDown)
   DECL_GEN_SIGNAL(OnLButtonUp)
   DECL_GEN_SIGNAL(OnTimer)
   DECL_GEN_SIGNAL(OnSetCursor)

   inline XfplayerViewLineSelection & GetSelection();


};



