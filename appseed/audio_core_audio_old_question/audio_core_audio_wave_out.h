#pragma once


namespace multimedia
{


   namespace audio_core_audio
   {


      class  CLASS_DECL_AUDIO_CORE_AUDIO wave_out :
         virtual public toolbox,
         virtual public ::multimedia::audio::wave_out
      {
      public:


         UInt32                                    m_NumPacketsToRead;
         array < AudioStreamPacketDescription * >  m_PacketDescs;
         bool                                      m_bDone;
         int32_t                                   m_iBufferCount;
         int32_t                                   m_iBufferSampleCount;


         wave_out(sp(::aura::application) papp);
         virtual ~wave_out();


         void install_message_routing(::message::sender * pinterface) override;

         virtual imedia_time wave_out_get_position_millis() override;
         imedia_position wave_out_get_position() override;
         virtual void wave_out_buffer_ready(int iBuffer);

         virtual ::multimedia::e_result wave_out_open(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount);
         virtual ::multimedia::e_result wave_out_open_ex(::thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose);
         virtual ::multimedia::e_result wave_out_stop() override;
         virtual ::multimedia::e_result wave_out_close() override;
         virtual ::multimedia::e_result wave_out_pause() override;
         virtual ::multimedia::e_result wave_out_restart() override;
         virtual void * get_os_data();
         AudioQueueRef wave_out_get_safe_AudioQueueRef();

         ::multimedia::e_result wave_out_start(const imedia_position & position) override;


         virtual void wave_out_on_playback_end() override;

         virtual bool initialize_thread() override;
         virtual int32_t exit_thread() override;


         void AudioQueueBufferCallback(AudioQueueRef inAQ, AudioQueueBufferRef inCompleteAQBuffer);

         //int run();


         virtual bool on_run_step();


         void OnOpen();

      } ;


   } // namespace audio_core_audio


} // namespace multimedia



