﻿/* (C) Copyright 2008 Nick Mudge <mudgen@gmail.com>
 * This code can be freely copied and modified.
 */
#include  "framework.h"

namespace datetime
{

   bool check_end_expression(const char * input, const char * & scanner)
   {
      scanner = input;
      while(gen::ch::is_space_char(scanner) && *scanner != '\0')
         scanner++;
      if(*scanner == '\0')
         return true;
      if(!gen::ch::is_digit(scanner) &&
         !gen::ch::is_letter(scanner))
         return true;
      return false;
   }

   bool check_expression_separator(const char * input, const char * & scanner)
   {
      scanner = input;
      while(gen::ch::is_space_char(scanner) && *scanner != '\0')
         scanner  =gen::str::utf8_inc(scanner);
      if(*scanner == '\0')
         return true;
      if(scanner == input)
      {
         if(gen::ch::is_letter(scanner))
            return true;
         else
            return false;
      }
      else
         return true;
   }

   string check_unit(::user::str_context * pcontext, const char * input, const char * & scanner)
   {
      static id idCalendarDays("calendar:days");
      scanner = input;
      while(gen::ch::is_space_char(scanner) && *scanner != '\0')
         scanner = gen::str::utf8_inc(scanner);
      if(*scanner == '\0')
         return "";
      const char * start = scanner;
      string strCandidate;
      if(gen::ch::is_letter(scanner))
      {
         while(gen::ch::is_letter(scanner))
            scanner = gen::str::utf8_inc(scanner);
         strCandidate = string(start, scanner - start + 1);
         strCandidate.make_lower();
         if(pcontext->matches(idCalendarDays, strCandidate))
         {
            return "days";
         }
      }
      else
      {
         strCandidate = string(input);
         if(pcontext->begins(idCalendarDays, strCandidate))
         {
            return "days";
         }
      }
      if(strCandidate == "sec")
      {
         return strCandidate;
      }
      else if(strCandidate == "secs")
      {
         return strCandidate;
      }
      else if(strCandidate == "second")
      {
         return strCandidate;
      }
      else if(strCandidate == "seconds")
      {
         return strCandidate;
      }
      else if(strCandidate == "min")
      {
         return strCandidate;
      }
      else if(strCandidate == "mins")
      {
         return strCandidate;
      }
      else if(strCandidate == "minute")
      {
         return strCandidate;
      }
      else if(strCandidate == "minutes")
      {
         return strCandidate;
      }
      else if(strCandidate == "hour")
      {
         return strCandidate;
      }
      else if(strCandidate == "hours")
      {
         return strCandidate;
      }
      else if(strCandidate == "day")
      {
         return strCandidate;
      }
      else if(strCandidate == "days")
      {
         return strCandidate;
      }
      else if(strCandidate == "week")
      {
         return strCandidate;
      }
      else if(strCandidate == "weeks")
      {
         return strCandidate;
      }
      else if(strCandidate == "month")
      {
         return strCandidate;
      }
      else if(strCandidate == "months")
      {
         return strCandidate;
      }
      else if(strCandidate == "year")
      {
         return strCandidate;
      }
      else if(strCandidate == "years")
      {
         return strCandidate;
      }
      else if(strCandidate == "century")
      {
         return strCandidate;
      }
      else if(strCandidate == "centuries")
      {
         return strCandidate;
      }
      return "";
   }

   string check_month(const char * input, const char * & scanner)
   {
      scanner = input;
      while(isspace_dup(*scanner) && *scanner != '\0')
         scanner++;
      if(*scanner == '\0')
         return "";
      const char * start = scanner;
      while(isalpha_dup(*scanner))
         scanner++;
      string strCandidate = string(input, scanner - start);
      strCandidate.make_lower();
      if(strCandidate == "jan")
      {
         return strCandidate;
      }
      else if(strCandidate == "janeiro")
      {
         return strCandidate;
      }
      else if(strCandidate == "fev")
      {
         return strCandidate;
      }
      else if(strCandidate == "fevereiro")
      {
         return strCandidate;
      }
      else if(strCandidate == "mar")
      {
         return strCandidate;
      }
      else if(strCandidate == unitext("março"))
      {
         return strCandidate;
      }
      else if(strCandidate == "abr")
      {
         return strCandidate;
      }
      else if(strCandidate == "abril")
      {
         return strCandidate;
      }
      else if(strCandidate == "mai")
      {
         return strCandidate;
      }
      else if(strCandidate == "maio")
      {
         return strCandidate;
      }
      else if(strCandidate == "jun")
      {
         return strCandidate;
      }
      else if(strCandidate == "junho")
      {
         return strCandidate;
      }
      else if(strCandidate == "jul")
      {
         return strCandidate;
      }
      else if(strCandidate == "julho")
      {
         return strCandidate;
      }
      else if(strCandidate == "ago")
      {
         return strCandidate;
      }
      else if(strCandidate == "agosto")
      {
         return strCandidate;
      }
      else if(strCandidate == "set")
      {
         return strCandidate;
      }
      else if(strCandidate == "setembro")
      {
         return strCandidate;
      }
      else if(strCandidate == "out")
      {
         return strCandidate;
      }
      else if(strCandidate == "outubro")
      {
         return strCandidate;
      }
      else if(strCandidate == "nov")
      {
         return strCandidate;
      }
      else if(strCandidate == "novembro")
      {
         return strCandidate;
      }
      else if(strCandidate == "dez")
      {
         return strCandidate;
      }
      else if(strCandidate == "dezembro")
      {
         return strCandidate;
      }
      return "";
   }

