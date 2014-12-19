#pragma once


namespace user
{


   class impact_system;
   class interaction;


   class CLASS_DECL_AURA create_context  // Creation information structure
      // All fields are optional and may be NULL
   {
   public:
      // for creating new views
      sp(type)                               m_typeinfoNewView; // runtime class of ::user::impact to create or NULL
      ::user::document *              m_pCurrentDoc;
      ::aura::user::interaction *           m_puiNew;

      // for creating MDI children (CMDIChildWnd::LoadFrame)
      ::user::impact_system *     m_pNewDocTemplate;

      // for sharing ::user::impact/frame state from the original ::user::impact/frame
      ::aura::user::interaction *           m_pLastView;
      ::aura::user::interaction *           m_pCurrentFrame;

      // Implementation
      create_context();
      virtual ~create_context();

   };


} // namespace user


