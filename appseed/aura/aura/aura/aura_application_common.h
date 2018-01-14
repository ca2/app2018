#pragma once


class service_base;
class not_installed;

#ifdef 

namespace hotplugin
{


   class host;
   class plugin;


} // namespace hotplugin

#endif


namespace file
{


   namespace dir
   {


      class application;
      class system;


   } // namespace dir


   class application;
   class system;


}  // namespace file


namespace aura
{


   class ipi;
   class application_heap;


} // namespace aura


class user_interaction_ptr_array;

class thread_startup;

namespace primitive { class command;  }



#include "aura/primitive/collection/collection_smart_pointer_array.h"
#include "aura/aura/aura/aura_draw_interface.h"
#include "aura/primitive/primitive_signal.h"
#include "aura/aura/aura/aura_command_msg.h"
#include "aura/primitive/primitive_check.h"
#include "aura/aura/aura/aura_::user::command.h"
#include "aura/aura/message/message.h"
#include "aura/aura/aura/aura_command_target.h"
#include "aura/user/user/user_primitive.h"
#include "aura/multithreading/multithreading_common.h"
#include "aura/multithreading/multithreading_pred_holder.h"
#include "aura/multithreading/multithreading_thread.h"
#include "aura/primitive/math/math_scalar_enum.h"
#include "aura/primitive/math/math_scalar.h"
#include "aura/filesystem/file/file_listing_provider.h"
#include "aura/filesystem/filesystem/filesystem_path.h"



//#include "aura/primitive/map.h"
//#include "aura/primitive/collection/collection_numeric_array_range.h"
//#include "aura/primitive/collection/collection_numeric_array.h"
//#include "aura/primitive/collection/collection_string_array.h"
