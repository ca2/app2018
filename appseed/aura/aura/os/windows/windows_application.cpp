#include "framework.h"


#define TGA_FAILED 0
#define TGA_SUCCESS_CLEAN 1
#define TGA_SUCCESS_KILL 2
#define TGA_SUCCESS_16 3

DWORD TerminateGuiApp(DWORD dwPID, DWORD dwTimeout);
//DWORD WINAPI Terminate16App(DWORD dwPID, DWORD dwThread,
//   WORD w16Task, DWORD dwTimeout);

//#endif
int SendCtrlShiftQToChrome(HWND chrome, int iSleep, ::aura::application * papp);
//int SendURLToChrome(HWND chrome, string strUrl, ::aura::application * papp);
class block_input :
   virtual public object
{
protected:
   mutex    m_mutex;
   bool     m_bBlocked;
public:
   block_input(::aura::application * papp, int iSleep = 200);


   virtual ~block_input();

};
block_input::block_input(::aura::application * papp, int iSleep) :
   ::object(papp),
   m_mutex(papp, "Global\\ca2_input")
{
   m_mutex.lock();
   //   repeat:
   m_bBlocked = ::BlockInput(TRUE) != FALSE;
   //{

   //   DWORD dw = ::get_last_error();
   //   m_bBlocked = false;
   //   goto repeat;
   //}
   //else
   //{

   //   m_bBlocked = true;

   //}


}


block_input::~block_input()
{

   if (m_bBlocked)
   {
      ::BlockInput(FALSE);
   }
   m_mutex.unlock();

}








