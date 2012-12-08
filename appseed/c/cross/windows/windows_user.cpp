//
//  windows_user.cpp
//  c
//
//  Created by Thomas Borregaard Sørensen on 12/8/12.
//
//


CLASS_DECL_c COLORREF GetSysColor(DWORD dw)
{
   
   switch(dw)
   {
      case COLOR_HIGHLIGHT:
         return ARGB(255, 200, 200, 196);
      case COLOR_BTNSHADOW:
         return ARGB(84, 192, 192, 187);
      case COLOR_BTNTEXT:
         return ARGB(255, 0, 0, 0);
      default:
         return ARGB(255, 0, 0, 0);
   };
   
}
