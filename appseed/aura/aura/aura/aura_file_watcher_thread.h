#pragma once


namespace file_watcher
{


   class CLASS_DECL_AURA thread :
      public file_watcher,
      public ::thread
   {
   public:

      
      thread(::aura::application * papp);

      virtual int32_t run();


   };


} // namespace file_watcher



