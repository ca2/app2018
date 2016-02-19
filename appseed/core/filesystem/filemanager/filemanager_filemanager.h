#pragma once


namespace filemanager
{


   class CLASS_DECL_CORE filemanager :
      virtual public ::aura::department,
      virtual public ::filemanager::callback,
      virtual public ::filemanager::file_list_callback,
      virtual public ::database::client
   {
   public:


      sp(::filemanager::manager_template)                        m_ptemplateStd;
      sp(::filemanager::manager_template)                        m_ptemplateFs;
      sp(::user::multiple_document_template)     m_ptemplateForm;
      sp(::user::single_document_template)       m_ptemplateOperation;

      string                                       m_strCopy;
      id                                           m_idFileManager;


      filemanager(::aura::application * papp);
      virtual ~filemanager();

      bool initialize();


      virtual void InitializeFileManager(const char * pszMatter);
      
      
      inline ::filemanager::manager_template & std()     { return *m_ptemplateStd   ; }
      inline ::filemanager::manager_template & fs()      { return *m_ptemplateFs    ; }


      void OnFileManagerOpenFile(::filemanager::data * pdata, ::fs::item_array & itema);





      virtual bool do_prompt_file_name(var & varFile, UINT nIDSTitle, uint32_t lFlags, bool bOpenFileDialog, ::user::impact_system * ptemplate, ::user::document * pdocument);

      string get_initial_browse_path(const char * pszDefault = NULL);


      virtual void on_request(sp(::create) pcreatecontext);


   };



} // namespace filemanager


