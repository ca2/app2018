#include "framework.h"


namespace axis
{


   namespace static_start
   {


      CLASS_DECL_AXIS void init()
      {

         xxdebug_box("axis.dll axis_static_start (0)", "box", MB_OK);



#if defined(LINUX)

//         set_defer_process_x_message(&axis_defer_process_x_message);

//         set_TranslateMessage(&axis_TranslateMessage);
//
//         set_DispatchMessage(&axis_DispatchMessage);
//
//         oswindow_data::s_pdataptra = new oswindow_dataptra;
//
//         oswindow_data::s_pmutex = new mutex;
//
//         osdisplay_data::s_pdataptra = new osdisplay_dataptra;
//
//         osdisplay_data::s_pmutex = new mutex;

#endif // defined(LINUX)



      }














































      CLASS_DECL_AXIS void term()
      {

//#if defined(LINUX)
//
//         delete osdisplay_data::s_pmutex;
//
//         osdisplay_data::s_pmutex = NULL;
//
//         delete osdisplay_data::s_pdataptra;
//
//         osdisplay_data::s_pdataptra = NULL;
//
//         delete oswindow_data::s_pmutex;
//
//         oswindow_data::s_pmutex = NULL;
//
//         delete oswindow_data::s_pdataptra;
//
//         oswindow_data::s_pdataptra = NULL;
//
//#endif // defined(LINUX)




      }



   } // namespace static_start


} // namespace axis





