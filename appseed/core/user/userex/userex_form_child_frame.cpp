#include "framework.h"


form_child_frame::form_child_frame(::aura::application * papp) :
   object(papp),
   simple_frame_window(papp),
   simple_child_frame(papp)
{
}