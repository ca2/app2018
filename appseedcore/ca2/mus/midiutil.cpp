#include "StdAfx.h"

MidiUtil::MidiUtil(::ca::application * papp) :
ca(papp), 
   m_tracks(papp)
{
}
MMRESULT MidiUtil::ConvertKarToXF(::mus::midi::sequence & seq, int iOpenMode)
{
   ::mus::midi::sequence seqKar(get_app());
   seqKar.OpenFile(seq,  ::mus::midi::OpenForPlaying);
   return ConvertKarToXF(seqKar, seq, iOpenMode);
}

MMRESULT MidiUtil::ConvertKarToXF(::mus::midi::sequence & seqKar, ::mus::midi::sequence & seqXF, int iOpenMode)
{
   ::mus::midi::file &               karfile = seqKar.GetFile();
   ::mus::midi::file &               xffile = seqXF.GetFile();
   //   string2a *       pXF2DTokens = NULL;
   //   imedia::position_2darray *               pXF1DTicks = NULL;
   imedia::position_2darray                  kar2DTicks;
   dword_array            karTrackIndexes;
   imedia::position_2darray                  tune10002DTicks;
   dword_array            tune1000TrackIndexes;
   imedia::position_array                    tkaTicksKar;
   MidiTrack *               pXFTrk;
   MidiTrack *               pmiditrackKar;
   ::mus::midi::e_file_result      smfrc;
   stringa *             pKar1DTokens = NULL;
   imedia::position_array *                pKar1DTicks = NULL;
   string                 str;
   imedia::position                     tkPosition;
   imedia::position                     tkLastPosition;
   MIDIFILEHDR             fileHdr;
   int                     i, j, k;
   bool                    bKarInfoValid = false;
   bool                    bTuneInfoValid = false;
   MidiTracks &            tracks = m_tracks;
   CTune1000Info           tki;


   xffile.GetFlags().signalize(::mus::midi::DisablePlayLevel1Operations);
   xffile.GetTracks().GetFlags().signalize(::mus::midi::DisablePlayLevel1Operations);
   tracks.GetFlags().signalize(::mus::midi::DisablePlayLevel1Operations);

   SoftKaraokeInfo ki;
   karfile.GetTracks().seek_begin();
   bKarInfoValid = ::mus::midi::Success == karfile.GetTracks().GetKarInfoHeader(&ki);

   seqXF.CloseFile();

   m_kar2DTokens.remove_all();
   m_tune10002DTokens.remove_all();

   for(i = 0; i < karfile.GetTracks().GetMidiTrackCount(); i++)
   {
      if(!bTuneInfoValid)
      {
         bTuneInfoValid = ::mus::midi::Success == karfile.GetTracks().GetTune1000InfoHeader(&tki, i);
      }
      pmiditrackKar = karfile.GetTracks().MidiTrackAt(i);
      pmiditrackKar->seek_begin();
      pmiditrackKar->ToWorkStorage();
      if(pmiditrackKar->WorkReadKarTokens(m_straTokens, &tkaTicksKar)
         == ::mus::midi::Success)
      {
         imedia::position_array  * lptka = new imedia::position_array();
         *lptka = tkaTicksKar;
         m_kar2DTokens.add(m_straTokens);
         kar2DTicks.add(lptka);
         karTrackIndexes.add(pmiditrackKar->m_iIndex);
      }
      if(pmiditrackKar->WorkReadTune1000Tokens(m_straTokens, &tkaTicksKar)
         == ::mus::midi::Success)
      {
         imedia::position_array  * lptka = new imedia::position_array();
         *lptka = tkaTicksKar;
         m_tune10002DTokens.add(m_straTokens);
         tune10002DTicks.add(lptka);
         tune1000TrackIndexes.add(pmiditrackKar->m_iIndex);
      }
      MidiTrack * pTrackXF = tracks.CreateTrack();
      if((smfrc = pTrackXF->WorkCopyWork(
         pmiditrackKar,
         MidiTrack::CopyExcludeXFMetaLyrics |
         MidiTrack::CopyExcludeKarMetaLyrics)) != ::mus::midi::Success)
         return smfrc;
   }

   {
      MidiTrack * pTrackUnion = xffile.GetTracks().CreateTrack();
      tracks.WorkCompactMidiTracksWork(*pTrackUnion, false);
   }

   XFInfoHeader xfInfoHeader;

   pXFTrk = xffile.GetTracks().CreateTrack(FOURCC_XFIH);

   if(bKarInfoValid)
   {
      xfInfoHeader.FromKarHeader(&ki);
      xfInfoHeader.SetID();
      pXFTrk->WorkWriteXFInfoHeader(
         NULL,
         &xfInfoHeader,
         NULL);
   }
   else if(bTuneInfoValid)
   {
      xfInfoHeader.FromTune1000Header(&tki);
      xfInfoHeader.SetID();
      pXFTrk->WorkWriteXFInfoHeader(
         ki.m_strSongName,
         &xfInfoHeader,
         NULL);
   }
   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
   pXFTrk->GetEvent().SetParam(NULL, 0);
   pXFTrk->GetEvent().SetDelta(0);
   pXFTrk->WorkWriteEvent();

   BYTE XFLYRICSHEADER[] = "$Lyrc:1:270:L1";
   BYTE CUEPOINTSOLO[] = "&s";

   if((i = karTrackIndexes.find_first(2)) >= 0)
   {
      tkLastPosition = 0;
      pXFTrk = xffile.GetTracks().CreateTrack(FOURCC_XFKM);
      pKar1DTokens = &m_kar2DTokens.element_at(i);
      pKar1DTicks = kar2DTicks.ptr_at(i);

      WorkConvertKarToXF(pKar1DTokens, pKar1DTicks, pXFTrk);
   }
   else if((i = tune1000TrackIndexes.find_first(2)) >= 0)
   {
      tkLastPosition = 0;
      pXFTrk = xffile.GetTracks().CreateTrack(FOURCC_XFKM);

      pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
      pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaXFLyricsHeader);
      pXFTrk->GetEvent().SetParam(XFLYRICSHEADER, sizeof(XFLYRICSHEADER) - 1);
      pXFTrk->GetEvent().SetDelta(0);
      pXFTrk->WorkWriteEvent();

      pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
      pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaCuePoint);
      pXFTrk->GetEvent().SetParam(CUEPOINTSOLO, sizeof(CUEPOINTSOLO) - 1);
      pXFTrk->GetEvent().SetDelta(0);
      pXFTrk->WorkWriteEvent();

      pKar1DTokens = &m_tune10002DTokens.element_at(i);
      pKar1DTicks = tune10002DTicks.ptr_at(i);
      for(j = 0; j < pKar1DTokens->get_size(); j++)
      {
         string strToken = pKar1DTokens->element_at(j);
         tkPosition = pKar1DTicks->get_at(j);
         if(strToken.Mid(0, 1).Compare("\\") == 0)
         {
            str = "<";
            str += strToken.Mid(1);
         }
         else if(strToken.Compare("\r") == 0)
         {
            str = "/";
            k = j;
            while(tkLastPosition >= (tkPosition = pKar1DTicks->get_at(k)))
            {
               if(++k >= pKar1DTicks->get_size())
                  break;
            }
         }
         else if(strToken.Compare("\n") == 0)
         {
            j++;
            str = "<";
            strToken = pKar1DTokens->element_at(j);
            tkPosition = pKar1DTicks->get_at(j);
            str += strToken;
         }
         else
         {
            str = strToken;
         }
         str.replace(" ", "^");
         ASSERT(tkPosition >= tkLastPosition);
         pXFTrk->WorkWriteXFLyricEvent(str, tkPosition);
         tkLastPosition = tkPosition;
      }
      pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
      pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
      pXFTrk->GetEvent().SetParam(NULL, 0);
      pXFTrk->GetEvent().SetDelta(0);
      pXFTrk->WorkWriteEvent();
   }
   else 
   {
      sort::BubbleSortByPtrAtGetSize(m_kar2DTokens, false);
      sort::BubbleSortByPtrAtGetSize(kar2DTicks, false);
      sort::BubbleSortByPtrAtGetSize(m_tune10002DTokens, false);
      sort::BubbleSortByPtrAtGetSize(tune10002DTicks, false);
      if(m_kar2DTokens.get_size() > 0 && (!bTuneInfoValid || m_tune10002DTokens.get_size() <= 0))
      {
         for(i = 0; i < m_kar2DTokens.get_size(); i++)
         {
            tkLastPosition = 0;

            pXFTrk = xffile.GetTracks().CreateTrack(FOURCC_XFKM);

            pKar1DTokens = &m_kar2DTokens.element_at(i);
            pKar1DTicks = kar2DTicks.ptr_at(i);

            WorkConvertKarToXF(pKar1DTokens, pKar1DTicks, pXFTrk);
         }
      }
      else if(m_tune10002DTokens.get_size() > 0)
      {
         for(i = 0; i < m_tune10002DTokens.get_size(); i++)
         {
            tkLastPosition = 0;

            pXFTrk = xffile.GetTracks().CreateTrack(FOURCC_XFKM);

            pXFTrk->GetWorkTrack().remove_all();

            pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
            pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaXFLyricsHeader);
            pXFTrk->GetEvent().SetParam(XFLYRICSHEADER, sizeof(XFLYRICSHEADER) - 1);
            pXFTrk->GetEvent().SetDelta(0);
            pXFTrk->WorkWriteEvent();

            pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
            pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaCuePoint);
            pXFTrk->GetEvent().SetParam(CUEPOINTSOLO, sizeof(CUEPOINTSOLO) - 1);
            pXFTrk->GetEvent().SetDelta(0);
            pXFTrk->WorkWriteEvent();

            pKar1DTokens = &m_tune10002DTokens.element_at(i);
            pKar1DTicks = tune10002DTicks.ptr_at(i);
            for(j = 0; j < pKar1DTokens->get_size(); j++)
            {
               while(tkLastPosition > (tkPosition = pKar1DTicks->get_at(j)))
               {
                  j++;
               }
               if(j >= pKar1DTokens->get_size())
                  break;
               string strToken = pKar1DTokens->element_at(j);
               if(strToken.Mid(0, 1).Compare("\\") == 0)
               {
                  str = "<";
                  str += strToken.Mid(1);
               }
               else if(strToken.Compare("\r") == 0)
               {
                  str = "/";
                  k = j;
                  while(tkLastPosition >= (tkPosition = pKar1DTicks->get_at(k)))
                  {
                     if(++k >= pKar1DTicks->get_size())
                        break;
                  }
               }
               else if(strToken.Compare("\n") == 0)
               {
                  j++;
                  if(j >= pKar1DTokens->get_size())
                     break;
                  str = "<";
                  strToken = pKar1DTokens->element_at(j);
                  tkPosition = pKar1DTicks->get_at(j);
                  str += strToken;
               }
               else
               {
                  str = strToken;
               }
               str.replace(" ", "^");
               pXFTrk->WorkWriteXFLyricEvent(str, tkPosition);
               tkLastPosition = tkPosition;
            }
            pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
            pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
            pXFTrk->GetEvent().SetParam(NULL, 0);
            pXFTrk->GetEvent().SetDelta(0);
            pXFTrk->WorkWriteEvent();

         }
      }
   }

   //   DWORD dwTime400 = timeGetTime();
   fileHdr.wFormat = WORDSWAP(0);
   fileHdr.wDivision = karfile.m_pFileHeader->wDivision;
   fileHdr.wTracks = WORDSWAP(1) ;

   BYTE XFVERSIONID[] = {
      0x43,  // YAMAHA ID
      0x7B,  //
      0x00,  //
      0x58,  // X
      0x46,  // F
      0x30,  // 0 
      0x32,   // 2
      0x00,
      0x11};

      MidiTrack & trackXF = *(MidiTrack *) xffile.GetTracks().TrackAt(0);
      trackXF.seek_begin();
      MidiEventV008 eventV008;
      eventV008.SetPosition(0);
      eventV008.SetFullType(::mus::midi::Meta);
      eventV008.SetMetaType(::mus::midi::MetaSeqSpecific);
      eventV008.SetParam(XFVERSIONID, sizeof(XFVERSIONID));
      trackXF.GetWorkTrack().insert_at(0, eventV008);

      primitive::memory memstorage;

      eventV008.clear();
      eventV008.SetPosition(0);
      eventV008.SetFullType(::mus::midi::Meta);
      eventV008.SetMetaType(CXF::MetaSongName);
      gen::international::OEMToMultiByte(
         gen::international::CodePageLatin1,
         memstorage,
         ki.m_strSongName);
      eventV008.SetParam(
         memstorage.GetAllocation(),
         memstorage.GetStorageSize());
      trackXF.GetWorkTrack().insert_at(1, eventV008);

      for(i = 0; i < xffile.GetTracks().GetTrackCount(); i++)
      {
         MidiTrack & trackXF = *(MidiTrack *) xffile.GetTracks().TrackAt(i);
         trackXF.WorkSeekBegin();
         while(::mus::midi::Success == trackXF.WorkSeek(::mus::midi::SeekKarID))
         {
            trackXF.WorkDeleteEvent();
         }
      }
      if((smfrc = xffile.WorkMergeTracks()) != ::mus::midi::Success)
         return smfrc;

      xffile.WriteHeader(&fileHdr);
      xffile.m_iOpenMode = iOpenMode;
      seqXF.m_iOpenMode = iOpenMode;

      if((smfrc = xffile.Build()) != ::mus::midi::Success)
         return smfrc;

      SMFFILEINFO             sfi;

      xffile.GetFileInfo(&sfi);

      seqXF.m_dwTimeDivision = sfi.dwTimeDivision;
      seqXF.m_tkLength       = sfi.tkLength;
      seqXF.m_msLength      = seqXF.TicksToMillisecs(seqXF.m_tkLength);

      seqXF.SetState(::mus::midi::sequence::StatusOpened);



      while(kar2DTicks.get_size() > 0)
      {
         kar2DTicks.remove_at(0);
      }

      while(tune10002DTicks.get_size() > 0)
      {
         tune10002DTicks.remove_at(0);
      }

      tracks.remove_all();

