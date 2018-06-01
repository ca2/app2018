#include "framework.h"
//#include <math.h>


os_max_int ceil_div(os_max_int num, os_max_int den)
{
   int64_t quo = num / den;
   if((num % den) > 0)
      quo++;
   return quo;
}


double tri(double angle)
{

   angle = fmod(angle,3.1415 * 2.0) * 2.0 / 3.1415;

   if(angle < 1.0)
      return angle;
   else if(angle >= 1.0 && angle < 3.0)
      return 2.0 - angle;
   else
      return angle - 4.0;


}


CLASS_DECL_AURA extern const char log2_LogTable256[256] =
{
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
   - 1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
   LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
   LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};
