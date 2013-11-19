#pragma once


namespace draw2d
{


   class CLASS_DECL_BASE path :
      virtual public ::draw2d::object
   {
   public:


     class CLASS_DECL_BASE arc
      {
      public:


         double   m_xCenter;
         double   m_yCenter;
         double   m_dRadiusX;
         double   m_dRadiusY;
         double   m_dAngle1;
         double   m_dAngle2;


      };

      class CLASS_DECL_BASE move
      {
      public:

         double   m_x;
         double   m_y;

      };


      class CLASS_DECL_BASE line
      {
      public:

         double   m_x;
         double   m_y;

      };

      class CLASS_DECL_BASE end
      {
      public:

         bool     m_bClose;

      };


      class CLASS_DECL_BASE element
      {
      public:


            enum e_type
            {
               type_begin,
               type_arc,
               type_line,
               type_move,
               type_end,
            };


            union
            {


               arc      m_arc;
               move     m_move;
               line     m_line;
               end      m_end;

            };

            e_type m_etype;


      };


      array < element >      m_elementa;
      bool                          m_bFill;
      ::draw2d::e_fill_mode             m_efillmode;
      bool                          m_bHasPoint;
      point                         m_pt;


      path();
      path(base_application * papp);
      virtual ~path();


      virtual bool has_current_point();
      virtual point last_point();



      virtual bool begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode);
      virtual bool end_figure(bool bClose);


      //virtual bool is_empty();
      //virtual bool has_current_point();
      //virtual point current_point();

      virtual bool add_arc(const RECT & rect, int32_t iStart, int32_t iAngle);

      virtual bool add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool add_line(point p1, point p2);

      virtual bool add_lines(const POINT * lppoint, int32_t nCount);

      virtual bool add_line(int32_t x, int32_t y);
      virtual bool add_line(point p);

      virtual bool add_move(int32_t x, int32_t y);
      virtual bool add_move(point p);

      virtual bool add_rect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool add_rect(LPCRECT lpcrect);

      virtual void * detach();


      void get_bounding_rect(LPRECT lprect);
      void get_bounding_rect(LPRECT lprect, element & e);
      void get_bounding_rect(LPRECT lprect, arc & a);
      void get_bounding_rect(LPRECT lprect, move & m);
      void get_bounding_rect(LPRECT lprect, line & l);


      //=============================================================================
      //
      // get_round_rect()
      //
      // Purpose:     Defines a Rounded Rectangle and places it in the GraphicsPath
      //
      // Parameters:  pPath		- [out] pointer to GraphicsPath that will recieve the
      //									path data
      //				r			- [in]	Rect that defines the round rectangle boundaries
      //				dia			- [in]	diameter of the rounded corners (2*radius)
      //
      // Returns:     None
      //
      bool add_round_rect(const RECT & rect, int32_t dia);
      bool add_round_top_left(const RECT & rect, int32_t dia);
      bool add_round_bottom_right(const RECT & rect, int32_t dia);




   };


   class CLASS_DECL_BASE path_sp :
      public smart_pointer < path >
   {
   public:

      path_sp()
      {
      }

      path_sp(const smart_pointer < path > & p) :
         smart_pointer < path > (p)
      {
      }

      path_sp(allocatorsp allocer) :
         smart_pointer < path > (allocer)
      {
      }

      path_sp & operator = (::draw2d::path * ppath)
      {

         smart_pointer < path >::operator = (ppath);
         
         return *this;
         
      }


   };


} // namespace draw2d






