/*
  TurboC, a library for porting Borland Turbo C to GNU gcc.
  Copyright 2002 Ronald S. Burkey
 
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
  
  Contact Ron Burkey at info@sandroid.org.

  Filename:	fillellipse.c
  Purpose:	A Borland Turbo C graphics.h function replacement,
  		for porting to *nix.
  Mod history:	05/27/02 RSB	Split off from arc.c.
*/

#ifdef WITH_X

//#include "graphics.h"

//----------------------------------------------------------------------------

void
fillellipse (int x, int y, int xradius, int yradius)
{
  //// Convert to viewport coordinates.
  //x += TcViewLeft - xradius;
  //y += TcViewTop - yradius;
  //xradius *= 2;
  //yradius *= 2;
  //// Now do the actual drawing.
  //XLockDisplay (TcDisplay);
  //XSetFillStyle (TcDisplay, TcGc, FillTiled);
  //XSetTile (TcDisplay, TcGc, TcTile);
  //XFillArc (TcDisplay, TcPixmaps[TcActivePage], TcGc, x, y, xradius, yradius,
	 //   0, 360 * 64);
  //if (TcActivePage == TcVisualPage)
  //  {
  //    XFillArc (TcDisplay, TcWindow, TcGc, x, y, xradius, yradius,
		//0, 360 * 64);
  //    XSync (TcDisplay, False);
  //  }
  //XSetFillStyle (TcDisplay, TcGc, FillSolid);
  //XUnlockDisplay (TcDisplay);
}


#endif // WITH_X
