#pragma once


namespace process
{


   struct CLASS_DECL_AURA exit_status
   {


      int      m_iExitCode;
      int      m_iExitSignal;
      int      m_iExitStop;

      exit_status();


   };


   CLASS_DECL_AURA bool set_priority(int32_t priority);

   CLASS_DECL_AURA string app_id_to_app_name(string strId);


} // namespace process



#include "process_pipe.h"
#include "process_bidi_pipe.h"
#include "process_process.h"
#include "process_department.h"
