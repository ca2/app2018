#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((uint32_t)(WORD)((BYTE)VERSION_MINOR | (((WORD)(BYTE)VERSION_MAJOR) << 8)))

#define MMSG_DONE                   (WM_USER+20)


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class buffer;
         class sequence;
         class sequence_thread;


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi_callback_data
         {
         public:


            sequence *          m_psequence;


         };


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM sequence :
            virtual public ::music::midi::sequence
         {
         public:

            class buffer_array;


            class buffer
            {
            public:


               MIDIHDR              m_midihdr;
               memory    m_storage;
               bool                 m_bPrepared;

               bool IsPrepared();
               void Reset();
               void Initialize(int32_t iSize, uint_ptr dwUser);

               ::multimedia::e_result midiStreamOut(HMIDISTRM hmidiout);
               ::multimedia::e_result midiOutPrepareHeader(HMIDIOUT hmidiout);
               ::multimedia::e_result midiOutUnprepareHeader(HMIDIOUT hmidiout);


               inline LPMIDIHDR GetMidiHdr() { return &m_midihdr; }

               void SetNextMidiHdr(LPMIDIHDR lpNext);


            };


            class buffer_array :
               public array < buffer, buffer >
            {
            public:


               void Reset();
               void Initialize(int32_t iCount, int32_t iSize, uint_ptr dwUser);


               ::multimedia::e_result midiStreamOut(HMIDISTRM hmidiout);
               ::multimedia::e_result midiOutPrepareHeader(HMIDIOUT hmidiout);
               ::multimedia::e_result midiOutUnprepareHeader(HMIDIOUT hmidiout);

            };

            class event :
               public ::music::midi::sequence::event
            {
            public:


               LPMIDIHDR                     m_lpmh;


            };


            buffer_array                  m_buffera;

            HMIDISTRM                     m_hstream;

            midi_callback_data            m_midicallbackdata;




            sequence(sp(base_application) papp);
            virtual ~sequence();


            inline uint32_t GetState();
            inline uint32_t GetPreviousState();
            uint32_t SetState(uint32_t uiState);

            void SetSpecialModeV001Flag(bool bSet = true);

            void MuteAll(bool bMute = true, int32_t iExcludeTrack = -1);
            void MuteTrack(int32_t iIndex, bool bMute = true);


            virtual int32_t GetDefaultCodePage();

            void Prepare(::ikaraoke::data & data);
            void Prepare(int32_t iTrack, ::ikaraoke::data & data);
            void Prepare(
               string2a & str2a,
               imedia_position_2darray & tka2DTokensTicks,
               int32_t iMelodyTrack,
               int2a & ia2TokenLine,
               ::ikaraoke::data & data);

            imedia_position GetPositionTicks();
            void SetLevelMeter(int32_t iLevel);
            ::multimedia::e_result CloseStream();
            bool SetMidiOutDevice(uint32_t uiDevice);
            int32_t SetKeyShift(int32_t iKeyShift);
            int32_t GetKeyShift();

            void OnMidiPlaybackEnd(::music::midi::sequence::event * pevent);
            int32_t SetTempoShift(int32_t iTempoShift);

            void OnPositionCB(LPMIDIHDR lpmidihdr);
            void OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr);
            virtual void GetTimeLength(imedia_time & time);
            virtual void GetPositionLength(imedia_position & position);

            using ::ikaraoke::karaoke::TimeToPosition;
            using ::ikaraoke::karaoke::PositionToTime;

            virtual imedia_position TimeToPosition(imedia_time time);
            virtual imedia_time PositionToTime(imedia_position position);

            virtual bool IsOpened();

            virtual bool IsNull();
            virtual void SetTempoChangeFlag(bool bSet = true);
            virtual bool IsChangingTempo();

            virtual int32_t GetTempoShift();
            virtual void GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd);
            virtual bool IsInSpecialModeV001();
            virtual bool WasInSpecialModeV001();
            virtual void SetSpecialModeV001Operation(uint32_t uiOperation);

            virtual void OnEvent(::music::midi::sequence::event * pevent);


            ::multimedia::e_result AllocBuffers();

            VOID FreeBuffers();

            //::multimedia::e_result OpenFile(const char * lpFileName, int32_t openMode);
            e_result OpenFile(::music::midi::sequence & sequence, int32_t iOpenMode);
            e_result OpenFile(::file::file & ar, int32_t openMode);
            e_result OpenFile(const char * lpFileName, int32_t openMode);
            e_result OpenFile(memory * pmemorystorage, int32_t openMode, ::music::e_storage estorage);

            e_result CloseFile();
            e_result SaveFile(const char * lpFileName);
            e_result SaveFile();
            e_result SaveFile(::file::file_sp &ar);

            ::multimedia::e_result Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow);
            ::multimedia::e_result Start();

            ::multimedia::e_result Pause();

            ::multimedia::e_result Restart();

            //::multimedia::e_result Stop(uint32_t dwEllapse);
            ::multimedia::e_result Stop();

            void get_position(imedia_position  & time);
            void get_time(imedia_time  & time);

            ::multimedia::e_result get_ticks(imedia_position & time);
            ::multimedia::e_result get_millis(imedia_time & time);


            imedia_position MillisecsToTicks(imedia_time msOffset);

            imedia_time TicksToMillisecs(imedia_position tkOffset);

            bool IsPlaying();

            static void CALLBACK MidiOutProc(HMIDIOUT hmo, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);


            bool IsSettingPosition();
            void SetSettingPositionFlag(bool bSet = TRUE);

            imedia_position GetQuarterNote();


            inline sp(::music::midi::mmsystem::buffer) file()
            {
               return get_file();
            }

            inline sp(::music::midi::mmsystem::sequence_thread) thread()
            {
               return m_pthread;
            }

            using ::music::midi::sequence::create_new_event;
            virtual ::music::midi::sequence::event * create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr);

         };


      } // namespace mmsystem


   } // namespace midi


} // namespace music







