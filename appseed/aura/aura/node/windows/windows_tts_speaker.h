#pragma once

//////////////////////////////////////////////////////////////////////////
//
// FILE: TextSpeaker.h
//
// Defines a simple C++ class to speak some text.
//
// by Giovanni Dicanio <gdicanio@mvps.org>
//
// 2010, December 28th
//
//////////////////////////////////////////////////////////////////////////


#include <sapi.h>        // SAPI (for windows::tts::speaker)


namespace windows
{


   namespace tts
   {


      //------------------------------------------------------------------------
      // Simple class to speak some text (using SAPI).
      //------------------------------------------------------------------------
      class speaker:
         virtual public ::tts::speaker
      {
      public:

         
         string_map < ::windows::comptr < ISpVoice > >         m_voice;
         string_map < ::windows::comptr < ISpObjectToken > >   m_token;
         string_map < DWORD > m_time;
         string_map < bool > m_tts;
         //--------------------------------------------------------------------
         // Initializes the text speaker.
         //--------------------------------------------------------------------
         speaker(::aura::application * papp);
         virtual ~speaker();

         virtual bool initialize(string strLang);
         virtual bool initialize_translator(string strLang);
         virtual bool finalize(string strLang);
         virtual bool finalize_translator(string strLang);

         //--------------------------------------------------------------------
         // Speaks some text.
         // (The input text must not be empty.)
         //--------------------------------------------------------------------
         virtual bool speak(const string & text) override;

         virtual bool speak(const string & strLang, const string & text, bool bSync = false) override;

         virtual bool is_speaking(string strLang) override;

         virtual bool stop(string strLang) override;


      };



   } // namespace tts

} // namespace windows




