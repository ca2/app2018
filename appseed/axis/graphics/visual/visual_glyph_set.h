#pragma once


namespace visual
{


   class glyph_set : 
      public array < glyph >
   {
   public:


      
      glyph_set();
      virtual ~glyph_set();


      virtual index add(glyph & glyph);

      void quick_sort();
      
      index find_first(UINT user);
      
      glyph * GetGlyph(UINT user);


   };


} // namespace visual


