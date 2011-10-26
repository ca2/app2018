#include "StdAfx.h"


DWORD midi_event_base::SetVDWord(
   byte * hpbImage,
   DWORD dwLeft,
   DWORD dw)
{
    DWORD                   dwUsed  = 0;
   DWORD               dwBuffer;

    ASSERT(hpbImage != NULL);
   ASSERT(dw <= 0x0fffffff);


   if(!dwLeft)
      return 0;
   dwBuffer = dw & 0x7f;
   dwLeft--;
   dwUsed++;
   while ((dw >>= 7) > 0)
   {
      if(!dwLeft)
         return 0;
      dwBuffer <<= 8;
      dwBuffer |= 0x80;
      dwBuffer |= (dw & 0x7f);
      dwLeft--;
      dwUsed++;
   }
   while (TRUE)
   {
      *hpbImage++ = (BYTE) dwBuffer;
      if (dwBuffer & 0x80)
         dwBuffer >>= 8;
      else
         break;
   }


    return dwUsed;
}


midi_event_base & midi_event_base::operator = (const midi_event_base &eventSrc)
{
   SetFullType(eventSrc.GetFullType());
   SetChB1(eventSrc.GetChB1());
   SetChB2(eventSrc.GetChB2());
   SetParam(eventSrc.GetParam(), eventSrc.GetParamSize());
   SetDelta(eventSrc.GetDelta());
   SetPosition(eventSrc.get_position());
   SetImage(eventSrc.GetImage());
   SetImageSize(eventSrc.GetImageSize());
   SetFlags(eventSrc.GetFlags());
   return *this;
}

midi_event_base & midi_event_base::operator = (const midi_event_v008 &eventSrc)
{
   SetFullType(eventSrc.GetFullType());
   SetChB1(eventSrc.GetChB1());
   SetChB2(eventSrc.GetChB2());
   SetParam(eventSrc.GetParam(), eventSrc.GetParamSize());
   SetDelta(eventSrc.GetDelta());
   SetPosition(eventSrc.get_position());
   SetFlags(eventSrc.GetFlags());
   return *this;
}

DWORD midi_event_base::GetVDWord(
   byte * hpbImage,
   DWORD dwLeft,
   DWORD * pDw)
{
    BYTE                    b;
    DWORD                   dwUsed  = 0;

    ASSERT(hpbImage != NULL);
    ASSERT(pDw != NULL);

    *pDw = 0;

    do
    {
        if (!dwLeft)
        {
            return 0;
        }

        b = *hpbImage++;
        dwLeft--;
        dwUsed++;

        *pDw = (*pDw << 7) | (b & 0x7F);
    } while (b&0x80);

    return dwUsed;
}

void midi_event_base::SetParam(primitive::memory & memstorage)
{
   SetParam(
      memstorage.get_data(),
      memstorage.get_size());
}

int midi_event_base::GetPitchBendLevel() const
{
   ASSERT(get_type() == ::mus::midi::PitchBend);
   return GetChB1() | (GetChB2() << 8);
}
