/*
   libcharguess   -   Guess the encoding/charset of a string
    Copyright (C) 2003  Stephane Corbe <noubi@users.sourceforge.net>
   Based on Mozilla sources

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef universal_h__
#define universal_h__

#include "types.h"

// #include "nsICharsetDetector.h"
// #include "nsIStringCharsetDetector.h"
// #include "nsICharsetDetectionObserver.h"

class nsCharSetProber;

#define NUM_OF_CHARSET_PROBERS  3

typedef enum {
  ePureAscii = 0,
  eEscAscii  = 1,
  eHighbyte  = 2
} nsInputState;

class nsUniversalDetector {
public:
   nsUniversalDetector();
   virtual ~nsUniversalDetector();
   virtual void HandleData(const char* aBuf, uint32_t aLen);
   virtual void DataEnd();
   virtual void Reset();
   virtual const char* GetCharset();

protected:
   virtual void Report(const char* aCharset);
   nsInputState  mInputState;
   bool  mDone;
   bool  mInTag;
   bool  mStart;
   bool  mGotData;
   char    mLastChar;
   const char *  mDetectedCharset;
   int32_t mBestGuess;

   nsCharSetProber  *mCharSetProbers[NUM_OF_CHARSET_PROBERS];
   nsCharSetProber  *mEscCharSetProber;
};

#endif