#ifdef _DEBUG

      //xffile.GetTracks().dump(afxdump);
#endif // _DEBUG


      return ::mus::midi::Success;
}


void MidiUtil::
   PrepareNoteOnOffEvents(
   MidiEventsV1 *pNoteOnEvents,
   MidiEventsV1 *pNoteOffEvents,
   int iTrack,
   int iFormat,
   MidiEventsV1 *pNoteOnOffEvents,
   imedia::position_array   & positiona)
{
   UNREFERENCED_PARAMETER(iFormat);
   imedia::position_array ticka;
   ticka = positiona;

   ASSERT(pNoteOnEvents->get_size() == 0);
   ASSERT(pNoteOffEvents->get_size() == 0);

   MidiEventsV1 & noteOnEvents = *pNoteOnEvents;
   MidiEventsV1 & noteOffEvents = *pNoteOffEvents;
   MidiEventV001 noteOn, noteOff;
   MidiEventV001 noteOnB, noteOffB;
   imedia::position tkTokenPosition, tkNextTokenPosition;
   int iNotesCount = 0;
   int iSize;

   // This assignement treats
   // positiona.get_size() <= 0
   // condition.
   noteOn.m_tkPosition = 0x7fffffff;
   int i, j, k, l;
   i= 0;
   if(false)
      //        if(iFormat == 0)
   {
      k = 0;
      iSize = pNoteOnOffEvents->get_size();
      iNotesCount = 0;
      tkTokenPosition = positiona.get_at(0);
      tkNextTokenPosition = positiona.get_at(1);
      for(l = 1, i = 0; i < iSize; i++)
      {   

         pNoteOnOffEvents->GetEvent(&noteOn, i);
         if((noteOn.GetNoteVelocity() == 0) ||
            (noteOn._GetType() == ::mus::midi::NoteOff) ||
            (noteOn.GetTrack() != iTrack))
            continue;
         for(j = i + 1;; j++)
         {
            if(j >= iSize)
            {
               noteOff.set_type((byte) ::mus::midi::NoteOff);
               noteOff.SetTrack((byte) iTrack);
               noteOff.SetNotePitch(noteOn.GetNotePitch());
               noteOff.SetNoteVelocity(0);
               noteOff.m_tkPosition = 0x7fffffff;
               break;
            }
            pNoteOnOffEvents->GetEvent(&noteOff, i);
            if((((noteOff.GetNoteVelocity() <= 15) &&
               (noteOff._GetType() == ::mus::midi::NoteOn)) ||
               (noteOff._GetType() == ::mus::midi::NoteOff)) &&
               (noteOn.GetNotePitch() == noteOff.GetNotePitch()) &&
               (noteOff.GetTrack() == iTrack))
            {
               if(noteOff.GetNoteVelocity() > 0 && noteOff._GetType() == ::mus::midi::NoteOn)
               {
                  ///int iNotVel = noteOff.GetNoteVelocity();
                  TRACE("dedo de veludo");
               }
               noteOff.set_type(::mus::midi::NoteOff);
               noteOff.SetTrack((byte) iTrack);
               noteOff.SetNoteVelocity(0);
               break;
            }
         }

         while(noteOn.m_tkPosition >= tkNextTokenPosition)
         {
            if(iNotesCount == 0)
            {
               //               pNoteOn = new CMidiEvent();
               //               pNoteOn->set_type(::mus::midi::NoteOn);
               //               pNoteOn->SetTrack(i);
               //               pNoteOn->SetNotePitch(0);
               //               pNoteOn->SetNoteVelocity(1);
               //               pNoteOn->tkPosition = tkTokenPosition;
               //               noteOnEvents.AddEvent(pNoteOn);
               //               pNoteOff = new CMidiEvent();
               //               pNoteOff->set_type(::mus::midi::NoteOff);
               //               pNoteOff->SetTrack(i);
               //               pNoteOff->SetNotePitch(0);
               //               pNoteOff->SetNoteVelocity(0);
               //               pNoteOff->tkPosition = tkNextTokenPosition;
               //               noteOffEvents.AddEvent(pNoteOff);

               noteOnB.set_type(::mus::midi::NoteOn);
               noteOnB.SetTrack((byte) i);
               noteOnB.SetNotePitch(0);
               noteOnB.SetNoteVelocity(1);
               noteOnB.m_tkPosition = tkTokenPosition;
               noteOnEvents.AddEvent(&noteOnB, tkTokenPosition);
               noteOffB.set_type(::mus::midi::NoteOff);
               noteOffB.SetTrack((byte) i);
               noteOffB.SetNotePitch(0);
               noteOffB.SetNoteVelocity(0);
               noteOffB.m_tkPosition = tkNextTokenPosition;
               noteOffEvents.AddEvent(&noteOffB, tkNextTokenPosition);
            }
            l++;
            if(l > positiona.get_size())
               break;
            iNotesCount = 0;
            tkTokenPosition = tkNextTokenPosition;
            if(l >= positiona.get_size())
               break;
            tkNextTokenPosition = positiona.get_at(l);
         }
         //         pNoteOn = new CMidiEvent();
         ///         pNoteOn->operator =(noteOn);
         //         noteOnEvents.AddEvent(pNoteOn);
         //         pNoteOff = new CMidiEvent();
         //         pNoteOff->operator =(noteOff);
         //         noteOffEvents.AddEvent(pNoteOff);
         noteOnEvents.AddEvent(&noteOn);
         noteOffEvents.AddEvent(&noteOff);
         iNotesCount++;
      }
      while(noteOn.m_tkPosition >= tkNextTokenPosition)
      {
         if(iNotesCount == 0)
         {

            noteOnB.set_type(::mus::midi::NoteOn);
            noteOnB.SetTrack((byte) i);
            noteOnB.SetNotePitch(0);
            noteOnB.SetNoteVelocity(1);
            noteOnB.m_tkPosition = tkTokenPosition;
            noteOnEvents.AddEvent(&noteOnB);
            noteOffB.set_type(::mus::midi::NoteOff);
            noteOffB.SetTrack((byte) i);
            noteOffB.SetNotePitch(0);
            noteOffB.SetNoteVelocity(0);
            noteOffB.m_tkPosition = tkNextTokenPosition;
            noteOffEvents.AddEvent(&noteOffB);
         }
         l++;
         if(l > positiona.get_size())
            break;
         iNotesCount = 0;
         tkTokenPosition = tkNextTokenPosition;
         if(l >= positiona.get_size())
            break;
         tkNextTokenPosition = positiona.get_at(l);
      }
      ASSERT(pNoteOnEvents != NULL);
      ASSERT(pNoteOffEvents != NULL);
      ASSERT(pNoteOnEvents->get_size() == 0);
      ASSERT(pNoteOffEvents->get_size() == 0);
      *pNoteOnEvents   = noteOnEvents;
      *pNoteOffEvents  = noteOffEvents;
   }
   //else if(iFormat == 1)
   else if(true)
   {
      k = 0;
      int iNoteCount = pNoteOnOffEvents->get_size();
      if(positiona.get_size() <= 0)
      {
      }
      else
      {  

         // Extract All Note On and Note Off Events

         int iNoteOn, iNoteOff;
         for(iNoteOn = 0; iNoteOn < iNoteCount; iNoteOn++)
         {   
            pNoteOnOffEvents->GetEvent(&noteOn, iNoteOn);
            if(noteOn.GetNoteVelocity() == 0 ||
               (noteOn._GetType() == ::mus::midi::NoteOff))
               continue;
            for(iNoteOff = iNoteOn + 1;; iNoteOff++)
            {
               if(iNoteOff >= iNoteCount)
               {
                  noteOff.set_type(::mus::midi::NoteOff);
                  noteOff.SetTrack((byte) iTrack);
                  noteOff.SetNotePitch(noteOn.GetNotePitch());
                  noteOff.SetNoteVelocity(0);
                  noteOff.m_tkPosition = 0x7fffffff;
                  break;
               }
               pNoteOnOffEvents->GetEvent(&noteOff, iNoteOff);
               if((((noteOff.GetNoteVelocity() <= 15) &&
                  (noteOff._GetType() == ::mus::midi::NoteOn)) ||
                  (noteOff._GetType() == ::mus::midi::NoteOff)) &&
                  (noteOn.GetNotePitch() == noteOff.GetNotePitch()))
               {
                  if(noteOff.GetNoteVelocity() > 0 && noteOff._GetType() == ::mus::midi::NoteOn)
                  {
                     //int iNotVel = noteOff.GetNoteVelocity();
                     TRACE("dedo de veludo");
                  }
                  noteOff.set_type(::mus::midi::NoteOff);
                  noteOff.SetTrack((byte) iTrack);
                  noteOff.SetNoteVelocity(0);
                  break;
               }
            }
            noteOnEvents.AddEvent(&noteOn);
            noteOffEvents.AddEvent(&noteOff);
         }

         // Insert a Note On and a Note Off event
         // for every token with no Note On and Note Off events.

         int iToken;
         int iSize = positiona.get_size();
         imedia::position tkBegin;
         imedia::position tkEnd;
         iNoteOn = 0;
         int iFoundCount;
         ticka.add(imedia::position(0xffffffff));
         for(iToken = 0; iToken < iSize; iToken++)
         {   
            tkBegin = ticka.element_at(iToken);
            tkEnd = ticka.element_at(iToken + 1);
            iFoundCount = 0;
            for(;iNoteOn  < noteOnEvents.get_size(); iNoteOn++)
            {

               noteOnEvents.GetEvent(&noteOn, iNoteOn);
               if(noteOn.m_tkPosition < tkBegin)
               {
                  continue;
               }
               if(noteOn.m_tkPosition < tkEnd)
               {
                  iFoundCount++;
                  continue;
               }
               break;
            }
            if(iFoundCount == 0)
            {
               noteOnB.set_type(::mus::midi::NoteOn);
               noteOnB.SetTrack((byte) i);
               noteOnB.SetNotePitch(0);
               noteOnB.SetNoteVelocity(1);
               noteOnB.m_tkPosition = tkBegin;
               noteOnEvents.InsertEvent(iNoteOn, noteOnB);
               noteOffB.set_type(::mus::midi::NoteOff);
               noteOffB.SetTrack((byte) i);
               noteOffB.SetNotePitch(0);
               noteOffB.SetNoteVelocity(0);
               noteOffB.m_tkPosition = tkEnd;
               noteOffEvents.InsertEvent(iNoteOn, noteOffB);
            }

         }


         /*            tkTokenPosition = positiona.get_at(0);
         if(positiona.get_size() > 1)
         {
         tkNextTokenPosition = positiona.get_at(1);
         }
         else
         {
         tkNextTokenPosition = 0x7fffffff;
         }

         for(l = 1, i = 0; i < iNoteCount; i++)
         {   
         pNoteOnOffEvents->GetEvent(&noteOn, i);
         if(noteOn.GetNoteVelocity() == 0 ||
         (noteOn._GetType() == ::mus::midi::NoteOff))
         continue;
         for(j = i + 1;; j++)
         {
         if(j >= iSize)
         {
         noteOff.set_type(::mus::midi::NoteOff);
         noteOff.SetTrack(iTrack);
         noteOff.SetNotePitch(noteOn.GetNotePitch());
         noteOff.SetNoteVelocity(0);
         noteOff.m_tkPosition = 0x7fffffff;
         break;
         }
         pNoteOnOffEvents->GetEvent(&noteOff, j);
         if((((noteOff.GetNoteVelocity() == 0) &&
         (noteOff._GetType() == ::mus::midi::NoteOn)) ||
         (noteOff._GetType() == ::mus::midi::NoteOff)) &&
         (noteOn.GetNotePitch() == noteOff.GetNotePitch()))
         {
         noteOff.set_type(::mus::midi::NoteOff);
         noteOff.SetTrack(iTrack);
         noteOff.SetNoteVelocity(0);
         break;
         }
         }
         while(noteOn.m_tkPosition >= tkNextTokenPosition)
         {
         if(iNotesCount == 0)
         {
         //               pNoteOn = new CMidiEvent();
         //               pNoteOn->set_type(::mus::midi::NoteOn);
         //               pNoteOn->SetTrack(i);
         //               pNoteOn->SetNotePitch(0);
         //               pNoteOn->SetNoteVelocity(1);
         //               pNoteOn->tkPosition = tkTokenPosition;
         //               noteOnEvents.AddEvent(pNoteOn);
         //               pNoteOff = new CMidiEvent();
         //               pNoteOff->set_type(::mus::midi::NoteOff);
         //               pNoteOff->SetTrack(i);
         //               pNoteOff->SetNotePitch(0);
         //               pNoteOff->SetNoteVelocity(0);
         //               pNoteOff->tkPosition = tkNextTokenPosition;
         //               noteOffEvents.AddEvent(pNoteOff);

         noteOnB.set_type(::mus::midi::NoteOn);
         noteOnB.SetTrack(i);
         noteOnB.SetNotePitch(0);
         noteOnB.SetNoteVelocity(1);
         noteOnB.m_tkPosition = tkTokenPosition;
         noteOnEvents.AddEvent(&noteOnB);
         noteOffB.set_type(::mus::midi::NoteOff);
         noteOffB.SetTrack(i);
         noteOffB.SetNotePitch(0);
         noteOffB.SetNoteVelocity(0);
         noteOffB.m_tkPosition = tkNextTokenPosition;
         noteOffEvents.AddEvent(&noteOffB);
         }
         l++;
         if(l > positiona.get_size())
         break;
         iNotesCount = 0;
         tkTokenPosition = tkNextTokenPosition;
         if(l >= positiona.get_size())
         break;
         tkNextTokenPosition = positiona.get_at(l);
         }
         //         pNoteOn = new CMidiEvent();
         //         pNoteOn->operator =(noteOn);
         //         noteOnEvents.AddEvent(pNoteOn);
         //         pNoteOff = new CMidiEvent();
         //         pNoteOff->operator =(noteOff);
         //         noteOffEvents.AddEvent(pNoteOff);
         noteOnEvents.AddEvent(&noteOn);
         noteOffEvents.AddEvent(&noteOff);
         iNotesCount++;
         }
         while(noteOn.m_tkPosition >= tkNextTokenPosition)
         {
         if(iNotesCount == 0)
         {

         noteOnB.set_type(::mus::midi::NoteOn);
         noteOnB.SetTrack(i);
         noteOnB.SetNotePitch(0);
         noteOnB.SetNoteVelocity(1);
         noteOnB.m_tkPosition = tkTokenPosition;
         noteOnEvents.AddEvent(&noteOnB);
         noteOffB.set_type(::mus::midi::NoteOff);
         noteOffB.SetTrack(i);
         noteOffB.SetNotePitch(0);
         noteOffB.SetNoteVelocity(0);
         noteOffB.m_tkPosition = tkNextTokenPosition;
         noteOffEvents.AddEvent(&noteOffB);
         }
         l++;
         if(l > positiona.get_size())
         break;
         iNotesCount = 0;
         tkTokenPosition = tkNextTokenPosition;
         if(l >= positiona.get_size())
         break;
         tkNextTokenPosition = positiona.get_at(l);
         }
         //          pNoteOnEvents- = noteOnEvents;
         //          pNoteOffEvents- = noteOffEvents;
         */   
      }
   }

}








