#include "StdAfx.h"

midi_event_v007::midi_event_v007(::mus::midi::file * pFile, midi_track * pTrack)
{
   m_abEvent[0] = 0;
   m_abEvent[1] = 0;
   m_abEvent[2] = 0;
   m_cbParm  = 0;
   m_hpbParm = 0;
//   m_idxParam = 0;
   m_tkDelta = 0;
   m_bAutoAllocation = false;
   m_dwAllocation = 0;
   m_dwAllocationAddUp = 512;
//   m_hpbImage =  0;
   m_cbImage = 0;
   m_hpbAllocation = NULL;
   m_pFile = pFile;
   m_pTrack = pTrack;

    m_iFlags = 0;

}

midi_event_v007::midi_event_v007(::mus::midi::file * pFile, midi_track * pTrack, midi_event_v001 * pEvent)
{
   m_abEvent[0] = pEvent->m_abEvent[0];
   m_abEvent[1] = pEvent->m_abEvent[1];
   m_abEvent[2] = pEvent->m_abEvent[2];
   SetParam(pEvent->m_hpbParm, pEvent->m_cbParm);
//   m_idxParam = 0;
   m_tkDelta = 0;
   m_bAutoAllocation = false;
   m_dwAllocation = 0;
   m_dwAllocationAddUp = 512;
//   m_hpbImage =  0;
   m_cbImage = 0;
   m_hpbAllocation = NULL;
   m_pFile = pFile;
   m_pTrack = pTrack;

    m_iFlags = 0;
}

midi_event_v007::~midi_event_v007()
{
   if(m_bAutoAllocation && m_hpbAllocation != NULL)
   {
      free(m_hpbAllocation);
   }
}

midi_event_v007 & midi_event_v007::operator = (const midi_event_v007 &eventSrc)
{
   m_abEvent[0]      = eventSrc.m_abEvent[0];
   m_abEvent[1]      = eventSrc.m_abEvent[1];
   m_abEvent[2]      = eventSrc.m_abEvent[2];
   m_cbParm            = eventSrc.m_cbParm;
   m_hpbParm         = eventSrc.m_hpbParm;
   //m_idxParam      = eventSrc.m_idxParam;
   m_tkDelta         = eventSrc.m_tkDelta;
   m_tkPosition      = eventSrc.m_tkPosition;
//   m_hpbImage         = eventSrc.m_hpbImage;
   m_idxImage         = eventSrc.m_idxImage;
   m_cbImage         = eventSrc.m_cbImage;
   m_pFile            = eventSrc.m_pFile;
   m_pTrack          = eventSrc.m_pTrack;
   return *this;
}




//midi_event_v007 * midi_event_v007::operator =(midi_event_v007 *pEventSrc)
//{
//   abEvent[0]   = pEventSrc->abEvent[0];
//   abEvent[1]   = pEventSrc->abEvent[1];
//   abEvent[2]   = pEventSrc->abEvent[2];
//   cbParm      = pEventSrc->cbParm;
//   hpbParm      = pEventSrc->hpbParm;
//   tkDelta      = pEventSrc->tkDelta;
//   return this;
//}


BOOL midi_event_v007::IsAutoAllocated()
{
   return m_bAutoAllocation;
}

void midi_event_v007::AutoAllocate()
{
   ASSERT(!m_bAutoAllocation);
   m_bAutoAllocation = true;
   byte * hpbImage = (byte *) malloc(m_cbImage);
   memcpy(hpbImage, GetImage(), m_cbImage);
   m_hpbAllocation = hpbImage;
}


