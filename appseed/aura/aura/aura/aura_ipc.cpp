#include "framework.h"


namespace aura
{


   namespace ipc
   {


      //ipc::ipc(::aura::application * papp) :
      //   object(papp),
      //   base(papp),
      //   tx(papp),
      //   m_rx(papp)
      //{

      //   m_dwTimeout = (5000) * 11;

      //}


      //ipc::~ipc()
      //{


      //}


      bool ipc::close()
      {

         tx::close();

         m_rx.destroy();

         return true;

      }

      void ipc::restart_aura_ipc()
      {
      }



      // calls restart if confirm_tx failed
      bool ipc::ensure_tx(const char * pszMessage, duration durationTimeout)
      {

         if(!send(pszMessage, durationTimeout))
         {

            restart_aura_ipc();

            return false;

         }

         return true;

      }


      bool ipc::ensure_tx(int message, void * pdata, int len, duration durationTimeout)
      {

         if(message == WM_APP + WM_USER)
         {

            if(!is_tx_ok())
            {

               restart_aura_ipc();

               return false;

            }

         }
         else if(!send(message, pdata, len, durationTimeout))
         {

            restart_aura_ipc();

            return false;

         }

         return true;

      }


      string app_install(string strPlatform)
      {

         return "::ca2::fontopus::ca2_spaboot_install_" + strPlatform + "::7807e510-5579-11dd-ae16-0800200c7784";

      }


   } // namespace ipc


} // namespace aura



