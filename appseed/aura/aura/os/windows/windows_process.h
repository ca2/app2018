#pragma once


CLASS_DECL_AURA void call_sync(const char * path, const char * param);


CLASS_DECL_AURA bool process_modules(stringa & stra, uint32_t processID);

   
CLASS_DECL_AURA bool load_modules_diff(stringa & straOld, stringa & straNew, const char * pszExceptDir);


namespace windows
{

   template < typename PRED >
   bool pred_process_module(uint32_t processid, PRED pred)
   {

      HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

      MODULEENTRY32 me32;

      bool bStopped = false;

      hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processid);

      if (hModuleSnap == INVALID_HANDLE_VALUE)
      {

         return false;

      }

      me32.dwSize = sizeof(MODULEENTRY32);

      if (!Module32First(hModuleSnap, &me32))
      {

         ::CloseHandle(hModuleSnap);

         return false;

      }

      do
      {

         if (!pred(me32))
         {

            bStopped = true;

            break;

         }

      } while (Module32Next(hModuleSnap, &me32));

      ::CloseHandle(hModuleSnap);

      return bStopped;

   }


   template < typename PRED >
   bool pred_process(PRED pred)
   {

      HANDLE hProcessSnap;

      PROCESSENTRY32 pe32;

      hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

      if (hProcessSnap == INVALID_HANDLE_VALUE)
      {

         return false;

      }

      pe32.dwSize = sizeof(PROCESSENTRY32);

      if (!Process32First(hProcessSnap, &pe32))
      {

         ::CloseHandle(hProcessSnap);

         return false;

      }

      bool bStopped = false;

      do
      {

         if (!pred(pe32.th32ProcessID))
         {

            bStopped = true;

            break;

         }

      } while (Process32Next(hProcessSnap, &pe32));

      ::CloseHandle(hProcessSnap);

      return bStopped;

   }


} // namespace windows