void MidiUtil::
   PrepareLevel2Events(
   MidiEventsV1 *      peventsLevel2Beg,
   MidiEventsV1 *      peventsLevel2End,
   int               iTrack,
   int               iFormat,
   MidiEventsV1 *      peventsLevel2,
   imedia::position_array & positiona)
{
   UNREFERENCED_PARAMETER(iFormat);
   ASSERT(peventsLevel2Beg->get_size() == 0);
   ASSERT(peventsLevel2End->get_size() == 0);
   imedia::position_array ticka;
   ticka = positiona;
   MidiEventV001 noteOn, noteOff;
   MidiEventV001 noteOnB, noteOffB;
   MidiEventsV1 & eventsBeg = *peventsLevel2Beg;
   MidiEventsV1 & eventsEnd = *peventsLevel2End;
   MidiEventV001 eventBeg, eventEnd;
   MidiEventV001 eventBegB, eventEndB;
   imedia::position tkTokenPosition, tkNextTokenPosition;
   int iEventCount;
   int iSize;
   eventBeg.m_tkPosition = 0x7fffffff;
   int i, j, k, l;
   k = 0;
   iSize = peventsLevel2->get_size();
   iEventCount = 0;
   if(positiona.get_size() <= 0)
   {
   }
   else
   {   
      tkTokenPosition = positiona.get_at(0);
      if(positiona.get_size() > 1)
      {
         tkNextTokenPosition = positiona.get_at(1);
      }
      else
      {
         tkNextTokenPosition = 0x7fffffff;
      }

      for(l = 1, i = 0; i < iSize; i++)
      {   
         peventsLevel2->GetEvent(&eventBeg, i);
         if(eventBeg.GetNoteVelocity() == 0 ||
            (eventBeg._GetType() == ::mus::midi::NoteOff))
            continue;
         for(j = i + 1;; j++)
         {
            if(j >= iSize)
            {
               eventEnd.set_type(::mus::midi::NoteOff);
               eventEnd.SetTrack((byte) iTrack);
               eventEnd.SetNotePitch(eventBeg.GetNotePitch());
               eventEnd.SetNoteVelocity(0);
               eventEnd.m_tkPosition = 0x7fffffff;
               break;
            }
            peventsLevel2->GetEvent(&eventEnd, j);
            if(
               (
               (
               (
               (eventEnd.GetNoteVelocity() == 0)
               && (eventEnd._GetType() == ::mus::midi::NoteOn)
               )
               || (eventEnd._GetType() == ::mus::midi::NoteOff)
               )
               && (eventBeg.GetNotePitch() == eventEnd.GetNotePitch())
               )
               )
            {
               eventEnd.set_type(::mus::midi::NoteOff);
               eventEnd.SetTrack((byte) iTrack);
               eventEnd.SetNoteVelocity(0);
               break;
            }
            else if(eventEnd._GetType() == ::mus::midi::PitchBend)
            {
               eventEnd.SetTrack((byte) iTrack);
               break;
            }
         }
         // add blank start and end events for
         // tokens events if there is no
         // events during the token event
         while(eventBeg.m_tkPosition >= tkNextTokenPosition)
         {
            if(iEventCount == 0)
            {
               eventBegB.set_type(::mus::midi::NoteOn);
               eventBegB.SetTrack((byte) (i & 0xff));
               eventBegB.SetNotePitch(0);
               eventBegB.SetNoteVelocity(1);
               eventBegB.m_tkPosition = tkTokenPosition;
               eventsBeg.AddEvent(&eventBegB);
               eventEndB.set_type(::mus::midi::NoteOff);
               eventEndB.SetTrack((byte) (i & 0xff));
               eventEndB.SetNotePitch(0);
               eventEndB.SetNoteVelocity(0);
               eventEndB.m_tkPosition = tkNextTokenPosition;
               eventsEnd.AddEvent(&eventEndB);
            }
            l++;
            if(l > positiona.get_size())
               break;
            iEventCount = 0;
            tkTokenPosition = tkNextTokenPosition;
            if(l >= positiona.get_size())
               break;
            tkNextTokenPosition = positiona.get_at(l);
         }
         eventsBeg.AddEvent(&eventBeg);
         eventsEnd.AddEvent(&eventEnd);
         iEventCount++;
      }
      // Insert a Note On and a Note Off event
      // for every token with no Note On and Note Off events.

      int iToken;
      int iSize = positiona.get_size();
      imedia::position tkBegin;
      imedia::position tkEnd;
      int iNoteOn = 0;
      int iFoundCount;
      ticka.add(imedia::position(0x7fffffff));
      for(iToken = 0; iToken < iSize; iToken++)
      {   
         tkBegin = ticka.element_at(iToken);
         tkEnd = ticka.element_at(iToken + 1);
         iFoundCount = 0;
         for(;iNoteOn  < eventsBeg.get_size(); iNoteOn++)
         {

            eventsBeg.GetEvent(&noteOn, iNoteOn);
            if(noteOn.m_tkPosition < tkBegin)
            {
               continue;
            }
            if(noteOn.m_tkPosition < tkEnd)
            {
               iFoundCount++;
               continue;
            }
            break;
         }
         if(iFoundCount == 0)
         {
            noteOnB.set_type(::mus::midi::NoteOn);
            noteOnB.SetTrack((byte) (i & 0xff));
            noteOnB.SetNotePitch(0);
            noteOnB.SetNoteVelocity(1);
            noteOnB.m_tkPosition = tkBegin;
            eventsBeg.InsertEvent(iNoteOn, noteOnB);
            noteOffB.set_type(::mus::midi::NoteOff);
            noteOffB.SetTrack((byte) (i & 0xff));
            noteOffB.SetNotePitch(0);
            noteOffB.SetNoteVelocity(0);
            noteOffB.m_tkPosition = tkEnd;
            eventsEnd.InsertEvent(iNoteOn, noteOffB);
         }

      }
      //      ASSERT(peventsLevel2Beg->get_size() == 0);
      //      ASSERT(peventsLevel2End->get_size() == 0);
      //      peventsLevel2Beg- = eventsBeg;
      //      peventsLevel2End- = eventsEnd;
   }


}



