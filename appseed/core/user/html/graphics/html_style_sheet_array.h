#pragma once



namespace html
{

   class CLASS_DECL_CORE style_sheet_array :
      public smart_pointer_array < style_sheet >
   {
   public:


      style_sheet_array();


      style * rfind(e_tag etag, const string & strClass, const string & strSubClass, id idName);
      style * rfind_border_width(e_tag etag, const string & strClass, const string & strSubClass, id idName, float & f);
      style * rfind_border_color(e_tag etag, const string & strClass, const string & strSubClass, id idName, COLORREF & cr);

      style * greater(style * pstyle1, style * pstyle2);

      // retunrs true if determined
      bool greater(style * & pstyleRet, index & iRet, var & varRet, style * pstyle1, index i1, const var & var1, style * pstyle2, index i2, const var & var2);

   };

} // namespace html