bool is_good_active_w(oswindow w)
{
   if (::GetForegroundWindow() == w || ::GetActiveWindow() == w || ::GetFocus() == w)
   {
      return true;
   }
   return false;
}
int SendCtrlShiftQToChrome(oswindow w, int iSleep, ::aura::application * papp)
{
   /*HWND h = ::GetWindow(chrome, GW_CHILD);
   SendMessage(chrome, 0x0272, 0, 0);
   SendMessage(h, 0x0090, 0, 0);
   return 1;
   }*/
   //   App(papp).simple_message_box_timeout(NULL, "Quiting browser...", seconds(3), MB_ICONASTERISK);
   block_input blockinput(papp);

   UINT ui;
   UINT character_count;
   WORD vka[3];
   char text[3];
   DWORD flag[3];

   vka[0] = VK_CONTROL;
   vka[1] = VK_SHIFT;
   vka[2] = 'Q';

   flag[0] = 0;
   flag[1] = 0;
   flag[2] = 0;

   text[0] = 0;
   text[1] = 0;
   text[2] = 0;


   ShowWindow(w, SW_SHOWNORMAL);

   ::SetWindowPos(w, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

   ::BringWindowToTop(w);

   ::SetForegroundWindow(w);

   if (!is_good_active_w(w))
      return 0;

   INPUT input;
   DWORD dwTime = 0;

   character_count = 3;
   for (ui = 0; ui < character_count; ui++)
   {
      if (GetKeyState(vka[character_count - ui - 1]) & 0x8000)
      {
         if (!is_good_active_w(w))
            return 0;
         input.type = INPUT_KEYBOARD;
         input.ki.wVk = vka[character_count - ui - 1];
         input.ki.wScan = text[character_count - ui - 1];
         input.ki.dwFlags = flag[character_count - ui - 1] | KEYEVENTF_KEYUP;
         input.ki.time = dwTime;
         input.ki.dwExtraInfo = GetMessageExtraInfo();
         if (!is_good_active_w(w))
            return 0;
         SendInput(1, &input, sizeof(input));
         if (!is_good_active_w(w))
            return 0;

         Sleep(iSleep / 8);
      }
   }

   for (ui = 0; ui < character_count; ui++)
   {
      //keystroke[i].type = INPUT_KEYBOARD;
      //keystroke[i].ki.wVk = vka[i];
      //keystroke[i].ki.wScan = text[i];
      //keystroke[i].ki.dwFlags = flag[i];
      //keystroke[i].ki.time = dwTime;
      //keystroke[i].ki.dwExtraInfo = GetMessageExtraInfo();
      if (!is_good_active_w(w))
         return 0;
      input.type = INPUT_KEYBOARD;
      input.ki.wVk = vka[ui];
      input.ki.wScan = text[ui];
      input.ki.dwFlags = flag[ui];
      input.ki.time = dwTime;
      input.ki.dwExtraInfo = GetMessageExtraInfo();
      if (!is_good_active_w(w))
         return 0;
      SendInput(1, &input, sizeof(input));
      if (!is_good_active_w(w))
         return 0;
      Sleep(iSleep / 8);
      if (!is_good_active_w(w))
         return 0;
   }
   for (ui = 0; ui < character_count; ui++)
   {

      //if (GetForegroundWindow() != chrome)
      //{

      //   return 0;

      //}
      input.type = INPUT_KEYBOARD;
      input.ki.wVk = vka[character_count - ui - 1];
      input.ki.wScan = text[character_count - ui - 1];
      input.ki.dwFlags = flag[character_count - ui - 1] | KEYEVENTF_KEYUP;
      input.ki.time = dwTime;
      input.ki.dwExtraInfo = GetMessageExtraInfo();
      //if (GetForegroundWindow() != chrome)
      //{

      //   return 0;

      //}
      SendInput(1, &input, sizeof(input));
      //if (GetForegroundWindow() != chrome)
      //{

      //   return 0;

      //}
      Sleep(iSleep / 8);
      //if (GetForegroundWindow() != chrome)
      //{

      //   return 0;

      //}
      //keystroke[i + character_count].type = INPUT_KEYBOARD;
      //keystroke[i + character_count].ki.wVk = vka[character_count-i-1];
      //keystroke[i + character_count].ki.wScan = text[character_count-i-1];
      //keystroke[i + character_count].ki.dwFlags = flag[character_count - i - 1] | KEYEVENTF_KEYUP;
      //keystroke[i + character_count].ki.time = dwTime;
      //keystroke[i + character_count].ki.dwExtraInfo = GetMessageExtraInfo();
      //      SendInput((UINT)keystrokes_to_send, keystroke, sizeof(*keystroke));
   }

   //keystrokes_sent = SendInput((UINT)keystrokes_to_send, keystroke, sizeof(*keystroke));

   //Send the keystrokes.
   //delete[] keystroke;

   Sleep(iSleep);

   //return keystrokes_sent == keystrokes_to_send;
   return 1;

}

//bool xor(bool a, bool b)
//{
//
//   return (!a) != (!b);
//
//}
//bool disable_caps(int iSleep)
//{
//   INPUT input;
//   if (GetKeyState(VK_CAPITAL) & 0x0001)
//   {
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_CAPITAL;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = 0;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep);
//
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_CAPITAL;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = KEYEVENTF_KEYUP;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep);
//
//   }
//   return true;
//}
//bool send_ctrl_t(int iSleep)
//{
//   INPUT input;
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = VK_CONTROL;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = 0x41 - 'A' + 'T';
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = 0x41 - 'A' + 'T';
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = VK_CONTROL;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//   return true;
//
//}
//bool send_f6(int iSleep)
//{
//   INPUT input;
//   if (GetKeyState(VK_CAPITAL) & 0x0001)
//   {
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_F6;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = 0;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep);
//
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_F6;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = KEYEVENTF_KEYUP;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep);
//
//   }
//   return true;
//}
//bool send_enter(int iSleep)
//{
//   INPUT input;
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = VK_RETURN;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep/8);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = VK_RETURN;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//   return true;
//}
//
//
//bool send_input_digit(int i, int iShift, int iSleep)
//{
//   INPUT input;
//   if (iShift)
//   {
//      ZERO(input);
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_SHIFT;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = 0;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep / 8);
//
//   }
//
//   ZERO(input);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = i - '0' + 0x30;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep / 8);
//
//   ZERO(input);
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = i - '0' + 0x30;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep/8);
//   if (iShift)
//   {
//      ZERO(input);
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_SHIFT;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = KEYEVENTF_KEYUP;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep / 8);
//
//   }
//
//   Sleep(iSleep);
//
//
//   return true;
//
//}
//
//bool send_input_alpha(int i, int iSleep)
//{
//   INPUT input;
//
//   ZERO(input);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = i - 'a' + 0x41;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep / 8);
//
//   ZERO(input);
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = i - 'a' + 0x41;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//
//
//   return true;
//
//}
//
//bool send_input_vk(int i, int iShift, int iSleep)
//{
//   INPUT input;
//
//   if (iShift)
//   {
//      ZERO(input);
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_SHIFT;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = 0;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep / 8);
//
//   }
//
//   ZERO(input);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = i;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep / 8);
//
//   ZERO(input);
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = i;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep / 8);
//
//
//   if (iShift)
//   {
//      ZERO(input);
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_SHIFT;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = KEYEVENTF_KEYUP;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep / 8);
//
//   }
//
//   Sleep(iSleep);
//
//
//   return true;
//
//}
//
//bool send_input_scan(int i, int iShift, int iSleep)
//{
//   INPUT input;
//
//   if (iShift)
//   {
//      ZERO(input);
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_SHIFT;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = 0;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep / 8);
//
//   }
//
//   ZERO(input);
//
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = 0;
//   input.ki.wScan = i;
//   input.ki.dwFlags = KEYEVENTF_SCANCODE;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep / 8);
//
//   ZERO(input);
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = 0;
//   input.ki.wScan = i;
//   input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep / 8);
//
//
//   if (iShift)
//   {
//      ZERO(input);
//      input.type = INPUT_KEYBOARD;
//      input.ki.wVk = VK_SHIFT;
//      input.ki.wScan = 0;
//      input.ki.dwFlags = KEYEVENTF_KEYUP;
//      input.ki.time = 0;
//      input.ki.dwExtraInfo = GetMessageExtraInfo();
//      SendInput(1, &input, sizeof(input));
//      Sleep(iSleep / 8);
//
//   }
//
//   Sleep(iSleep);
//
//
//   return true;
//
//}
//
//bool send_input_caps_alpha(int i, int iSleep)
//{
//   INPUT input;
//   ZERO(input);
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = VK_SHIFT;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = 0;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep/8);
//
//   send_input_alpha(i + 'a' - 'A', iSleep / 8);
//
//   ZERO(input);
//   input.type = INPUT_KEYBOARD;
//   input.ki.wVk = VK_SHIFT;
//   input.ki.wScan = 0;
//   input.ki.dwFlags = KEYEVENTF_KEYUP;
//   input.ki.time = 0;
//   input.ki.dwExtraInfo = GetMessageExtraInfo();
//   SendInput(1, &input, sizeof(input));
//   Sleep(iSleep);
//   return true;
//
//}
//
//bool send_input_unicode(int i, int iSleep)
//{
//   if (i >= 'A' &&  i <= 'Z')
//   {
//
//      send_input_caps_alpha(i, iSleep);
//
//   }
//   else if (i >= 'a' &&  i <= 'z')
//   {
//
//      send_input_alpha(i, iSleep);
//
//   }
//   else if (i >= '0' &&  i <= '9')
//   {
//
//      send_input_digit(i,0, iSleep);
//
//   }
//   else if (i == ':')
//   {
//
//      send_input_scan(53, 1, iSleep);
//
//   }
//   else if (i == '/')
//   {
//
//      send_input_scan(115, 0, iSleep);
//
//   }
//   else if (i == '?')
//   {
//
//      send_input_scan(115, 1, iSleep);
//
//   }
//   else if (i == ')')
//   {
//
//      send_input_digit('0', 1, iSleep);
//
//   }
//   else if (i == '!')
//   {
//
//      send_input_digit('1', 1, iSleep);
//
//   }
//   else if (i == '@')
//   {
//
//      send_input_digit('2', 1, iSleep);
//
//   }
//   else if (i == '#')
//   {
//
//      send_input_digit('3', 1, iSleep);
//
//   }
//   else if (i == '$')
//   {
//
//      send_input_digit('4', 1, iSleep);
//
//   }
//   else if (i == '%')
//   {
//
//      send_input_digit('5', 1, iSleep);
//
//   }
//   else if (i == '�')
//   {
//
//      send_input_digit('6', 1, iSleep);
//
//   }
//   else if (i == '&')
//   {
//
//      send_input_digit('7', 1, iSleep);
//
//   }
//   else if (i == '*')
//   {
//
//      send_input_digit('8', 1, iSleep);
//
//   }
//   else if (i == '(')
//   {
//
//      send_input_digit('9', 1, iSleep);
//
//   }
//   else if (i == '-')
//   {
//
//      send_input_scan(12, 0, iSleep);
//
//   }
//   else if (i == '_')
//   {
//
//      send_input_scan(12, 1, iSleep);
//
//   }
//   else if (i == '=')
//   {
//
//      send_input_scan(13, 0, iSleep);
//
//   }
//   else if (i == '+')
//   {
//
//      send_input_scan(13, 1, iSleep);
//
//   }
//   else if (i == '<')
//   {
//
//      send_input_scan(51, 1, iSleep);
//
//   }
//   else if (i == ',')
//   {
//
//      send_input_scan(51, 0, iSleep);
//
//   }
//   else if (i == '>')
//   {
//
//      send_input_scan(52, 1, iSleep);
//
//   }
//   else if (i == '.')
//   {
//
//      send_input_scan(52, 0, iSleep);
//
//   }
//   else if (i == ' ')
//   {
//
//      send_input_vk(VK_SPACE, 0, iSleep);
//
//   }
//   return true;
//
//}
//
////bool add_input_unicode(array < INPUT> & ia, int ch, HKL hkl)
////{
////   INPUT Event = { 0 };
////
////
////
////   const SHORT Vk = VkKeyScanExW(ch, hkl);
////   //const UINT VKey = ::MapVirtualKey(LOBYTE(Vk), 0);
////
////   if (HIBYTE(Vk) == 1) // Check if shift key needs to be pressed for this key
////   {
////      // Press shift key
////      ::ZeroMemory(&Event, sizeof(Event));
////      Event.type = INPUT_KEYBOARD;
////      Event.ki.dwFlags = 0;
////      Event.ki.wVk = VK_LSHIFT;
////      ia.add(Event);
////   }
////
////   // Keydown
////   ::ZeroMemory(&Event, sizeof(Event));
////   Event.type = INPUT_KEYBOARD;
////   Event.ki.dwFlags = 0;
////   Event.ki.wVk = Vk;
////   ia.add(Event);
////
////   // Keyup
////   ::ZeroMemory(&Event, sizeof(Event));
////   Event.type = INPUT_KEYBOARD;
////   Event.ki.dwFlags = KEYEVENTF_KEYUP;
////   Event.ki.wVk = Vk;
////   ia.add(Event);
////
////   if (HIBYTE(Vk) == 1)// Release if previouly pressed
////   {
////      // Release shift key
////      ::ZeroMemory(&Event, sizeof(Event));
////      Event.type = INPUT_KEYBOARD;
////      Event.ki.dwFlags = KEYEVENTF_KEYUP;
////      Event.ki.wVk = VK_LSHIFT;
////      ia.add(Event);
////   }
////   return true;
////
////}
////bool add_input_unicode(array < INPUT> & ia, int ch, HKL hkl)
////bool add_input_unicode(array < INPUT> & ia, int ch, int iSleep)
////bool send_input_unicode(array < INPUT> & ia, int ch, int iSleep)
////{
////   INPUT Event[2];
////
////   // Keydown
////   ::ZeroMemory(Event, sizeof(Event));
////   Event[0].type = INPUT_KEYBOARD;
////   Event[0].ki.dwFlags = KEYEVENTF_UNICODE;
////   Event[0].ki.wScan = ch;
////
////
////   // Keyup
////   //::ZeroMemory(&Event, sizeof(Event));
////   Event[1].type = INPUT_KEYBOARD;
////   Event[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
////   Event[1].ki.wScan = ch;
////   //ia.add(Event);
////   SendInput(2, Event, sizeof(Event));
////   Sleep(iSleep);
////
////   //const SHORT Vk = VkKeyScanExW(ch, hkl);
////   ////const UINT VKey = ::MapVirtualKey(LOBYTE(Vk), 0);
////
////   //if (HIBYTE(Vk) == 1) // Check if shift key needs to be pressed for this key
////   //{
////   //   // Press shift key
////   //   ::ZeroMemory(&Event, sizeof(Event));
////   //   Event.type = INPUT_KEYBOARD;
////   //   Event.ki.dwFlags = KEYEVENTF_SCANCODE;
////   //   Event.ki.wScan = MapVirtualKeyEx(VK_LSHIFT, 0, hkl);
////   //   ia.add(Event);
////   //}
////
////   //// Keydown
////   //::ZeroMemory(&Event, sizeof(Event));
////   //Event.type = INPUT_KEYBOARD;
////   //Event.ki.dwFlags = KEYEVENTF_SCANCODE;
////   //Event.ki.wScan = MapVirtualKeyEx(Vk, 0, hkl);
////   //ia.add(Event);
////
////   //// Keyup
////   //::ZeroMemory(&Event, sizeof(Event));
////   //Event.type = INPUT_KEYBOARD;
////   //Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
////   //Event.ki.wScan = MapVirtualKeyEx(Vk, 0, hkl);
////   //ia.add(Event);
////
////   //if (HIBYTE(Vk) == 1)// Release if previouly pressed
////   //{
////   //   // Release shift key
////   //   ::ZeroMemory(&Event, sizeof(Event));
////   //   Event.type = INPUT_KEYBOARD;
////   //   Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
////   //   Event.ki.wScan = MapVirtualKeyEx(VK_LSHIFT, 0, hkl);
////   //   ia.add(Event);
////   //}
////   return true;
////
////}
//
//
//bool send_input_string(string str, int iSleep)
//{
//
//   const char * psz = str;
//
//   while (psz != NULL && *psz != '\0')
//   {
//
//      int iIndex = ::str::ch::uni_index(psz);
//
//      send_input_unicode(iIndex, iSleep);
//
//      psz = ::str::utf8_inc(psz);
//
//   }
//
//   return true;
//
//}
//
//
//int SendURLToChrome(HWND chrome, string strUrl, ::aura::application * papp)
//{
//
//   App(pbapp).simple_message_box_timeout(NULL, "Sending URL to browser...", seconds(3), MB_ICONASTERISK);
//
//   block_input blockinput(papp);
//
//   if (!ShowWindow(chrome, SW_SHOWNORMAL))
//      return 0;
//
//   if (!SetForegroundWindow(chrome))
//      return 0;
//
//   int iSleep = 40;
//
//   disable_caps(iSleep);
//
//   send_ctrl_t(iSleep);
//
//   Sleep(500);
//
//   send_f6(iSleep);
//
//   Sleep(500);
//
//   send_input_string(strUrl, iSleep);
//
//   send_enter(iSleep);
//
//
//
//   return 1;
//
//}
//
//#endif

