#pragma once


class point_array;


/////////////////////////////////////////////////////////////////////////////
// rect - A 2-D rectangle, similar to Windows RECT structure.

class CLASS_DECL_AURA rect : public tagRECT
{
// Constructors
public:
   // uninitialized rectangle
   rect() throw();
   // from left, top, right, and bottom
   rect(int64_t l, int64_t t, int64_t r, int64_t b) throw();
   // copy constructor
   rect(const RECT& srcRect) throw();
   rect(const RECT64& srcRect) throw();
   // from a pointer to another rect
   rect(LPCRECT lpSrcRect) throw();
   // from a point and size
   rect(POINT point, SIZE size) throw();
   // from two points
   rect(POINT topLeft, POINT bottomRight) throw();

// Attributes (in addition to RECT members)

   // retrieves the width
   int32_t width() const throw();
   // returns the height
   int32_t height() const throw();
   // returns the size
   class size size() const throw();
   class size size(LONG cx, LONG cy) throw();
   // reference to the top-left point
   point& top_left() throw();
   // reference to the bottom-right point
   point& bottom_right() throw();
   // const reference to the top-left point
   const point& top_left() const throw();
   // const reference to the bottom-right point
   const point& bottom_right() const throw();
   // the geometric center point of the rectangle
   point center() const throw();
   // swap the left and right
   void swap_left_right() throw();
   static void WINAPI swap_left_right(LPRECT lpRect) throw();

   // convert between rect and LPRECT/LPCRECT (no need for &)
   operator LPRECT() throw();
   operator LPCRECT() const throw();

   // returns TRUE if rectangle has no area
   bool is_empty() const throw();
   // returns TRUE if rectangle is at (0,0) and has no area
   bool is_null() const throw();
   // returns TRUE if point is within rectangle
   bool contains(POINT point) const throw();

// Operations

   // set rectangle from left, top, right, and bottom
   void set(int32_t i) throw();
   void set(int32_t x, int32_t y) throw();
   void set(int32_t x1, int32_t y1, int32_t x2, int32_t y2) throw();
   void set(POINT topLeft, POINT bottomRight) throw();
   // is_empty the rectangle
   void null() throw();
   // copy from another rectangle
   void copy(LPCRECT lpSrcRect) throw();
   // TRUE if exactly the same as another rectangle
   bool is_equal(LPCRECT lpRect) const throw();

   // Inflate rectangle's width and height by
   // x units to the left and right ends of the rectangle
   // and y units to the top and bottom.
   void inflate(int32_t x, int32_t y) throw();
   // Inflate rectangle's width and height by
   // size.cx units to the left and right ends of the rectangle
   // and size.cy units to the top and bottom.
   void inflate(SIZE size) throw();
   // Inflate rectangle's width and height by moving individual sides.
   // Left side is moved to the left, right side is moved to the right,
   // top is moved up and bottom is moved down.
   void inflate(LPCRECT lpRect) throw();
   void inflate(int32_t l, int32_t t, int32_t r, int32_t b) throw();

   // deflate the rectangle's width and height without
   // moving its top or left
   void deflate(int32_t x, int32_t y) throw();
   void deflate(SIZE size) throw();
   void deflate(LPCRECT lpRect) throw();
   void deflate(int32_t l, int32_t t, int32_t r, int32_t b) throw();

   // translate the rectangle by moving its top and left
   void offset(int32_t x, int32_t y) throw();
   void offset(SIZE size) throw();
   void offset(POINT point) throw();
   void normalize() throw();

   // absolute position of rectangle
   void move_to_y(int32_t y) throw();
   void move_to_x(int32_t x) throw();
   void move_to(int32_t x, int32_t y) throw();
   void move_to(POINT point) throw();

   // set this rectangle to intersection of two others
   bool intersect_x(LPCRECT lpRect1,LPCRECT lpRect2) throw();
   bool intersect_y(LPCRECT lpRect1,LPCRECT lpRect2) throw();
   bool intersect(LPCRECT lpRect1, LPCRECT lpRect2) throw();
   rect & intersect(LPCRECT lpRect2) throw();
   rect intersect(LPCRECT lpRect2) const throw();

   bool null_intersect(LPCRECT lpRect1,LPCRECT lpRect2) throw();

   bool top_left_null_intersect(LPCRECT lpRect1,LPCRECT lpRect2) throw();

   // set this rectangle to bounding union of two others
   bool unite(LPCRECT lpRect1, LPCRECT lpRect2) throw();

