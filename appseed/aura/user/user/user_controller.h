#pragma once


namespace user
{


   class CLASS_DECL_AURA controller :
      virtual public command_target,
      virtual public ::data::data_container_base,
      virtual public ::object
   {
   public:


      id                               m_id;
      string                           m_strTitle;
      ::file::path                     m_filepath;
      ::file::path                     m_filepathEx;
      bool                             m_bModified;
      bool                             m_bNew;
      bool                             m_bAutoDelete;     // TRUE => delete document when no more views
      bool                             m_bEmbedded;       // TRUE => document is being created by OLE
      property_set                     m_set;
      ::file::file_sp                  m_file;


      controller(::aura::application * papp);
      virtual ~controller();


      virtual void io(stream & serialize) override;


   };


} // namespace user