MMRESULT MidiUtil::ConvertTune1000ToXF(::mus::midi::sequence &seq, int iOpenMode)
{
   ::mus::midi::sequence seqTune1000(get_app());
   MMRESULT vmsr = seqTune1000.OpenFile(seq,  ::mus::midi::OpenForPlaying);
   if(vmsr != VMSR_SUCCESS)
      return vmsr;
   return ConvertTune1000ToXF(seqTune1000, seq, iOpenMode);
}


MMRESULT MidiUtil::ConvertTune1000ToXF(::mus::midi::sequence &seqTune1000, ::mus::midi::sequence &seqXF, int iOpenMode)
{
   ::mus::midi::file &              tune1000file = seqTune1000.GetFile();
   ::mus::midi::file &              xffile = seqXF.GetFile();
   string2a                str2aTokens;
   imedia::position_2darray                tka2DTicks;
   stringa                 straTokens;
   imedia::position_array                  tkaTicks;
   stringa *               lpstraTokens;
   imedia::position_array *                 lptkaTicks;
   MidiTrack *               pXFTrk;
   MidiTrack *               pTune1000Trk;
   ::mus::midi::e_file_result      smfrc;
   string                  str;
   imedia::position                     tkPosition;
   imedia::position                     tkLastPosition;
   MIDIFILEHDR             fileHdr;
   string                  strToken;
   SoftKaraokeInfo         ski;
   CTune1000Info           ki;
   int                     i, j, k;
   bool                    bKarInfoValid;
   bool                    bTuneInfoValid = false;
   MidiTracks &            tracks = m_tracks;


   tracks.GetFlags().signalize(::mus::midi::DisablePlayLevel1Operations);

   // try to get SoftKaraoke Information Header because IsTune1000 file may have wrongly identified a Tune1000 File
   tune1000file.GetTracks().seek_begin();
   bKarInfoValid = ::mus::midi::Success == tune1000file.GetTracks().GetKarInfoHeader(&ski);

   seqXF.CloseFile();

   int iTokenCount = -1;
   int iFoundTrack = -1;
   for(i = 0; i < tune1000file.GetTracks().GetTrackCount(); i++)
   {
      tune1000file.GetTracks().seek_begin();
      if(!bTuneInfoValid)
      {
         bTuneInfoValid = ::mus::midi::Success == tune1000file.GetTracks().GetTune1000InfoHeader(&ki, i);
      }
      pTune1000Trk = (MidiTrack *) tune1000file.GetTracks().TrackAt(i);
      if(pTune1000Trk->get_type() != MidiTrackBase::TypeMidi)
         continue;
      pTune1000Trk->ToWorkStorage();
      pTune1000Trk->WorkSeekBegin();
      if(pTune1000Trk->WorkReadKarTokens(straTokens, &tkaTicks)
         == ::mus::midi::Success)
      {
         lptkaTicks = new imedia::position_array();
         *lptkaTicks = tkaTicks;
         tka2DTicks.add(lptkaTicks);

         lpstraTokens = new stringa();
         *lpstraTokens = straTokens;
         str2aTokens.add(lpstraTokens);

         if(straTokens.get_size() > iTokenCount)
         {
            iTokenCount = straTokens.get_size();
            iFoundTrack = i;
         }
      }
      pTune1000Trk->WorkSeekBegin();
      if(pTune1000Trk->WorkReadTune1000Tokens(straTokens, &tkaTicks)
         == ::mus::midi::Success)
      {

         lptkaTicks = new imedia::position_array();
         *lptkaTicks = tkaTicks;
         tka2DTicks.add(lptkaTicks);

         lpstraTokens = new stringa();
         *lpstraTokens = straTokens;
         str2aTokens.add(lpstraTokens);

         if(straTokens.get_size() > iTokenCount)
         {
            iTokenCount = straTokens.get_size();
            iFoundTrack = i;
         }
      }

      MidiTrack * pTrackXF = tracks.CreateTrack();
      if((smfrc = pTrackXF->WorkCopyWork(
         pTune1000Trk,
         MidiTrack::CopyExcludeXFMetaLyrics |
         MidiTrack::CopyExcludeKarMetaLyrics)) != ::mus::midi::Success)
         return smfrc;
   }
   
   {
      MidiTrack * pTrackUnion = xffile.GetTracks().CreateTrack();
      tracks.WorkCompactMidiTracksWork(*pTrackUnion, false);
   }

   XFInfoHeader xfInfoHeader;

   pXFTrk = xffile.GetTracks().CreateTrack(FOURCC_XFIH);

   if(bTuneInfoValid)
   {
      xfInfoHeader.FromTune1000Header(&ki);
      xfInfoHeader.SetID();
      pXFTrk->WorkWriteXFInfoHeader(
         ki.m_strSongName,
         &xfInfoHeader,
         NULL);
   }
   else if(bKarInfoValid)
   {
      xfInfoHeader.FromKarHeader(&ski);
      xfInfoHeader.SetID();
      pXFTrk->WorkWriteXFInfoHeader(
         NULL,
         &xfInfoHeader,
         NULL);
   }

   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
   pXFTrk->GetEvent().SetParam(NULL, 0);
   pXFTrk->GetEvent().SetDelta(0);
   pXFTrk->WorkWriteEvent();

   if(str2aTokens.get_size() > 0)
   {
      for(i = 0; i < str2aTokens.get_size(); i++)
      {
         tkLastPosition = 0;
         pXFTrk = xffile.GetTracks().CreateTrack();
         pXFTrk->SetAutoAllocation();
         pXFTrk->seek_begin();
         lpstraTokens = str2aTokens.ptr_at(i);
         lptkaTicks = tka2DTicks.ptr_at(i);
         //      tkLastPosition = 0;
         for(j = 0; j < lpstraTokens->get_size(); j++)
         {
            strToken = lpstraTokens->element_at(j);
            tkPosition = lptkaTicks->get_at(j);
            //str.Empty();
            if(strToken.Mid(0, 1).Compare("\\") == 0)
            {
               str = "<";
               str += strToken.Mid(1);
            }
            else if(strToken.Mid(0, 1).Compare("/") == 0)
            {
               str = ">";
               str += strToken.Mid(1);
            }
            else if(strToken.Compare("\r") == 0)
            {
               str = "/";
               k = j;
               while(tkLastPosition >= (tkPosition = lptkaTicks->get_at(k)))
               {
                  if(++k >= lptkaTicks->get_size())
                     break;
               }
               //            pXFTrk->WriteXFLyricEvent(str, tkPosition);
               //            str = pStr->Mid(1);
            }
            else if(strToken.Compare("\n") == 0)
            {
               j++;
               str = "<";
               if(j < lpstraTokens->get_size())
               {
                  strToken = lpstraTokens->element_at(j);
                  tkPosition = lptkaTicks->get_at(j);
                  str += strToken;
               }
               //            pXFTrk->WriteXFLyricEvent(str, tkPosition);
               //            str = pStr->Mid(1);
            }
            else
            {
               str = strToken;
            }
            // if next token is a tab, a carriage return is needed here
            if((j + 1) < lpstraTokens->get_size())
            {
               if(lpstraTokens->element_at(j + 1).Mid(0, 1) == "/")
               {
                  str += "/";
               }
            }
            str.replace(" ", "^");
            pXFTrk->WriteXFLyricEvent(str, tkPosition);
            tkLastPosition = tkPosition;
         }
         pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
         pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
         pXFTrk->GetEvent().SetParam(NULL, 0);
         pXFTrk->GetEvent().SetDelta(0);
         pXFTrk->WorkWriteEvent();

      }
   }
   fileHdr.wFormat = WORDSWAP(0);
   fileHdr.wDivision = tune1000file.m_pFileHeader->wDivision;
   fileHdr.wTracks = WORDSWAP(1) ;

   BYTE XFVERSIONID[] = {
      0x43,  // YAMAHA ID
      0x7B,  //
      0x00,  //
      0x58,  // X
      0x46,  // F
      0x30,  // 0 
      0x32,   // 2
      0x00,
      0x11};

      MidiTrack & trackXF = *(MidiTrack *) xffile.GetTracks().TrackAt(0);
      trackXF.seek_begin();
      MidiEventV008 eventV008;
      eventV008.SetPosition(0);
      eventV008.SetFullType(::mus::midi::Meta);
      eventV008.SetMetaType(::mus::midi::MetaSeqSpecific);
      eventV008.SetParam(XFVERSIONID, sizeof(XFVERSIONID));
      trackXF.GetWorkTrack().insert_at(0, eventV008);

      primitive::memory memstorage;

      eventV008.clear();
      eventV008.SetPosition(0);
      eventV008.SetFullType(::mus::midi::Meta);
      eventV008.SetMetaType(CXF::MetaSongName);
      gen::international::OEMToMultiByte(
         gen::international::CodePageLatin1,
         memstorage,
         ki.m_strSongName);
      eventV008.SetParam(
         memstorage.GetAllocation(),
         memstorage.GetStorageSize());
      trackXF.GetWorkTrack().insert_at(1, eventV008);

      for(i = 0; i < xffile.GetTracks().GetTrackCount(); i++)
      {
         MidiTrack & trackXF = *(MidiTrack *) xffile.GetTracks().TrackAt(i);
         trackXF.WorkSeekBegin();
         while(::mus::midi::Success == trackXF.WorkSeek(::mus::midi::SeekKarID))
         {
            trackXF.WorkDeleteEvent();
         }
      }
      if((smfrc = xffile.WorkMergeTracks()) != ::mus::midi::Success)
         return smfrc;

      xffile.WriteHeader(&fileHdr);
      xffile.m_iOpenMode = iOpenMode;
      seqXF.m_iOpenMode = iOpenMode;

      if((smfrc = xffile.Build()) != ::mus::midi::Success)
         return smfrc;

      SMFFILEINFO             sfi;

      xffile.GetFileInfo(&sfi);

      seqXF.m_dwTimeDivision        = sfi.dwTimeDivision;
      seqXF.m_tkLength              = sfi.tkLength;
      seqXF.m_msLength               = seqXF.TicksToMillisecs(seqXF.m_tkLength);

      seqXF.SetState(::mus::midi::sequence::StatusOpened);



      return ::mus::midi::Success;

}

