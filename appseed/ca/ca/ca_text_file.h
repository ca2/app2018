#pragma once

namespace ca
{

   class CLASS_DECL_ca text_file : 
      virtual public ca::file
   {
   public:
      text_file();
      virtual ~text_file();

      virtual void write_string(const char * lpsz);
      virtual UINT read_string(string & str);
      virtual UINT read_full_string(string & str);

      inline text_file & operator << (const char * lpsz)
      {
         write_string(lpsz);
         return *this;
      }
   };

   typedef ca::smart_pointer < text_file > text_file_sp;

} // namespace ca