   // set this rectangle to minimum of two others
   bool subtract(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2) throw();

// Additional Operations
   void operator=(const RECT& srcRect) throw();
   bool operator==(const RECT& rect) const throw();
   bool operator!=(const RECT& rect) const throw();
   void operator+=(POINT point) throw();
   void operator+=(SIZE size) throw();
   void operator+=(LPCRECT lpRect) throw();
   void operator-=(POINT point) throw();
   void operator-=(SIZE size) throw();
   void operator-=(LPCRECT lpRect) throw();
   void operator&=(const RECT& rect) throw();
   void operator|=(const RECT& rect) throw();

// Operators returning rect values
   rect operator+(POINT point) const throw();
   rect operator-(POINT point) const throw();
   rect operator+(LPCRECT lpRect) const throw();
   rect operator+(SIZE size) const throw();
   rect operator-(SIZE size) const throw();
   rect operator-(LPCRECT lpRect) const throw();
   rect operator&(const RECT& rect2) const throw();
   rect operator|(const RECT& rect2) const throw();
   rect MulDiv(int32_t nMultiplier, int32_t nDivisor) const throw();

   int64_t area();

   bool contains(const RECT & rect) const;
   void constraint_v5(const RECT & rect, const class size sizeMin);
   void constraint_v7(const RECT & rect);
   void Align(int32_t align,const RECT & rect);
   void ScaleHeightAspect(int32_t iNewHeight, int32_t iCenterX, int32_t iCenterY);
   void ScaleRect(double dx, double dy, int32_t ix, int32_t iy);
   void ExtendOnCenter(const RECT & rect);
   void FitOnCenterOf(const RECT & rect);
   void FitOnCenterOf(const RECT & rect, SIZE size);
   void CenterOf(const RECT & rect);
   void CenterOf(const RECT & rect,SIZE size);
   void DeflateBottomRightSizeByRate(double dRate);
   void SetBottomRightSize(int32_t iWidth, int32_t iHeight);
   void SetBottomRightSize(SIZE sz);
   inline point top_right();
   inline point bottom_left();

   void SubtractRectMajor(LPCRECT lpcrectMajor, LPCRECT lpcrectMinor);
   void SubtractRectMinor(LPCRECT lpcrectMajor, LPCRECT lpcrectMinor);


   void get_bounding_rect(const POINT * lppoint, ::count count);
   void get_bounding_rect(const point_array & pointa);

   void assign(const RECT & rect,e_orientation eorientation) throw();
   void assign_normal(const RECT & rect,e_orientation eorientation) throw();

   LONG minimum_dimension() const throw(){ return MIN(width(),height()); }
   LONG maximum_dimension() const throw(){ return MAX(width(),height()); }
   LONG min_dimension() const throw(){ return minimum_dimension(); }
   LONG max_dimension() const throw(){ return maximum_dimension(); }
   LONG min_dim() const throw(){ return min_dimension(); }
   LONG max_dim() const throw(){ return max_dimension(); }

   LONG minimum_natural_dimension() const throw(){ return MIN(MAX(0, width()),MAX(0,height())); }
   LONG maximum_natural_dimension() const throw(){ return MAX(MAX(0, width()),MAX(0, height())); }

   LONG minimum_absolute_dimension() const throw(){ return MIN(abs(width()),abs(height())); }
   LONG maximum_absolute_dimension() const throw(){ return MIN(abs(width()),abs(height())); }

   LONG minimum_signed_absolute_dimension(bool bNegativePreference = true) const throw();
   LONG maximum_signed_absolute_dimension(bool bPositivePreference = true) const throw();

};

/////////////////////////////////////////////////////////////////////////////
// rect64 - A 2-D rect64angle, similar to Windows RECT structure.

class CLASS_DECL_AURA rect64 : public RECT64
{
// Constructors
public:
   // uninitialized rect64angle
   rect64() throw();
   // from left, top, right, and bottom
   rect64(int64_t l, int64_t t, int64_t r, int64_t b) throw();
   // copy constructor
   rect64(const RECT& srcRect) throw();
   // copy constructor
   rect64(const RECT64 & srcRect) throw();
   // from a point64_ter to another rect64
   rect64(const RECT * lpSrcRect) throw();
   // from a point64_ter to another rect64
   rect64(const RECT64 * lpSrcRect) throw();
   // from a POINT64 and size
   rect64(POINT64 POINT64, SIZE64 size) throw();
   // from two point64_ts
   rect64(POINT64 topLeft, POINT64 bottomRight) throw();

// Attributes (in addition to RECT members)

