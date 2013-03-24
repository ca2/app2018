#pragma once


namespace database
{


   namespace user
   {


      class CLASS_DECL_ca interaction :
         virtual public ::user::interaction,
         virtual public client
      {
      public:


         string         m_strDisplay;

         bool           m_bEnableSaveWindowRect;
         id             m_dataidWindow;


         interaction();
         virtual ~interaction();

         void install_message_handling(::ca::message::dispatch * pinterface);

         DECL_GEN_SIGNAL(_001OnCreate)
         DECL_GEN_SIGNAL(_001OnSize)
         DECL_GEN_SIGNAL(_001OnMove)
         DECL_GEN_SIGNAL(_001OnShowWindow)

         virtual void _001WindowRestore();

         string calc_display();


         virtual bool IsFullScreen();
         void WindowDataEnableSaveWindowRect(bool bEnable);
         bool WindowDataSaveWindowRect();
         bool WindowDataLoadWindowRect(bool bForceRestore = false);

         virtual bool LoadWindowRect_(class id key, class id idIndex, ::user::interaction * pWnd, bool bForceRestore = false);
         virtual bool SaveWindowRect_(class id key, class id idIndex, ::user::interaction * pWnd);


      };


   } // namespace user


} // namespace database