DWORD midi_event_v007::SetVDWord(
   byte * hpbImage,
   DWORD dw)
{
    DWORD                   dwUsed  = 0;
   DWORD               dwBuffer;

   ASSERT(m_bAutoAllocation);
   ASSERT(dw <= 0x0fffffff);


   dwBuffer = dw & 0x7f;
   dwUsed++;
   while ((dw >>= 7) > 0)
   {
      dwBuffer <<= 8;
      dwBuffer |= 0x80;
      dwBuffer |= (dw & 0x7f);
      dwUsed++;
   }
   if(!allocate_add_up(dwUsed))
      return 0;
//   byte *   hpbImage = m_hpbImage;
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
BOOL midi_event_v007::allocate(DWORD dwNewLength)
{
   ASSERT(m_bAutoAllocation);
   if(m_hpbAllocation == NULL)
   {
      m_hpbAllocation = (byte *) malloc(dwNewLength);
      if(m_hpbAllocation!= NULL)
      {
         m_dwAllocation = dwNewLength;
         m_idxImage = 0;
         //m_hpbImage = m_hpbAllocation;
         m_cbImage = dwNewLength;
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
   else
   {
      if(dwNewLength > m_dwAllocation)
      {
         DWORD dwAllocation = dwNewLength +  m_dwAllocationAddUp;
         LPVOID lpVoid = realloc(m_hpbAllocation, dwAllocation);
         if(lpVoid == NULL)
         {
//            DWORD dw = GetLastError();
            TRACELASTERROR();
            return FALSE;
         }
         else
         {
            m_dwAllocation = dwAllocation;
//            m_hpbImage = (byte *) lpVoid + (m_hpbImage - m_hpbAllocation);
            m_hpbAllocation = (byte *) lpVoid;
            m_cbImage = dwNewLength;
            return TRUE;
         }
      }
      else
      {
         m_cbImage = dwNewLength;
         return TRUE;
      }

   }
}

BOOL midi_event_v007::allocate_add_up(DWORD dwAddUp)
{
   ASSERT(m_bAutoAllocation);
   return allocate(m_cbImage + dwAddUp);
}

void midi_event_v007::SetAutoAllocation(BOOL bValue)
{
   if(bValue)
   {
      if(!m_bAutoAllocation)
      {
         ASSERT(m_dwAllocation == 0);
         ASSERT(!m_bAutoAllocation);
//         m_hpbImage = NULL;
         m_idxImage = 0;
         m_bAutoAllocation = TRUE;
      }
   }
   else
   {
      if(m_bAutoAllocation)
      {
         if(m_hpbAllocation != NULL)
         {
            free(m_hpbAllocation);
            m_hpbAllocation = NULL;
            m_dwAllocation = 0;
         }
      }
      m_bAutoAllocation = FALSE;
   }
}

SMFRESULT midi_event_v007::CreateXFInfoHeaderEvent(imedia::position tkDelta, XFInfoHeader *pXfih)
{
   SMFRESULT smfrc;
   primitive::memory memstorage;
   memstorage.allocate(pXfih->GetDataCount());
   //char * lpStr = pXfih->get_data();
   pXfih->get_data(
      (char *) memstorage.get_data(),
      memstorage.get_size());
   if(::mus::midi::Success != (smfrc = CreateMetaEvent(
      tkDelta,
      ::mus::xf::MetaXFInfoHdr,
      (byte *) memstorage.get_data(),
      memstorage.get_size())))
   {
      return smfrc;
   }
   //free(lpStr);
   return ::mus::midi::Success;
}

SMFRESULT midi_event_v007::CreateMetaEvent(imedia::position tkDelta, BYTE bMetaType, byte * hpbParam, DWORD cbParam)
{

   DWORD dwUsed;

   m_cbImage = 0;
   m_tkDelta = tkDelta;
   SetFullType(::mus::midi::Meta);
   SetMetaType(bMetaType);
   m_cbParm = cbParam;

   SetAutoAllocation();

   if(!(dwUsed = SetVDWord(m_tkDelta)))
   {
//      TRACE("midi_event_v007::CreateMetaEvent NoMemory");
      return (SMFRESULT) ::mus::midi::ENoMemory;
   }

   m_idxImage = dwUsed;

   if(!allocate_add_up(2))
   {
      //TRACE("midi_event_v007::CreateMetaEvent NoMemory");
      return (SMFRESULT) ::mus::midi::ENoMemory;
   }

   *GetImage() = ::mus::midi::Meta;
   m_idxImage++;
    *GetImage() = GetMetaType();
   m_idxImage++;

   if(!(dwUsed = SetVDWord(cbParam)))
   {
//      TRACE("midi_event_v007::CreateMetaEvent NoMemory");
      return (SMFRESULT) ::mus::midi::ENoMemory;
   }

   m_idxImage += dwUsed;

    if(!allocate_add_up(cbParam))
   {
      //TRACE("midi_event_v007::CreateMetaEvent NoMemory");
      return (SMFRESULT) ::mus::midi::ENoMemory;
   }

   m_hpbParm = GetImage();

   memcpy(m_hpbParm, hpbParam, cbParam);

//   m_hpbImage = m_hpbAllocation;
    m_idxParam = m_idxImage;
   m_idxImage = 0;

   return ::mus::midi::Success;

}

DWORD midi_event_v007::SetVDWord(uint64_t ui)
{
   DWORD dw = throw_cast < DWORD > (ui);
   DWORD dwUsed = 0;
   DWORD dwBuffer;

   ASSERT(m_bAutoAllocation);
   ASSERT(dw <= 0x0fffffff);


   dwBuffer = dw & 0x7f;
   dwUsed++;
   while ((dw >>= 7) > 0)
   {
      dwBuffer <<= 8;
      dwBuffer |= 0x80;
      dwBuffer |= (dw & 0x7f);
      dwUsed++;
   }
   if(!allocate_add_up(dwUsed))
      return 0;
   byte *   hpbImage = GetImage();
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

void midi_event_v007::SetImage(byte * hpbImage)
{
   if(m_bAutoAllocation)
   {
      m_idxImage = hpbImage - m_hpbAllocation;
   }
   else
   {
      m_idxImage = hpbImage - (m_pFile->GetImage() + m_pTrack->m_idxTrack);
   }

}

byte * midi_event_v007::GetImage() const
{
   if(m_bAutoAllocation)
   {
      return m_hpbAllocation + m_idxImage;
   }
   else
   {
      return m_pFile->GetImage() + m_pTrack->m_idxTrack + m_idxImage;
   }

}

byte * midi_event_v007::GetParam() const
{
   if(m_bAutoAllocation)
   {
      return m_hpbAllocation + m_idxParam;
   }
   else
   {
      return m_pFile->GetImage()+
         m_pTrack->m_idxTrack +
         m_idxImage +

         m_idxParam;
   }

}

/*void midi_event_v007::SetParam(byte * hpbParam)
{
   if(m_bAutoAllocation)
   {
      m_idxParam = hpbParam - m_hpbAllocation;
   }
   else
   {
      m_idxParam = hpbParam -
         m_pFile->m_hpbImage +
         m_pTrack->m_idxTrack +
         m_idxImage +
         m_idxParam;
   }

}*/

void midi_event_v007::SetParam(void * hpbParam, int iSize)
{
    m_cbParm = iSize;
    if(m_bAutoAllocation)
   {
      m_idxParam = (byte *) hpbParam - m_hpbAllocation;
   }
   else
   {
      m_idxParam = (byte *) hpbParam -
         m_pFile->GetImage() +
         m_pTrack->m_idxTrack +
         m_idxImage +
         m_idxParam;
   }

}

VMSRESULT midi_event_v007::SetParam(ex1::file & file, int iLength)
{
   UNREFERENCED_PARAMETER(file);
   UNREFERENCED_PARAMETER(iLength);
   ASSERT(FALSE);
   return VMSR_E_FAIL;
}


int midi_event_v007::GetFlags() const
{
    return m_iFlags;
}

void midi_event_v007::SetFlags(int iFlags)
{
    m_iFlags = iFlags;
}

void midi_event_v007::clear()
{
   m_tkDelta = 0;
   m_tkPosition= 0;
    m_abEvent[0]= 0;
   m_abEvent[1]= 0;
   m_abEvent[2]= 0;
    m_cbParm= 0;
    m_hpbParm= 0;
   m_cbImage= 0;
   m_idxImage= 0;
   m_idxParam= 0;
   m_pFile= 0;
   m_pTrack= 0;
   m_hpbAllocation= 0;
   m_dwAllocation= 0;
   m_bAutoAllocation= 0;
   m_dwAllocationAddUp= 0;

    m_iFlags= 0;



}



imedia::position midi_event_v007::GetDelta() const
{
   return m_tkDelta;
}

void midi_event_v007::SetDelta(imedia::position tkDelta)
{
   m_tkDelta = tkDelta;
}

imedia::position midi_event_v007::get_position() const
{
   return m_tkPosition;
}

void midi_event_v007::SetPosition(imedia::position tkPosition)
{
   m_tkPosition = tkPosition;
}

int midi_event_v007::GetImageSize() const
{
   return m_cbImage;
}

BYTE midi_event_v007::GetFullType() const
{
   return m_abEvent[0];
}
void midi_event_v007::SetFullType(BYTE bValue)
{
   m_abEvent[0] = bValue;
}
BYTE midi_event_v007::get_type() const
{
   return m_abEvent[0] & 0xf0;
}
void midi_event_v007::set_type(BYTE bValue)
{
   m_abEvent[0] &= 0x0f;
   m_abEvent[0] |= bValue & 0xf0;
}
BYTE midi_event_v007::GetTrack() const
{
   return m_abEvent[0] & 0x0f;
}
void midi_event_v007::SetTrack(BYTE bValue)
{
   m_abEvent[0] &= 0xf0;
   m_abEvent[0] |= bValue & 0x0f;
}
BYTE midi_event_v007::GetMetaType() const
{
   return m_abEvent[1];
}
void midi_event_v007::SetMetaType(BYTE bValue)
{
   m_abEvent[1] = bValue;
}
BYTE midi_event_v007::GetChB1() const
{
   return m_abEvent[1];
}
void midi_event_v007::SetChB1(BYTE bValue)
{
   m_abEvent[1] = bValue;
}
BYTE midi_event_v007::GetChB2() const
{
   return m_abEvent[2];
}
void midi_event_v007::SetChB2(BYTE bValue)
{
   m_abEvent[3] = bValue;
}
BYTE midi_event_v007::GetNotePitch() const
{
   return m_abEvent[1];
}
void midi_event_v007::SetNotePitch(BYTE bValue)
{
   m_abEvent[1] = bValue;
}
BYTE midi_event_v007::GetNoteVelocity() const
{
   return m_abEvent[2];
}
void midi_event_v007::SetNoteVelocity(BYTE bValue)
{
   m_abEvent[2] = bValue;
}
void midi_event_v007::SetImageSize(int cbSize)
{
   m_cbImage = cbSize;
}
//   void midi_event_v007::SetParamSize(DWORD cbSize)
//   {
//      m_cbParm = cbSize;
//   }
int midi_event_v007::GetParamSize() const
{
   return m_cbParm;
}
