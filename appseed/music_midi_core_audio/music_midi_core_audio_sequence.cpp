#include "framework.h"


OSStatus LoadSMF(const char* data, int dataSize, MusicSequence& sequence, MusicSequenceLoadFlags loadFlags);


CFStringRef ConnectedEndpointName(MIDIEndpointRef endpoint);


// Get general midi notifications
void MyMIDINotifyProc (const MIDINotification  *message, void *refCon) {
   printf("MIDI Notify, messageId=%d,", message->messageID);
}

static void MyMIDIReadProc(const MIDIPacketList * pktlist, void * refCon, void * connRefCon)
{

   ::music::midi_core_midi::sequence * pseq = (::music::midi_core_midi::sequence * )refCon;
   
   MIDIPacket *packet = (MIDIPacket *)pktlist->packet;

   for (int i = 0; i < pktlist->numPackets; i++)
   {
      
      Byte midiStatus = packet->data[0];
      Byte velocity = packet->data[2] & 0x7F;
      

      if(!pseq->m_bStart)
      {
         if(((midiStatus) &0xf0) == music::midi::NoteOn && velocity > 0)
      {
         pseq->m_bStart = true;
         pseq->m_uiStart = get_tick_count();
      }
      }
      pseq->m_posPlay = packet->timeStamp;
      //Byte midiCommand = midiStatus >> 4;
   
      Byte note = packet->data[1] & 0x7F;

      OSStatus result = noErr;
      
      result = MusicDeviceMIDIEvent(pseq->m_pau->m_synth_unit, midiStatus, note, velocity, 0);
      
      packet = MIDIPacketNext(packet);
      
   }
   
}


namespace music
{
   
   
   namespace midi_core_midi
   {
      
      
      sequence::sequence(::aura::application * papp) :
      ::object(papp),
      ::music::midi::object(papp),
      ::ikaraoke::karaoke(papp),
      ::music::midi::sequence(papp)
      {
         
         defer_create_mutex();
         
         m_posPlay = 0;
         
         m_buffera.Initialize(16, 4 * 1024, 0);
         
         m_midicallbackdata.m_psequence = this;
         
//         m_iClient = 128;
         //m_iClient = 16;
  //       m_iPort = 0;
         
         
      };
      
      sequence::~sequence()
      {
         
         CloseFile();
         
      }
      
      
      
      
       
      
      /***************************************************************************
       *
       * seqPreroll
       *
       * Prepares the file for playback at the given position.
       *
       * pSeq                      - The sequencer instance.
       *
       * lpPreroll                 - Specifies the starting and ending tick
       *                             positions to play between.
       *
       * Returns
       *   ::multimedia::result_success If the operation is successful.
       *
       *   ::music::EFunctionNotSupported If the sequencer instance is not
       *     opened or prerolled.
       *
       * open the device so we can initialize channels.
       *
       * Loop through the tracks. For each track, seek to the given position and
       * send the init data SMF gives us to the handle.
       *
       * Wait for all init buffers to finish.
       *
       * Unprepare the buffers (they're only ever sent here; the sequencer
       * engine merges them into a single stream during normal playback) and
       * refill them with the first chunk of data from the track.
       *
       *
       ****************************************************************************/
      ::multimedia::e_result sequence::Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow)
      {
         UNREFERENCED_PARAMETER(pthread);
         single_lock sl(m_pmutex, TRUE);
         // int32_t i;
         // midi_callback_data *      lpData = &m_midicallbackdata;
         ::music::e_result     smfrc;
         ::multimedia::e_result                mmrc        = ::multimedia::result_success;
         //         MIDIPROPTIMEDIV         mptd;
         LPMIDIHDR               lpmh = NULL;
         // LPMIDIHDR               lpmhPreroll = NULL;
         // uint32_t                    uDeviceID;
         
         
         ASSERT(m_iOpenMode == ::music::midi::file::OpenForPlaying
                || IsInSpecialModeV001());
         
         m_flags.unsignalize(FlagEOF);
         
         m_mmrcLastErr = ::multimedia::result_success;
         
         if(GetState() != status_opened &&
            GetState() != status_pre_rolled &&
            !IsInSpecialModeV001())
            return ::music::translate(::music::EFunctionNotSupported);
         
         m_tkBase = lpPreroll->tkBase;
         m_tkEnd = lpPreroll->tkEnd;
//         if(m_pseq != NULL)
         {
            // Recollect buffers from MMSYSTEM back into free queue
            //
            SetState(::music::midi::sequence::status_reset);
            
  //          seq_free_context(m_pseq);
            
         }
    //     else
         {
         }
         
         //m_uBuffersInMMSYSTEM = 0;
         SetState(::music::midi::sequence::status_pre_rolling);
         
         //
         // We've successfully opened the file and all of the tracks; now
         // open the MIDI device and set the time division.
         //
         // NOTE: seqPreroll is equivalent to seek; device might already be open
         //
         
         
         mmrc = ::multimedia::result_success;
         
         
         
         
         // Create a new music sequence
         // Initialise the music sequence
         //NewMusicSequence(&s);
         

         
//         maxLen;
         
         //while (1) { // kill time until the music is over
           // usleep (3 * 1000 * 1000);
           // MusicTimeStamp now = 0;
           // MusicPlayerGetTime (p, &now);
            //if (now >= maxLen)
         //}
         
         // Stop the player and dispose of the objects
         
         
         
         
         m_buffera.Reset();
         lpmh = &m_buffera[0].m_midihdr;
         
         if(IsInSpecialModeV001())
         {
            
            if(m_uiSpecialModeV001Operation == SPMV001GMReset)
            {
               /*const uchar gmModeOn[] = {
                //        0x00, 0x00, 0x00, 0x00,
                //        0x00, 0x00, 0x00, 0x00,
                //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x06, 0x00, 0x00, MEVT_LONGMSG,
                0xf0, 0x7e, 0x7f, 0x09,
                0x01, 0xf7, 0x00, 0x00};
                char * lpch = lpmh->lpData + lpmh->dwBytesRecorded;
                lpmh->dwBytesRecorded += sizeof(gmModeOn);
                memcpy(lpch, gmModeOn, sizeof(gmModeOn));
                SetSpecialModeV001Flag(false);*/
            }
            
         }
         
         smfrc = file()->WorkSeek(m_tkBase);
         
         m_tkPrerollBase = get_file()->GetPosition();
         
         m_flags.unsignalize(FlagEOF);
         
         file()->m_flags &= ~::music::midi::file::EndOfFile;
         
         
         /*
          
          for(i = 1; i < m_buffera.get_size(); i++)
          {
          
          lpmh = m_buffera[i].GetMidiHdr();
          
          smfrc = file()->WorkStreamRender(lpmh, m_tkEnd, m_cbPrerollNominalMax);
          
          if(::music::success != smfrc && ::music::SEndOfFile != smfrc)
          {
          TRACE( "SFP: smfReadEvents() -> %u", (uint32_t)smfrc);
          mmrc = ::music::translate(smfrc);
          //if(bThrow)
          //{
          //  SetState(status_opened);
          //  _throw(exception(mmrc, MIDIPLAYERPRERROLLREADEVENTSEXCEPTION));
          //}
          
          // goto seq_Preroll_Cleanup;
          // }
          //
          //
          //
          // if (::music::SEndOfFile == smfrc)
          // {
          //   m_flags.signalize(FlagEOF);
          //   break;
          // }
          //}
          
         
         // mmrc = m_buffera.midiOutPrepareHeader((HMIDIOUT) m_hstream);
         // if (mmrc != ::multimedia::result_success)
         // {
         // TRACE( "midiOutPrepare(preroll) -> %lu!", (uint32_t)mmrc);
         // mmrc = MCIERR_DEVICE_NOT_READY;
         // if(bThrow)
         // {
         // SetState(status_opened);
         // _throw(exception(get_app(), mmrc, MIDIPLAYERPRERROLLPREPAREHEADEREXCEPTION));
         // }
         // else
         // {
         // goto seq_Preroll_Cleanup;
         // }
         // }
          
         
         
         //mmrc = m_buffera.midiStreamOut(m_hstream);
         //if (mmrc != ::multimedia::result_success)
         //{
         //}
         //         m_uBuffersInMMSYSTEM +=(uint32_t)  m_buffera.get_size();
          
          */
         
      seq_Preroll_Cleanup:
         
         if (mmrc != ::multimedia::result_success)
         {
            SetState(status_opened);
            m_flags.unsignalize(::music::midi::sequence::FlagWaiting);
         }
         else
         {
            SetState(status_pre_rolled);
         }
         
         return mmrc;
      }
      
