#pragma once


class CLASS_DECL_AXIS window_graphics
{
public:


   int32_t        cx;
   int32_t        cy;
   int32_t        scan;


   window_graphics();
   virtual ~window_graphics();


   virtual void create_window_graphics(oswindow interaction_impl, int64_t cx, int64_t cy, int iStride = -1);
   virtual void destroy_window_graphics();


   virtual void update_window(COLORREF * pOsBitmapData,const RECT & rect,int cxParam,int cyParam,int iStride = -1,bool bTransferBuffer = true);


   static void update_window(window_graphics * & pdata,oswindow interaction_impl,COLORREF * pOsBitmapData,const RECT & rect,int cxParam,int cyParam,int iStride = -1,bool bTransferBuffer = true);

   

};