   // retrieves the width
   int64_t width() const throw();
   // returns the height
   int64_t height() const throw();
   // returns the size
   size64 size() const throw();
   // reference to the top-left POINT64
   point64& top_left() throw();
   // reference to the bottom-right POINT64
   point64& bottom_right() throw();
   // const reference to the top-left POINT64
   const point64& top_left() const throw();
   // const reference to the bottom-right POINT64
   const point64& bottom_right() const throw();
   // the geometric center POINT64 of the rect64angle
   point64 center() const throw();
   // swap the left and right
   void swap_left_right() throw();
   static void WINAPI swap_left_right(RECT64 * lpRect) throw();

   // convert between rect64 and RECT64 */const RECT64 * (no need for &)
   operator RECT64 *() throw();
   operator const RECT64 *() const throw();
   operator rect() const;

   // returns TRUE if rect64angle has no area
   bool is_empty() const throw();
   // returns TRUE if rect64angle is at (0,0) and has no area
   bool is_null() const throw();
   // returns TRUE if POINT64 is within rect64angle
   bool contains(POINT64 pt) const throw();

// Operations

   // set rect64angle from left, top, right, and bottom
   void set(int64_t x1, int64_t y1, int64_t x2, int64_t y2) throw();
   void set(POINT64 topLeft, POINT64 bottomRight) throw();
   // is_empty the rect64angle
   void null() throw();
   // copy from another rect64angle
   void copy(const RECT64 * lpSrcRect) throw();
   // TRUE if exactly the same as another rect64angle
   bool is_equal(const RECT64 * lpRect) const throw();

   // Inflate rect64angle's width and height by
   // x units to the left and right ends of the rect64angle
   // and y units to the top and bottom.
   void inflate(int64_t x, int64_t y) throw();
   // Inflate rect64angle's width and height by
   // size.cx units to the left and right ends of the rect64angle
   // and size.cy units to the top and bottom.
   void inflate(SIZE64 size) throw();
   // Inflate rect64angle's width and height by moving individual sides.
   // Left side is moved to the left, right side is moved to the right,
   // top is moved up and bottom is moved down.
   void inflate(const RECT64 * lpRect) throw();
   void inflate(int64_t l, int64_t t, int64_t r, int64_t b) throw();

   // deflate the rect64angle's width and height without
   // moving its top or left
   void deflate(int64_t x, int64_t y) throw();
   void deflate(SIZE64 size) throw();
   void deflate(const RECT64 * lpRect) throw();
   void deflate(int64_t l, int64_t t, int64_t r, int64_t b) throw();

   // translate the rect64angle by moving its top and left
   void offset(int64_t x, int64_t y) throw();
   void offset(SIZE64 size) throw();
   void offset(POINT64 POINT64) throw();
   void normalize() throw();

   void size(SIZE64 size) throw();
   void rsize(SIZE64 size) throw();
   void size(int64_t cx,int64_t cy) throw();
   void rsize(int64_t cx,int64_t cy) throw();

   // absolute position of rect64angle
   void move_to_y(int64_t y) throw();
   void move_to_x(int64_t x) throw();
   void move_to(int64_t x, int64_t y) throw();
   void move_to(POINT64 pt) throw();

   // set this rect64angle to int64_tersection of two others
   bool intersect(const RECT64 * lpRect1, const RECT64 * lpRect2) throw();

   // set this rect64angle to bounding union of two others
   bool unite(const RECT64 * lpRect1, const RECT64 * lpRect2) throw();

   // set this rect64angle to minimum of two others
//   bool subtract(const RECT64 * lpRectSrc1, const RECT64 * lpRectSrc2) throw();

// Additional Operations
   void operator=(const RECT64& srcRect) throw();
   bool operator==(const RECT64& rect64) const throw();
   bool operator!=(const RECT64& rect64) const throw();
   void operator+=(POINT64 POINT64) throw();
   void operator+=(SIZE64 size) throw();
   void operator+=(const RECT64 * lpRect) throw();
   void operator-=(POINT64 POINT64) throw();
   void operator-=(SIZE64 size) throw();
   void operator-=(const RECT64 * lpRect) throw();
   void operator&=(const RECT64& rect64) throw();
   void operator|=(const RECT64& rect64) throw();

// Operators returning rect64 values
   rect64 operator+(POINT64 POINT64) const throw();
   rect64 operator-(POINT64 POINT64) const throw();
   rect64 operator+(const RECT64 * lpRect) const throw();
   rect64 operator+(SIZE64 size) const throw();
   rect64 operator-(SIZE64 size) const throw();
   rect64 operator-(const RECT64 * lpRect) const throw();
   rect64 operator&(const RECT64 & rect642) const throw();
   rect64 operator|(const RECT64& rect642) const throw();
   rect64 MulDiv(int64_t nMultiplier, int64_t nDivisor) const throw();

