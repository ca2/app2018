#pragma once

class MidiEventV001;

class CLASS_DECL_ca MidiEventV508 :
    public MidiEventBase,
    public ::radix::object
{
public:
   union
   {
      DWORD                m_lpdwData[3];
      short                m_lpshData[3 * sizeof(DWORD) / sizeof(short)];
      unsigned short       m_lpushData[3 * sizeof(DWORD) / sizeof(short)];
      BYTE                 m_lpbData[3 * sizeof(DWORD)];
      struct
      {
         DWORD               m_dw1;
         int               m_iParamSize;
         void *            m_pParam;
      };
   };
   int      m_iFlags;
   int      m_iAllocate;
   void *   m_pvoid;
   DWORD    m_dwInternal;

   MidiEventV508();
   virtual ~MidiEventV508();

#if !core_level_1
   using ::MidiEventBase::SetParam;
#endif

   virtual void clear();

   virtual VMSRESULT WriteParam(LPVOID lpData, int iSize);
   virtual VMSRESULT FromData(LPVOID lpData, LPINT lpiLeft, BYTE &bRunningStatus);

   virtual MidiEventBase & operator = (const MidiEventBase & eventSrc) { return MidiEventBase::operator = (eventSrc); }
   virtual MidiEventBase & operator = (const MidiEventV008 & eventSrc) { return MidiEventBase::operator = (eventSrc); }
   virtual MidiEventV508 & operator = (const MidiEventV508 & eventSrc);

   imedia::position GetDelta() const;
   void SetDelta(imedia::position tkDelta);
   imedia::position GetPosition() const;
   void SetPosition(imedia::position tkPosition);
   BYTE GetFullType() const;
   void SetFullType(BYTE bValue);
   BYTE get_type() const;
   void set_type(BYTE bValue);
   BYTE GetTrack() const;
   void SetTrack(BYTE bValue);
   BYTE GetMetaType() const;
   void SetMetaType(BYTE bValue);
   BYTE GetChB1() const;
   void SetChB1(BYTE bValue);
   BYTE GetChB2() const;
   void SetChB2(BYTE bValue);
   BYTE GetNotePitch() const;
   void SetNotePitch(BYTE bValue);
   BYTE GetNoteVelocity() const;
   void SetNoteVelocity(BYTE bValue);
   void SetImage(byte * hpbImage);
   byte * GetImage() const;
   int GetImageSize() const;
   void SetImageSize(int cbSize);
   virtual void SetParam(void * pVoid, int iSize);
   void _Free();
   bool _Allocate(int iSize);
   byte * GetParam() const;
   int GetParamSize() const;
   virtual VMSRESULT SetParam(ex1::file & file, int iLength = -1);
   virtual int GetFlags() const;
   virtual void SetFlags(int iFlag);

    VMSRESULT GetMidiEventV001(MidiEventV001 * lpMidiEventV001,  DWORD dwPositionTicks);


   BYTE     _GetType() const;
   BYTE     _GetTrack() const;
   BYTE     _GetMetaType() const;
   void     _SetMetaType(BYTE bValue);
   BYTE     _GetFullType() const;
   void     _SetFullType(BYTE bValue);
   BYTE     _GetChB1() const;
   void     _SetChB1(BYTE bValue);
   BYTE     _GetChB2() const;
   void     _SetChB2(BYTE bValue);

   void     _SetImage(byte * hpbImage);
   byte *   _GetImage() const;
   int      _GetImageSize() const;
   void     _SetImageSize(int cbSize);

   int      _GetFlags() const;
   void     _SetFlags(int iFlag);

protected:
   void SetParamSize(int iSize);
};

