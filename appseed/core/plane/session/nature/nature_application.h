#pragma once


namespace nature
{


   class database;


   class CLASS_DECL_CORE application :
      virtual public ::application
   {
   public:


      sp(::user::single_document_template)  m_ptemplate_html;
      database *                    m_pdatabase;


      application();
      virtual ~application();

      using thread::construct;
      using ::application::construct;

      virtual void construct();

      virtual bool initialize_instance();
      virtual int32_t exit_instance();

      bool InitializeLocalDataCentral();

      void _001OnFileNew();

      virtual bool _001OnCmdMsg(base_cmd_msg * pcmdmsg);

   };


} // namespace nature