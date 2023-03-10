#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((uint32_t)(WORD)((BYTE)VERSION_MINOR | (((WORD)(BYTE)VERSION_MAJOR) << 8)))

#define MMSG_DONE                   (WM_USER+20)

// much more/less and so'nso code from rtmidi

/*! \class RtMidi
 \brief An abstract base class for realtime MIDI input/output.
 
 This class implements some common functionality for the realtime
 MIDI input/output subclasses RtMidiIn and RtMidiOut.
 
 RtMidi WWW site: http://music.mcgill.ca/~gary/rtmidi/
 
 RtMidi: realtime MIDI i/o C++ classes
 Copyright (c) 2003-2016 Gary P. Scavone
 
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation files
 (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 Any person wishing to distribute modifications to the Software is
 asked to send the modifications to the original developer so that
 they can be incorporated into the canonical version.  This is,
 however, not a binding provision of this license.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


// *************************************************** //
//
// OS/API-specific methods.
//
// *************************************************** //

//#if defined(__MACOSX_CORE__)

// The CoreMIDI API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// OS-X CoreMIDI header files.
//#include <CoreMIDI/CoreMIDI.h>
//#include <CoreAudio/HostTime.h>
//#include <CoreServices/CoreServices.h>

// A structure to hold variables related to the CoreMIDI API
// implementation.
//struct core_midi_data {
//   MIDIClientRef client;
//   MIDIPortRef port;
//   MIDIEndpointRef endpoint;
//   MIDIEndpointRef destinationId;
//   unsigned long long lastTime;
//   MIDISysexSendRequest sysexreq;
//};

namespace music
{

   
   namespace midi_core_midi
   {
      

      class file;
      class sequence;
      class sequence_thread;


      class CLASS_DECL_VERIWELL_MULTIMEDIA midi_callback_data
      {
      public:


         sequence *          m_psequence;


      };


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_CORE_MIDI sequence :
         virtual public ::music::midi::sequence
      {
      public:

         class buffer_array;
         
         enum e_message
         {
            
            message_run = WM_APP + 1050,
            
         };
         
         imedia_position m_posPlay;
         class buffer
         {
         public:
            
            
            MIDIHDR              m_midihdr;
            memory               m_storage;
            bool                 m_bPrepared;
            
            bool IsPrepared();
            void Reset();
            void Initialize(int32_t iSize, uint32_t dwUser);
            
//            ::multimedia::e_result midiStreamOut(seq_context_t * hmidiout);
            //::multimedia::e_result midiOutPrepareHeader(HMIDIOUT hmidiout);
            //::multimedia::e_result midiOutUnprepareHeader(HMIDIOUT hmidiout);
            
            
            inline LPMIDIHDR GetMidiHdr() { return &m_midihdr; }
            
            void SetNextMidiHdr(LPMIDIHDR lpNext);
            
            
         };
         
         
         class buffer_array :
         public array < buffer, buffer >
         {
         public:
            
            
            void Reset();
            void Initialize(int32_t iCount, int32_t iSize, uint32_t dwUser);
            
            
//            ::multimedia::e_result midiStreamOut(seq_context_t * hmidiout);
            //::multimedia::e_result midiOutPrepareHeader(HMIDIOUT hmidiout);
            //::multimedia::e_result midiOutUnprepareHeader(HMIDIOUT hmidiout);
            
         };
         
         class event :
         public ::music::midi::sequence::event
         {
         public:
            
            
            LPMIDIHDR                     m_lpmh;
            
            
         };
         
            buffer_array m_buffera;

         //core_midi_data   m_data;
//         MIDIPortRef                      m_port;
//         MIDIClientRef                     m_client;

         midi_callback_data            m_midicallbackdata;
         int64_array m_iaBuffered;

 array < ::music::midi::event * >    m_evptra;
            int m_iBuffered;
         imedia_position            m_tkPosition;

         
         MusicSequence     m_sequence;
         MusicPlayer       m_player;
         
//         CAClockRef m_cl;
         
         // Create a client
         MIDIClientRef m_virtualMidi;
         MIDIEndpointRef m_virtualEndpoint;
         
         //CoreMidiOutput * m_pcmo;
         
         AudioUnitOutput * m_pau;
         
         uint64_t m_uiStart;
         bool m_bStart;
         
         
         

         sequence(::aura::application * papp);
         virtual ~sequence();

         
//         virtual uint32_t get_status() override;
//         inline uint32_t GetPreviousState() override;
//         uint32_t SetState(uint32_t uiState) override;
//
//         void SetSpecialModeV001Flag(bool bSet = true) override;
         
         void MuteAll(bool bMute = true, int32_t iExcludeTrack = -1) override;
         void MuteTrack(int32_t iIndex, bool bMute = true) override;
         
         
         virtual int32_t GetDefaultCodePage() override;
         
         void Prepare(::ikaraoke::data & data) override;
         void Prepare(int32_t iTrack, ::ikaraoke::data & data) override;
         void Prepare(
                      string2a & str2a,
                      imedia_position_2darray & tka2DTokensTicks,
                      int32_t iMelodyTrack,
                      int2a & ia2TokenLine,
                      ::ikaraoke::data & data) override;
         
         void SetLevelMeter(int32_t iLevel) override;
         ::multimedia::e_result CloseStream() override;
         bool SetMidiOutDevice(uint32_t uiDevice) override;
         int32_t SetKeyShift(int32_t iKeyShift) override;
         int32_t GetKeyShift() override;
         
         void OnMidiPlaybackEnd(::music::midi::sequence::event * pevent) override;
         int32_t SetTempoShift(int32_t iTempoShift);
         
         void OnPositionCB(LPMIDIHDR lpmidihdr);
         
         virtual ::music::midi::message_out * get_message_out() override;
         
         using ::ikaraoke::karaoke::TimeToPosition;
         using ::ikaraoke::karaoke::PositionToTime;
         
         virtual imedia_position TimeToPosition(imedia_time time) override;
         virtual imedia_time PositionToTime(imedia_position position) override;
         
         virtual bool IsOpened() override;
         
         virtual bool IsNull();
         virtual void SetTempoChangeFlag(bool bSet = true) override;
         virtual bool IsChangingTempo() override;
         
         virtual double GetTempoShift() override;
         //virtual void GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd) override;
         //virtual bool IsInSpecialModeV001() override;
         //virtual bool WasInSpecialModeV001() override;
         //virtual void SetSpecialModeV001Operation(uint32_t uiOperation) override;
         
         virtual void OnEvent(::music::midi::sequence::event * pevent) override;
         
         
         ::multimedia::e_result Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow) override;
         ::multimedia::e_result Start() override;
         
         ::multimedia::e_result Pause() override;
         
         ::multimedia::e_result Restart() override;
         
         //::multimedia::e_result Stop(uint32_t dwEllapse);
         ::multimedia::e_result Stop() override;
         

         ::multimedia::e_result get_ticks(imedia_position & time) override;
         ::multimedia::e_result get_millis(imedia_time & time) override;
         
         
         imedia_position MillisecsToTicks(imedia_time msOffset) override;
         
         imedia_time TicksToMillisecs(imedia_position tkOffset) override;
         
         bool IsPlaying() override;
         
         bool IsSettingPosition() override;
         void SetSettingPositionFlag(bool bSet = TRUE) override;
         
         imedia_position GetQuarterNote() override;
         
         
         inline sp(::music::midi_core_midi::file) file()
         {
            return get_file();
         }
         
         inline sp(::music::midi_core_midi::sequence_thread) thread()
         {
            return m_pthread;
         }
         
         using ::music::midi::sequence::create_new_event;
         virtual ::music::midi::sequence::event * create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr);
         
         int seq_dump();
         ::music::midi::event * seq_get_next_event();
         int seq_play(::music::midi::event * pevent);
         ::multimedia::e_result seq_start();

      };



   } // namespace midi_core_midi


} // namespace music







