#pragma once


namespace windows
{


   class CLASS_DECL_AURA folder_watch :
      virtual public ::file::folder_watch,
      virtual public ::thread
   {
   public:
      

      folder_watch(::aura::application * papp);


      virtual bool watch(const char * pszPath);

      virtual e_action translate_os_action(int32_t iAction);

      virtual int32_t run();

   };


} // namespace windows