   int64_t area();
   bool contains(const RECT64 * lpcrect) const;
   bool contains(const RECT64 * lpcrect);
   void ConstraintV5(const RECT64 * lpcrect, const class size sizeMin);
   void Align(int64_t align, const RECT64 * lpcrect);
   void ScaleHeightAspect(int64_t iNewHeight, int64_t iCenterX, int64_t iCenterY);
   void ScaleRect(double dx, double dy, int64_t ix, int64_t iy);
   void ExtendOnCenter(const RECT64 * lpcrect);
   void FitOnCenterOf(const RECT64 * lpcrect);
   void DeflateBottomRightSizeByRate(double dRate);
   void SetBottomRightSize(int64_t iWidth, int64_t iHeight);

   inline point64 top_right() const throw();
   inline point64 bottom_left() const throw();

   void SubtractRectMajor(const RECT64 * lpcrectMajor, const RECT64 * lpcrectMinor);
   void SubtractRectMinor(const RECT64 * lpcrectMajor, const RECT64 * lpcrectMinor);


   void assign(const RECT64 * lpcrect,e_orientation eorientation) throw();
   void assign_normal(const RECT64 * lpcrect,e_orientation eorientation) throw();


   int64_t minimum_dimension() const throw(){ return MIN(width(),height()); }
   int64_t maximum_dimension() const throw(){ return MAX(width(),height()); }
   int64_t min_dimension() const throw(){ return minimum_dimension(); }
   int64_t max_dimension() const throw(){ return maximum_dimension(); }
   int64_t min_dim() const throw(){ return min_dimension(); }
   int64_t max_dim() const throw(){ return max_dimension(); }

   int64_t minimum_natural_dimension() const throw(){ return MIN(MAX(0,width()),MAX(0,height())); }
   int64_t maximum_natural_dimension() const throw(){ return MAX(MAX(0,width()),MAX(0,height())); }

   int64_t minimum_absolute_dimension() const throw();
   int64_t maximum_absolute_dimension() const throw();

   int64_t minimum_signed_absolute_dimension(bool bNegativePreference = true) const throw();
   int64_t maximum_signed_absolute_dimension(bool bPositivePreference = true) const throw();

};





inline point rect::top_right()
{
   return point(right, top);
}

inline point rect::bottom_left()
{
   return point(left, bottom);
}

inline point rect::center() const throw()
{
   return point((left + right) / 2, (top + bottom) / 2);
}

inline bool rect::is_null() const throw()
{
   return left == 0 && top == 0 && right == 0 && bottom == 0;
}

inline bool rect::is_empty() const throw()
{
   return width() == 0 && height() == 0;
}









inline point64 rect64::top_right() const throw()
{
   return point64(right, top);
}

inline point64 rect64::bottom_left() const throw()
{
   return point64(left, bottom);
}

inline point64 rect64::center() const throw()
{
   return point64((left + right) / 2, (top + bottom) / 2);
}

inline bool rect64::is_null() const throw()
{
   return left == 0 && top == 0 && right == 0 && bottom == 0;
}

inline bool rect64::is_empty() const throw()
{
   return width() == 0 && height() == 0;
}

















class pointd_array;


/////////////////////////////////////////////////////////////////////////////
// rect - A 2-D rectangle, similar to Windows RECT structure.

class CLASS_DECL_AURA rectd : public RECTD
{
// Constructors
public:
   // uninitialized rectangle
   rectd() throw();
   // from left, top, right, and bottom
   rectd(double l, double t, double r, double b) throw();
   // copy constructor
   rectd(const RECTD& srcRect) throw();
   rectd(const RECT64& srcRect) throw();
   // from a pointer to another rectd
   rectd(LPCRECTD lpSrcRect) throw();
   // from a point and size
   rectd(POINTD point, SIZED size) throw();
   // from two points
   rectd(POINTD topLeft, POINTD bottomRight) throw();

// Attributes (in addition to RECTD members)

