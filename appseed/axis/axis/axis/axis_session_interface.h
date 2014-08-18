#pragma once


namespace axis
{


   class CLASS_DECL_AXIS session_interface:
      virtual public ::axis::application_interface
   {
   public:


      session_interface *     m_pplatformcomposite;


      session_interface();
      virtual ~session_interface();


      virtual ::count get_monitor_count();
      virtual bool  get_monitor_rect(index iMonitor,LPRECT lprect);


      virtual ::visual::cursor * get_cursor();
      virtual ::visual::cursor * get_default_cursor();



   };


} // namespace axis
















