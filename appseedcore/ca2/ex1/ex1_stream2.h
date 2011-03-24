#pragma once

namespace ex1
{

   class CLASS_DECL_ca output_stream2 :
      virtual public output_stream
   {
   public:
      output_stream2();
      output_stream2(writer * pwriter);
      virtual ~output_stream2();

      output_stream2 & operator << (unsigned char uch);
      output_stream2 & operator << (unsigned short ush);
      output_stream2 & operator << (int i);
      output_stream2 & operator << (unsigned long i);
      output_stream2 & operator << (LPCRECT lpcrect);
      output_stream2 & operator << (serializable & serializable);
      output_stream2 & operator << (const char * psz);
      output_stream2 & operator << (const string & str);
   };


}
