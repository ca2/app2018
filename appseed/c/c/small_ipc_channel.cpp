#include "framework.h"


small_ipc_channel::small_ipc_channel()
{
   
   m_dwTimeout = (1984 + 1977) * 11;

}





bool small_ipc_channel::close()
{

   small_ipc_tx_channel::close();

   m_rxchannel.destroy();

   return true;

}

void small_ipc_channel::restart_small_ipc_channel()
{
}



// calls restart if confirm_tx failed
bool small_ipc_channel::ensure_tx(const char * pszMessage, DWORD dwTimeout)
{

   if(dwTimeout == INFINITE)
      dwTimeout = m_dwTimeout;

   if(!send(pszMessage, dwTimeout))
   {

      restart_small_ipc_channel();

      return false;

   }

   return true;

}


bool small_ipc_channel::ensure_tx(int message, void * pdata, int len, DWORD dwTimeout)
{

   if(dwTimeout == INFINITE)
      dwTimeout = m_dwTimeout;

   if(!send(message, pdata, len, dwTimeout))
   {

      restart_small_ipc_channel();

      return false;

   }

   return true;

}