   // retrieves the width
   double width() const throw();
   // returns the height
   double height() const throw();
   // returns the size
   class sized size() const throw();
   // reference to the top-left point
   pointd& top_left() throw();
   // reference to the bottom-right point
   pointd& bottom_right() throw();
   // const reference to the top-left point
   const pointd& top_left() const throw();
   // const reference to the bottom-right point
   const pointd& bottom_right() const throw();
   // the geometric center point of the rectangle
   pointd center() const throw();
   // swap the left and right
   void swap_left_right() throw();
   static void WINAPI swap_left_right(LPRECTD lpRect) throw();

   // convert between rectd and LPRECTD/LPCRECTD (no need for &)
   operator LPRECTD() throw();
   operator LPCRECTD() const throw();

   // returns TRUE if rectangle has no area
   bool is_empty() const throw();
   // returns TRUE if rectangle is at (0,0) and has no area
   bool is_null() const throw();
   // returns TRUE if point is within rectangle
   bool contains(POINTD point) const throw();

// Operations

   // set rectangle from left, top, right, and bottom
   void set(double x1, double y1, double x2, double y2) throw();
   void set(POINTD topLeft, POINTD bottomRight) throw();
   // is_empty the rectangle
   void null() throw();
   // copy from another rectangle
   void copy(LPCRECTD lpSrcRect) throw();
   // TRUE if exactly the same as another rectangle
   bool is_equal(LPCRECTD lpRect) const throw();

   // Inflate rectangle's width and height by
   // x units to the left and right ends of the rectangle
   // and y units to the top and bottom.
   void inflate(double x, double y) throw();
   // Inflate rectangle's width and height by
   // size.cx units to the left and right ends of the rectangle
   // and size.cy units to the top and bottom.
   void inflate(SIZED size) throw();
   // Inflate rectangle's width and height by moving individual sides.
   // Left side is moved to the left, right side is moved to the right,
   // top is moved up and bottom is moved down.
   void inflate(LPCRECTD lpRect) throw();
   void inflate(double l, double t, double r, double b) throw();

   // deflate the rectangle's width and height without
   // moving its top or left
   void deflate(double x, double y) throw();
   void deflate(SIZED size) throw();
   void deflate(LPCRECTD lpRect) throw();
   void deflate(double l, double t, double r, double b) throw();

   // translate the rectangle by moving its top and left
   void offset(double x, double y) throw();
   void offset(SIZED size) throw();
   void offset(POINTD point) throw();
   void normalize() throw();

   // absolute position of rectangle
   void move_to_y(double y) throw();
   void move_to_x(double x) throw();
   void move_to(double x, double y) throw();
   void move_to(POINTD point) throw();

   // set this rectangle to intersection of two others
   bool intersect(LPCRECTD lpRect1, LPCRECTD lpRect2) throw();

   // set this rectangle to bounding union of two others
   bool unite(LPCRECTD lpRect1, LPCRECTD lpRect2) throw();

   // set this rectangle to minimum of two others
   //bool subtract(LPCRECTD lpRectSrc1, LPCRECTD lpRectSrc2) throw();

// Additional Operations
   void operator=(const RECTD& srcRect) throw();
   bool operator==(const RECTD& rectd) const throw();
   bool operator!=(const RECTD& rectd) const throw();
   void operator+=(POINTD point) throw();
   void operator+=(SIZED size) throw();
   void operator+=(LPCRECTD lpRect) throw();
   void operator-=(POINTD point) throw();
   void operator-=(SIZED size) throw();
   void operator-=(LPCRECTD lpRect) throw();
   void operator&=(const RECTD& rectd) throw();
   void operator|=(const RECTD& rectd) throw();

// Operators returning rectd values
   rectd operator+(POINTD point) const throw();
   rectd operator-(POINTD point) const throw();
   rectd operator+(LPCRECTD lpRect) const throw();
   rectd operator+(SIZED size) const throw();
   rectd operator-(SIZED size) const throw();
   rectd operator-(LPCRECTD lpRect) const throw();
   rectd operator&(const RECTD& rect2) const throw();
   rectd operator|(const RECTD& rect2) const throw();
   rectd MulDiv(double nMultiplier, double nDivisor) const throw();

   double area();

   bool contains(LPCRECTD lpcrect) const;
   void ConstraintV5(LPCRECTD lpcrect, const class size sizeMin);
   void Align(double align, LPCRECTD lpcrect);
   void ScaleHeightAspect(double iNewHeight, double iCenterX, double iCenterY);
   void ScaleRect(double dx, double dy, double ix, double iy);
   void ExtendOnCenter(LPCRECTD lpcrect);
   void FitOnCenterOf(LPCRECTD lpcrect);
   void CenterOf(LPCRECTD lpcrect);
   void CenterOf(LPCRECTD lpcrect,SIZED size);
   void DeflateBottomRightSizeByRate(double dRate);
   void SetBottomRightSize(double iWidth, double iHeight);
   inline pointd top_right();
   inline pointd bottom_left();

