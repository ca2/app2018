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


typedef signed long   PRInt32;
typedef uint32_t long PRUint32;
typedef signed int16_t    PRInt16;
typedef uint16_t  PRUint16;
typedef signed char   PRInt8;
typedef uchar PRUint8;
typedef uint32_t PRUintn;
typedef char PRBool;

#define PR_FALSE 0
#define PR_TRUE  1
#define nsnull   NULL

#define PR_MALLOC malloc
#define PR_FREEIF free


#define NS_ASSERTION(a,b) { if (!(a)) { puts(b); abort(); } }
