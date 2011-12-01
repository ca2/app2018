#pragma once


#include "cube5/cube5.h"


namespace cube8
{

   class application;

} // namespace cube8

#undef App
#define cubeApp(pcaapp) (pcaapp->cast_app < cube8::application >())
#define cubeApplication (cubeApp(get_app()))
#define App(pcaapp) cubeApp(pcaapp)
#define Application (App(get_app()))



#include "bergedge/bergedge.h"

#include "cube8_application.h"


#include "cube8_system.h"
#include "cube8_system_ca_template.h"
#include "cube8_system_allocate.h"
#include "cube8_system_template_trace.h"
#include "collection/gen_lemon_array.h"



#include "cube8_main.h"

#include "plugin/plugin.h"

