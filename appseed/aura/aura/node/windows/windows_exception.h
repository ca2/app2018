#pragma once


namespace aura
{


   #ifndef _NO_EXCEPTIONS


   // Throw a atl_exception corresponding to the result of ::get_last_error
   NOINLINE DECLSPEC_NO_RETURN inline void WINAPI gen_ThrowLastWin32(::aura::application * papp)
   {
      DWORD dwError = ::get_last_error();
      _throw(hresult_exception(papp, HRESULT_FROM_WIN32( dwError ) ));
   }

   #else  // no exception handling

   // Throw a atl_exception corresponding to the result of ::get_last_error
   NOINLINE inline void WINAPI gen_ThrowLastWin32(::aura::application * papp)
   {
      DWORD dwError = ::get_last_error();
      _throw(hresult_exception(papp, HRESULT_FROM_WIN32( dwError ) ));
   }

   #endif  // no exception handling


}  // namespace aura




