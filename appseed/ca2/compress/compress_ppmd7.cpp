/* Ppmd7.c -- PPMdH codec
2010-03-12 : Igor Pavlov : Public domain
This code is based on PPMd var.H (2001): Dmitry Shkarin : Public domain */
// from 7-zip on 2012-12-23, dawn
#include "framework.h"

const byte PPMD7_kExpEscape[16] = { 25, 14, 9, 7, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2 };
static const uint16_t kInitBinEsc[] = { 0x3CDD, 0x1F3F, 0x59BF, 0x48F3, 0x64A1, 0x5ABC, 0x6632, 0x6051};

#define MAX_FREQ 124
#define UNIT_SIZE 12

#define U2B(nu) ((uint32_t)(nu) * UNIT_SIZE)
#define U2I(nu) (p->Units2Indx[(nu) - 1])
#define I2U(indx) (p->Indx2Units[indx])

#ifdef PPMD_32BIT
  #define REF(ptr) (ptr)
#else
  #define REF(ptr) ((uint32_t)((byte *)(ptr) - (p)->Base))
#endif

#define STATS_REF(ptr) ((CPpmd_State_Ref)REF(ptr))

#define CTX(ref) ((CPpmd7_Context *)Ppmd7_GetContext(p, ref))
#define STATS(ctx) Ppmd7_GetStats(p, ctx)
#define ONE_STATE(ctx) Ppmd7Context_OneState(ctx)
#define SUFFIX(ctx) CTX((ctx)->Suffix)

typedef CPpmd7_Context * CTX_PTR;

struct CPpmd7_Node_;

typedef
  #ifdef PPMD_32BIT
    struct CPpmd7_Node_ *
  #else
    uint32_t
  #endif
  CPpmd7_Node_Ref;

typedef struct CPpmd7_Node_
{
  uint16_t Stamp; /* must be at offset 0 as CPpmd7_Context::NumStats. Stamp=0 means free */
  uint16_t NU;
  CPpmd7_Node_Ref Next; /* must be at offset >= 4 */
  CPpmd7_Node_Ref Prev;
} CPpmd7_Node;

#ifdef PPMD_32BIT
  #define NODE(ptr) (ptr)
#else
  #define NODE(offs) ((CPpmd7_Node *)(p->Base + (offs)))
#endif

void Ppmd7_Construct(CPpmd7 *p)
{
  uint32_t i, k, m;

  p->Base = 0;

  for (i = 0, k = 0; i < PPMD_NUM_INDEXES; i++)
  {
    uint32_t step = (i >= 12 ? 4 : (i >> 2) + 1);
    do { p->Units2Indx[k++] = (byte)i; } while(--step);
    p->Indx2Units[i] = (byte)k;
  }

  p->NS2BSIndx[0] = (0 << 1);
  p->NS2BSIndx[1] = (1 << 1);
  memset(p->NS2BSIndx + 2, (2 << 1), 9);
  memset(p->NS2BSIndx + 11, (3 << 1), 256 - 11);

  for (i = 0; i < 3; i++)
    p->NS2Indx[i] = (byte)i;
  for (m = i, k = 1; i < 256; i++)
  {
    p->NS2Indx[i] = (byte)m;
    if (--k == 0)
      k = (++m) - 2;
  }

  memset(p->HB2Flag, 0, 0x40);
  memset(p->HB2Flag + 0x40, 8, 0x100 - 0x40);
}

void Ppmd7_Free(CPpmd7 *p, ::ca::ISzAlloc *alloc)
{
  alloc->Free(alloc, p->Base);
  p->Size = 0;
  p->Base = 0;
}

bool Ppmd7_Alloc(CPpmd7 *p, uint32_t size, ::ca::ISzAlloc *alloc)
{
  if (p->Base == 0 || p->Size != size)
  {
    Ppmd7_Free(p, alloc);
    p->AlignOffset =
      #ifdef PPMD_32BIT
        (4 - size) & 3;
      #else
        4 - (size & 3);
      #endif
    if ((p->Base = (byte *)alloc->Alloc(alloc, p->AlignOffset + size
        #ifndef PPMD_32BIT
        + UNIT_SIZE
        #endif
        )) == 0)
      return false;
    p->Size = size;
  }
  return true;
}

