#include "StdAfx.h"

midi_track_v008::midi_track_v008()
{
}

midi_track_v008::~midi_track_v008()
{

}

midi_track_v008 & midi_track_v008::operator =(const midi_track_v008 &eventSrc)
{
   m_chunkHeader = eventSrc.m_chunkHeader;
   m_eventa = eventSrc.m_eventa;
   return *this;
}

void midi_track_v008::insert_at(index iIndex, midi_event_v008 & event)
{
   m_eventa.insert_at(iIndex, event);
}

void midi_track_v008::insert_at(index iIndex, imedia::position tkDelta, midi_event_v008 & event)
{
   m_eventa.insert_at(iIndex, event);
   m_eventa[iIndex]._SetDelta(tkDelta);
}

void midi_track_v008::remove_all()
{
   m_eventa.remove_all();
}

void midi_track_v008::remove_at(index iIndex)
{
   ASSERT(iIndex >= 0);
   ASSERT(iIndex < m_eventa.get_size());
   m_eventa.remove_at(iIndex);
}

void midi_track_v008::clear()
{
   remove_all();
}




count midi_track_v008::GetEventCount() const
{
   return m_eventa.get_size();
}

midi_event_v008 & midi_track_v008::EventAt(index iIndex)
{
   ASSERT(iIndex >= 0);
   ASSERT(iIndex < m_eventa.get_size());
   return m_eventa.element_at(iIndex);
}

midi_event_v008 midi_track_v008::EventAt(index iIndex) const
{
   ASSERT(iIndex >= 0);
   ASSERT(iIndex < m_eventa.get_size());
   return m_eventa.element_at(iIndex);
}

void midi_track_v008::add(midi_event_base &event)
{
   m_eventAdd = event;
   add(m_eventAdd);
}

void midi_track_v008::add(midi_event_v001 &event)
{
   m_eventAdd = event;
   add(m_eventAdd);
}

void midi_track_v008::add(midi_event_v008 &event)
{
   insert_at(GetEventCount(), event);
}

void midi_track_v008::add(imedia::position tkDelta, midi_event_v008 &event)
{
   insert_at(GetEventCount(), tkDelta, event);
}

midi_event_v008 & midi_track_v008::LastEvent()
{
   ASSERT(GetEventCount() > 0);
   return EventAt(GetLastEventIndex());
}

count midi_track_v008::GetLastEventIndex()
{
   return GetEventCount() - 1;
}
