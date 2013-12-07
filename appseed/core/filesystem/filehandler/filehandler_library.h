#pragma once


namespace filehandler
{


   class CLASS_DECL_CORE library :
      virtual public ::core::library
   {
   public:


      library(sp(base_application) papp);
      virtual ~library();


      virtual bool handles_extension(const char * pszExtension);
      virtual bool handles_mime_type(const char * pszMimeType);

      virtual void get_extension_app(stringa & straAppId, const char * pszExtension);
      virtual void get_mime_type_app(stringa & straAppId, const char * pszMimeType);


      // impl
      virtual void get_extension_list(stringa & stra);
      virtual void get_mime_type_list(stringa & stra);


   };


} // namespace filehandler





