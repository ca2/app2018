#include "StdAfx.h"

namespace html
{

   html_ex::html_ex(::ca::application * papp) :
      ca(papp),
      m_clrInvalid(0xffffffff),
      m_percentMax(USHRT_MAX)
   {

      /** 28 system colors */
      m_namedColors["activeborder"]      = (COLORREF)0x8000000A;
      m_namedColors["activecaption"]      = (COLORREF)0x80000002;
      m_namedColors["appworkspace"]      = (COLORREF)0x8000000C;
      m_namedColors["background"]         = (COLORREF)0x80000001;
      m_namedColors["buttonface"]         = (COLORREF)0x8000000F;
      m_namedColors["buttonhighlight"]      = (COLORREF)0x80000014;
      m_namedColors["buttonshadow"]      = (COLORREF)0x80000010;
      m_namedColors["buttontext"]         = (COLORREF)0x80000012;
      m_namedColors["captiontext"]         = (COLORREF)0x80000009;
      m_namedColors["graytext"]         = (COLORREF)0x80000011;
      m_namedColors["highlight"]         = (COLORREF)0x8000000D;
      m_namedColors["highlighttext"]      = (COLORREF)0x8000000E;
      m_namedColors["inactiveborder"]      = (COLORREF)0x8000000B;
      m_namedColors["inactivecaption"]      = (COLORREF)0x80000003;
      m_namedColors["inactivecaptiontext"]   = (COLORREF)0x80000013;
      m_namedColors["infobackground"]      = (COLORREF)0x80000018;
      m_namedColors["infotext"]         = (COLORREF)0x80000017;
      m_namedColors["menu"]            = (COLORREF)0x80000004;
      m_namedColors["menutext"]         = (COLORREF)0x80000007;
      m_namedColors["scrollbar"]         = (COLORREF)0x80000000;
      m_namedColors["threeddarkshadow"]   = (COLORREF)0x80000015;
      m_namedColors["threedface"]         = (COLORREF)0x8000000F;
      m_namedColors["threedhighlight"]      = (COLORREF)0x80000014;
      m_namedColors["threedlightshadow"]   = (COLORREF)0x80000016;
      m_namedColors["threedshadow"]      = (COLORREF)0x80000010;
      m_namedColors["::ca::window"]            = (COLORREF)0x80000005;
      m_namedColors["windowframe"]         = (COLORREF)0x80000006;
      m_namedColors["windowtext"]         = (COLORREF)0x80000008;

      /** 16 basic colors */
      m_namedColors["black"]            = RGB(0x00, 0x00, 0x00);
      m_namedColors["gray"]            = RGB(0x80, 0x80, 0x80);
      m_namedColors["silver"]            = RGB(0xC0, 0xC0, 0xC0);
      m_namedColors["white"]            = RGB(0xFF, 0xFF, 0xFF);
      m_namedColors["yellow"]            = RGB(0xFF, 0xFF, 0x00);
      m_namedColors["olive"]            = RGB(0x80, 0x80, 0x00);
      m_namedColors["red"]               = RGB(0xFF, 0x00, 0x00);
      m_namedColors["maroon"]            = RGB(0x80, 0x00, 0x00);
      m_namedColors["fuchsia"]            = RGB(0xFF, 0x00, 0xFF);
      m_namedColors["purple"]            = RGB(0x80, 0x00, 0x80);
      m_namedColors["blue"]            = RGB(0x00, 0x00, 0xFF);
      m_namedColors["navy"]            = RGB(0x00, 0x00, 0x80);
      m_namedColors["aqua"]            = RGB(0x00, 0xFF, 0xFF);
      m_namedColors["teal"]            = RGB(0x00, 0x80, 0x80);
      m_namedColors["lime"]            = RGB(0x00, 0xFF, 0x00);
      m_namedColors["green"]            = RGB(0x00, 0x80, 0xFF);

      /** additional named colors */
      m_namedColors["darkolivegreen"]      = RGB(0x55, 0x6B, 0x2F);
      m_namedColors["olivedrab"]         = RGB(0x6B, 0x8E, 0x23);
      m_namedColors["yellowgreen"]         = RGB(0x9A, 0xCD, 0x32);
      m_namedColors["lawngreen"]         = RGB(0x7C, 0xFC, 0x00);
      m_namedColors["chartreuse"]         = RGB(0x7F, 0xFF, 0x00);
      m_namedColors["greenyellow"]         = RGB(0xAD, 0xFF, 0x2F);
      m_namedColors["palegreen"]         = RGB(0x98, 0xFB, 0x98);
      m_namedColors["lightgreen"]         = RGB(0x90, 0xEE, 0x90);
      m_namedColors["darkgreen"]         = RGB(0x00, 0x64, 0x00);
      m_namedColors["forestgreen"]         = RGB(0x22, 0x8B, 0x22);
      m_namedColors["seagreen"]         = RGB(0x2E, 0x8B, 0x57);
      m_namedColors["mediumseagreen"]      = RGB(0x3C, 0xB3, 0x71);
      m_namedColors["limegreen"]         = RGB(0x32, 0xCD, 0x32);
      m_namedColors["darkseagreen"]      = RGB(0x8F, 0xBC, 0x8B);
      m_namedColors["springgreen"]         = RGB(0x00, 0xFF, 0x7F);
      m_namedColors["mediumspringgreen"]   = RGB(0x00, 0xFA, 0x99);
      m_namedColors["darkslategray"]      = RGB(0x2F, 0x4F, 0x4F);
      m_namedColors["darkcyan"]         = RGB(0x00, 0x8B, 0x8B);
      m_namedColors["cadetblue"]         = RGB(0x5F, 0x9E, 0xA0);
      m_namedColors["lightseagreen"]      = RGB(0x20, 0xB2, 0xAA);
      m_namedColors["mediumaquamarine"]   = RGB(0x66, 0xCD, 0xAA);
      m_namedColors["turquoise"]         = RGB(0x40, 0xE0, 0xD0);
      m_namedColors["aquamarine"]         = RGB(0x7F, 0xFF, 0xD4);
      m_namedColors["paleturquoise"]      = RGB(0xAF, 0xEE, 0xEE);
      m_namedColors["slategray"]         = RGB(0x70, 0x80, 0x90);
      m_namedColors["lightslategray"]      = RGB(0x77, 0x88, 0x99);
      m_namedColors["steelblue"]         = RGB(0x46, 0x82, 0xB4);
      m_namedColors["deepskyblue"]         = RGB(0x00, 0xBF, 0xFF);
      m_namedColors["darkturquoise"]      = RGB(0x00, 0xCE, 0xD1);
      m_namedColors["mediumturquoise"]      = RGB(0x48, 0xD1, 0xCC);
      m_namedColors["powderblue"]         = RGB(0xB0, 0xE0, 0xE6);
      m_namedColors["lightcyan"]         = RGB(0xE0, 0xFF, 0xFF);
      m_namedColors["darkblue"]         = RGB(0x00, 0x00, 0x8B);
      m_namedColors["mediumblue"]         = RGB(0x00, 0x00, 0xCD);
      m_namedColors["royalblue"]         = RGB(0x41, 0x69, 0xe1);
      m_namedColors["dodgerblue"]         = RGB(0x1E, 0x90, 0xFF);
      m_namedColors["cornflowerblue"]      = RGB(0x64, 0x95, 0xED);
      m_namedColors["skyblue"]            = RGB(0x87, 0xCE, 0xEB);
      m_namedColors["lightskyblue"]      = RGB(0x87, 0xCE, 0xFA);
      m_namedColors["lightblue"]         = RGB(0xAD, 0xD8, 0xE6);
      m_namedColors["midnightblue"]      = RGB(0x19, 0x19, 0x70);
      m_namedColors["darkslateblue"]      = RGB(0x48, 0x3D, 0x8B);
      m_namedColors["blueviolet"]         = RGB(0x8A, 0x2B, 0xE2);
      m_namedColors["slateblue"]         = RGB(0x6A, 0x5A, 0xCD);
      m_namedColors["mediumslateblue"]      = RGB(0x7B, 0x68, 0xEE);
      m_namedColors["mediumpurple"]      = RGB(0x93, 0x70, 0xDB);
      m_namedColors["lightsteelblue"]      = RGB(0xB0, 0xC4, 0xDE);
      m_namedColors["lavender"]         = RGB(0xE6, 0xE6, 0xFA);
      m_namedColors["indigo"]            = RGB(0x4B, 0x00, 0x82);
      m_namedColors["darkviolet"]         = RGB(0x94, 0x00, 0xD3);
      m_namedColors["darkorchid"]         = RGB(0x99, 0x32, 0xCC);
      m_namedColors["mediumorchid"]      = RGB(0xBA, 0x55, 0xD3);
      m_namedColors["orchid"]            = RGB(0xDA, 0x70, 0xD6);
      m_namedColors["violet"]            = RGB(0xEE, 0x82, 0xEE);
      m_namedColors["plum"]            = RGB(0xDD, 0xA0, 0xDD);
      m_namedColors["thistle"]            = RGB(0xD8, 0xDF, 0xD8);
      m_namedColors["darkmagenta"]         = RGB(0x8B, 0x00, 0x8B);
      m_namedColors["mediumvioletred"]      = RGB(0xC7, 0x15, 0x85);
      m_namedColors["deeppink"]         = RGB(0xFF, 0x14, 0x93);
      m_namedColors["palmvioletred"]      = RGB(0xDB, 0x70, 0x93);
      m_namedColors["hotpink"]            = RGB(0xFF, 0x69, 0xB4);
      m_namedColors["lightpink"]         = RGB(0xFF, 0xB6, 0xC1);
      m_namedColors["pink"]            = RGB(0xFF, 0xC0, 0xCB);
      m_namedColors["mistyrose"]         = RGB(0xFF, 0xE4, 0xE1);
      m_namedColors["brown"]            = RGB(0xA5, 0x2A, 0x2A);
      m_namedColors["indianred"]         = RGB(0xCD, 0x5C, 0x5C);
      m_namedColors["rosybrown"]         = RGB(0xBC, 0x8F, 0x8F);
      m_namedColors["salmon"]            = RGB(0xFA, 0x80, 0x72);
      m_namedColors["lightcoral"]         = RGB(0xF0, 0x80, 0x80);
      m_namedColors["darksalmon"]         = RGB(0xE9, 0x96, 0x7A);
      m_namedColors["lightsalmon"]         = RGB(0xFF, 0xA0, 0x7A);
      m_namedColors["peachpuff"]         = RGB(0xFF, 0xDA, 0xB9);
      m_namedColors["darkred"]            = RGB(0x8B, 0x00, 0x00);
      m_namedColors["firebrick"]         = RGB(0xB2, 0x22, 0x22);
      m_namedColors["crimson"]            = RGB(0xDC, 0x14, 0x3C);
      m_namedColors["orangered"]         = RGB(0xFF, 0x45, 0x00);
      m_namedColors["tomato"]            = RGB(0xFF, 0x63, 0x47);
      m_namedColors["coral"]            = RGB(0xFF, 0x7F, 0x50);
      m_namedColors["wheat"]            = RGB(0xF5, 0xDE, 0xB3);
      m_namedColors["papayawhip"]         = RGB(0xFF, 0xEF, 0xD5);
      m_namedColors["sienna"]            = RGB(0xA0, 0x52, 0x2D);
      m_namedColors["chocolate"]         = RGB(0xD2, 0x69, 0x1E);
      m_namedColors["darkorange"]         = RGB(0xFF, 0x8C, 0x00);
      m_namedColors["sandybrown"]         = RGB(0xF4, 0xA4, 0x60);
      m_namedColors["orange"]            = RGB(0xFF, 0xA5, 0x00);
      m_namedColors["navajowhite"]         = RGB(0xFF, 0xDE, 0xAD);
      m_namedColors["moccasin"]         = RGB(0xFF, 0xE4, 0xB5);
      m_namedColors["saddlebrown"]         = RGB(0x8B, 0x45, 0x13);
      m_namedColors["peru"]            = RGB(0xCD, 0x85, 0x3F);
      m_namedColors["burlywood"]         = RGB(0xDE, 0xB8, 0x87);
      m_namedColors["tan"]               = RGB(0xD2, 0xB4, 0x8C);
      m_namedColors["bisque"]            = RGB(0xFF, 0xE4, 0xC4);
      m_namedColors["blanchedalmond"]      = RGB(0xFF, 0xEB, 0xCD);
      m_namedColors["antiquewhite"]      = RGB(0xFA, 0xEB, 0xD7);
      m_namedColors["darkgoldenrod"]      = RGB(0xB8, 0x86, 0x0B);
      m_namedColors["goldenrod"]         = RGB(0xDA, 0xA5, 0x20);
      m_namedColors["darkkhaki"]         = RGB(0xBD, 0xB7, 0x6B);
      m_namedColors["gold"]            = RGB(0xFF, 0xD7, 0x00);
      m_namedColors["khaki"]            = RGB(0xF0, 0xE6, 0x8C);
      m_namedColors["palegoldenrod"]      = RGB(0xEE, 0xE8, 0xAA);
      m_namedColors["lemonchiffon"]      = RGB(0xFF, 0xFA, 0xCD);
      m_namedColors["beige"]            = RGB(0xF5, 0xF5, 0xDC);
      m_namedColors["lightgoldenrodyellow"]= RGB(0xFA, 0xFA, 0xD2);
      m_namedColors["lightyellow"]         = RGB(0xFF, 0xFF, 0xE0);
      m_namedColors["ivory"]            = RGB(0xFF, 0xFF, 0x00);
      m_namedColors["cornsilk"]         = RGB(0xFF, 0xF8, 0xDC);
      m_namedColors["oldlace"]            = RGB(0xFD, 0xF5, 0xE6);
      m_namedColors["florawhite"]         = RGB(0xFF, 0xFA, 0xF0);
      m_namedColors["honeydew"]         = RGB(0xF0, 0xFF, 0xF0);
      m_namedColors["mintcream"]         = RGB(0xF5, 0xFF, 0xFA);
      m_namedColors["azure"]            = RGB(0xF0, 0xFF, 0xFF);
      m_namedColors["ghostwhite"]         = RGB(0xF8, 0xF8, 0xFF);
      m_namedColors["linen"]            = RGB(0xFA, 0xF0, 0xE6);
      m_namedColors["seashell"]         = RGB(0xFF, 0xF5, 0xEE);
      m_namedColors["snow"]            = RGB(0xFF, 0xFA, 0xFA);
      m_namedColors["dimgray"]            = RGB(0x69, 0x69, 0x69);
      m_namedColors["darkgray"]         = RGB(0xA9, 0xA9, 0xA9);
      m_namedColors["lightgray"]         = RGB(0xD3, 0xD3, 0xD3);
      m_namedColors["gainsboro"]         = RGB(0xDC, 0xDC, 0xDC);
      m_namedColors["whitesmoke"]         = RGB(0xF5, 0xF5, 0xF5);
      m_namedColors["ghostwhite"]         = RGB(0xF8, 0xF8, 0xFF);
      m_namedColors["aliceblue"]         = RGB(0xF0, 0xF8, 0xFF);

   }

   html_ex::~html_ex()
   {
   }

   int html_ex::resolve_entity(const char * lpszEntity, string & strChar)
   {
      return m_entityresolver.resolveEntity(lpszEntity, strChar);
   }

} // namespace html