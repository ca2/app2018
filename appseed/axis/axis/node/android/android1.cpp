//#include "framework.h"
//#include "android.h"


namespace android
{

   int32_t function()
   {
      return 0;
   }


   DWORD GetTempPath(string & str)
   {
      return ::GetTempPathW(MAX_PATH * 8, ::str::international::utf8_to_unicode(str, MAX_PATH * 8));
   }

} // namespace android