static void InsertNode(CPpmd7 *p, void *node, uint32_t indx)
{
  *((CPpmd_Void_Ref *)node) = p->FreeList[indx];
  p->FreeList[indx] = REF(node);
}

static void *RemoveNode(CPpmd7 *p, uint32_t indx)
{
  CPpmd_Void_Ref *node = (CPpmd_Void_Ref *)Ppmd7_GetPtr(p, p->FreeList[indx]);
  p->FreeList[indx] = *node;
  return node;
}

static void SplitBlock(CPpmd7 *p, void *ptr, uint32_t oldIndx, uint32_t newIndx)
{
  uint32_t i, nu = I2U(oldIndx) - I2U(newIndx);
  ptr = (byte *)ptr + U2B(I2U(newIndx));
  if (I2U(i = U2I(nu)) != nu)
  {
    uint32_t k = I2U(--i);
    InsertNode(p, ((byte *)ptr) + U2B(k), nu - k - 1);
  }
  InsertNode(p, ptr, i);
}

static void GlueFreeBlocks(CPpmd7 *p)
{
  #ifdef PPMD_32BIT
  CPpmd7_Node headItem;
  CPpmd7_Node_Ref head = &headItem;
  #else
  CPpmd7_Node_Ref head = p->AlignOffset + p->Size;
  #endif
  
  CPpmd7_Node_Ref n = head;
  uint32_t i;

  p->GlueCount = 255;

  /* create doubly-linked list of free blocks */
  for (i = 0; i < PPMD_NUM_INDEXES; i++)
  {
    uint16_t nu = I2U(i);
    CPpmd7_Node_Ref next = (CPpmd7_Node_Ref)p->FreeList[i];
    p->FreeList[i] = 0;
    while (next != 0)
    {
      CPpmd7_Node *node = NODE(next);
      node->Next = n;
      n = NODE(n)->Prev = next;
      next = *(const CPpmd7_Node_Ref *)node;
      node->Stamp = 0;
      node->NU = (uint16_t)nu;
    }
  }
  NODE(head)->Stamp = 1;
  NODE(head)->Next = n;
  NODE(n)->Prev = head;
  if (p->LoUnit != p->HiUnit)
    ((CPpmd7_Node *)p->LoUnit)->Stamp = 1;
  
  /* Glue free blocks */
  while (n != head)
  {
    CPpmd7_Node *node = NODE(n);
    uint32_t nu = (uint32_t)node->NU;
    for (;;)
    {
      CPpmd7_Node *node2 = NODE(n) + nu;
      nu += node2->NU;
      if (node2->Stamp != 0 || nu >= 0x10000)
        break;
      NODE(node2->Prev)->Next = node2->Next;
      NODE(node2->Next)->Prev = node2->Prev;
      node->NU = (uint16_t)nu;
    }
    n = node->Next;
  }
  
  /* Fill lists of free blocks */
  for (n = NODE(head)->Next; n != head;)
  {
    CPpmd7_Node *node = NODE(n);
    uint32_t nu;
    CPpmd7_Node_Ref next = node->Next;
    for (nu = node->NU; nu > 128; nu -= 128, node += 128)
      InsertNode(p, node, PPMD_NUM_INDEXES - 1);
    if (I2U(i = U2I(nu)) != nu)
    {
      uint32_t k = I2U(--i);
      InsertNode(p, node + k, nu - k - 1);
    }
    InsertNode(p, node, i);
    n = next;
  }
}

static void *AllocUnitsRare(CPpmd7 *p, uint32_t indx)
{
  uint32_t i;
  void *retVal;
  if (p->GlueCount == 0)
  {
    GlueFreeBlocks(p);
    if (p->FreeList[indx] != 0)
      return RemoveNode(p, indx);
  }
  i = indx;
  do
  {
    if (++i == PPMD_NUM_INDEXES)
    {
      uint32_t numBytes = U2B(I2U(indx));
      p->GlueCount--;
      return ((uint32_t)(p->UnitsStart - p->Text) > numBytes) ? (p->UnitsStart -= numBytes) : (::null());
    }
  }
  while (p->FreeList[i] == 0);
  retVal = RemoveNode(p, i);
  SplitBlock(p, retVal, i, indx);
  return retVal;
}

