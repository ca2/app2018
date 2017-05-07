#include "framework.h"


namespace tts
{


   speaker::speaker(::aura::application * papp):
      object(papp)
   {

   }

   speaker::~speaker()
   {
   }


   bool speaker::speak(const string & strText)
   {

      if (m_pspeakerFallback.is_null())
      {

         return false;

      }

      return m_pspeakerFallback->speak(strText);

   }

   bool speaker::speak(const string & strLang, const string & strText, bool bSynch)
   {

      if (m_pspeakerFallback.is_null())
      {

         return false;

      }

      return m_pspeakerFallback->speak(strLang, strText, bSynch);

   }

   bool speaker::stop(string strLang)
   {
   
      if (m_pspeakerFallback.is_null())
      {

         return false;

      }

      return m_pspeakerFallback->stop(strLang);

   }

   
   bool speaker::is_speaking(string strLang)
   {

      if (m_pspeakerFallback.is_null())
      {

         return false;

      }

      return m_pspeakerFallback->speak(strLang);

   }



} // namespace tts




