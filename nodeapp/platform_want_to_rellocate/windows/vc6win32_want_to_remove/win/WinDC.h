#pragma once

class WinDC : public base_object
{
	// // DECLARE_DYNCREATE(WinDC)
public:

// Attributes
	HDC m_hDC;          // The output DC (must be first data member)
	HDC m_hAttribDC;    // The Attribute DC
	operator HDC() const;
	HDC get_os_data() const; // Always returns the Output DC
	::ca::window * GetWindow() const;

	static WinDC* PASCAL from_handle(HDC hDC);
	static void PASCAL DeleteTempMap();
	BOOL Attach(HDC hDC);   // Attach/Detach affects only the Output DC
	HDC Detach();

	virtual void SetAttribDC(HDC hDC);  // Set the Attribute DC
	virtual void SetOutputDC(HDC hDC);  // Set the Output DC
	virtual void ReleaseAttribDC();     // Release the Attribute DC
	virtual void ReleaseOutputDC();     // Release the Output DC

	BOOL IsPrinting() const;            // TRUE if being used for printing

	::ca::pen* GetCurrentPen() const;
	::ca::brush* GetCurrentBrush() const;
	::ca::palette* GetCurrentPalette() const;
	::ca::font* GetCurrentFont() const;
	::ca::bitmap* GetCurrentBitmap() const;

	// for bidi and mirrored localization
	DWORD GetLayout() const;
	DWORD SetLayout(DWORD dwLayout);

// Constructors
	WinDC();
	BOOL CreateDC(const char * lpszDriverName, const char * lpszDeviceName,
		const char * lpszOutput, const void * lpInitData);
	BOOL CreateIC(const char * lpszDriverName, const char * lpszDeviceName,
		const char * lpszOutput, const void * lpInitData);
	BOOL CreateCompatibleDC(WinDC* pgraphics);

	BOOL DeleteDC();

// Device-Context Functions
	virtual int SaveDC();
	virtual BOOL RestoreDC(int nSavedDC);
	int GetDeviceCaps(int nIndex) const;
	UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags);
	UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
	BOOL ResetDC(const DEVMODE* lpDevMode);

// Drawing-Tool Functions
	point GetBrushOrg() const;
	point SetBrushOrg(int x, int y);
	point SetBrushOrg(POINT point);
	int EnumObjects(int nObjectType,
			int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);

// Type-safe selection helpers
public:
	virtual ::ca::graphics_object* SelectStockObject(int nIndex);
	::ca::pen* SelectObject(::ca::pen* pPen);
	::ca::brush* SelectObject(::ca::brush* pBrush);
	virtual ::ca::font* SelectObject(::ca::font* pFont);
	::ca::bitmap* SelectObject(::ca::bitmap* pBitmap);
	int SelectObject(::ca::rgn* pRgn);       // special return for regions
	::ca::graphics_object* SelectObject(::ca::graphics_object* pObject);
		// ::ca::graphics_object* provided so compiler doesn't use SelectObject(HGDIOBJ)

// color and color Palette Functions
	COLORREF GetNearestColor(COLORREF crColor) const;
	::ca::palette* SelectPalette(::ca::palette* pPalette, BOOL bForceBackground);
	UINT RealizePalette();
	void UpdateColors();

// Drawing-Attribute Functions
	COLORREF GetBkColor() const;
	int GetBkMode() const;
	int GetPolyFillMode() const;
	int GetROP2() const;
	int GetStretchBltMode() const;
	COLORREF GetTextColor() const;

	virtual COLORREF SetBkColor(COLORREF crColor);
	int SetBkMode(int nBkMode);
	int SetPolyFillMode(int nPolyFillMode);
	int SetROP2(int nDrawMode);
	int SetStretchBltMode(int nStretchMode);
	virtual COLORREF SetTextColor(COLORREF crColor);

	BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
	BOOL SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);

// Mapping Functions
	int GetMapMode() const;
	point GetViewportOrg() const;
	virtual int SetMapMode(int nMapMode);
	// Viewport Origin
	virtual point SetViewportOrg(int x, int y);
			point SetViewportOrg(POINT point);
	virtual point OffsetViewportOrg(int nWidth, int nHeight);

	// Viewport Extent
	size GetViewportExt() const;
	virtual size SetViewportExt(int cx, int cy);
			size SetViewportExt(SIZE size);
	virtual size ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

	// Window Origin
	point GetWindowOrg() const;
	point SetWindowOrg(int x, int y);
	point SetWindowOrg(POINT point);
	point OffsetWindowOrg(int nWidth, int nHeight);

	// Window extent
	size GetWindowExt() const;
	virtual size SetWindowExt(int cx, int cy);
			size SetWindowExt(SIZE size);
	virtual size ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);

// Coordinate Functions
	void DPtoLP(LPPOINT lpPoints, int nCount = 1) const;
	void DPtoLP(LPRECT lpRect) const;
	void DPtoLP(LPSIZE lpSize) const;
	void LPtoDP(LPPOINT lpPoints, int nCount = 1) const;
	void LPtoDP(LPRECT lpRect) const;
	void LPtoDP(LPSIZE lpSize) const;

