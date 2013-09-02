#pragma once




class CLASS_DECL_c point_array :
   public array < point >
{
public:
   __inline point_array() : array < point >() {}
   __inline point_array(const point_array & pointset) { operator=(pointset); }
   virtual ~point_array();

   void offset(int32_t x, int32_t y);
   void offset(class point point);

   void rotate(double dAngle);

   void get_bounding_rect(LPRECT lprect) const;

   static void get_bounding_rect(LPRECT lprect, const POINT * lppoint, ::count count);

   bool bounding_rect_contains_pt(point pt);

   __inline index add(int32_t x, int32_t y) {return array < point >::add(point(x, y)); }
   __inline index add(POINT point) {return add(point.x, point.y); }
   __inline point_array & operator =(const point_array & pointset) { copy(pointset); return *this; }

};

class CLASS_DECL_c point64_array :
   public array < point64 >
{
public:
   virtual ~point64_array();

   void offset(int64_t x, int64_t y);
   void offset(class point64 point);
};


class CLASS_DECL_c pointd_array :
   public array < pointd >
{
public:
   __inline pointd_array() : array < pointd >() {}
   __inline pointd_array(const pointd_array & pointset) { operator=(pointset); }
   virtual ~pointd_array();

   void offset(double x, double y);
   void offset(class point point);

   void get_bounding_rect(LPRECTD lprect) const;

   static void get_bounding_rect(LPRECTD lprect, const POINTD * lppoint, ::count count);

   __inline index add(double x, double y) {return array < pointd >::add(pointd(x, y)); }
   __inline index add(pointd point) {return add(point.x, point.y); }
   __inline pointd_array & operator =(const pointd_array & pointset) { copy(pointset); return *this; }

};