      /***************************************************************************
       *
       * seqStart
       *
       * Starts playback at the current position.
       *
       * pSeq                      - The sequencer instance.
       *
       * Returns
       *   ::multimedia::result_success If the operation is successful.
       *
       *   ::music::EFunctionNotSupported If the sequencer instance is not
       *     stopped.
       *
       *   MCIERR_DEVICE_NOT_READY If the underlying MIDI device could
       *     not be opened or fails any call.
       *
       * The sequencer must be prerolled before seqStart may be called.
       *
       * Just feed everything in the ready queue to the device.
       *
       ***************************************************************************/
      ::multimedia::e_result sequence::Start()
      {
         
         single_lock sl(m_pmutex, TRUE);
         
         if (::music::midi::sequence::status_pre_rolled != GetState())
         {
            
            TRACE( "seqStart(): State is wrong! [%u]", GetState());
            
            return ::music::translate(::music::EFunctionNotSupported);
            
         }
         
         m_evMmsgDone.ResetEvent();
         
         ::multimedia::e_result mmrc = ::multimedia::result_success;
         
         sl.unlock();
         
         if(mmrc == ::multimedia::result_success)
         {
            
            thread()->PostMidiSequenceEvent(this, ::music::midi::sequence::EventMidiPlaybackStart);
            
         }
         
         return mmrc;
         
      }
      
      ::multimedia::e_result sequence::seq_start()
      {
         
         single_lock sl(m_pmutex, TRUE);
         
         if(GetState() != status_pre_rolled)
            return ::music::translate(::music::EFunctionNotSupported);
         
         m_posPlay = 0;
         // CFStringRef nameRef;
         // MIDIEndpointRef portRef;
         // char name[128];

         int portNumber = 0;
         std::string stringName;
         CFRunLoopRunInMode( kCFRunLoopDefaultMode, 0, false );
         if ( portNumber >= MIDIGetNumberOfDestinations() )
{
         }
         
         //char name[2048];
//         portRef = MIDIGetDestination( 0 );
//         nameRef = ConnectedEndpointName(portRef);
//         CFStringGetCString( nameRef, name, sizeof(name), CFStringGetSystemEncoding());
//         CFRelease( nameRef );
//
//         stringName = name;
         
         //m_pcmo = new CoreMidiOutput(stringName);
         
         //m_bStart = false;
         
         m_pau = new AudioUnitOutput(NULL);
         
         //m_pau->note_on(128, 128, 0);
         
         OSStatus result;
         
//         m_cl = NULL;
//         
//         
//         result = CAClockNew(0, &m_cl);
//         
//         
//         if(result != noErr)
//         {
//            
//            return translate_os_status(result);
//            
//         }
//         // Create a client
         //MIDIClientRef virtualMidi;
         result = MIDIClientCreate(CFSTR("Virtual Client"),
                                   MyMIDINotifyProc,
                                   NULL,
                                   &m_virtualMidi);


         if(result != noErr)
         {

            return translate_os_status(result);

         }

         // Create an endpoint
         //MIDIEndpointRef virtualEndpoint;
         result = MIDIDestinationCreate(m_virtualMidi, CFSTR("Virtual Destination"), MyMIDIReadProc, this, &m_virtualEndpoint);


         if(result != noErr)
         {

            return translate_os_status(result);

         }
         m_sequence = NULL;

         OSStatus os = LoadSMF((const char *)m_pfile->get_data(),
                               (int) m_pfile->get_size(),
                               m_sequence, 0);

         if(os != noErr)
         {

            return translate_os_status(os);

         }
         
         MusicSequenceType outType;
         
         MusicSequenceGetSequenceType ( m_sequence, &outType );

         // ************* Set the endpoint of the sequence to be our virtual endpoint
         MusicSequenceSetMIDIEndpoint(m_sequence, m_virtualEndpoint);

         // Create a new music player
         // Initialise the music player
         os = NewMusicPlayer(&m_player);
         if(os != noErr)
         {

            return translate_os_status(os);

         }

         // Load the sequence into the music player
         os = MusicPlayerSetSequence(m_player, m_sequence);
         if(os != noErr)
         {

            return translate_os_status(os);

         }
         m_bStart = false;
         // Called to do some MusicPlayer setup. This just
         // reduces latency when MusicPlayerStart is called
         os = MusicPlayerPreroll(m_player);
         if(os != noErr)
         {

            return translate_os_status(os);

         }
         
         // Starts the music playing
         os = MusicPlayerStart(m_player);

         if(os != noErr)
         {

            return translate_os_status(os);

         }

         SetState(status_playing);


         m_uiStart = get_tick_count();

         return ::multimedia::result_success;
         
      }
      
      /***************************************************************************
       *
       * seqPause
       *
       * Pauses playback of the instance.
       *
       * pSeq                      - The sequencer instance.
       *
       * Returns
       *   ::multimedia::result_success If the operation is successful.
       *
       *   ::music::EFunctionNotSupported If the sequencer instance is not
       *     playing.
       *
       * The sequencer must be playing before seqPause may be called.
       * Pausing the sequencer will cause all currently on notes to be turned
       * off. This may cause playback to be slightly inaccurate on restart
       * due to missing notes.
       *
       ***************************************************************************/
      ::multimedia::e_result sequence::Pause()
      {
         single_lock sl(m_pmutex, TRUE);
         
         //    assert(NULL != pSeq);
         
         if (status_playing != GetState())
            return ::music::translate(::music::EFunctionNotSupported);
         
         SetState(status_paused);
         
         //::multimedia::e_result mmrc = ::multimedia::result_success;
         //    single_lock slStream(&m_csStream, false);
         //  slStream.lock();
//         if(m_pseq != NULL)
         // {
  //          seq_stop_timer(m_pseq);
         // }
         //slStream.unlock();
         
         SetLevelMeter(0);
         
         return ::multimedia::result_success;
      }
      
      /***************************************************************************
       *
       * seqRestart
       *
       * Restarts playback of an instance after a pause.
       *
       * pSeq                      - The sequencer instance.
       *
       * Returns
       *    ::multimedia::result_success If the operation is successful.
       *
       *    ::music::EFunctionNotSupported If the sequencer instance is not
       *     paused.
       *
       * The sequencer must be paused before seqRestart may be called.
       *
       ***************************************************************************/
      ::multimedia::e_result sequence::Restart()
      {
         //    assert(NULL != pSeq);
         
         single_lock sl(m_pmutex, TRUE);
         
         if (status_paused != GetState())
            return ::music::translate(::music::EFunctionNotSupported);
         
         SetState(status_playing);
         m_evMmsgDone.ResetEvent();
         
         //    ::multimedia::e_result mmrc = 0;
         //    single_lock slStream(&m_csStream, false);
         //  slStream.lock();
//         if(m_pseq != NULL)
         {
  //          seq_start_timer(m_pseq);
         }
         //slStream.unlock();
         return ::multimedia::result_success;
      }
      
