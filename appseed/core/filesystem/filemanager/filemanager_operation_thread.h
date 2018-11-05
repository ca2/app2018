﻿#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE operation_thread:
      public simple_thread
   {
   public:


      ::filemanager::e_state            m_estate;
      operation_view *                 m_pview;
      spa(::filemanager::operation)              m_fileoperationa;
      int32_t                          m_iOperation;
      bool                             m_bStep;
      mutex                            m_mutexFileOperationA;
      mutex                            m_mutexStep;
      bool                             m_bStop;


      operation_thread(::aura::application * papp);

      void queue_copy(::file::listing & stra,const ::file::path & pszDest,const ::file::path & pszSrcBase,bool bExpand,bool bReplaceAll,bool bDeleteOriginOnSuccessfulCopy, sp(::user::interaction) oswindowCallback = NULL,UINT uiMessage = WM_USER,WPARAM wparam = 0x1000);
      void kick();

      int32_t get_item_count();
      string get_item_message(int32_t iItem);
      double get_item_progress(int32_t iItem);

      double get_progress_rate();

      bool step();

      virtual void run() override;

   };


} // namespace filemanager













