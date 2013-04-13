#pragma once

namespace calculator
{

   class CLASS_DECL_ca2 token
   {
   public:
      token();
      ~token();
      /* Tokens */
      enum e_type
      {
         none,
         identifier,
         function,
         number,
         imaginary,
         keyword,
         end,
         error,
         addition,
         subtraction,
         multiplication,
         division,
         equal,
         semi_colon,
         open_paren,
         close_paren,
         char_error,
         virgula, // ,
      };
      e_type         value;
      string      m_str;
      int32_t         m_iType;
      bool        m_bKeyword;
   };

} // namespace calculator