   string check_lang_date(const char * input, const char * & scanner)
   {
      scanner = input;
      while(gen::ch::is_whitespace(scanner) && *scanner != '\0')
         scanner = gen::str::utf8_inc(scanner);
      if(*scanner == '\0')
         return "";
      const char * start = scanner;
      while(gen::ch::is_letter(scanner))
         scanner = gen::str::utf8_inc(scanner);
      string strCandidate = string(input, scanner - start);
      strCandidate.make_lower();
      if(strCandidate == "today")
      {
         return strCandidate;
      }
      else if(strCandidate == "tomorrow")
      {
         return strCandidate;
      }
      else if(strCandidate == "yesterday")
      {
         return strCandidate;
      }
      else if(strCandidate == "hoje")
      {
         return strCandidate;
      }
      else if(strCandidate == unitext("amanhã"))
      {
         return strCandidate;
      }
      else if(strCandidate == "ontem")
      {
         return strCandidate;
      }
      return "";
   }

   string check_natural(const char * input, const char * & scanner)
   {
      scanner = input;
      if(*scanner == '\0')
         return "";
      if(!gen::ch::is_digit(scanner))
         return "";
      const char * start = scanner;
      scanner = gen::str::utf8_inc(scanner);
      while(gen::ch::is_digit(scanner))
      {
         scanner = gen::str::utf8_inc(scanner);
      }
      return string(start, scanner - start);
   }


   string check_lang_offset(const char * input, const char * & scanner)
   {
      if(check_end_expression(input, scanner))
         return "";
      const char * start = input;
      while(!gen::ch::is_space_char(scanner) && *scanner != '\0')
         scanner = gen::str::utf8_inc(scanner);
      string strCandidate = string(input, scanner - start);
      strCandidate.make_lower();
      if(strCandidate == "ago")
      {
         return strCandidate;
      }
      else if(strCandidate == unitext("atrás"))
      {
         return strCandidate;
      }
      return "";
   }

   string check_natural_separator(const char * input, const char * & scanner)
   {
      scanner = input;
      if(*scanner == '\0')
         return "";
      if(*scanner == '/')
      {
         scanner++;
         return "/";
      }
      else if(*scanner == '-')
      {
         scanner++;
         return "/";
      }
      else if(*scanner == '.')
      {
         scanner++;
         return ".";
      }
      else
      {
         return "";
      }
   }

   string check_date(const char * input, const char * & scanner)
   {
      if(check_end_expression(input, scanner))
         return "";
      const char * start = input;
      string strNat1 = check_natural(input, scanner);
      if(strNat1.has_char())
      {
         input = scanner;
         string strSep1 = check_natural_separator(input, scanner);
         if(strSep1.has_char())
         {
            input = scanner;
            string strNat2 = check_natural(input, scanner);
            if(strNat2.has_char())
            {
               input = scanner;
               string strSep2 = check_natural_separator(input, scanner);
               if(strSep2.has_char() && strSep2 == strSep1)
               {
                  input = scanner;
                  string strNat3 = check_natural(input, scanner);
                  if(strNat3.has_char())
                  {
                     input = scanner;
                     if(!check_expression_separator(input, scanner))
                     {
                        throw "invalid date";
                     }
                     return string(start, scanner - start);
                  }

               }
               else if(strSep1 == strSep2)
               {
                  input = scanner;
                  string strMonth = check_month(input, scanner);
                  if(strMonth.has_char())
                  {
                     input = scanner;
                     if(!check_expression_separator(input, scanner))
                     {
                        throw "invalid date";
                     }
                     return string(start, scanner - start);
                  }
                  else
                  {
                     throw "invalid date";
                  }
               }
               else if(!strSep2.has_char())
               {
                  input = scanner;
                  if(!check_expression_separator(input, scanner))
                  {
                     throw "invalid date";
                  }
                  return string(start, scanner - start);
               }
               else
               {
                  throw "invalid date";
               }
            }
            else
            {
               input = scanner;
               string strMonth = check_month(input, scanner);
               if(strMonth.has_char())
               {
                  input = scanner;
                  string strSep2 = check_natural_separator(input, scanner);
                  if(strSep2.has_char() && strSep2 == strSep1)
                  {
                     input = scanner;
                     string strNat3 = check_natural(input, scanner);
                     if(strNat3.has_char())
                     {
                        input = scanner;
                        if(!check_expression_separator(input, scanner))
                        {
                           throw "invalid date";
                        }
                        return string(start, scanner - start);
                     }

                  }
                  else if(strSep2 == strSep1)
                  {
                     input = scanner;
                     string strNat2 = check_natural(input, scanner);
                     if(strNat2.has_char())
                     {
                        input = scanner;
                        if(!check_expression_separator(input, scanner))
                        {
                           throw "invalid date";
                        }
                        return string(start, scanner - start);
                     }
                     else
                     {
                        throw "invalid date";
                     }
                  }
                  else
                  {
                     throw "invalid date";
                  }
               }
            }
         }
         else
         {
            return "";
         }
         return "";
      }
      else
      {
         return "";
      }
   }

