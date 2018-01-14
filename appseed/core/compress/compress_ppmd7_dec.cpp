/* Ppmd7Dec.ca -- PPMdH decoder
2010-03-12 : Igor Pavlov : Public domain
This code is based on PPMd var.H (2001): Dmitry Shkarin : Public domain */
// from 7-zip on 2012-12-23, dawn
#include "framework.h"

#define kTopValue (1 << 24)

bool Ppmd7z_RangeDec_Init(CPpmd7z_RangeDec *p)
{
  uint32_t i;
  p->Code = 0;
  p->range = 0xFFFFFFFF;
  if (p->Stream->Read((void *)p->Stream) != 0)
    return false;
  for (i = 0; i < 4; i++)
    p->Code = (p->Code << 8) | p->Stream->Read((void *)p->Stream);
  return (p->Code < 0xFFFFFFFF);
}

static uint32_t Range_GetThreshold(void *pp, uint32_t total)
{
  CPpmd7z_RangeDec *p = (CPpmd7z_RangeDec *)pp;
  return (p->Code) / (p->range /= total);
}

static void Range_Normalize(CPpmd7z_RangeDec *p)
{
  if (p->range < kTopValue)
  {
    p->Code = (p->Code << 8) | p->Stream->Read((void *)p->Stream);
    p->range <<= 8;
    if (p->range < kTopValue)
    {
      p->Code = (p->Code << 8) | p->Stream->Read((void *)p->Stream);
      p->range <<= 8;
    }
  }
}

static void Range_Decode(void *pp, uint32_t start, uint32_t size)
{
  CPpmd7z_RangeDec *p = (CPpmd7z_RangeDec *)pp;
  p->Code -= start * p->range;
  p->range *= size;
  Range_Normalize(p);
}

static uint32_t Range_DecodeBit(void *pp, uint32_t size0)
{
  CPpmd7z_RangeDec *p = (CPpmd7z_RangeDec *)pp;
  uint32_t newBound = (p->range >> 14) * size0;
  uint32_t symbol;
  if (p->Code < newBound)
  {
    symbol = 0;
    p->range = newBound;
  }
  else
  {
    symbol = 1;
    p->Code -= newBound;
    p->range -= newBound;
  }
  Range_Normalize(p);
  return symbol;
}

void Ppmd7z_RangeDec_CreateVTable(CPpmd7z_RangeDec *p)
{
  p->p.GetThreshold = Range_GetThreshold;
  p->p.Decode = Range_Decode;
  p->p.DecodeBit = Range_DecodeBit;
}


#define MASK(sym) ((signed char *)charMask)[sym]

int32_t Ppmd7_DecodeSymbol(CPpmd7 *p, IPpmd7_RangeDec *rc)
{
  size_t charMask[256 / sizeof(size_t)];
  if (p->MinContext->NumStats != 1)
  {
    CPpmd_State *s = Ppmd7_GetStats(p, p->MinContext);
    uint32_t i;
    uint32_t count, hiCnt;
    if ((count = rc->GetThreshold(rc, p->MinContext->SummFreq)) < (hiCnt = s->Freq))
    {
      byte symbol;
      rc->Decode(rc, 0, s->Freq);
      p->FoundState = s;
      symbol = s->Symbol;
      Ppmd7_Update1_0(p);
      return symbol;
    }
    p->PrevSuccess = 0;
    i = p->MinContext->NumStats - 1;
    do
    {
      if ((hiCnt += (++s)->Freq) > count)
      {
        byte symbol;
        rc->Decode(rc, hiCnt - s->Freq, s->Freq);
        p->FoundState = s;
        symbol = s->Symbol;
        Ppmd7_Update1(p);
        return symbol;
      }
    }
    while (--i);
    if (count >= p->MinContext->SummFreq)
      return -2;
    p->HiBitsFlag = p->HB2Flag[p->FoundState->Symbol];
    rc->Decode(rc, hiCnt, p->MinContext->SummFreq - hiCnt);
    PPMD_SetAllBitsIn256Bytes(charMask);
    MASK(s->Symbol) = 0;
    i = p->MinContext->NumStats - 1;
    do { MASK((--s)->Symbol) = 0; } while (--i);
  }
  else
  {
    uint16_t *prob = Ppmd7_GetBinSumm(p);
    if (rc->DecodeBit(rc, *prob) == 0)
    {
      byte symbol;
      *prob = (uint16_t)PPMD_UPDATE_PROB_0(*prob);
      symbol = (p->FoundState = Ppmd7Context_OneState(p->MinContext))->Symbol;
      Ppmd7_UpdateBin(p);
      return symbol;
    }
    *prob = (uint16_t)PPMD_UPDATE_PROB_1(*prob);
    p->InitEsc = PPMD7_kExpEscape[*prob >> 10];
    PPMD_SetAllBitsIn256Bytes(charMask);
    MASK(Ppmd7Context_OneState(p->MinContext)->Symbol) = 0;
    p->PrevSuccess = 0;
  }
  for (;;)
  {
    CPpmd_State *ps[256], *s;
    uint32_t freqSum, count, hiCnt;
    CPpmd_See *see;
    uint32_t i, num, numMasked = p->MinContext->NumStats;
    do
    {
      p->OrderFall++;
      if (!p->MinContext->Suffix)
        return -1;
      p->MinContext = Ppmd7_GetContext(p, p->MinContext->Suffix);
    }
    while (p->MinContext->NumStats == numMasked);
    hiCnt = 0;
    s = Ppmd7_GetStats(p, p->MinContext);
    i = 0;
    num = p->MinContext->NumStats - numMasked;
    do
    {
      int32_t k = (int32_t)(MASK(s->Symbol));
      hiCnt += (s->Freq & k);
      ps[i] = s++;
      i -= k;
    }
    while (i != num);
    
    see = Ppmd7_MakeEscFreq(p, numMasked, &freqSum);
    freqSum += hiCnt;
    count = rc->GetThreshold(rc, freqSum);
    
    if (count < hiCnt)
    {
      byte symbol;
      CPpmd_State **pps = ps;
      for (hiCnt = 0; (hiCnt += (*pps)->Freq) <= count; pps++);
      s = *pps;
      rc->Decode(rc, hiCnt - s->Freq, s->Freq);
      Ppmd_See_Update(see);
      p->FoundState = s;
      symbol = s->Symbol;
      Ppmd7_Update2(p);
      return symbol;
    }
    if (count >= freqSum)
      return -2;
    rc->Decode(rc, hiCnt, freqSum - hiCnt);
    see->Summ = (uint16_t)(see->Summ + freqSum);
    do { MASK(ps[--i]->Symbol) = 0; } while (i != 0);
  }
}
