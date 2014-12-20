#pragma once


namespace user
{


   class place_holder;


   class CLASS_DECL_AXIS view_creator_data
   {
   public:


      enum eflag
      {
         
         flag_hide_all_others_on_show
            
      };


      id                               m_id;
      ::user::interaction *            m_pwnd;
      ::user::place_holder *           m_pholder;
      ::aura::document *     m_pdoc;
      id                               m_idSplit;
      bool                             m_bTabSplitSwap;
      int32_t                              m_iExtendOnParent;
      flags < eflag >                  m_eflag;
      string                           m_strTitle;
      void *                           m_pviewdata;
      rect m_rectCreate;

      view_creator_data();


      ::user::interaction *   get_wnd();


   };


} // namespace user