      /***************************************************************************
       *
       * seqStop
       *
       * Totally stops playback of an instance.
       *
       * pSeq                      - The sequencer instance.
       *
       * Returns
       *   ::multimedia::result_success If the operation is successful.
       *
       *   ::music::EFunctionNotSupported If the sequencer instance is not
       *     paused or playing.
       *
       * The sequencer must be paused or playing before seqStop may be called.
       *
       ***************************************************************************/
      ::multimedia::e_result sequence::Stop()
      {
         
         single_lock sl(m_pmutex, TRUE);
         
         if(GetState() == status_stopping)
            return ::multimedia::result_success;
         
         // Automatic success if we're already stopped
         if (GetState() != status_playing
             && GetState() != status_paused)
         {
            m_flags.unsignalize(::music::midi::sequence::FlagWaiting);
            GetPlayerLink().OnFinishCommand(::music::midi::player::command_stop);
            return ::multimedia::result_success;
         }
         
         SetState(status_stopping);
         m_flags.signalize(::music::midi::sequence::FlagWaiting);
         
         m_eventMidiPlaybackEnd.ResetEvent();
         
         
         MusicPlayerStop(m_player);
         DisposeMusicSequence(m_sequence);
         DisposeMusicPlayer(m_player);
//         if(m_pseq != NULL)
  //       {
    //        seq_stop_timer(m_pseq);
      //      if(::multimedia::result_success != m_mmrcLastErr)
        //    {
          //     TRACE( "::music::midi::sequence::Stop() -> midiOutStop() returned %lu in seqStop()!\n", (uint32_t)m_mmrcLastErr);
            //   m_flags.unsignalize(FlagWaiting);
              // return ::multimedia::result_not_ready;
            //}
        // }
         
         //m_eventMidiPlaybackEnd.lock();
         
         SetLevelMeter(0);
         
         return ::multimedia::result_success;
      }
      
      /***************************************************************************
       *
       * seqTime
       *
       * Determine the current position in playback of an instance.
       *
       * pSeq                      - The sequencer instance.
       *
       * pTicks                    - A pointer to a uint32_t where the current position
       *                             in ticks will be returned.
       *
       * Returns
       *   ::multimedia::result_success If the operation is successful.
       *
       *   MCIERR_DEVICE_NOT_READY If the underlying device fails to report
       *     the position.
       *
       *   ::music::EFunctionNotSupported If the sequencer instance is not
       *     paused or playing.
       *
       * The sequencer must be paused, playing or prerolled before seqTime
       * may be called.
       *
       ***************************************************************************/
      ::multimedia::e_result sequence::get_ticks(imedia_position &  pTicks)
      {
         
         synch_lock sl(m_pmutex);
         
         //::multimedia::e_result                mmr;
         
         if (::music::midi::sequence::status_playing != GetState() &&
             ::music::midi::sequence::status_paused != GetState() &&
             ::music::midi::sequence::status_pre_rolling != GetState() &&
             ::music::midi::sequence::status_pre_rolled != GetState() &&
             ::music::midi::sequence::status_opened != GetState() &&
             ::music::midi::sequence::status_stopping != GetState())
         {
            
            TRACE( "seqTime(): State wrong! [is %u]", GetState());
            
            return ::music::translate(::music::EFunctionNotSupported);
            
         }
         
         
         imedia_time t = 0;
         
         if(get_millis(t) == ::multimedia::result_success)
         {
            
            pTicks = MillisecsToTicks(t);
            
            return ::multimedia::result_success;
            
         }
         
         pTicks = 0;
         
         if (status_opened != GetState())
         {

            pTicks = m_tkBase;
            
         }
         
         return ::multimedia::result_success;
         
      }
      
      
      ::multimedia::e_result sequence::get_millis(imedia_time & time)
      {
         
         single_lock sl(m_pmutex);
         
         if (status_playing != GetState() &&
             status_paused != GetState() &&
             status_pre_rolling != GetState() &&
             status_pre_rolled != GetState() &&
             status_opened != GetState() &&
             status_stopping != GetState())
         {
            
            TRACE( "seqTime(): State wrong! [is %u]", GetState());
            
            return ::music::translate(::music::EFunctionNotSupported);
            
         }
         
         if(m_player != NULL)
         {

            Boolean bPlaying = false;
            
            OSStatus os = MusicPlayerIsPlaying(m_player, &bPlaying);
            
            if(os == noErr && bPlaying)
            {
               
               MusicTimeStamp now = 0;
               
               os = MusicPlayerGetTime (m_player, &now);
               
               if(os == noErr)
               {
                  
                  Float64 f;
                  
                  os = MusicSequenceGetSecondsForBeats(m_sequence, now,  &f);
                  
                  if(os == noErr)
                  {
                    
                     UInt64 outHostTime = 0;
                     
                     MusicPlayerGetHostTimeForBeats ( m_player, now, &outHostTime );
               
                     //m_posPlay = time = now * 1000;
                     
                     ///m_posPlay =  (outHostTime - m_uiStart) / (1000.0 * 1000.0);
                     
                     //m_posPlay = get_tick_count() - m_uiStart;
                     
                     //time = m_posPlay;
                     
                     time = f * 1000.0;
                     
                     // time = outHostTime;
                     
                     return ::multimedia::result_success;
                     
                  }
                  
               }
               
            }
            
         }

         time = 0;
         
         if (status_opened != GetState())
         {
            
            time = (int_ptr) TicksToMillisecs(m_tkBase);
            
            if (status_pre_rolled != GetState())
            {
               
            }
            
         }
         
         return ::multimedia::result_success;
         
      }
      
      /***************************************************************************
       *
       * seqMillisecsToTicks
       *
       * Given a millisecond offset in the output stream, returns the associated
       * tick position.
       *
       * pSeq                      - The sequencer instance.
       *
       * msOffset                  - The millisecond offset into the stream.
       *
       * Returns the number of ticks into the stream.
       *
       ***************************************************************************/
      imedia_position sequence::MillisecsToTicks(imedia_time msOffset)
      {
         return file()->MillisecsToTicks(msOffset);
      }
      
      /***************************************************************************
       *
       * seqTicksToMillisecs
       *
       * Given a tick offset in the output stream, returns the associated
       * millisecond position.
       *
       * pSeq                      - The sequencer instance.
       *
       * tkOffset                  - The tick offset into the stream.
       *
       * Returns the number of milliseconds into the stream.
       *
       ***************************************************************************/
      imedia_time sequence::TicksToMillisecs(imedia_position tkOffset)
      {
         return file()->TicksToMillisecs(tkOffset);
      }
      
      
      void sequence::OnPositionCB(LPMIDIHDR lpmidihdr)
      {
         /*ASSERT(lpmidihdr != NULL);
          LPBYTE lpbData = (LPBYTE) (lpmidihdr->lpData + lpmidihdr->dwOffset);
          MIDIEVENT * lpme = (MIDIEVENT *) lpbData;
          file::midi_stream_event_header * pheader = (file::midi_stream_event_header *) &lpme->dwParms[0];
          lpbData = (LPBYTE) pheader;
          LPDWORD lpdwParam;
          
          int32_t iSize = pheader->m_dwLength;
          switch(pheader->m_dwType)
          {
          case 0:
          {
          array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> * plyriceventa = NULL;
          array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> lyriceventa;
          for(int32_t i = sizeof(file::midi_stream_event_header); i < iSize;)
          {
          pheader = (file::midi_stream_event_header *) &lpbData[i];
          lpdwParam = (LPDWORD) &lpbData[i + sizeof(file::midi_stream_event_header)];
          ASSERT(*lpdwParam == pheader->m_dwType);
          switch(pheader->m_dwType)
          {
          case EVENT_ID_LYRIC_V1:
          {
          if(plyriceventa == NULL)
          {
          plyriceventa = new array <::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &>;
          }
          ::memory_file memFile(get_app(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(uint32_t));
          // x2x                  CArchive ar(&memFile, CArchive::load);
          lyriceventa.Serialize(ar);
          plyriceventa->append(lyriceventa); //
         //          }
          break;
          case EVENT_ID_PAD:
          {
          }
          break;
          case EVENT_ID_NOTE_ON:
          {
          ::file::byte_stream_memory_file memFile(get_app(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(uint32_t));
          for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
          {
          BYTE b;
          memFile >> b;
          m_iaLevel.element_at(i) = b;
          }
          }
          break;
          }
          i += pheader->m_dwLength + sizeof(file::midi_stream_event_header);
          }
          //         if(plyriceventa != NULL)
          {
          ::PostMessage(m_midicallbackdata.oswindow, WM_APP + 3388, 3388, (LPARAM) plyriceventa);
          }//
         //}
          break;
          case EVENT_ID_PAD:
          break;
          default:
          ASSERT(FALSE);
          break;
          }*/
         
      }
      