static void *AllocUnits(CPpmd7 *p, uint32_t indx)
{
  uint32_t numBytes;
  if (p->FreeList[indx] != 0)
    return RemoveNode(p, indx);
  numBytes = U2B(I2U(indx));
  if (numBytes <= (uint32_t)(p->HiUnit - p->LoUnit))
  {
    void *retVal = p->LoUnit;
    p->LoUnit += numBytes;
    return retVal;
  }
  return AllocUnitsRare(p, indx);
}

#define MyMem12Cpy(dest, src, num) \
  { uint32_t *d = (uint32_t *)dest; const uint32_t *s = (const uint32_t *)src; uint32_t n = num; \
    do { d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; s += 3; d += 3; } while(--n); }

static void *ShrinkUnits(CPpmd7 *p, void *oldPtr, uint32_t oldNU, uint32_t newNU)
{
  uint32_t i0 = U2I(oldNU);
  uint32_t i1 = U2I(newNU);
  if (i0 == i1)
    return oldPtr;
  if (p->FreeList[i1] != 0)
  {
    void *ptr = RemoveNode(p, i1);
    MyMem12Cpy(ptr, oldPtr, newNU);
    InsertNode(p, oldPtr, i0);
    return ptr;
  }
  SplitBlock(p, oldPtr, i0, i1);
  return oldPtr;
}

#define SUCCESSOR(p) ((CPpmd_Void_Ref)((p)->SuccessorLow | ((uint32_t)(p)->SuccessorHigh << 16)))

static void SetSuccessor(CPpmd_State *p, CPpmd_Void_Ref v)
{
  (p)->SuccessorLow = (uint16_t)((uint32_t)(v) & 0xFFFF);
  (p)->SuccessorHigh = (uint16_t)(((uint32_t)(v) >> 16) & 0xFFFF);
}

static void RestartModel(CPpmd7 *p)
{
  uint32_t i, k, m;

  memset(p->FreeList, 0, sizeof(p->FreeList));
  p->Text = p->Base + p->AlignOffset;
  p->HiUnit = p->Text + p->Size;
  p->LoUnit = p->UnitsStart = p->HiUnit - p->Size / 8 / UNIT_SIZE * 7 * UNIT_SIZE;
  p->GlueCount = 0;

  p->OrderFall = p->MaxOrder;
  p->RunLength = p->InitRL = -(int32_t)((p->MaxOrder < 12) ? p->MaxOrder : 12) - 1;
  p->PrevSuccess = 0;

  p->MinContext = p->MaxContext = (CTX_PTR)(p->HiUnit -= UNIT_SIZE); /* AllocContext(p); */
  p->MinContext->Suffix = 0;
  p->MinContext->NumStats = 256;
  p->MinContext->SummFreq = 256 + 1;
  p->FoundState = (CPpmd_State *)p->LoUnit; /* AllocUnits(p, PPMD_NUM_INDEXES - 1); */
  p->LoUnit += U2B(256 / 2);
  p->MinContext->Stats = REF(p->FoundState);
  for (i = 0; i < 256; i++)
  {
    CPpmd_State *s = &p->FoundState[i];
    s->Symbol = (byte)i;
    s->Freq = 1;
    SetSuccessor(s, 0);
  }

  for (i = 0; i < 128; i++)
    for (k = 0; k < 8; k++)
    {
      uint16_t *dest = p->BinSumm[i] + k;
      uint16_t val = (uint16_t)(PPMD_BIN_SCALE - kInitBinEsc[k] / (i + 2));
      for (m = 0; m < 64; m += 8)
        dest[m] = val;
    }
  
  for (i = 0; i < 25; i++)
    for (k = 0; k < 16; k++)
    {
      CPpmd_See *s = &p->See[i][k];
      s->Summ = (uint16_t)((5 * i + 10) << (s->Shift = PPMD_PERIOD_BITS - 4));
      s->Count = 4;
    }
}

