#pragma once


namespace user
{


   class CLASS_DECL_CORE document_manager :
      virtual public ::user::server
   {
   public:


      comparable_array < sp(::aura::impact_system) >  m_templateptra;


      static const char gen_ShellOpenFmt[];
      static const char gen_ShellPrintFmt[];
      static const char gen_ShellPrintToFmt[];
      static const char gen_DefaultIconFmt[];
      static const char gen_ShellNewFmt[];

#define DEFAULT_ICON_INDEX 0

      static  const char gen_IconIndexFmt[];
      static  const char gen_Command[];
      static  const char gen_OpenArg[];
      static  const char gen_PrintArg[];
      static  const char gen_PrintToArg[];
      static  const char gen_DDEArg[];

      static  const char gen_DDEExec[];
      static  const char gen_DDEOpen[];

      static const char gen_DDEPrint[];
      static const char gen_DDEPrintTo[];

      static const char gen_ShellNewValueName[];
      static const char gen_ShellNewValue[];


      document_manager(::aura::application * papp);
      virtual ~document_manager();


      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      virtual void add_document_template(sp(::aura::impact_system) ptemplate);
      virtual ::count get_template_count() const;
      virtual sp(::aura::impact_system) get_template(index index) const;
      virtual void RegisterShellFileTypes(bool bCompat);
      void UnregisterShellFileTypes();
      virtual void request(sp(::create) pcreatecontext); // open named file
      virtual bool save_all_modified(); // save before exit
      virtual void close_all_documents(bool bEndSession); // close documents before exiting
      virtual ::count get_open_document_count();

      virtual bool do_prompt_file_name(var & varFile, UINT nIDSTitle, uint32_t lFlags, bool bOpenFileDialog, ::aura::impact_system * ptemplate, ::user::document * pdocument);

      virtual bool OnDDECommand(LPTSTR lpszCommand);
      virtual void _001OnFileNew();
      virtual void on_file_open();

      ::count get_document_count(); // helper to ::count number of total documents



   };


} // namespace user






