#pragma once


namespace file_watcher
{


   class CLASS_DECL_BOOT thread :
      public file_watcher,
      public thread_layer
   {
   public:

         
      virtual int32_t run();

         
   };


} // namespace file_watcher