void Ppmd7_Init(CPpmd7 *p, uint32_t maxOrder)
{
  p->MaxOrder = maxOrder;
  RestartModel(p);
  p->DummySee.Shift = PPMD_PERIOD_BITS;
  p->DummySee.Summ = 0; /* unused */
  p->DummySee.Count = 64; /* unused */
}

static CTX_PTR CreateSuccessors(CPpmd7 *p, int32_t skip)
{
  CPpmd_State upState;
  CTX_PTR c = p->MinContext;
  CPpmd_Byte_Ref upBranch = (CPpmd_Byte_Ref)SUCCESSOR(p->FoundState);
  CPpmd_State *ps[PPMD7_MAX_ORDER];
  uint32_t numPs = 0;
  
  if (!skip)
    ps[numPs++] = p->FoundState;
  
  while (c->Suffix)
  {
    CPpmd_Void_Ref successor;
    CPpmd_State *s;
    c = SUFFIX(c);
    if (c->NumStats != 1)
    {
      for (s = STATS(c); s->Symbol != p->FoundState->Symbol; s++);
    }
    else
      s = ONE_STATE(c);
    successor = SUCCESSOR(s);
    if (successor != upBranch)
    {
      c = CTX(successor);
      if (numPs == 0)
        return c;
      break;
    }
    ps[numPs++] = s;
  }
  
  upState.Symbol = *(const byte *)Ppmd7_GetPtr(p, upBranch);
  SetSuccessor(&upState, upBranch + 1);
  
  if (c->NumStats == 1)
    upState.Freq = ONE_STATE(c)->Freq;
  else
  {
    uint32_t cf, s0;
    CPpmd_State *s;
    for (s = STATS(c); s->Symbol != upState.Symbol; s++);
    cf = s->Freq - 1;
    s0 = c->SummFreq - c->NumStats - cf;
    upState.Freq = (byte)(1 + ((2 * cf <= s0) ? (5 * cf > s0) : ((2 * cf + 3 * s0 - 1) / (2 * s0))));
  }

  do
  {
    /* Create Child */
    CTX_PTR c1; /* = AllocContext(p); */
    if (p->HiUnit != p->LoUnit)
      c1 = (CTX_PTR)(p->HiUnit -= UNIT_SIZE);
    else if (p->FreeList[0] != 0)
      c1 = (CTX_PTR)RemoveNode(p, 0);
    else
    {
      c1 = (CTX_PTR)AllocUnitsRare(p, 0);
      if (!c1)
        return ::null();
    }
    c1->NumStats = 1;
    *ONE_STATE(c1) = upState;
    c1->Suffix = REF(c);
    SetSuccessor(ps[--numPs], REF(c1));
    c = c1;
  }
  while (numPs != 0);
  
  return c;
}

static void SwapStates(CPpmd_State *t1, CPpmd_State *t2)
{
  CPpmd_State tmp = *t1;
  *t1 = *t2;
  *t2 = tmp;
}

