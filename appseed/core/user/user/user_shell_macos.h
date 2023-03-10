#pragma once


namespace user
{

   
   namespace shell
   {


      class CLASS_DECL_CORE macos :
         virtual public ::user::shell::shell
      {
      public:

         class per_fork
         {
         public:

            string                                                      m_strFolder;

            per_fork(bool bInit = true);
            ~per_fork();

            void init();

         };

         string m_strEmpty;
         string   m_strFoo;
         bool                                                        m_bStarted;
         delay_thread *                                              m_pdelayRelease;
         pointer_array < image_key * >                                     m_keyptra;
         manual_reset_event                                          m_evKey;
         mutex                                                       m_mutexQueue;
         spa(::thread)                                               m_threadaGetImage;

         
         macos(::aura::application * papp);
         virtual ~macos();



         virtual int32_t get_image_foo(oswindow oswindow, const string & strExtension, e_file_attribute eattribute, e_icon eicon, COLORREF crBk = 0) override;
         virtual int32_t get_image(oswindow oswindow, const string & strPath, e_file_attribute eattribute, e_icon eicon, COLORREF crBk = 0) override;

         virtual ::user::shell::e_folder get_folder_type(::aura::application * papp, const unichar * lpcszPath) override;
         virtual ::user::shell::e_folder get_folder_type(::aura::application * papp, const char * lpcszPath) override;

         
         int32_t get_image(per_fork * pfork, oswindow oswindow, image_key key, const unichar * lpcszExtra, COLORREF crBk);
         int32_t get_image_by_extension(per_fork * pfork, oswindow oswindow, image_key & key, COLORREF crBk);
         int32_t get_foo_image(per_fork * pfork, oswindow oswindow, image_key key, COLORREF crBk);

         
         virtual void do_initialize() override;
         

      };


   } // namespace windows


} // namespace filemanager