BOOL CALLBACK TerminateGuiAppEnum(HWND hwnd, LPARAM lParam);

DWORD TerminateGuiApp(DWORD dwPID, DWORD dwTimeout)
{
   HANDLE   hProc;
   DWORD   dwRet;

   // If we can't open the process with PROCESS_TERMINATE rights,
   // then we give up immediately.
   hProc = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE,
      dwPID);

   if (hProc == NULL)
   {
      return TGA_FAILED;
   }

   // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
   // matches your process's.
   EnumWindows((WNDENUMPROC)TerminateGuiAppEnum, (LPARAM)dwPID);

   // Wait on the handle. If it signals, great. If it times out,
   // then you kill it.
   if (WaitForSingleObject(hProc, dwTimeout) != WAIT_OBJECT_0)
      dwRet = (TerminateProcess(hProc, 0) ? TGA_SUCCESS_KILL : TGA_FAILED);
   else
      dwRet = TGA_SUCCESS_CLEAN;

   CloseHandle(hProc);

   return dwRet;
}



BOOL CALLBACK TerminateGuiAppEnum(HWND hwnd, LPARAM lParam)
{
   DWORD dwID;

   GetWindowThreadProcessId(hwnd, &dwID);

   if (dwID == (DWORD)lParam)
   {
      PostMessage(hwnd, WM_CLOSE, 0, 0);
   }

   return TRUE;
}