      //void CALLBACK sequence::MidiOutProc(HMIDIOUT hmo, uint32_t wMsg, uint32_t dwInstance, uint32_t dwParam1, uint32_t dwParam2)
      //{
      
      /*UNREFERENCED_PARAMETER(hmo);
       UNREFERENCED_PARAMETER(dwInstance);
       UNREFERENCED_PARAMETER(dwParam2);
       
       LPMIDIHDR               lpmh         = (LPMIDIHDR) dwParam1;
       
       switch(wMsg)
       {
       case MOM_POSITIONCB:
       ((midi_callback_data *) lpmh->dwUser)->m_psequence->OnPositionCB(lpmh);
       break;
       case MOM_DONE:
       ((midi_callback_data *) lpmh->dwUser)->m_psequence->OnDone(NULL,lpmh);
       return;
       }*/
      
      //}
      
      
      bool sequence::IsPlaying()
      {
         return GetState() == status_playing ||
         GetState() == status_stopping;
      }
      
      
      
      
      
      
      uint32_t sequence::SetState(uint32_t uiState)
      {
         m_uiPreviousState = m_uiState;
         m_uiState = uiState;
         return m_uiPreviousState;
      }
      
      void sequence::SetSpecialModeV001Flag(bool bSet)
      {
         if(m_flags.is_signalized(FlagIsInSpecialModeV001))
            m_flags.signalize(FlagWasInSpecialModeV001);
         else
            m_flags.unsignalize(FlagWasInSpecialModeV001);
         
         if(bSet)
            m_flags.signalize(FlagIsInSpecialModeV001);
         else
            m_flags.unsignalize(FlagIsInSpecialModeV001);
      }
      
      void sequence::SetSpecialModeV001Operation(uint32_t uiOperation)
      {
         m_uiPreSpecialModeV001State = m_uiState;
         SetSpecialModeV001Flag();
         m_uiSpecialModeV001Operation = uiOperation;
      }
      
      bool sequence::IsInSpecialModeV001()
      {
         return m_flags.is_signalized(FlagIsInSpecialModeV001);
      }
      