// Special Coordinate Functions (useful for dealing with metafiles and OLE)
	void DPtoHIMETRIC(LPSIZE lpSize) const;
	void LPtoHIMETRIC(LPSIZE lpSize) const;
	void HIMETRICtoDP(LPSIZE lpSize) const;
	void HIMETRICtoLP(LPSIZE lpSize) const;

// Region Functions
	BOOL FillRgn(::ca::rgn* pRgn, ::ca::brush* pBrush);
	BOOL FrameRgn(::ca::rgn* pRgn, ::ca::brush* pBrush, int nWidth, int nHeight);
	BOOL InvertRgn(::ca::rgn* pRgn);
	BOOL PaintRgn(::ca::rgn* pRgn);

// Clipping Functions
	virtual int GetClipBox(LPRECT lpRect) const;
	virtual BOOL PtVisible(int x, int y) const;
			BOOL PtVisible(POINT point) const;
	virtual BOOL RectVisible(LPCRECT lpRect) const;
			int SelectClipRgn(::ca::rgn* pRgn);
			int ExcludeClipRect(int x1, int y1, int x2, int y2);
			int ExcludeClipRect(LPCRECT lpRect);
			int ExcludeUpdateRgn(::ca::window * pWnd);
			int IntersectClipRect(int x1, int y1, int x2, int y2);
			int IntersectClipRect(LPCRECT lpRect);
			int OffsetClipRgn(int x, int y);
			int OffsetClipRgn(SIZE size);
	int SelectClipRgn(::ca::rgn* pRgn, int nMode);

// Line-Output Functions
	point GetCurrentPosition() const;
	point MoveTo(int x, int y);
	point MoveTo(POINT point);
	BOOL LineTo(int x, int y);
	BOOL LineTo(POINT point);
	BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	BOOL Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	BOOL Polyline(LPPOINT lpPoints, int nCount);

	BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
	BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	BOOL ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	int GetArcDirection() const;
	int SetArcDirection(int nArcDirection);

	BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount);
	BOOL PolylineTo(const POINT* lpPoints, int nCount);
	BOOL PolyPolyline(const POINT* lpPoints,
		const DWORD* lpPolyPoints, int nCount);

	BOOL PolyBezier(const POINT* lpPoints, int nCount);
	BOOL PolyBezierTo(const POINT* lpPoints, int nCount);

// Simple Drawing Functions
	void FillRect(LPCRECT lpRect, ::ca::brush* pBrush);
	void FrameRect(LPCRECT lpRect, ::ca::brush* pBrush);
	void InvertRect(LPCRECT lpRect);
	BOOL DrawIcon(int x, int y, HICON hIcon);
	BOOL DrawIcon(POINT point, HICON hIcon);
#if (WINVER >= 0x400)
	BOOL DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags,
		HBRUSH hBrush = NULL);
	BOOL DrawState(point pt, size size, ::ca::bitmap* pBitmap, UINT nFlags,
		::ca::brush* pBrush = NULL);
	BOOL DrawState(point pt, size size, HICON hIcon, UINT nFlags,
		HBRUSH hBrush = NULL);
	BOOL DrawState(point pt, size size, HICON hIcon, UINT nFlags,
		::ca::brush* pBrush = NULL);
	BOOL DrawState(point pt, size size, const char * lpszText, UINT nFlags,
		BOOL bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL);
	BOOL DrawState(point pt, size size, const char * lpszText, UINT nFlags,
		BOOL bPrefixText = TRUE, int nTextLen = 0, ::ca::brush* pBrush = NULL);
	BOOL DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
		LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
	BOOL DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
		LPARAM lData, UINT nFlags, ::ca::brush* pBrush = NULL);
#endif

// Ellipse and Polygon Functions
	BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3,
		int x4, int y4);
	BOOL Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	void DrawFocusRect(LPCRECT lpRect);
	BOOL Ellipse(int x1, int y1, int x2, int y2);
	BOOL Ellipse(LPCRECT lpRect);
	BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	BOOL Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
	BOOL Polygon(LPPOINT lpPoints, int nCount);
	BOOL PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount);
	BOOL Rectangle(int x1, int y1, int x2, int y2);
	BOOL Rectangle(LPCRECT lpRect);
	BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
	BOOL RoundRect(LPCRECT lpRect, POINT point);

