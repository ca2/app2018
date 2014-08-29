/*
	libcharguess	-	Guess the encoding/charset of a string
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

#include "framework.h"




extern "C" charguess_det CharGuessInit()
{

   nsUniversalDetector* det = new nsUniversalDetector;

	if (det != NULL)
      return (charguess_det) det;
	else
		return NULL;

}

const char* GuessChardet(charguess_det p,const string & str)
{
   
   nsUniversalDetector* det = (nsUniversalDetector*) p;

	det->Reset();
	det->HandleData(str, str.length());
	det->DataEnd();
	
   return det->GetCharset();

}

extern "C" const char* GuessChardet(charguess_det p,const char *str)
{
   
   return GuessChardet(p,(const string &)str);

}

extern "C" void CharGuessDestroy(charguess_det p)
{

   try
   {

      nsUniversalDetector* det = (nsUniversalDetector*)p;

      if(det != NULL)
         delete det;

   }
   catch(...)
   {

   }

}
