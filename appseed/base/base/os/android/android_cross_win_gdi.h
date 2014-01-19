#pragma once

/*HDC GetDC(oswindow hwnd);
HFONT CreateFontIndirect(const LOGFONT * lf);
HPEN CreatePen(int32_t iPenStyle, int32_t iWidth, COLORREF crColor);
HGDIOBJ GetStockObject(int32_t iStockObject);
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj);
int_bool Rectangle(HDC hdc, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int_bool DeleteObject(HGDIOBJ hgdiobj);
HDC CreateCompatibleDC(HDC hdc);
//HBITMAP WINAPI CreateDIBSection(HDC hdc, const BITMAPINFO *lpbmi, UINT usage, void **ppvBits, HANDLE hSection, DWORD offset);
int_bool SetTextColor(HDC hdc, COLORREF cr);
int_bool SetBkMode(HDC hdc, int32_t iMode);
int_bool DeleteDC(HDC hdc);
int_bool AlphaBlend(HDC hdcDest, int32_t xoriginDest, int32_t yoriginDest, int32_t wDest, int32_t hDest, HDC hdcSrc, int32_t xoriginSrc, int32_t yoriginSrc, int32_t wSrc, int32_t hSrc, BLENDFUNCTION ftn);
int_bool GetObject(HGDIOBJ hgdiobj, int32_t iSize, void * object);
int_bool GetTextExtentPoint(HDC hdc, const char * pszText, int32_t iSize, SIZE * psize);
/*int_bool WINAPI TransparentBlt(HDC hdcDest,
    __in int32_t xoriginDest,
    __in int32_t yoriginDest,
    __in int32_t wDest,
    __in int32_t hDest,
    __in HDC hdcSrc,
    __in int32_t xoriginSrc,
    __in int32_t yoriginSrc,
    __in int32_t wSrc,
    __in int32_t hSrc,
    __in UINT crTransparent);

*/
