#pragma once


namespace datetime
{

   #define INITIALIZE_TABLE(table, function) if(table[0] == 0) function(table)

   /*
   var:
   a(ad)*
   a = a-z, A-Z, _
   d = 0-9
   operators:
   + -
   * /
   =
   numbers:
   0-9
   */


   
   CLASS_DECL_ca2 bool check_end_expression(const char * input, const char * & scanner);
   CLASS_DECL_ca2 bool check_expression_separator(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_unit(::user::str_context * pcontext, const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_month(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_lang_date(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_natural(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_lang_offset(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_natural_separator(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_date(const char * input, const char * & scanner);
   CLASS_DECL_ca2 string check_offset(::user::str_context * pcontext, const char * input, const char * & scanner);
   CLASS_DECL_ca2 string consume_date_expression(::user::str_context * pcontext, const char * & input);


   class CLASS_DECL_ca2 scanner :
      virtual public ::ca::object
   {
   public:
      
      
      const char *            input;
      const char *            next_input;

      class token *           m_ptoken;
      ::user::str_context *   m_pstrcontext;


      scanner(sp(::ca::application) papp, ::user::str_context * pstrcontext);
      virtual ~scanner();

      void initialize(const char * psz);
      void peek();
      void next();
      token * look_ahead();

   };


} // namespace calculator