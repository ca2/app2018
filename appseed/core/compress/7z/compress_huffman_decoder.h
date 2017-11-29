// Compress/HuffmanDecoder.h
// from 7-zip on 2012-12-23, dawn
#pragma once

//#include "../../Common/Types.h"


namespace libcompress
{

   namespace huffman
   {

      const int32_t kNumTableBits = 9;

      template <int32_t kNumBitsMax, uint32 m_NumSymbols>
      class decoder
      {
         uint32 m_Limits[kNumBitsMax + 1];     // m_Limits[i] = value limit for symbols with length = i
         uint32 m_Positions[kNumBitsMax + 1];  // m_Positions[i] = index in m_Symbols[] of first symbol with length = i
         uint32 m_Symbols[m_NumSymbols];
         byte m_Lengths[1 << kNumTableBits];   // Table oh length for short codes.

      public:

         bool SetCodeLengths(const byte *codeLengths)
         {
            int32_t lenCounts[kNumBitsMax + 1];
            uint32 tmpPositions[kNumBitsMax + 1];
            int32_t i;
            for(i = 1; i <= kNumBitsMax; i++)
               lenCounts[i] = 0;
            uint32 symbol;
            for (symbol = 0; symbol < m_NumSymbols; symbol++)
            {
               int32_t len = codeLengths[symbol];
               if (len > kNumBitsMax)
                  return false;
               lenCounts[len]++;
               m_Symbols[symbol] = 0xFFFFFFFF;
            }
            lenCounts[0] = 0;
            m_Positions[0] = m_Limits[0] = 0;
            uint32 startPos = 0;
            uint32 index = 0;
            const uint32 kMaxValue = (1 << kNumBitsMax);
            for (i = 1; i <= kNumBitsMax; i++)
            {
               startPos += lenCounts[i] << (kNumBitsMax - i);
               if (startPos > kMaxValue)
                  return false;
               m_Limits[i] = (i == kNumBitsMax) ? kMaxValue : startPos;
               m_Positions[i] = m_Positions[i - 1] + lenCounts[i - 1];
               tmpPositions[i] = m_Positions[i];
               if(i <= kNumTableBits)
               {
                  uint32 limit = (m_Limits[i] >> (kNumBitsMax - kNumTableBits));
                  for (; index < limit; index++)
                     m_Lengths[index] = (byte)i;
               }
            }
            for (symbol = 0; symbol < m_NumSymbols; symbol++)
            {
               int32_t len = codeLengths[symbol];
               if (len != 0)
                  m_Symbols[tmpPositions[len]++] = symbol;
            }
            return true;
         }

         template <class TBitDecoder>
         uint32 DecodeSymbol(TBitDecoder *bitStream)
         {
            int32_t numBits;
            uint32 value = bitStream->GetValue(kNumBitsMax);
            if (value < m_Limits[kNumTableBits])
               numBits = m_Lengths[value >> (kNumBitsMax - kNumTableBits)];
            else
               for (numBits = kNumTableBits + 1; value >= m_Limits[numBits]; numBits++);
            bitStream->MovePos(numBits);
            uint32 index = m_Positions[numBits] +
               ((value - m_Limits[numBits - 1]) >> (kNumBitsMax - numBits));
            if (index >= m_NumSymbols)
               // _throw(CDecoderException()); // test it
               return 0xFFFFFFFF;
            return m_Symbols[index];
         }
      };

   } // namespace huffman

} // namespace libcompress

