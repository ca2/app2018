#pragma once


#if defined(LINUX) || defined(MACOS)


class CLASS_DECL_ca string_format_printer
{
public:



   typedef void (string_format_printer:: * PRINTER)(void *, const char *);


};



/*
format
String that contains the text to be written to stdout.
It can optionally contain embedded format tags that are substituted by the values specified in subsequent argument(s) and formatted as requested.
The number of arguments following the format parameters should at least be as much as the number of format tags.
The format tags follow this prototype:

%[flags][width][.precision][length]specifier
Where specifier is the most significant one and defines the type and the interpretation of the value of the coresponding argument:
specifier	Output	Example
c	Character	a
d or i	Signed decimal integer	392
e	Scientific notation (mantissa/exponent) using e character	3.9265e+2
E	Scientific notation (mantissa/exponent) using E character	3.9265E+2
f	Decimal floating point	392.65
g	Use the shorter of %e or %f	392.65
G	Use the shorter of %E or %f	392.65
o	Unsigned octal	610
s	String of characters	sample
u	Unsigned decimal integer	7235
x	Unsigned hexadecimal integer	7fa
X	Unsigned hexadecimal integer (capital letters)	7FA
p	Pointer address	B800:0000
n	Nothing printed. The argument must be a pointer to a signed int, where the number of characters written so far is stored.
%	A % followed by another % character will write % to stdout.	%

The tag can also contain flags, width, .precision and length sub-specifiers, which are optional and follow these specifications:

flags	description
-	Left-justify within the given field width; Right justification is the default (see width sub-specifier).
+	Forces to precede the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign.
(space)	If no sign is going to be written, a blank space is inserted before the value.
#	Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
Used with e, E and f, it forces the written output to contain a decimal point even if no digits would follow. By default, if no digits follow, no decimal point is written.
Used with g or G the result is the same as with e or E but trailing zeros are not removed.
0	Left-pads the number with zeroes (0) instead of spaces, where padding is specified (see width sub-specifier).

width	description
(number)	Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
*	The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.

.precision	description
.number	For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
For e, E and f specifiers: this is the number of digits to be printed after the decimal point.
For g and G specifiers: This is the maximum number of significant digits to be printed.
For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.
For c type: it has no effect.
When no precision is specified, the default is 1. If the period is specified without an explicit value for precision, 0 is assumed.
.*	The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.

length	description
h	The argument is interpreted as a short int or unsigned short int (only applies to integer specifiers: i, d, o, u, x and X).
l	The argument is interpreted as a long int or unsigned long int for integer specifiers (i, d, o, u, x and X), and as a wide character or wide character string for specifiers c and s.
L	The argument is interpreted as a long double (only applies to floating point specifiers: e, E, f, g and G).

additional arguments
Depending on the format string, the function may expect a sequence of additional arguments, each containing one value to be inserted instead of each %-tag specified in the format parameter, if any. There should be the same number of these arguments as the number of %-tags that expect a value.


*/

class CLASS_DECL_ca string_format
{
public:


   enum e_state
   {
      state_initial,
      state_waiting_width,
      state_parse_precision,
      state_waiting_precision,
      state_parse_length,
   };


   char *                           m_pszBuffer;
   int                              m_iSize;
   int                              m_iLength;


   string_format_printer *          m_pprinter;
   string_format_printer::PRINTER   m_pfnPrinter;
   void *                           m_pvoidPrinter;


   e_state                          m_estate;

   bool        m_bLeftJustify;
   bool        m_bForceShowSign;
   bool        m_bSharp;
   bool        m_bZeroPadding;

   int         m_iWidth;
   int         m_iPrecision;

   char        m_chLength;
   char        m_chSpec;



   template < class T > string_format(const T * pprinter, void (T::*pfnPrinter)(void *, const char *), void * pvoidPrinter)
   {
      construct(
         (string_format_printer *) (const_cast < T * > (pprinter)),
         reinterpret_cast < string_format_printer::PRINTER > (pfnPrinter),
         pvoidPrinter);

   }
   ~string_format();



   void construct(string_format_printer * pprinter, string_format_printer::PRINTER pfnPrinter, void * pvoidPrinter);

   void allocate_add_up(int iLenAddUp);

   inline void append(char ch)
   {

      if((m_iLength + 1 + 1) > m_iSize)
      {
         allocate_add_up(1);
      }

      m_iLength++;

      m_pszBuffer[m_iLength] = ch;



   }


   inline void append(const char * psz)
   {

      int iLen = strlen(psz);

      if((m_iLength + iLen + 1) > m_iSize)
      {

         allocate_add_up(1);

      }

      strncpy(&m_pszBuffer[m_iLength], psz, iLen);

      m_iLength += iLen;

   }

   bool parse(const char * & s);


   inline bool defer_get_additional_argument(const char * & s)
   {

      throw "missing argument value";

   }


   template < typename T, typename... Args>
   inline void defer_get_additional_argument(const char * & s, const T & value, Args... args)
   {

      if(m_estate == state_initial || m_estate == state_parse_precision || m_estate == state_parse_length)
      {

         if(!parse(s))
         {

            format(value);

            printf(s, args...);

            return;

         }

      }

      if(m_estate == state_waiting_width)
      {

         throw "width should plain int";

      }
      else if(m_estate == state_waiting_precision)
      {

         throw "width should plain int";

      }

      defer_get_additional_argument(s, args...);

   }

   template < typename T, typename... Args>
   inline void defer_get_additional_argument(const char * & s, const int & value, Args... args)
   {

      if(m_estate == state_initial || m_estate == state_parse_precision || m_estate == state_parse_length)
      {

         if(!parse(s))
         {

            format(value);

            printf(s, args...);

            return;

         }

      }

      if(m_estate == state_waiting_width)
      {

         m_iWidth = value;

         m_estate = state_parse_precision;

      }
      else if(m_estate == state_waiting_precision)
      {

         m_iPrecision = value;

         m_estate = state_parse_length;

      }

      defer_get_additional_argument(s, args...);

   }

   void format(char const &  i);

   void format(unsigned char const &  i);

   void format(short const &  i);

   void format(unsigned short const &  i);

   void format(int const &  i);

   void format(unsigned int const &  i);

   void format(long const & i);

   void format(unsigned long const & i);

   void format(long long const & i);

   void format(unsigned long long const & i);

   void format(double const &  i);

   void format(float const &  i);

   void format(void * const & p);

   void format(const void * const & p);

   void format(unsigned char * const & p);

   void format(const char * const & psz);

   inline void printf(const char * & s)
   {

      while (*s)
      {

         if(*s == '%' && *(++s) != '%')
            throw simple_exception(::ca::get_thread_app(), "invalid format string: missing arguments");

         append(*s++);

      }

      (m_pprinter->*m_pfnPrinter)(m_pvoidPrinter, m_pszBuffer);

   }

   template<typename T, typename... Args>
   inline void printf(const char * & s, const T & value, Args... args)
   {

      while (*s)
      {

         if (*s == '%' && *(++s) != '%')
         {

            defer_get_additional_argument(s, value);

            return;

         }

         append(*s++);

      }

      throw simple_exception(::ca::get_thread_app(), "extra arguments provided to printf");

   }

};

#endif
