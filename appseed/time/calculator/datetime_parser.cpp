/* (C) Copyright 2008 Nick Mudge <mudgen@gmail.com>
 * This code can be freely copied and modified.
 */
#include "framework.h"


namespace datetime
{

   parser::parser(::aura::application * papp, ::aura::str_context * pcontext) :
      element(papp),
      m_scanner(papp, pcontext)
   {
   }

   parser::~parser()
   {
   }

   element * parser::new_node()
   {
      return m_elementa.add(canew(::datetime::element));
   }

   //element * parser::parse(const char * psz)
/********************************************/
/* Parsing functions */

/*
   Grammer:
   expr   -> term + expr | term - expr | term
   term   -> func * term | func / term | func
   factor -> NUM | (expr) | term(expr) | -(factor) | +(factor)
   NUM    -> 1|2|3|4|5 ...
*/


element * parser::parse(const char * psz)
 {
   ::datetime::element *node;
   m_scanner.initialize(psz);
   node = expr(term(factor()));
   if(m_scanner.m_ptoken->value != token::end)
      syntax_error("Possible errors: illegal character, missing beginning parenthesis or missing operation");
   return node;
}

element * parser::expr(::datetime::element * pelement1)
{
   ::datetime::element * top_node;
   m_scanner.peek();
   if(m_scanner.m_ptoken->value == token::addition || m_scanner.m_ptoken->value == token::subtraction)
   {
      m_scanner.next();
      top_node = new_node();
      top_node->m_ptoken      = m_scanner.m_ptoken;
      top_node->m_pelement1   = pelement1;
      top_node->m_pelement2   = expr(term(factor()));
      return top_node;
   }
   return pelement1;
}

   element *parser::term(::datetime::element *m_pelement1)
   {
      ::datetime::element *top_node;
      m_scanner.peek();
      if(m_scanner.m_ptoken->value == token::multiplication || m_scanner.m_ptoken->value == token::division)
      {
         m_scanner.next();
         top_node                = new_node();
         top_node->m_ptoken      = m_scanner.m_ptoken;
         top_node->m_pelement1   = m_pelement1;
         top_node->m_pelement2   = term(factor());
         return top_node;
      }
      return m_pelement1;
   }


   element *parser::factor()
   {
      ::datetime::element *node;
      m_scanner.peek();
      if(m_scanner.m_ptoken->value == token::identifier)
      {
         m_scanner.next();
         node = new_node();
         node->m_ptoken = m_scanner.m_ptoken;
         return node;
      }
      else if(m_scanner.m_ptoken->value == token::addition)
      {
         m_scanner.next();
         node                = new_node();
         node->m_ptoken      = m_scanner.m_ptoken;
         node->m_pelement1   = factor();
         return node;
      }
      else if(m_scanner.m_ptoken->value == token::subtraction)
      {
         m_scanner.next();
         node                = new_node();
         node->m_ptoken      = m_scanner.m_ptoken;
         node->m_pelement1   = factor();
         return node;
      }
      else if(m_scanner.m_ptoken->value == token::open_paren)
      {
         m_scanner.next();
         node = expr(term(factor()));
         expect(token::close_paren);
         return node;
      }
      else if(m_scanner.m_ptoken->value == token::number)
      {
         m_scanner.next();
         node = new_node();
         node->m_ptoken = m_scanner.m_ptoken;
         return node;
      }
      else if(m_scanner.m_ptoken->value == token::function)
      {
         m_scanner.next();
         node                = new_node();
         node->m_ptoken      = m_scanner.m_ptoken;
         int32_t iCount;
         if(node->m_ptoken->m_str == "sqr")
         {
            iCount = 1;
         }
         else if(node->m_ptoken->m_str == "sqrt")
         {
            iCount = 1;
         }
         else if(node->m_ptoken->m_str == "exp")
         {
            iCount = 1;
         }
         else if(node->m_ptoken->m_str == "ln")
         {
            iCount = 1;
         }
         else if(node->m_ptoken->m_str == "log")
         {
            iCount = 1;
         }
         else if(node->m_ptoken->m_str == "pow")
         {
            iCount = 2;
         }
         else
         {
            throw "unknown function";
         }
         expect(token::open_paren);
         int32_t iElem = 1;
         if(iCount > 0)
         {
            while(true)
            {
               if(iElem == 1)
               {
                    node->m_pelement1   = expr(term(factor()));
               }
               else if(iElem == 2)
               {
                  node->m_pelement2   = expr(term(factor()));
               }
               else
               {
                  node->m_pelement3  = expr(term(factor()));
               }
               iCount--;
               if(iCount <= 0)
                  break;
               iElem++;
               expect(token::virgula);
            }
         }
         expect(token::close_paren);
         return node;
      }

      syntax_error("missing number or ending parenthesis");
      return NULL;
   }

   void parser::expect(char value)
   {
      char error_msg[11] = "expected ";
      error_msg[9] = value;
      error_msg[10] = '\0';
      m_scanner.peek();
      if(m_scanner.m_ptoken->value == (::datetime::token::e_type) value)
         m_scanner.next();
      else
         syntax_error(error_msg);
   }


   void parser::syntax_error(const char * psz)
   {
      error(string("syntax") + psz);
   }
   void parser::error(const char * psz)
   {
      string str;
      str = "error: ";
      str += psz;
      throw_datetime_parser_exception(str);

   }


} // namespace datetime
