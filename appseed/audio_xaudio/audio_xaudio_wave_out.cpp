#include "framework.h"
// thank you (casey) : tahnk oy coed prjsoetc China Cai Tao

namespace multimedia
{


   namespace audio_xaudio
   {



      wave_out::wave_out(::aura::application * papp) :
         ::object(papp),
         ::thread(papp),
         wave_base(papp),
         ::multimedia::audio::wave_out(papp)
      {

         m_estate             = state_initial;
         m_pthreadCallback    = NULL;

      }


      wave_out::~wave_out()
      {

      }


      void wave_out::install_message_routing(::message::sender * pinterface)
      {

         ::multimedia::audio::wave_out::install_message_routing(pinterface);

      }


      bool wave_out::init_thread()
      {

         if (!::multimedia::audio::wave_out::init_thread())
         {

            return false;

         }

         set_thread_priority(::multithreading::priority_time_critical);

         return true;

      }


      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose)
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_pxaudio.is_set() && m_pvoice != NULL && m_psourcevoice != NULL && m_estate != state_initial)
            return ::multimedia::result_success;

         m_iBuffer = 0;

         m_pthreadCallback = pthreadCallback;
         ::multimedia::e_result mmr;
         ASSERT(m_pxaudio.is_null());
         ASSERT(m_pvoice == NULL);
         ASSERT(m_psourcevoice == NULL);
         ASSERT(m_estate == state_initial);

         CoInitializeEx(nullptr,COINIT_MULTITHREADED);

         // by (indirect casey)
         if(FAILED(XAudio2Create(&m_pxaudio,0,XAUDIO2_DEFAULT_PROCESSOR)))
         {

            return ::multimedia::result_error;

         }

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
         // To see the trace output, you need to view ETW logs for this application:
         //    Go to Control Panel, Administrative Tools, Event Viewer.
         //    View->Show Analytic and Debug Logs.
         //    Applications and Services Logs / Microsoft / Windows / XAudio2.
         //    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK
         XAUDIO2_DEBUG_CONFIGURATION debug = {0};
         debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
         debug.BreakMask = XAUDIO2_LOG_ERRORS;
         m_pxaudio->SetDebugConfiguration(&debug,0);
#endif


         HRESULT hr;

         if(FAILED(hr = m_pxaudio->CreateMasteringVoice(&m_pvoice,uiChannelCount, uiSamplesPerSec)))
         {
            return ::multimedia::result_error;
         }
         int iSampleRate = XAUDIO2_MIN_SAMPLE_RATE;
         mmr = ::multimedia::result_success;
         m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
         m_pwaveformat->nChannels = uiChannelCount;
         m_pwaveformat->nSamplesPerSec = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample = sizeof(::multimedia::audio::WAVEBUFFERDATA) * 8 ;
         m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample  * uiChannelCount / 8;
         m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize = 0;
         sp(::multimedia::audio::wave) audiowave = Application.audiowave();

         //if(FAILED(hr = m_pxaudio->CreateSourceVoice(&m_psourcevoice,wave_format(),XAUDIO2_VOICE_NOSRC | XAUDIO2_VOICE_NOPITCH,1.0f,this)))
         if(FAILED(hr = m_pxaudio->CreateSourceVoice(&m_psourcevoice,wave_format(),0,1.0f,this)))
         {
            return ::multimedia::result_error;
         }

         if(mmr != ::multimedia::result_success)
         {

            TRACE(::multimedia::strerror(mmr));

            return mmr;

         }
         ::count iBufferCount;
//Opened:

         ::count iBufferSampleCount = 4096;

         if(epurpose == ::multimedia::audio::purpose_playback)
         {

            iBufferCount = 8;
            iBufferSampleCount = (1 << 9);

         }
         else if(epurpose == ::multimedia::audio::purpose_playground)
         {

            iBufferCount = 2;
            iBufferSampleCount = (1 << 9);

         }
         else
         {

            iBufferCount = 4;
            iBufferSampleCount = (1 << 9);

         }

         int iAlign = 2048;

         auto uiBufferSize = iBufferSampleCount * m_pwaveformat->nChannels * 2;

         uiBufferSize = MAX(uiBufferSize,2048);

         ASSERT((uiBufferSize % 2048) == 0);// Streaming size must be 2K aligned to use for async I/O

         iBufferSampleCount = uiBufferSize / (m_pwaveformat->nChannels * 2);



         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, iBufferCount,iAlign, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(m_pwaveformat->nChannels, iBufferCount, iBufferSampleCount);

         m_estate = state_opened;

         m_epurpose = epurpose;

         return ::multimedia::result_success;

      }



      ::multimedia::e_result wave_out::wave_out_close()
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_estate == state_playing)
         {
            wave_out_stop();
         }

         if(m_estate != state_opened)
            return ::multimedia::result_success;

         ::multimedia::e_result mmr;

