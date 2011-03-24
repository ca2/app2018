#pragma once

class CLASS_DECL_ca MidiPlayerCallbackInterface
{
public:


   MidiPlayerCallbackInterface(void);
   virtual ~MidiPlayerCallbackInterface(void);

   virtual void OnMmsgDone(::mus::midi::sequence *pSeq, ::mus::midi::LPMIDIDONEDATA lpmdd);
   virtual void OnMidiPlayerNotifyEvent(::mus::midi::player::NotifyEvent * pevent); 
   virtual void OnMidiLyricEvent(base_array<LyricEventV1, LyricEventV1&> * pevents);

};