static void UpdateModel(CPpmd7 *p)
{
  CPpmd_Void_Ref successor, fSuccessor = SUCCESSOR(p->FoundState);
  CTX_PTR c;
  uint32_t s0, ns;
  
  if (p->FoundState->Freq < MAX_FREQ / 4 && p->MinContext->Suffix != 0)
  {
    c = SUFFIX(p->MinContext);
    
    if (c->NumStats == 1)
    {
      CPpmd_State *s = ONE_STATE(c);
      if (s->Freq < 32)
        s->Freq++;
    }
    else
    {
      CPpmd_State *s = STATS(c);
      if (s->Symbol != p->FoundState->Symbol)
      {
        do { s++; } while (s->Symbol != p->FoundState->Symbol);
        if (s[0].Freq >= s[-1].Freq)
        {
          SwapStates(&s[0], &s[-1]);
          s--;
        }
      }
      if (s->Freq < MAX_FREQ - 9)
      {
        s->Freq += 2;
        c->SummFreq += 2;
      }
    }
  }

  if (p->OrderFall == 0)
  {
    p->MinContext = p->MaxContext = CreateSuccessors(p, true);
    if (p->MinContext == 0)
    {
      RestartModel(p);
      return;
    }
    SetSuccessor(p->FoundState, REF(p->MinContext));
    return;
  }
  
  *p->Text++ = p->FoundState->Symbol;
  successor = REF(p->Text);
  if (p->Text >= p->UnitsStart)
  {
    RestartModel(p);
    return;
  }
  
  if (fSuccessor)
  {
    if (fSuccessor <= successor)
    {
      CTX_PTR cs = CreateSuccessors(p, false);
      if (cs == ::null())
      {
        RestartModel(p);
        return;
      }
      fSuccessor = REF(cs);
    }
    if (--p->OrderFall == 0)
    {
      successor = fSuccessor;
      p->Text -= (p->MaxContext != p->MinContext);
    }
  }
  else
  {
    SetSuccessor(p->FoundState, successor);
    fSuccessor = REF(p->MinContext);
  }
  
  s0 = p->MinContext->SummFreq - (ns = p->MinContext->NumStats) - (p->FoundState->Freq - 1);
  
  for (c = p->MaxContext; c != p->MinContext; c = SUFFIX(c))
  {
    uint32_t ns1;
    uint32_t cf, sf;
    if ((ns1 = c->NumStats) != 1)
    {
      if ((ns1 & 1) == 0)
      {
        /* Expand for one UNIT */
        uint32_t oldNU = ns1 >> 1;
        uint32_t i = U2I(oldNU);
        if (i != U2I(oldNU + 1))
        {
          void *ptr = AllocUnits(p, i + 1);
          void *oldPtr;
          if (!ptr)
          {
            RestartModel(p);
            return;
          }
          oldPtr = STATS(c);
          MyMem12Cpy(ptr, oldPtr, oldNU);
          InsertNode(p, oldPtr, i);
          c->Stats = STATS_REF(ptr);
        }
      }
      c->SummFreq = (uint16_t)(c->SummFreq + (2 * ns1 < ns) + 2 * ((4 * ns1 <= ns) & (c->SummFreq <= 8 * ns1)));
    }
    else
    {
      CPpmd_State *s = (CPpmd_State*)AllocUnits(p, 0);
      if (!s)
      {
        RestartModel(p);
        return;
      }
      *s = *ONE_STATE(c);
      c->Stats = REF(s);
      if (s->Freq < MAX_FREQ / 4 - 1)
        s->Freq <<= 1;
      else
        s->Freq = MAX_FREQ - 4;
      c->SummFreq = (uint16_t)(s->Freq + p->InitEsc + (ns > 3));
    }
    cf = 2 * (uint32_t)p->FoundState->Freq * (c->SummFreq + 6);
    sf = (uint32_t)s0 + c->SummFreq;
    if (cf < 6 * sf)
    {
      cf = 1 + (cf > sf) + (cf >= 4 * sf);
      c->SummFreq += 3;
    }
    else
    {
      cf = 4 + (cf >= 9 * sf) + (cf >= 12 * sf) + (cf >= 15 * sf);
      c->SummFreq = (uint16_t)(c->SummFreq + cf);
    }
    {
      CPpmd_State *s = STATS(c) + ns1;
      SetSuccessor(s, successor);
      s->Symbol = p->FoundState->Symbol;
      s->Freq = (byte)cf;
      c->NumStats = (uint16_t)(ns1 + 1);
    }
  }
  p->MaxContext = p->MinContext = CTX(fSuccessor);
}
  
