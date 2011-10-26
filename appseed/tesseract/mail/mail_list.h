#pragma once


namespace tesseract
{

namespace mail
{

   class document;

   class CLASS_DECL_CA2_TESSERACT list :
      virtual public ::userbase::view,
      virtual public ::mail::list_interface
   {
   public:


      simple_list_header_control    m_headerctrl;


      list(::ca::application * papp);
      virtual ~list();

      void open_folder(const char * pszFolder);

      document * get_document();

      DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)

      void _001OnClick(UINT uiFlags, point point);

      virtual void install_message_handling(::user::win::message::dispatch * pdispatch);
      

   };

   } // namespace mail


} // namespace tesseract