   void SubtractRectMajor(LPCRECTD lpcrectMajor, LPCRECTD lpcrectMinor);
   void SubtractRectMinor(LPCRECTD lpcrectMajor, LPCRECTD lpcrectMinor);


   void get_bounding_rect(const POINTD * lppoint, ::count count);
   void get_bounding_rect(const pointd_array & pointa);

   void assign(const RECTD * lpcrect,e_orientation eorientation) throw();
   void assign_normal(const RECTD * lpcrect,e_orientation eorientation) throw();

   double minimum_dimension() const throw(){ return MIN(width(),height()); }
   double maximum_dimension() const throw(){ return MAX(width(),height()); }
   double min_dimension() const throw(){ return minimum_dimension(); }
   double max_dimension() const throw(){ return maximum_dimension(); }
   double min_dim() const throw(){ return min_dimension(); }
   double max_dim() const throw(){ return max_dimension(); }

   double minimum_natural_dimension() const throw(){ return MIN(MAX(0,width()),MAX(0,height())); }
   double maximum_natural_dimension() const throw(){ return MAX(MAX(0,width()),MAX(0,height())); }

   double minimum_absolute_dimension() const throw();
   double maximum_absolute_dimension() const throw();

   double minimum_signed_absolute_dimension(bool bNegativePreference = true) const throw();
   double maximum_signed_absolute_dimension(bool bPositivePreference = true) const throw();


};


inline CLASS_DECL_AURA ::rect null_rect() { return ::rect(0, 0, 0, 0); }
inline CLASS_DECL_AURA ::rect64 null_rect64() { return ::rect64(0, 0, 0, 0); }


inline void rect::assign(const RECT & rect,e_orientation eorientation) throw()
{

   if(eorientation == orientation_horizontal)
   {

      left = rect.left;
      right = rect.right;

   }
   else if(eorientation == orientation_vertical)
   {

      top = rect.top;
      bottom = rect.bottom;

   }

}


inline void rect::assign_normal(const RECT & rect,e_orientation eorientation) throw()
{

   if(eorientation == orientation_horizontal)
   {

      top = rect.top;
      bottom = rect.bottom;

   }
   else if(eorientation == orientation_vertical)
   {

      left = rect.left;
      right = rect.right;

   }

}

inline void rect64::assign(const RECT64 * lpcrect,e_orientation eorientation) throw()
{

   if(eorientation == orientation_horizontal)
   {

      left = lpcrect->left;
      right = lpcrect->right;

   }
   else if(eorientation == orientation_vertical)
   {

      top = lpcrect->top;
      bottom = lpcrect->bottom;

   }

}


inline void rect64::assign_normal(const RECT64 * lpcrect,e_orientation eorientation) throw()
{

   if(eorientation == orientation_horizontal)
   {

      top = lpcrect->top;
      bottom = lpcrect->bottom;

   }
   else if(eorientation == orientation_vertical)
   {

      left = lpcrect->left;
      right = lpcrect->right;

   }

}

inline void rectd::assign(const RECTD * lpcrect,e_orientation eorientation) throw()
{

   if(eorientation == orientation_horizontal)
   {

      left = lpcrect->left;
      right = lpcrect->right;

   }
   else if(eorientation == orientation_vertical)
   {

      top = lpcrect->top;
      bottom = lpcrect->bottom;

   }

}


inline void rectd::assign_normal(const RECTD * lpcrect,e_orientation eorientation) throw()
{

   if(eorientation == orientation_horizontal)
   {

      top = lpcrect->top;
      bottom = lpcrect->bottom;

   }
   else if(eorientation == orientation_vertical)
   {

      left = lpcrect->left;
      right = lpcrect->right;

   }

}



template<> inline UINT HashKey<rect> (rect key)
{
   return key.left | key.top | key.bottom | key.right;
}






class CLASS_DECL_AURA rect3d
{
public:

   rect3d() {};
   rect3d(double x,double y,double z,double x2,double y2,double z2,double,double cx,double cy): m_p(x,y,z),m_d(x2,y2,z2),m_size(cx,cy) {}

   point3d    m_p;
   point3d    m_d; // direction
   pointd     m_size; // size


};


