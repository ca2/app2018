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

#ifndef nsUTF8Prober_h__
#define nsUTF8Prober_h__

#include "charsetProber.h"
#include "codingStateMachine.h"

class nsUTF8Prober: public nsCharSetProber {
public:
  nsUTF8Prober(){mNumOfMBChar = 0; 
                mCodingSM = new nsCodingStateMachine(&UTF8SMModel);
                Reset(); };
  virtual ~nsUTF8Prober(){delete mCodingSM;};
  nsProbingState HandleData(const char* aBuf, uint32_t aLen);
  const char* GetCharSetName() {return "UTF-8";};
  nsProbingState GetState() {return mState;};
  void      Reset();
  float     GetConfidence();
  void      SetOpion() {};

protected:
  nsCodingStateMachine* mCodingSM;
  nsProbingState mState;
  uint32_t mNumOfMBChar;
};

#endif /* nsUTF8Prober_h__ */