// Bitmap Functions
	BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
	BOOL BitBlt(int x, int y, int nWidth, int nHeight, WinDC* pgraphicsSrc,
		int xSrc, int ySrc, DWORD dwRop);
	BOOL StretchBlt(int x, int y, int nWidth, int nHeight, WinDC* pgraphicsSrc,
		int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
	COLORREF GetPixel(int x, int y) const;
	COLORREF GetPixel(POINT point) const;
	COLORREF SetPixel(int x, int y, COLORREF crColor);
	COLORREF SetPixel(POINT point, COLORREF crColor);
	BOOL FloodFill(int x, int y, COLORREF crColor);
	BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
	BOOL MaskBlt(int x, int y, int nWidth, int nHeight, WinDC* pgraphicsSrc,
		int xSrc, int ySrc, ::ca::bitmap& maskBitmap, int xMask, int yMask,
		DWORD dwRop);
	BOOL PlgBlt(LPPOINT lpPoint, WinDC* pgraphicsSrc, int xSrc, int ySrc,
		int nWidth, int nHeight, ::ca::bitmap& maskBitmap, int xMask, int yMask);
	BOOL SetPixelV(int x, int y, COLORREF crColor);
	BOOL SetPixelV(POINT point, COLORREF crColor);

// Text Functions
	virtual BOOL TextOut(int x, int y, const char * lpszString, int nCount);
			BOOL TextOut(int x, int y, const string & str);
	virtual BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				const char * lpszString, UINT nCount, LPINT lpDxWidths);
			BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				const string & str, LPINT lpDxWidths);
	virtual size TabbedTextOut(int x, int y, const char * lpszString, int nCount,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
			size TabbedTextOut(int x, int y, const string & str,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
	virtual int DrawText(const char * lpszString, int nCount, LPRECT lpRect,
				UINT nFormat);
			int DrawText(const string & str, LPRECT lpRect, UINT nFormat);
	size GetTextExtent(const char * lpszString, int nCount) const;
	size GetTextExtent(const string & str) const;
	size GetOutputTextExtent(const char * lpszString, int nCount) const;
	size GetOutputTextExtent(const string & str) const;
	size GetTabbedTextExtent(const char * lpszString, int nCount,
		int nTabPositions, LPINT lpnTabStopPositions) const;
	size GetTabbedTextExtent(const string & str,
		int nTabPositions, LPINT lpnTabStopPositions) const;
	size GetOutputTabbedTextExtent(const char * lpszString, int nCount,
		int nTabPositions, LPINT lpnTabStopPositions) const;
	size GetOutputTabbedTextExtent(const string & str,
		int nTabPositions, LPINT lpnTabStopPositions) const;
	virtual BOOL GrayString(::ca::brush* pBrush,
		BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData,
			int nCount, int x, int y, int nWidth, int nHeight);
	UINT GetTextAlign() const;
	UINT SetTextAlign(UINT nFlags);
	int GetTextFace(int nCount, LPTSTR lpszFacename) const;
	int GetTextFace(string & rString) const;
	BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const;
	BOOL GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const;
	int SetTextJustification(int nBreakExtra, int nBreakCount);
	int GetTextCharacterExtra() const;
	int SetTextCharacterExtra(int nCharExtra);

// Advanced Drawing
#if (WINVER >= 0x400)
	BOOL DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
	BOOL DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);
#endif

// Scrolling Functions
	BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip,
		::ca::rgn* pRgnUpdate, LPRECT lpRectUpdate);

// font Functions
	BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
	BOOL GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
	DWORD SetMapperFlags(DWORD dwFlag);
	size GetAspectRatioFilter() const;

	BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
	DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const;
	int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
	UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const;
	DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
		DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

	BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
		LPABCFLOAT lpABCF) const;
	BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar,
		float* lpFloatBuffer) const;

// Printer/Device Escape Functions
	virtual int Escape(int nEscape, int nCount,
		const char * lpszInData, LPVOID lpOutData);
	int Escape(int nEscape, int nInputSize, const char * lpszInputData,
		int nOutputSize, char * lpszOutputData);
	int DrawEscape(int nEscape, int nInputSize, const char * lpszInputData);

	// Escape helpers
	int StartDoc(const char * lpszDocName);  // old Win3.0 version
	int StartDoc(LPDOCINFO lpDocInfo);
	int StartPage();
	int EndPage();
	int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int));
	int AbortDoc();
	int EndDoc();

// MetaFile Functions
	BOOL PlayMetaFile(HMETAFILE hMF);
	BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds);
	BOOL AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
		// can be used for enhanced metafiles only

// Path Functions
	BOOL AbortPath();
	BOOL BeginPath();
	BOOL CloseFigure();
	BOOL EndPath();
	BOOL FillPath();
	BOOL FlattenPath();
	BOOL StrokeAndFillPath();
	BOOL StrokePath();
	BOOL WidenPath();
	float GetMiterLimit() const;
	BOOL SetMiterLimit(float fMiterLimit);
	int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const;
	BOOL SelectClipPath(int nMode);

// Misc Helper Functions
	static ::ca::brush* PASCAL GetHalftoneBrush();
	void DrawDragRect(LPCRECT lpRect, SIZE size,
		LPCRECT lpRectLast, SIZE sizeLast,
		::ca::brush* pBrush = NULL, ::ca::brush* pBrushLast = NULL);
	void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
	void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
	void Draw3dRect(int x, int y, int cx, int cy,
		COLORREF clrTopLeft, COLORREF clrBottomRight);

// ementation
public:
	virtual ~WinDC();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext & dumpcontext) const;
#endif

	// advanced use and implementation
	BOOL m_bPrinting;
	HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions

protected:
	// used for implementation of non-virtual SelectObject calls
	static ::ca::graphics_object* PASCAL SelectGdiObject(HDC hDC, HGDIOBJ h);
};
