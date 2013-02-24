#pragma once

namespace ca
{

   enum e_resource
   {
      resource_memory,
      resource_processing,
      resource_display_bandwidth,
      resource_blur_background,
      resource_blurred_text_embossing,
      resource_translucent_background,
   };

   class CLASS_DECL_ca savings :
      virtual public ::ca::object
   {
   public:
      savings(::ca::application * papp);
      virtual ~savings();

      flags_ex < e_resource > m_eresourceflagsShouldSave;
      flags_ex < e_resource > m_eresourceflagsWarning;

      virtual void save(e_resource eresource);
      virtual void try_to_save(e_resource eresource);

      virtual void use(e_resource eresource);
      virtual void may_use(e_resource eresource);

      virtual bool should_save(e_resource eresource);
      virtual bool is_trying_to_save(e_resource eresource);
    
      virtual void warn(e_resource eresource);

      virtual void clear_warning(e_resource eresource);

      virtual bool is_warning(e_resource eresource);

   };

} // namespace ca