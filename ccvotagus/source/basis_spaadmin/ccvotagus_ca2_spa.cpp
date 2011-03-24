#include "../spalib/os.h"
#include "../spalib/spalib.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
   spa_set_admin(true);
   return spalib_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}