static void Rescale(CPpmd7 *p)
{
  uint32_t i, adder, sumFreq, escFreq;
  CPpmd_State *stats = STATS(p->MinContext);
  CPpmd_State *s = p->FoundState;
  {
    CPpmd_State tmp = *s;
    for (; s != stats; s--)
      s[0] = s[-1];
    *s = tmp;
  }
  escFreq = p->MinContext->SummFreq - s->Freq;
  s->Freq += 4;
  adder = (p->OrderFall != 0);
  s->Freq = (byte)((s->Freq + adder) >> 1);
  sumFreq = s->Freq;
  
  i = p->MinContext->NumStats - 1;
  do
  {
    escFreq -= (++s)->Freq;
    s->Freq = (byte)((s->Freq + adder) >> 1);
    sumFreq += s->Freq;
    if (s[0].Freq > s[-1].Freq)
    {
      CPpmd_State *s1 = s;
      CPpmd_State tmp = *s1;
      do
        s1[0] = s1[-1];
      while (--s1 != stats && tmp.Freq > s1[-1].Freq);
      *s1 = tmp;
    }
  }
  while (--i);
  
  if (s->Freq == 0)
  {
    uint32_t numStats = p->MinContext->NumStats;
    uint32_t n0, n1;
    do { i++; } while ((--s)->Freq == 0);
    escFreq += i;
    p->MinContext->NumStats = (uint16_t)(p->MinContext->NumStats - i);
    if (p->MinContext->NumStats == 1)
    {
      CPpmd_State tmp = *stats;
      do
      {
        tmp.Freq = (byte)(tmp.Freq - (tmp.Freq >> 1));
        escFreq >>= 1;
      }
      while (escFreq > 1);
      InsertNode(p, stats, U2I(((numStats + 1) >> 1)));
      *(p->FoundState = ONE_STATE(p->MinContext)) = tmp;
      return;
    }
    n0 = (numStats + 1) >> 1;
    n1 = (p->MinContext->NumStats + 1) >> 1;
    if (n0 != n1)
      p->MinContext->Stats = STATS_REF(ShrinkUnits(p, stats, n0, n1));
  }
  p->MinContext->SummFreq = (uint16_t)(sumFreq + escFreq - (escFreq >> 1));
  p->FoundState = STATS(p->MinContext);
}

CPpmd_See *Ppmd7_MakeEscFreq(CPpmd7 *p, uint32_t numMasked, uint32_t *escFreq)
{
  CPpmd_See *see;
  uint32_t nonMasked = p->MinContext->NumStats - numMasked;
  if (p->MinContext->NumStats != 256)
  {
    see = p->See[p->NS2Indx[nonMasked - 1]] +
        (nonMasked < (uint32_t)SUFFIX(p->MinContext)->NumStats - p->MinContext->NumStats) +
        2 * (p->MinContext->SummFreq < 11 * p->MinContext->NumStats) +
        4 * (numMasked > nonMasked) +
        p->HiBitsFlag;
    {
      uint32_t r = (see->Summ >> see->Shift);
      see->Summ = (uint16_t)(see->Summ - r);
      *escFreq = r + (r == 0);
    }
  }
  else
  {
    see = &p->DummySee;
    *escFreq = 1;
  }
  return see;
}

static void NextContext(CPpmd7 *p)
{
  CTX_PTR c = CTX(SUCCESSOR(p->FoundState));
  if (p->OrderFall == 0 && (byte *)c > p->Text)
    p->MinContext = p->MaxContext = c;
  else
    UpdateModel(p);
}

void Ppmd7_Update1(CPpmd7 *p)
{
  CPpmd_State *s = p->FoundState;
  s->Freq += 4;
  p->MinContext->SummFreq += 4;
  if (s[0].Freq > s[-1].Freq)
  {
    SwapStates(&s[0], &s[-1]);
    p->FoundState = --s;
    if (s->Freq > MAX_FREQ)
      Rescale(p);
  }
  NextContext(p);
}

void Ppmd7_Update1_0(CPpmd7 *p)
{
  p->PrevSuccess = (2 * p->FoundState->Freq > p->MinContext->SummFreq);
  p->RunLength += p->PrevSuccess;
  p->MinContext->SummFreq += 4;
  if ((p->FoundState->Freq += 4) > MAX_FREQ)
    Rescale(p);
  NextContext(p);
}

void Ppmd7_UpdateBin(CPpmd7 *p)
{
  p->FoundState->Freq = (byte)(p->FoundState->Freq + (p->FoundState->Freq < 128 ? 1: 0));
  p->PrevSuccess = 1;
  p->RunLength++;
  NextContext(p);
}

void Ppmd7_Update2(CPpmd7 *p)
{
  p->MinContext->SummFreq += 4;
  if ((p->FoundState->Freq += 4) > MAX_FREQ)
    Rescale(p);
  p->RunLength = p->InitRL;
  UpdateModel(p);
}