      bool sequence::WasInSpecialModeV001()
      {
         return m_flags.is_signalized(FlagWasInSpecialModeV001);
      }
      
      
      void sequence::GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd)
      {
         lpmdd->m_flags = m_flags;
         lpmdd->m_uiSeqPreviousState = m_uiPreviousState;
         lpmdd->m_uiSeqState = m_uiState;
      }
      
      int32_t sequence::GetKeyShift()
      {
         
         if(m_pfile == NULL)
            return 0;
         
         return file()->GetKeyShift();
         
      }
      
      int32_t sequence::SetKeyShift(int32_t iKeyShift)
      {
         /*//    single_lock slStream(&m_csStream, false);
          //  slStream.lock();
          if(m_hstream)
          {
          midiStreamPause(m_hstream);
          if(!file()->SetKeyShift(iKeyShift))
          return false;
          midiStreamRestart(m_hstream);
          }
          //slStream.unlock();
          return true;
          void ::music::midi::sequence::SetKeyShift(int32_t iShift)
          {*/
         bool bPlay = IsPlaying();
         imedia_position ticks = 0;
         if(bPlay)
         {
            
            ticks = get_position_ticks();
            
            Stop();
            
         }
         get_file()->SetKeyShift(iKeyShift);
         if(bPlay)
         {
            m_pthread->PrerollAndWait(ticks);
            Start();
         }
         //}
         return true;
      }
      
      
      double sequence::GetTempoShift()
      {
         
         return file()->GetTempoShift();
         
      }
      
      
      int32_t sequence::SetTempoShift(int32_t iTempoShift)
      {
         
         //midiStreamPause(m_hstream);
         if(::music::failed(file()->SetTempoShift(iTempoShift)))
            return false;
         //midiStreamRestart(m_hstream);
         return true;
      }
      
      
      bool sequence::SetMidiOutDevice(uint32_t uiDevice)
      {
         
         if(uiDevice == ::music::midi::device_default)
         {
            
            //m_uiDeviceID = MIDI_MAPPER;
            
         }
         else
         {
            
            m_uiDeviceID = uiDevice;
            
         }
         
         return true;
      }
      
      ::multimedia::e_result sequence::CloseStream()
      {
         single_lock sl(m_pmutex, TRUE);
         if(IsPlaying())
         {
            Stop();
         }
         
         /* If we were prerolled, need to clean up -- have an open MIDI handle
          ** and buffers in the ready queue
          */
         
         //         m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hstream);
         
//         if (m_pseq != NULL)
  //       {
    //        seq_free_context(m_pseq);
      //      m_pseq = NULL;
        // }
         
         SetState(status_opened);
         
         return ::multimedia::result_success;
      }
      
      void sequence::SetLevelMeter(int32_t iLevel)
      {
         for(int32_t i = 0; i < m_iaLevel.get_size(); i++)
         {
            m_iaLevel.element_at(i) = iLevel;
         }
      }
      
      void sequence::OnMidiPlaybackEnd(::music::midi::sequence::event * pevent)
      {
         UNREFERENCED_PARAMETER(pevent);
         single_lock sl(m_pmutex, TRUE);
         //   LPMIDIHDR lpmh = pevent->m_lpmh;
         //   midi_callback_data * lpData = &m_midicallbackdata;
         //::multimedia::e_result mmrc;
         
         
         //         if(0 == m_uBuffersInMMSYSTEM)
         {
            TRACE( "seqBufferDone: normal sequencer shutdown.");
            
            /* Totally done! Free device and notify.
             */
//            if(m_pseq != NULL)
            {
               /*if((mmrc = m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hstream))
                != ::multimedia::result_success)
                {
                TRACE( "midiOutUnprepareHeader failed in seqBufferDone! (%lu)", (uint32_t)mmrc);
  //              }*/
    //           seq_free_context(m_pseq);
      //         m_pseq = NULL;
            }
            
            m_mmrcLastErr = ::multimedia::result_success;
            m_flags.unsignalize(FlagWaiting);
            
            m_evMmsgDone.SetEvent();
         }
      }
      
      void sequence::OnEvent(::music::midi::sequence::event * pevent)
      {
         switch(pevent->m_eevent)
         {
            case EventSpecialModeV001End:
            {
               SetSpecialModeV001Flag(false);
               OnMidiPlaybackEnd(pevent);
               SetState(m_uiPreSpecialModeV001State);
            }
               break;
            case EventStopped:
            {
               OnMidiPlaybackEnd(pevent);
               SetState(status_opened);
            }
               break;
            case EventMidiPlaybackEnd:
            {
               OnMidiPlaybackEnd(pevent);
               SetState(status_opened);
            }
               break;
            default:
               break;
         }
      }
      
      /*imedia_position sequence::GetPositionTicks()
       {
       single_lock sl(m_pmutex);
       if(!sl.lock(millis(0)))
       return -1;
       MMTIME mmt;
       mmt.wType = TIME_TICKS;
       if(::multimedia::result_success ==
       midiStreamPosition(
       m_hstream,
       &mmt,
       sizeof(mmt)))
       return mmt.u.ticks + m_tkPrerollBase;
       else
       return -1;
       }*/
      
      
      bool sequence::IsChangingTempo()
      {
         return m_flags.is_signalized(::music::midi::sequence::FlagTempoChange);
      }
      void sequence::SetTempoChangeFlag(bool bSet)
      {
         if(bSet)
            m_flags.signalize(::music::midi::sequence::FlagTempoChange);
         else
            m_flags.unsignalize(::music::midi::sequence::FlagTempoChange);
      }
      
      
      bool sequence::IsNull()
      {
         
         if(is_null(this))
         {
          
            return true;
            
         }
         
         return false;
         
      }
      
      
      imedia_position sequence::TimeToPosition(imedia_time millis)
      {
         return imedia_position(MillisecsToTicks((int_ptr) millis));
      }
      
      imedia_time sequence::PositionToTime(imedia_position tk)
      {
         return imedia_time(TicksToMillisecs((imedia_position) (int_ptr) tk));
      }
      
      
      bool sequence::IsOpened()
      {
         
         return GetState() != status_no_file;
         
      }
      
      
      void sequence::Prepare(
                             string2a & str2a,
                             imedia_position_2darray & tka2DTokensTicks,
                             int32_t iMelodyTrack,
                             int2a & ia2TokenLine,
                             ::ikaraoke::data & data)
      {
         UNREFERENCED_PARAMETER(str2a);
         ::music::midi_core_midi::file & file = *this->file();
         ::music::midi::tracks & tracks = file.GetTracks();
         
         ASSERT(!file.IsNull());
         file.GetTracks().seek_begin();
         // imedia_position               tkMax = file.m_tkLength;
         // imedia_position               tkLastPosition = 0;
         
         
         ::ikaraoke::static_data & staticdata = data.GetStaticData();
         
         if(staticdata.m_LyricsDisplayOffset < 480)
         {
            staticdata.m_LyricsDisplayOffset = 480;
         }
         if(staticdata.m_LyricsDisplayOffset > 720)
         {
            staticdata.m_LyricsDisplayOffset = 720;
         }
         staticdata.m_LyricsDisplay = 30;
         
         imedia_position_2darray tk2DNoteOnPositions(get_app());
         imedia_position_2darray tk2DNoteOffPositions(get_app());
         imedia_position_2darray tk2DBegPositions(get_app());
         imedia_position_2darray tk2DEndPositions(get_app());
         imedia_time_2darray ms2DTokensMillis(get_app());
         imedia_time_2darray ms2DNoteOnMillis(get_app());
         imedia_time_2darray ms2DNoteOffMillis(get_app());
         imedia_time_2darray ms2DBegMillis(get_app());
         imedia_time_2darray ms2DEndMillis(get_app());
         ::music::midi::events midiEvents;
         
         
         
         
         // Note on and off events
         // and maximum and minimum
         // pitch bend peaks.
         ::music::midi::events midiEventsLevel2;
         
         ::music::midi::events noteOnEvents;
         ::music::midi::events noteOffEvents;
         
         ::music::midi::events eventsLevel2Beg;
         ::music::midi::events eventsLevel2End;
         ::ikaraoke::events_tracks_v1 lyricEventsForPositionCB;
         ::ikaraoke::events_tracks_v1 lyricEventsForBouncingBall;
         ::ikaraoke::events_tracks_v1 lyricEventsForScoring;
         ::ikaraoke::events_tracks_v1 lyricEvents;
         
         //   tracks.seek_begin();
         // tracks.GetXFInfoHeaders(
         //  &m_xfInfoHeaders);
         
         
         
         file.PositionToTime(
                             ms2DTokensMillis,
                             tka2DTokensTicks,
                             0);
         
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2;
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2_;
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2B;
         ::ikaraoke::lyric_events_v2 *pLyricEventsV2C;
         ::music::midi::events *pMidiEventsV1;
         
         tk2DNoteOnPositions.set_size_create(tka2DTokensTicks.get_size());
         tk2DNoteOffPositions.set_size_create(tka2DTokensTicks.get_size());
         tk2DBegPositions.set_size_create(tka2DTokensTicks.get_size());
         tk2DEndPositions.set_size_create(tka2DTokensTicks.get_size());
         int32_t i;
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = new ::ikaraoke::lyric_events_v2();
            pLyricEventsV2B = new ::ikaraoke::lyric_events_v2();
            pLyricEventsV2C = new ::ikaraoke::lyric_events_v2();
            pLyricEventsV2_ = new ::ikaraoke::lyric_events_v2();
            staticdata.m_eventsv1.add(pLyricEventsV2);
            staticdata.m_eventsv1.add(pLyricEventsV2B);
            staticdata.m_eventsv1.add(pLyricEventsV2C);
            staticdata.m_eventsv1.add(pLyricEventsV2_);
            lyricEvents.add(pLyricEventsV2_);
            lyricEventsForScoring.add(pLyricEventsV2);
            lyricEventsForPositionCB.add(pLyricEventsV2B);
            lyricEventsForBouncingBall.add(pLyricEventsV2C);
            pLyricEventsV2->m_iType = 0;
            pLyricEventsV2->m_iOrder = i;
            pLyricEventsV2->m_iType = 2;
            pLyricEventsV2->m_iOrder = i;
            pLyricEventsV2B->m_iType = ikaraoke::EventRunningElement;
            pLyricEventsV2B->m_iOrder = i;
            pLyricEventsV2C->m_iType = 4;
            pLyricEventsV2C->m_iOrder = i;
            pMidiEventsV1 = NULL;
            if(iMelodyTrack < 0)
            {
               pLyricEventsV2->m_iTrack =
               file.WorkCalcMelodyTrack(
                                        &pMidiEventsV1,
                                        tka2DTokensTicks.operator()(i),
                                        ia2TokenLine[i]);
            }
            else
            {
               pLyricEventsV2->m_iTrack = iMelodyTrack;
            }
            pLyricEventsV2B->m_iTrack = pLyricEventsV2->m_iTrack;
            pLyricEventsV2C->m_iTrack = pLyricEventsV2->m_iTrack;
            pLyricEventsV2_->m_iTrack = pLyricEventsV2->m_iTrack;
            if(pLyricEventsV2->m_iTrack < 0)
            {
               pLyricEventsV2->m_iTrack = tracks.m_iMelodyTrackTipA;
               pLyricEventsV2B->m_iTrack = tracks.m_iMelodyTrackTipA;
               pLyricEventsV2C->m_iTrack = tracks.m_iMelodyTrackTipA;
               pLyricEventsV2_->m_iTrack = tracks.m_iMelodyTrackTipA;
            }
            staticdata.SetGuessMelodyTrack(pLyricEventsV2->m_iTrack);
            if(pLyricEventsV2->m_iTrack >= 0)
            {
               if(file.GetFormat() == 0)
               {
                  tracks.TrackAt(0)->WorkSeekBegin();
                  ((::music::midi::track *)tracks.TrackAt(0))->WorkGetNoteOnOffEventsV1(
                                                                                        &midiEvents,
                                                                                        (int32_t) pLyricEventsV2->m_iTrack,
                                                                                        file.GetFormat() == 1);
                  tracks.TrackAt(0)->WorkSeekBegin();
                  ((::music::midi::track *)tracks.TrackAt(0))->WorkGetLevel2Events(
                                                                                   &midiEventsLevel2,
                                                                                   (int32_t) pLyricEventsV2->m_iTrack,
                                                                                   file.GetFormat() == 1);
               }
               else
               {
                  tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                  ((::music::midi::track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                                                                                                      &midiEvents,
                                                                                                      (int32_t) pLyricEventsV2->m_iTrack,
                                                                                                      file.GetFormat() == 1);
                  tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                  ((::music::midi::track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                                                                                                      &midiEventsLevel2,
                                                                                                      (int32_t) pLyricEventsV2->m_iTrack,
                                                                                                      file.GetFormat() == 1);
               }
            }
            
            ::music::midi::util miditutil(get_app());
            
            miditutil.PrepareNoteOnOffEvents(
                                             &noteOnEvents,
                                             &noteOffEvents,
                                             (int32_t) pLyricEventsV2->m_iTrack,
                                             file.GetFormat(),
                                             &midiEvents,
                                             tka2DTokensTicks.operator()(i));
            
            miditutil.PrepareLevel2Events(
                                          &eventsLevel2Beg,
                                          &eventsLevel2End,
                                          (int32_t) pLyricEventsV2->m_iTrack,
                                          file.GetFormat(),
                                          &midiEventsLevel2,
                                          tka2DTokensTicks.operator()(i));
            
            
            tk2DNoteOnPositions(i)     = noteOnEvents.m_tkaEventsPosition;
            tk2DNoteOffPositions(i)    = noteOffEvents.m_tkaEventsPosition;
            tk2DBegPositions(i)        = eventsLevel2Beg.m_tkaEventsPosition;
            tk2DEndPositions(i)        = eventsLevel2End.m_tkaEventsPosition;
            pLyricEventsV2->m_dwaNotesData.copy(noteOnEvents.m_dwaEventsData);
            pLyricEventsV2B->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
            pLyricEventsV2C->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
            pLyricEventsV2_->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
            midiEvents.remove_all();
            noteOnEvents.remove_all();
            noteOffEvents.remove_all();
            midiEventsLevel2.remove_all();
            eventsLevel2Beg.remove_all();
            eventsLevel2End.remove_all();
            delete pMidiEventsV1;
         }
         
         file.PositionToTime(
                             ms2DNoteOnMillis,
                             tk2DNoteOnPositions,
                             0);
         
         file.PositionToTime(
                             ms2DNoteOffMillis,
                             tk2DNoteOffPositions,
                             0);
         
         file.PositionToTime(
                             ms2DBegMillis,
                             tk2DBegPositions,
                             0);
         
         file.PositionToTime(
                             ms2DEndMillis,
                             tk2DEndPositions,
                             0);
         
         
         ::ikaraoke::lyric_events_v1 *pLyricEventsV1;
         
         
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEvents.get_at(i);
            staticdata.m_eventstracks.add(pLyricEventsV2);
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaTokensPosition,
                                ms2DTokensMillis(i),
                                0);
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaNotesPosition,
                                ms2DNoteOnMillis(i),
                                0);
            
            imedia_time time1(0);
            imedia_time time2(0);
            
            pLyricEventsV2->m_msaTokensPosition.CopySorted(
                                                           ms2DTokensMillis(i),
                                                           time1,
                                                           time2);
            
            pLyricEventsV2->m_msaNotesDuration.Diff(
                                                    ms2DNoteOffMillis(i),
                                                    ms2DNoteOnMillis(i));
            
            imedia_time time3(0);
            imedia_time time4(0);
            
            pLyricEventsV2->m_msaNotesPosition.CopySorted(
                                                          ms2DNoteOnMillis(i),
                                                          time3,
                                                          time4);
            
            imedia_time time5(0x7fffffff);
            
            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
                                                            ms2DTokensMillis(i),
                                                            time5);
            
         }
         
         
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV1 = new ::ikaraoke::lyric_events_v1();
            pLyricEventsV1->m_iType = ikaraoke::EventAdvanceShow;
            pLyricEventsV1->m_iOrder = i;
            //staticdata.m_eventsTracksForPositionCB.add(pLyricEventsV1);
            file.TimeToPosition(
                                pLyricEventsV1->m_tkaTokensPosition,
                                ms2DTokensMillis(i),
                                -1000);
            //lyric_track * pLyricTrk = file.GetTracks().CreateLyricTrack();
            //pLyricTrk->Prepare(*pLyricEventsV1);
         }
         
         
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForScoring.get_at(i);
            staticdata.m_eventsTracksForScoring.add(pLyricEventsV2);
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaTokensPosition,
                                ms2DTokensMillis(i),
                                0);
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaNotesPosition,
                                ms2DNoteOnMillis(i),
                                0);
            
            imedia_time time1(-100);
            imedia_time time2(0);
            
            pLyricEventsV2->m_msaTokensPosition.CopySorted(
                                                           ms2DTokensMillis(i),
                                                           time1,
                                                           time2);
            
            pLyricEventsV2->m_msaNotesDuration.Diff(
                                                    ms2DNoteOffMillis(i),
                                                    ms2DNoteOnMillis(i));
            
            imedia_time time3(-100);
            imedia_time time4(0);
            
            pLyricEventsV2->m_msaNotesPosition.CopySorted(
                                                          ms2DNoteOnMillis(i),
                                                          time3,
                                                          time4);
            
            imedia_time time5(0x7fffffff);
            
            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
                                                            ms2DTokensMillis(i),
                                                            time5);
            
            pLyricEventsV2->PrepareForScoring(this);
         }
         
         
         
         
         
         
         
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForPositionCB.get_at(i);
            staticdata.m_eventsTracksForPositionCB.add(pLyricEventsV2);
            
            staticdata.m_eventstracksV002.add(pLyricEventsV2);
            
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaTokensPosition,
                                ms2DTokensMillis(i),
                                -100);
            
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaNotesPosition,
                                ms2DNoteOnMillis(i),
                                -100);
            
            imedia_time time1(-100);
            imedia_time time2(0);
            
            pLyricEventsV2->m_msaTokensPosition.CopySorted(
                                                           ms2DTokensMillis(i),
                                                           time1,
                                                           time2);
            
            pLyricEventsV2->m_msaNotesDuration.Diff(
                                                    ms2DNoteOffMillis(i),
                                                    ms2DNoteOnMillis(i));
            
            /*
             pLyricEventsV2->m_msaNotesDuration.Diff(
             ms2DNoteOffMillis[i],
             ms2DNoteOnMillis[i]);
             */
            
            
            imedia_time time3(-100);
            imedia_time time4(0);
            
            pLyricEventsV2->m_msaNotesPosition.CopySorted(
                                                          ms2DNoteOnMillis(i),
                                                          time3,
                                                          time4);
            
            imedia_time time5(0x7fffffff);
            
            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
                                                            ms2DTokensMillis(i),
                                                            time5);
            
            pLyricEventsV2->PrepareForLyricsDisplay(this);
            //lyric_track * pLyricTrk = file.GetTracks().CreateLyricTrack();
            //pLyricTrk->Prepare(*pLyricEventsV2);
         }
         
         
         
         
         
         
         
         
         
         
         for(i = 0; i < tka2DTokensTicks.get_size(); i++)
         {
            pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForBouncingBall.get_at(i);
            staticdata.m_eventsTracksForBouncingBall.add(pLyricEventsV2);
            
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaTokensPosition,
                                ms2DTokensMillis(i),
                                -100);
            
            file.TimeToPosition(
                                pLyricEventsV2->m_tkaNotesPosition,
                                ms2DNoteOnMillis(i),
                                -100);
            
            imedia_time time1(-100);
            imedia_time time2(0);
            
            pLyricEventsV2->m_msaTokensPosition.CopySorted(
                                                           ms2DTokensMillis(i),
                                                           time1,
                                                           time2);
            
            pLyricEventsV2->m_msaNotesDuration.Diff(
                                                    ms2DEndMillis(i),
                                                    ms2DBegMillis(i));
            
            /*
             pLyricEventsV2->m_msaNotesDuration.Diff(
             ms2DNoteOffMillis[i],
             ms2DNoteOnMillis[i]);
             */
            
            imedia_time time3(-100);
            imedia_time time4(0);
            
            pLyricEventsV2->m_msaNotesPosition.CopySorted(
                                                          ms2DNoteOnMillis(i),
                                                          time3,
                                                          time4);
            
            imedia_time time5(0x7fffffff);
            
            pLyricEventsV2->m_msaTokensDuration.ElementDiff(
                                                            ms2DTokensMillis(i),
                                                            time5);
            
            pLyricEventsV2->PrepareForBouncingBall(this);
         }
         
      }
      
      void sequence::Prepare(int32_t iTrack, ::ikaraoke::data & data)
      {
         ::music::midi_core_midi::file & file = *this->file();
         ::music::midi::tracks & tracks = file.GetTracks();
         string2a & str2a = data.GetStaticData().m_str2aRawTokens;
         imedia_position_2darray position2a;
         int2a ia2TokenLine;
         
         
         ia2TokenLine.add_new();
         
         tracks.WorkSeekBegin();
         tracks.WorkGetEmptyXFTokens(
                                     iTrack,
                                     str2a,
                                     position2a,
                                     NULL);
         
         Prepare(
                 str2a,
                 position2a,
                 iTrack,
                 ia2TokenLine,
                 data);
         
      }
      
      void sequence::Prepare(::ikaraoke::data & data)
      {
         ::music::midi_core_midi::file & file = *this->file();
         ::music::midi::tracks & tracks = file.GetTracks();
         string2a & str2a = data.GetStaticData().m_str2aRawTokens;
         imedia_position_2darray position2a;
         int2a i2aTokenLine;
         
         ::music::xf::info_headers xfihs;
         get_file()->GetTracks().GetXFInfoHeaders(&xfihs);
         
         ::ikaraoke::static_data & staticdata = data.GetStaticData();
         string str;
         
         // add Title
         staticdata.m_straTitleFormat.add("%0");
         staticdata.m_str2aTitle.set_app(get_app());
         staticdata.m_str2aTitle.add_new();
         staticdata.m_str2aTitle[0].add(xfihs.m_strSongName);
         
         if(xfihs.m_xfInfoHeader.m_straComposer.get_size() > 0)
         {
            // add Performer
            staticdata.m_straTitleFormat.add("Performer: %0");
            xfihs.m_xfInfoHeader.m_straPerformer.get_format_string(str, ", ");
            staticdata.m_str2aTitle.add_new();
            staticdata.m_str2aTitle[1].add(str);
            
            // add Composer
            staticdata.m_straTitleFormat.add("Composer: %0");
            xfihs.m_xfInfoHeader.m_straComposer.get_format_string(str, ", ");
            staticdata.m_str2aTitle.add_new();
            staticdata.m_str2aTitle[1].add(str);
            
            
            //      staticdata.m_straPerformer = xfihs.m_xfInfoHeader.m_straPerformer;
            //    staticdata.m_straArtist = xfihs.m_xfInfoHeader.m_straPerformer;
         }
         
         staticdata.m_dwDefaultCodePage = 1252; // Latin (Default of All Default Code Pages)
         
         if(xfihs.m_xfaInfoHeaderLS.get_count())
         {
            staticdata.m_dwDefaultCodePage = ::music::xf::SymbolCharCodeToCodePage(xfihs.m_xfaInfoHeaderLS[0].m_strLanguage);
         }
         
         tracks.WorkSeekBegin();
         //tracks.WorkGetXFTokens(staticdata.m_dwDefaultCodePage, str2a, position2a, i2aTokenLine, NULL);
         tracks.WorkGetXFTokens((uint32_t) -1, str2a, position2a, i2aTokenLine, NULL, false);
         
         Prepare(
                 str2a,
                 position2a,
                 -1,
                 i2aTokenLine,
                 data);
         
         tracks.WorkSeekBegin();
         tracks.WorkGetLongestXFLyrics(staticdata.m_strLyrics, false);
         tracks.WorkGetLongestXFLyrics(staticdata.m_strLyricsEx1, true);
         
         
         
      }
      
      
      void sequence::buffer::Initialize(int32_t iSize, uint32_t dwUser)
      {

         m_storage.allocate(iSize);
         m_midihdr.lpData           = (char *) m_storage.get_data();
         m_midihdr.dwBufferLength   = (uint32_t) m_storage.get_size();
         m_midihdr.dwUser           = dwUser;
         m_bPrepared                = false;
         
      }
      
      
      void sequence::buffer_array::Initialize(int32_t iCount, int32_t iSize, uint32_t dwUser)
      {
         set_size(iCount);
         
         int32_t i;
         
         /*   for(int32_t i = 0; i < this->get_size() - 1; i++)
          {
          this->element_at(i).SetNextMidiHdr(this->element_at(i + 1).GetMidiHdr());
          }
          
          if(this->get_size() > 1)
          {
          this->element_at(get_upper_bound()).SetNextMidiHdr(this->element_at(0).GetMidiHdr());
          }
          else
          {
          this->element_at(0).SetNextMidiHdr(NULL);
          }*/
         
         for(i = 0; i < this->get_size(); i++)
         {
            this->element_at(i).Initialize(iSize, dwUser);
         }
      }
      
      void sequence::buffer_array::Reset()
      {
         for(int32_t i = 0; i < this->get_size(); i++)
         {
            this->element_at(i).Reset();
         }
      }
      
      void sequence::buffer::Reset()
      {
         m_midihdr.dwBytesRecorded  = 0;
         m_midihdr.dwFlags          = 0;
      }
      
      bool sequence::buffer::IsPrepared()
      {
         return m_bPrepared;
      }
      
      
      /*      ::multimedia::e_result sequence::buffer::midiOutPrepareHeader(HMIDIOUT hmidiout)
       {
       ::multimedia::e_result mmr = 0;
       if(hmidiout == NULL)
       return mmr;
       if(m_bPrepared)
       return mmr;
       mmr = ::midiOutPrepareHeader(hmidiout, &m_midihdr, sizeof(m_midihdr));
       if(mmr == ::multimedia::result_success)
       {
       m_bPrepared = true;
       }
       return mmr;
       }
       
       ::multimedia::e_result sequence::buffer::midiOutUnprepareHeader(HMIDIOUT hmidiout)
       {
       ::multimedia::e_result mmr = 0;
       if(hmidiout == NULL)
       return mmr;
       if(!m_bPrepared)
       return mmr;
       mmr = ::midiOutUnprepareHeader(hmidiout, &m_midihdr, sizeof(m_midihdr));
       if(mmr == ::multimedia::result_success)
       {
       m_bPrepared = false;
       }
       return mmr;
       }
       
       ::multimedia::e_result sequence::buffer_array::midiOutUnprepareHeader(HMIDIOUT hmidiout)
       {
       ::multimedia::e_result mmr = ::multimedia::result_success;
       
       for (int32_t i = 0; i < this->get_size(); i++)
       {
       ::multimedia::e_result mmrBuffer = this->element_at(i).midiOutUnprepareHeader(hmidiout);
       if(mmrBuffer != ::multimedia::result_success)
       {
       mmr = mmrBuffer;
       }
       }
       return mmr;
       }
       
       ::multimedia::e_result sequence::buffer_array::midiOutPrepareHeader(HMIDIOUT hmidiout)
       {
       ::multimedia::e_result mmrc = ::multimedia::result_success;
       for(int32_t i = 0; i < this->get_size(); i++)
       {
       mmrc = this->element_at(i).midiOutPrepareHeader(
       hmidiout);
       if(mmrc != ::multimedia::result_success)
       {
       for(; i >= 0; i--)
       {
       this->element_at(i).midiOutUnprepareHeader(hmidiout);
       }
       return mmrc;
       }
       }
       return mmrc;
       }*/
      
      void sequence::buffer::SetNextMidiHdr(LPMIDIHDR lpNext)
      {
         m_midihdr.lpNext = lpNext;
      }
      
      
