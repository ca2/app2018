#pragma once



namespace uinteraction
{


   namespace frame
   {

      class WorkSet;
      class ControlBoxButton;

      class CLASS_DECL_CORE appearance :
         public CWorkSetDownUpInterface,
         virtual public signalizable
      {
         friend class WorkSet;
      public:

         appearance(WorkSet * pworkset);
         virtual ~appearance();

         class CTool001 :
            virtual public ::object
         {
         public:

            CTool001(sp(base_application) papp);

            void Update(::draw2d::graphics * pdc, LPCRECT lpcrect,
               COLORREF crHighlight,
               COLORREF crLight,
               COLORREF crBody,
               COLORREF crShadow,
               COLORREF crDkShadow);
            void draw(::draw2d::graphics * pdc);

            class CColorBezier
            {
            public:
               void draw(::draw2d::brush & brush, ::draw2d::graphics * pdc);
               point_array m_pointset;
               ::draw2d::pen  *   m_ppen;
               void clear();
            };
         public:
            ::draw2d::pen_sp      m_penHighlight;
            ::draw2d::pen_sp      m_penLight;
            ::draw2d::pen_sp      m_penShadow;
            ::draw2d::pen_sp      m_penDkShadow;
            ::draw2d::brush_sp      m_brushNull;
            ::draw2d::brush_sp      m_brushBody;

            point_array m_pointsetBody;
            array < CColorBezier, CColorBezier & > m_colorbezieraOutsideBorder;
         };

         CWorkSetDownUpInterface   *   m_pwndframeworkdownupinterface;
         bool                          m_fActive;
         bool                          m_bUseNc;

         bool                          m_bAutomaticModeSwitching;

         WorkSet *                     m_pworkset;
         bool                          m_bEnabled;
         EAppearanceMode               m_emode;
         EAppearanceTransparency       m_nTransparency;
         CTool001 *                    m_ptool001;

         bool IsNotifyIconEnabled();
         virtual sp(::user::interaction) get_guie();
         sp(::user::interaction) GetWnd();

         bool IsInAutomaticModeSwitching();
         void SetAutomaticModeSwitching(bool bSetting);


         void UpdateAppearanceMode(bool bFullScreen);
         void SetDownUpInterface(CWorkSetDownUpInterface * pinterface);
         bool WndFrameworkDownUpGetDownEnable();
         virtual bool WndFrameworkDownUpGetUpEnable();
         EAppearanceMode GetAppearanceMode();
         bool update();
         bool IsIconic();
         bool IsFullScreen();
         bool IsZoomed();
         bool IsEnabled();
         void Enable(bool bEnable);

         void SetTransparency(EAppearanceTransparency nTransparency);
         void SetAppearanceMode(EAppearanceMode nMode);
         void SetAppearanceMode();

         EHitTest hit_test(point pt);

         virtual void frame_Attach();
         virtual void frame_Detach();

      };

   } // namespace frame


} // namespace uinteraction