   string check_offset(::user::str_context * pcontext, const char * input, const char * & scanner)
   {
      if(check_end_expression(input, scanner))
         return "";
      const char * start = input;
      int iCount = 0;
      while(true)
      {
         input = scanner;
         string strNat = check_natural(input, scanner);
         if(!strNat.has_char())
         {
            if(iCount > 0)
               break;
            return "";
         }
         input = scanner;
         string strUnit = check_unit(pcontext, input, scanner);
         if(!strNat.has_char())
         {
            return "";
         }
         iCount++;
      }
      string strOff = check_lang_offset(input, scanner);
      return string(start, scanner - start);
   }

   string consume_date_expression(::user::str_context * pcontext, const char * & input)
   {
      const char * scanner;
      if(check_end_expression(input, scanner))
         return "";
      const char * start = input;
      string strDate = check_date(input, scanner);
      if(strDate.has_char())
      {
         input = scanner;
         string strOffset = check_offset(pcontext, input, scanner);
         if(strOffset.has_char())
         {
            input = scanner;
            if(check_end_expression(input, scanner))
               return string(start, scanner - start);
            else
            {
               throw "invalid date expression";
            }
         }
         else
         {
            input = scanner;
            if(check_end_expression(input, scanner))
               return string(start, scanner - start);
            else
            {
               throw "invalid date expression";
            }
         }
      }
      else
      {
         string strLangDate = check_lang_date(input, scanner);
         if(strLangDate.has_char())
         {
            input = scanner;
            string strOffset = check_offset(pcontext, input, scanner);
            if(strOffset.has_char())
            {
               input = scanner;
               if(check_end_expression(input, scanner))
                  return string(start, scanner - start);
               else
               {
                  throw "invalid date expression";
               }
            }
            else
            {
               input = scanner;
               if(check_end_expression(input, scanner))
                  return string(start, scanner - start);
               else
               {
                  throw "invalid date expression";
               }
            }
         }
         else
         {
            input = scanner;
            string strOffset = check_offset(pcontext, input, scanner);
            if(strOffset.has_char())
            {
               input = scanner;
               if(check_end_expression(input, scanner))
                  return string(start, scanner - start);
               else
               {
                  throw "invalid date expression";
               }
            }
            else
            {
               input = scanner;
               if(check_end_expression(input, scanner))
                  return string(start, scanner - start);
               else
               {
                  throw "invalid date expression";
               }
            }
         }
      }
   }

   scanner::scanner(::ca::application * papp, ::user::str_context * pcontext)
   {
      m_pstrcontext     = pcontext;
      m_ptoken          = NULL;
      input             = NULL;
   }

   scanner::~scanner()
   {
   }


   void scanner::initialize(const char * psz)
   {
      input          = psz;
      next_input     = input;
   }


   void scanner::peek()
   {
      const char *beginning;
      if(input != next_input && next_input != NULL)
         return;
      beginning = input;
      m_ptoken = look_ahead();
      next_input = input;
      input = beginning;
   }

   void scanner::next()
   {
      input = next_input;
   }

   token * scanner::look_ahead()
   {
      token * token;
      token = new class token;
      if(token == NULL)
         throw memory_exception(get_app());
      while(isspace_dup(*input))
         input++;
      if(*input == '\0')
      {
         token->value = token::end;
         return token;
      }
      if(*input == '+')
      {
         token->value = token::addition;
         input++;
         return token;
      }
      else if(*input == '-')
      {
         token->value = token::subtraction;
         input++;
         return token;
      }
      else if(*input == '*')
      {
         token->value = token::multiplication;
         input++;
         return token;
      }
      else if(*input == '/')
      {
         token->value = token::division;
         input++;
         return token;
      }
      else if(*input == '(')
      {
         token->value = token::open_paren;
         input++;
         return token;
      }
      else if(*input == ',')
      {
         token->value = token::virgula;
         input++;
         return token;
      }
      else if(*input == ')')
      {
         token->value = token::close_paren;
         input++;
         return token;
      }
      else
      {
         token->m_str = consume_date_expression(m_pstrcontext, input);
         while(gen::ch::is_space_char(input))
            input = gen::str::utf8_inc(input);
         if(*input == '(')
         {
            token->value = token::function;
         }
         else
         {
            token->value = token::identifier;
         }
         return token;
      }
   }


} // namespace datetime