//      ::multimedia::e_result sequence::buffer::midiStreamOut(seq_context_t * hmidiout)
  //    {
         /*         ASSERT(hmidiout != NULL);
          return ::midiStreamOut(hmidiout, &m_midihdr, sizeof(m_midihdr));*/
    //     return ::multimedia::result_success;
      //}
      
  //    ::multimedia::e_result sequence::buffer_array::midiStreamOut(seq_context_t * hmidiout)
    //  {
         /*
          ::multimedia::e_result mmrc = ::multimedia::result_success;
          for(int32_t i = 0; i < this->get_size(); i++)
          {
          mmrc = this->element_at(i).midiStreamOut(
          hmidiout);
          if(mmrc != ::multimedia::result_success)
          {
          //         for(; i >= 0; i--)
          //       {
          //        this->element_at(i).midiOutUnprepareHeader(hmidiout);
          //   }
          return mmrc;
          }
          }
          return mmrc;*/
      //   return ::multimedia::result_success;
     // }
      

      void sequence::MuteAll(bool bMute, int32_t iExcludeTrack)
      {
         
         bool bPlay = IsPlaying();
         
         imedia_position ticks = 0;
         
         if(bPlay)
         {
            
            ticks = get_position_ticks();
            
            Stop();
            
         }
         
         get_file()->MuteAll(bMute, iExcludeTrack);
         
         if(bPlay)
         {
            
            m_pthread->PrerollAndWait(ticks);
            
            Start();
            
         }
         
      }
      
      
      void sequence::MuteTrack(int32_t iIndex, bool bMute)
      {
         
         bool bPlay = IsPlaying();
         
         imedia_position ticks = 0;
         
         if(bPlay)
         {
            
            ticks = get_position_ticks();
            
            Stop();
            
         }
         
         get_file()->MuteTrack(iIndex, bMute);
         
         if(bPlay)
         {
            
            m_pthread->PrerollAndWait(ticks);
            
            Start();
            
         }
         
      }
      
      
      imedia_position sequence::GetQuarterNote()
      {
         return get_file()->m_pFileHeader->GetQuarterNoteTicks();
      }
      
      
      int32_t sequence::GetDefaultCodePage()
      {
         return 1252;
      }
      
      
      bool sequence::IsSettingPosition()
      {
         return m_flags.is_signalized(::music::midi::sequence::FlagSettingPos);
      }
      
      
      void sequence::SetSettingPositionFlag(bool bSet)
      {
         if(bSet)
            m_flags.signalize(::music::midi::sequence::FlagSettingPos);
         else
            m_flags.unsignalize(::music::midi::sequence::FlagSettingPos);
      }
      
      uint32_t sequence::GetPreviousState()
      {
         return m_uiPreviousState;
      }
      
      uint32_t sequence::GetState()
      {
         return m_uiState;
      }
      
      ::music::midi::sequence::event * sequence::create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr)
      {
         
         ASSERT(this != NULL);
         
         event * pevent          = canew(event());
         
         pevent->m_eevent        = eevent;
         pevent->m_psequence     = this;
         pevent->m_lpmh          = lpmidihdr;
         
         return pevent;
         
      }
      
      
      ::music::midi::event * sequence::seq_get_next_event()
      {
         
         ::music::midi::event * pevent = NULL;
         
         array < ::music::midi::event * > & evptra = m_evptra;
         
         if(evptra.get_size() > 0)
         {
            pevent = evptra[0];
            evptra.remove_at(0);
         }
         else
         {
            
            get_file()->WorkGetNextEvent(pevent, ::numeric_info< imedia_position >::get_maximum_value  (), false);
            
         }
         
         return pevent;
         
      }
      
      int sequence::seq_dump()
      {
         
         ::music::midi::event * pevent = seq_get_next_event();
         
         if(pevent == NULL)
            return -1;
         
         array < ::music::midi::event * > & evptra = m_evptra;
         
         m_tkPosition = pevent->m_tkPosition;
         
         for(index iBuffer = 0; iBuffer < 8; iBuffer++)
         {
            
            int iCount = 0;
            
            while(pevent != NULL)
            {
               
               if(pevent->GetType() == ::music::midi::NoteOn && pevent->GetNoteVelocity() > 0)
               {
                  
                  ::music::midi::event * peventOff = NULL;
                  
                  bool bFound = false;
                  
                  for(index i = 0; i < evptra.get_size(); i++)
                  {
                     
                     peventOff = evptra[i];
                     
                     if(pevent->on_match_off(peventOff))
                     {
                        
                        bFound = true;
                        
                        break;
                        
                     }
                     
                  }
                  
                  if(!bFound)
                  {
                     
                     while(true)
                     {
                        
                        if(get_file()->WorkGetNextEvent(peventOff, ::numeric_info <imedia_position >::get_maximum_value (), false) != ::music::success)
                           break;
                        
                        if(pevent->on_match_off(peventOff))
                        {
                           
                           bFound = true;
                           
                           break;
                           
                        }
                        
                        evptra.add(peventOff);
                        
                     }
                     
                  }
                  
                  if(bFound)
                  {

/*
                     snd_seq_event_t ev;
                     
                     imedia_position tkPosition = pevent->m_tkPosition - m_tkPrerollBase;
                     
                     seq_midi_event_init(m_pseq, &ev, tkPosition, pevent->GetTrack());
                     
                     imedia_position tkDuration = peventOff->m_tkPosition - pevent->m_tkPosition;
                     
                     seq_midi_note(
                                   m_pseq,
                                   &ev,
                                   pevent->GetTrack(),
                                   pevent->GetNotePitch(),
                                   pevent->GetNoteVelocity(),
                                   tkDuration,
                                   peventOff->GetNoteVelocity());
                     */
                     iCount++;
                     
                  }
                  
               }
               else
               {
                  
                  iCount += seq_play(pevent);
                  
               }
               
               if(iCount >= 32 && pevent != NULL && pevent->m_tkPosition != m_tkPosition)
                  break;
               
               pevent = seq_get_next_event();
               
            }
            
            if(pevent != NULL)
            {
               
               m_tkPosition = pevent->m_tkPosition;
               
               m_iaBuffered.add(m_tkPosition);
               
               m_iBuffered++;
               
               //seq_midi_echo(m_pseq, m_tkPosition - m_tkPrerollBase);
               
            }
            else
            {
               
               m_iaBuffered.add(get_file()->m_tkLength);
               
               m_iBuffered++;
               
               //seq_midi_echo(m_pseq, get_file()->m_tkLength - m_tkPrerollBase);
               
            }
            
//            snd_seq_drain_output(m_pseq->handle);
            
         }
         
         return 1;
         
      }
      
      int sequence::seq_play(::music::midi::event * pevent)
      {
         
//         snd_seq_event_t ev;
         
  //       seq_midi_event_init(m_pseq, &ev, pevent->m_tkPosition - m_tkPrerollBase, pevent->GetTrack());
         
         int iCount = 1;
         
         switch (pevent->GetType())
         {
               /*         case ::music::midi::NoteOn:
                seq_midi_note_on(m_pseq, &ev, pevent->GetTrack(), pevent->GetNotePitch(), pevent->GetNoteVelocity());
                break;
                case ::music::midi::NoteOff:
                seq_midi_note_off(m_pseq, &ev, pevent->GetTrack(), pevent->GetNotePitch(), pevent->GetNoteVelocity());
                break;*/
            case ::music::midi::NoteOff:
               break;
            case ::music::midi::PolyPressure:
        //       seq_midi_keypress(m_pseq, &ev, pevent->GetTrack(), pevent->GetNotePitch(), pevent->GetNoteVelocity());
               break;
            case ::music::midi::ControlChange:
          //     seq_midi_control(m_pseq, &ev, pevent->GetTrack(), pevent->GetController(), pevent->GetControllerValue());
               break;
            case ::music::midi::ProgramChange:
            //   seq_midi_program(m_pseq, &ev, pevent->GetTrack(), pevent->GetProgram());
               break;
            case ::music::midi::PitchBend:
              // seq_midi_pitchbend(m_pseq, &ev, pevent->GetTrack(), pevent->GetPitchBendLevel());
               break;
            case ::music::midi::ChanPressure:
               //seq_midi_chanpress(m_pseq, &ev, pevent->GetTrack(), pevent->GetChannelPressure());
               break;
            case 0xf0:
            {
               switch(pevent->GetFullType())
               {
                  case ::music::midi::Meta:
                  {
                     switch(pevent->GetMetaType())
                     {
                        case ::music::midi::MetaTempo:
                        {
                           DWORD dwTempo = (((uint32_t)pevent->GetData()[0])<<16)|
                           (((uint32_t)pevent->GetData()[1])<<8)|
                           ((uint32_t)pevent->GetData()[2]);
                           dwTempo = (uint32_t) ((double) dwTempo / get_file()->GetTempoShiftRate());
                           
                 //          seq_midi_tempo(m_pseq, &ev, dwTempo);
                        }
                           break;
                        case ::music::midi::MetaLyric:
                        case ::music::midi::MetaKarLyric:
                        case ::music::midi::MetaTimeSig:
                        case ::music::midi::MetaKeySig:
                           //case MD_TYPE_SMPTEOFFSET:
                           iCount = 0;
                           /* Ones that have no sequencer action */
                           break;
                        default:
                           iCount = 0;
                           printf("WARNING: play: not implemented yet Meta %d\n", pevent->GetMetaType());
                           break;
                     }
                  }
                     break;
                  case ::music::midi::SysEx:
                  case ::music::midi::SysExEnd:
                   //  seq_midi_sysex(m_pseq, &ev, pevent->GetFullType(), pevent->GetParam(), pevent->GetParamSize());
                     break;
                  default:
                     iCount = 0;
                     printf("WARNING: play: not implemented yet %d\n", pevent->GetFullType());
                     break;
               }
            }
               break;
            default:
               iCount = 0;
               printf("WARNING: play: not implemented yet %d\n", pevent->GetType());
               break;
         }
         
         return iCount;
         
      }
      
      
   } // namespace midi_core_midi
   
} // namespace music