//         int32_t i, iSize;
         //int32_t iSize;

         auto iSize =  wave_out_get_buffer()->GetBufferCount();

         //for(i = 0; i < iSize; i++)
         //{

         mmr = xaudio::translate(m_psourcevoice->Stop());


         //for(i = 0; i < iSize; i++)
         //{

         //   delete m_bufferptra[i];

         //}


         //m_psourcevoice->DestroyVoice();

         try
         {

            m_psourcevoice->DestroyVoice();

         }
         catch(...)
         {

         }

         //m_bufferptra.remove_all();

         m_psourcevoice = NULL;


         try
         {


            if(m_pvoice != NULL)
            {

               m_pvoice->DestroyVoice();

            }
         }
         catch (...)
         {

         }

         m_pvoice = NULL;

         try
         {


            if(m_pxaudio.is_set())
            {

               m_pxaudio->StopEngine();

            }

         }
         catch (...)
         {

         }

         m_pxaudio.Release();

         //mmr = xaudio::translate(waveOutClose(m_hwaveout));

         //m_hwaveout = NULL;

         ::multimedia::audio::wave_out::wave_out_close();

         return ::multimedia::result_success;

      }


      /*
      * Find the first occurrence of the byte string s in byte string l.
      */

      void *
      memmem(const void *l,size_t l_len,const void *s,size_t s_len)
      {
         register char *cur,*last;
         const char *cl = (const char *)l;
         const char *cs = (const char *)s;

         /* we need something to compare */
         if(l_len == 0 || s_len == 0)
            return NULL;

         /* "s" must be smaller or equal to "l" */
         if(l_len < s_len)
            return NULL;

         /* special case where s_len == 1 */
         if(s_len == 1)
            return memchr((void *) l,(int)*cs,l_len);

         /* the last position where its possible to find "s" in "l" */
         last = (char *)cl + l_len - s_len;

         for (cur = (char *)cl; cur <= last; cur++)
         {

            if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0)
            {

               return cur;

            }

         }

         return NULL;

      }


      void wave_out::wave_out_filled(index iBuffer)
      {

         synch_lock sl(m_pmutex);

         if(wave_out_get_state() != state_playing)
         {

            TRACE("ERROR wave_out::BufferReady while wave_out_get_state() != state_playing");

            return;

         }

         ::multimedia::audio::wave_buffer * pwbuffer = wave_out_get_buffer();

         ::multimedia::audio::wave_buffer::buffer * pbuffer = pwbuffer->get_buffer(iBuffer);

         ::multimedia::e_result mmr;

         XAUDIO2_BUFFER b;

         ZERO(b);

         b.pContext = pbuffer;
         b.AudioBytes = (UINT32) (pwbuffer->m_uiBufferSize);
         b.pAudioData = (const BYTE *)pbuffer->m_pData;
         b.Flags = m_bEOS ? XAUDIO2_END_OF_STREAM : 0;

         //single_lock sLock(m_pmutex,TRUE);

#if 0

         char sz[256];

         ZERO(sz);

         if(memmem(b.pAudioData,pwbuffer->m_uiBufferSize,sz,sizeof(sz)))
         {

            output_debug_string("too much zeros in audio buffer\n");

         }

#endif

         static DWORD g_dwLastBuffer;

         DWORD dwTick = get_tick_count();

         if(dwTick - g_dwLastBuffer > 50)
         {

            output_debug_string("too much delay for submitting audio buffer\n");

         }

         g_dwLastBuffer = dwTick;

         m_iBufferedCount++;

         mmr = xaudio::translate(m_psourcevoice->SubmitSourceBuffer(&b));

         VERIFY(::multimedia::result_success == mmr);

         if(mmr != ::multimedia::result_success)
         {

            m_iBufferedCount--;

         }

      }


      ::multimedia::e_result wave_out::wave_out_stop()
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_estate != state_playing && m_estate != state_paused)
            return ::multimedia::result_error;

         m_pprebuffer->stop();

         m_estate = state_stopping;

         //// waveOutReset
         //// The waveOutReset function stops playback on the given
         //// waveform-audio_xaudio output device and resets the current position
         //// to zero. All pending playback buffers are marked as done and
         //// returned to the application.
         m_mmr = xaudio::translate(m_psourcevoice->Stop());

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_opened;

         }

         return m_mmr;

      }


      ::multimedia::e_result wave_out::wave_out_pause()
      {

         single_lock sLock(m_pmutex, TRUE);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.

         m_mmr = xaudio::translate(m_psourcevoice->Stop());

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {
            m_estate = state_paused;
         }

         return m_mmr;

      }


      ::multimedia::e_result wave_out::wave_out_start(const imedia_position & position)
      {

         synch_lock sl(m_pmutex);

         //if(m_estate == state_playing)
         //   return result_success;

         //ASSERT(m_estate == state_opened || m_estate == state_stopped);

         //m_bEOS = false;

         //m_estate = state_playing;

         //m_pprebuffer->start(position);

         //for(index i = 0; i < wave_out_get_buffer()->GetBufferCount(); i++)
         //{

         //   wave_out_free(i);

         //}

         m_mmr = ::multimedia::audio::wave_out::wave_out_start(position);

         if(m_mmr != result_success)
         {

            return m_mmr;

         }

         m_mmr = xaudio::translate(m_psourcevoice->Start(0,XAUDIO2_COMMIT_NOW));

         return result_success;

      }


      ::multimedia::e_result wave_out::wave_out_restart()
      {

         single_lock sLock(m_pmutex, TRUE);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return ::multimedia::result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         m_mmr = xaudio::translate(m_psourcevoice->Start(0,XAUDIO2_COMMIT_NOW));

         ASSERT(m_mmr == ::multimedia::result_success);

         if(m_mmr == ::multimedia::result_success)
         {

            m_estate = state_playing;

         }

         return m_mmr;

      }


      imedia_time wave_out::wave_out_get_position_millis()
      {

         single_lock sLock(m_pmutex, TRUE);

//         ::multimedia::e_result                mmr;

         XAUDIO2_VOICE_STATE s;

         if(m_psourcevoice != NULL)
         {

            m_psourcevoice->GetState(&s);

            int64_t i = s.SamplesPlayed;
            i *= 1000;
            i /= m_pwaveformat->nSamplesPerSec;
            return i;

         }
         else
            return 0;


      }

      /*imedia_position wave_out::get_position_for_synch()
      {
         imedia_position position = get_position();
         if(m_pprebuffer != NULL && m_pprebuffer->m_pdecoder != NULL)
            return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->audio_plugin_get_lost_position_offset(position) - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         else
            return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
      }*/

      imedia_position wave_out::wave_out_get_position()
      {

         single_lock sLock(m_pmutex, TRUE);

//         ::multimedia::e_result                mmr;

         XAUDIO2_VOICE_STATE s;

         if (m_psourcevoice != NULL)
         {

            m_psourcevoice->GetState(&s);

            return s.SamplesPlayed;

         }
         else
         {

            return 0;

         }

      }


      void wave_out::wave_out_free(index iBuffer)
      {

         ::multimedia::audio::wave_out::wave_out_free(iBuffer);

      }


      void wave_out::wave_out_on_playback_end()
      {

         wave_out_stop();

         if(m_pprebuffer->m_pstreameffectOut != NULL)
         {
            ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
            m_pprebuffer->m_pstreameffectOut = NULL;
            delete peffect;
         }

         m_pplayer->post_event(::multimedia::audio::wave_player::EventPlaybackEnd);

      }


      ::count wave_out::wave_out_get_buffered_buffer_count()
      {

         return ::multimedia::audio::wave_out::wave_out_get_buffered_buffer_count();

      }


      WAVEFORMATEX * wave_out::wave_format()
      {

         xaudio::translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }


      void * wave_out::get_os_data()
      {
         return m_psourcevoice;
      }


      //void wave_out::wave_out_run_step()
      //{

      //   int iPlay =  -1;


      //}

      void wave_out::wave_out_prebuffer_eof()
      {

         ::multimedia::audio::wave_out::wave_out_prebuffer_eof();

      }


      void wave_out::OnStreamEnd()
      {

         //wave_out_on_playback_end();

      }


      //
      // Callback handlers, only implement the buffer events for maintaining play state
      //
      void wave_out::OnVoiceProcessingPassStart(UINT32 /*bytesRequired*/)
      {
      }
      void wave_out::OnVoiceProcessingPassEnd()
      {
      }

      void wave_out::OnBufferStart(void* pBufferContext)
      {

         ::multimedia::audio::wave_buffer::buffer * pbuffer = (::multimedia::audio::wave_buffer::buffer *)pBufferContext;

         //pbuffer->m_bIsPlaying =  true;

      }

      void wave_out::OnBufferEnd(void* pBufferContext)
      {

         //if(get_thread_priority() != ::multithreading::priority_time_critical)
         //{

         //   set_thread_priority(::multithreading::priority_time_critical);

         //}

         unsigned __int64 freq;
         QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
         double timerFrequency = (1.0 / freq);
         unsigned __int64 startTime;
         QueryPerformanceCounter((LARGE_INTEGER *)&startTime);


         ::multimedia::audio::wave_buffer::buffer * pbuffer = (::multimedia::audio::wave_buffer::buffer *)pBufferContext;

         //pbuffer->m_bIsPlaying = false;

         int32_t iBuffer = (int32_t)pbuffer->m_iIndex;

         m_psynththread->post_message(message_free, iBuffer);

         unsigned __int64 endTime;
         QueryPerformanceCounter((LARGE_INTEGER *)&endTime);
         double timeDifferenceInseconds = ((endTime - startTime) * timerFrequency);
         if(timeDifferenceInseconds > 0.040)
         {
            ::output_debug_string("too much delay to create audio\n");
         }



      }


      void wave_out::OnLoopEnd(void* /*bufferContext*/)
      {
      }
      void wave_out::OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/)
      {
      }
      //wave_out::run_step_thread::run_step_thread(wave_out * pout):
      //   ::thread(pout->get_app())
      //{
      //}


      //int32_t wave_out::run_step_thread::run()
      //{

      //   while(m_bRun && m_pout->m_estate == wave_out::state_playing)
      //   {
      //      m_pout->wave_out_run_step();
      //   }


      //   return 0;
      //}


   } // namespace audio_xaudio


} // namespace multimedia






