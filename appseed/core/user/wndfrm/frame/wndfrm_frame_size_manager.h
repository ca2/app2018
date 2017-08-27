#pragma once


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         class CLASS_DECL_CORE SizeManager :
            virtual public ::message::receiver
         {
         protected:
            friend class         WorkSet;

            WorkSet *            m_pworkset;
            EHitTest             m_ehittestMode;
            EHitTest             m_ehittestCursor;
            point                m_ptCursorOrigin;
            rect                 m_rcWindowOrigin;
            size                 m_minSize;
            size                 m_minBorder;
            UINT                 m_uiSWPFlags;
            EGrip                  m_egripMask;
            int32_t                  m_iPaintCount;
         public:
            SizeManager(WorkSet * pworkset);
            virtual ~SizeManager();

            EHitTest hit_test(point ptCursor);
            //void _000InstallEventHandling(iguimessage::MessageDispatch * pdispatch);

            bool _000OnLButtonDown(::message::mouse * pmouse);
            bool _000OnMouseMove(::message::mouse * pmouse);
            bool _000OnLButtonUp(::message::mouse * pmouse);

            bool Relay(::message::mouse * pmouse);

            void NotifyFramework(EHitTest emode);
            EGrip EModeToEGrip(EHitTest emode);
            EGrip GetGripMask();
            void SetGripMask(EGrip egrip);
            bool IsSizing();
            void SetSWPFlags(UINT uiFlags);
            void SetMinSize(size size);
            ::visual::e_cursor translate(EHitTest ehittest);
            bool update(WorkSet * pset);

            sp(::user::interaction) GetSizingWindow();
            sp(::user::interaction) GetEventWindow();


            void SizeWindow(sp(::user::interaction) pwnd, point pt, bool bTracking);
            void MoveWindow(sp(::user::interaction) pwnd, const RECT & rect);

            
            void message_handler(::user::interaction * pui, ::message::base * pbase);


         };


      } // namespace frame


   } // namespace wndfrm


} // namespace user