::mus::midi::e_file_type MidiUtil::GetFileType(::mus::midi::sequence &seq)
{
   ::mus::midi::e_file_type e_type;

   ::mus::midi::file & midifile = seq.GetFile();

   if(midifile.IsSoftKaraokeFile())
   {
      e_type = ::mus::midi::TypeKar;
   }
   else if(midifile.IsXFFile())
   {   
      e_type = ::mus::midi::TypeXF;
   }
   else if(midifile.IsTune1000File())
   {
      e_type = ::mus::midi::TypeTune1000;
   }
   else
   {
      e_type = ::mus::midi::TypeGM;
   }
   return e_type;

}

MMRESULT MidiUtil::ConvertToXF(::mus::midi::sequence &seq, int iOpenMode)
{
   int iFileType = GetFileType(seq);
   MMRESULT vmsr;
   if(iFileType == ::mus::midi::TypeKar)
   {
      vmsr = ConvertKarToXF(seq, iOpenMode);
   }
   else if(iFileType == ::mus::midi::TypeXF)
   {
      vmsr = 0;
   }
   else
   {
      vmsr = ConvertKarToXF(seq, iOpenMode);
   }
   return vmsr;

}

MMRESULT MidiUtil::ConvertKarToXF(stringa * pKar1DTokens, imedia::position_array * pKar1DTicks, MidiTrack * pXFTrk)
{
   BYTE XFLYRICSHEADER[] = "$Lyrc:1:270:L1";
   BYTE CUEPOINTSOLO[] = "&s";
   string str;
   imedia::position tkPosition;
   CHUNKHDR chHdr;



   pXFTrk->SetAutoAllocation();
   pXFTrk->seek_begin();

   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaXFLyricsHeader);
   //         pXFTrk->GetEvent().SetParamSize(sizeof(XFLYRICSHEADER) - 1);
   pXFTrk->GetEvent().SetParam(XFLYRICSHEADER, sizeof(XFLYRICSHEADER) - 1);
   pXFTrk->WriteCompleteEvent(0x7fffffff, false);

   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaCuePoint);
   //pXFTrk->GetEvent().SetParamSize(sizeof(CUEPOINTSOLO) - 1);
   pXFTrk->GetEvent().SetParam(CUEPOINTSOLO, sizeof(CUEPOINTSOLO) - 1);
   pXFTrk->WriteCompleteEvent(0x7fffffff, false);

   //      tkLastPosition = 0;
   for(int j = 0; j < pKar1DTokens->get_size(); j++)
   {
      string strToken = pKar1DTokens->element_at(j);
      tkPosition = pKar1DTicks->get_at(j);
      //str.Empty();
      if(strToken[0] == '\\')
      {
         str = "<";
         str += strToken.Mid(1);
      }
      else if(strToken[0] == '/')
      {
         str = "/";
         pXFTrk->WriteXFLyricEvent(str, tkPosition);
         str = strToken.Mid(1);
      }
      else
      {
         str = strToken;
      }
      str.replace(" ", "^");
      pXFTrk->WriteXFLyricEvent(str, tkPosition);
   }
   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
   pXFTrk->GetEvent().SetParam(NULL, 0);
   pXFTrk->WriteCompleteEvent(0x7fffffff, false);
   chHdr.fourccType = FOURCC_XFKM;
   chHdr.dwLength = pXFTrk->m_smti.m_cbLength;
   pXFTrk->WriteHeader(&chHdr);

   return 0;

}

