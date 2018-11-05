#include "framework.h"

namespace aura
{
   
   
void shell_launcher::execute()
{
   
   if(m_iMode == 0)
   {
      
      if(m_vssOperation.is_empty())
      {
         m_dwResult = call_sync(m_vssFile, m_vssParameters, m_vssDirectory, m_iShow, -1, 284, NULL, 0);
      }
      else if(stricmp_dup(m_vssOperation, "open") == 0)
      {
         m_dwResult = call_sync("open", m_vssFile, m_vssDirectory, m_iShow, -1, 284, NULL, 0);
      }
              
   }
              
}
              
              
              
              
bool shell_launcher::succeeded()
{
         
   return m_dwResult >= 0;
         
}
              
              
              
              
              
              
              
              
              
              
} // namespace aura




