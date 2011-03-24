#pragma once

namespace ca5
{

   namespace user
   {

      class CLASS_DECL_ca presence :
         virtual public ::radix::object
      {
      public:

         enum e_predefined_status
         {
            status_online     = 1000000006,
            status_offline    = 1000000025,
         };


         DWORD          m_dwLastActivity;
         __int64        m_iShortStatusCynceTag;
         string         m_strLongStatus;


         presence(::ca::application * papp);
         virtual ~presence();

         virtual void report_activity();


         virtual void defer_pulse_user_presence();
         virtual void pulse_user_presence();

      };

   } // namespace user

} // namespace ca5