MMRESULT MidiUtil::WorkConvertKarToXF(stringa * pKar1DTokens, imedia::position_array * pKar1DTicks, MidiTrack * pXFTrk)
{
   BYTE XFLYRICSHEADER[] = "$Lyrc:1:270:L1";
   BYTE CUEPOINTSOLO[] = "&s";
   string str;
   imedia::position tkPosition;

   pXFTrk->GetWorkTrack().remove_all();

   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaXFLyricsHeader);
   pXFTrk->GetEvent().SetParam(XFLYRICSHEADER, sizeof(XFLYRICSHEADER) - 1);
   pXFTrk->GetEvent().SetDelta(0);
   pXFTrk->WorkWriteEvent();

   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaCuePoint);
   pXFTrk->GetEvent().SetParam(CUEPOINTSOLO, sizeof(CUEPOINTSOLO) - 1);
   pXFTrk->GetEvent().SetDelta(0);
   pXFTrk->WorkWriteEvent();

   for(int j = 0; j < pKar1DTokens->get_size(); j++)
   {
      string strToken = pKar1DTokens->element_at(j);
      tkPosition = pKar1DTicks->get_at(j);
      //str.Empty();
      if(strToken.Mid(0, 1).Compare("\\") == 0)
      {
         str = "<";
         str += strToken.Mid(1);
      }
      else if(strToken.Mid(0, 1).Compare("/") == 0)
      {
         str = "/";
         pXFTrk->WorkWriteXFLyricEvent(str, tkPosition);
         pXFTrk->m_tkPosition = tkPosition;
         str = strToken.Mid(1);
      }
      else
      {
         str = strToken;
      }
      str.replace(" ", "^");
      pXFTrk->WorkWriteXFLyricEvent(str, tkPosition);
      pXFTrk->m_tkPosition = tkPosition;
   }
   pXFTrk->GetEvent().SetFullType(::mus::midi::Meta);
   pXFTrk->GetEvent().SetMetaType(::mus::midi::MetaEOT);
   pXFTrk->GetEvent().SetParam(NULL, 0);
   pXFTrk->WorkWriteEvent();

   return 0;

}


bool MidiUtil::ExtractLyrics(string &str, ::mus::midi::sequence &seq)
{
   UNREFERENCED_PARAMETER(str);
   UNREFERENCED_PARAMETER(seq);
   return false;
}
