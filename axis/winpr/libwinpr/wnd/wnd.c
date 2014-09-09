/**
 * WinPR: Windows Portable Runtime
 * Window Notification System
 *
 * Copyright 2014 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/crt.h>
#include <winpr/collections.h>

#include <winpr/wnd.h>

#ifndef _WIN32

#include "wnd.h"

/**
 * Custom Functions
 */

static wArrayList* g_WindowClasses = NULL;

void InitializeWindowClasses()
{
	if (g_WindowClasses)
		return;

	g_WindowClasses = ArrayList_New(TRUE);
}

WNDCLASSEXA* CloneWindowClass(CONST WNDCLASSEXA* lpwcx)
{
	WNDCLASSEXA* _lpwcx = NULL;

	_lpwcx = malloc(sizeof(WNDCLASSEXA));

	if (!_lpwcx)
		return NULL;

	CopyMemory(_lpwcx, lpwcx, sizeof(WNDCLASSEXA));

	_lpwcx->lpszClassName = _strdup(lpwcx->lpszClassName);
	_lpwcx->lpszMenuName = _strdup(lpwcx->lpszMenuName);

	return _lpwcx;
}

WNDCLASSEXA* FindWindowClass(LPCSTR lpClassName)
{
	int index;
	int count;
	WINBOOL found = FALSE;
	WNDCLASSEXA* lpwcx = NULL;

	ArrayList_Lock(g_WindowClasses);

	count = ArrayList_Count(g_WindowClasses);

	for (index = 0; index < count; index++)
	{
		lpwcx = (WNDCLASSEXA*) ArrayList_GetItem(g_WindowClasses, index);

		if (strcmp(lpClassName, lpwcx->lpszClassName) == 0)
		{
			found = TRUE;
			break;
		}
	}

	ArrayList_Unlock(g_WindowClasses);

	return (found) ? lpwcx : NULL;
}

/**
 * Standard Functions
 */

WORD WINAPI GetWindowWord(HWND hWnd, int nIndex)
{
	return 0;
}

WORD WINAPI SetWindowWord(HWND hWnd, int nIndex, WORD wNewWord)
{
	return 0;
}

LONG WINAPI GetWindowLongA(HWND hWnd, int nIndex)
{
	return 0;
}

LONG WINAPI GetWindowLongW(HWND hWnd, int nIndex)
{
	return 0;
}

LONG WINAPI SetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong)
{
	return 0;
}

LONG WINAPI SetWindowLongW(HWND hWnd, int nIndex, LONG dwNewLong)
{
	return 0;
}

LONG_PTR WINAPI GetWindowLongPtrA(HWND hWnd, int nIndex)
{
	return 0;
}

LONG_PTR WINAPI GetWindowLongPtrW(HWND hWnd, int nIndex)
{
	return 0;
}

LONG_PTR WINAPI SetWindowLongPtrA(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
	return 0;
}

LONG_PTR WINAPI SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
	return 0;
}

WINBOOL WINAPI DestroyWindow(HWND hWnd)
{
	WINPR_WND* pWnd;

	pWnd = (WINPR_WND*) hWnd;

	if (!pWnd)
		return FALSE;

	free(pWnd->lpClassName);

	if (pWnd->lpWindowName)
		free(pWnd->lpWindowName);

	free(pWnd);

	return TRUE;
}

VOID WINAPI PostQuitMessage(int nExitCode)
{

}

ATOM WINAPI RegisterClassA(CONST WNDCLASSA* lpWndClass)
{
	return 1;
}

ATOM WINAPI RegisterClassW(CONST WNDCLASSW* lpWndClass)
{
	return 1;
}

ATOM WINAPI RegisterClassExA(CONST WNDCLASSEXA* lpwcx)
{
	WNDCLASSEXA* _lpwcx;

	InitializeWindowClasses();

	_lpwcx = CloneWindowClass(lpwcx);

	ArrayList_Add(g_WindowClasses, (void*) _lpwcx);

	return 1;
}

ATOM WINAPI RegisterClassExW(CONST WNDCLASSEXW* lpwcx)
{
	return 1;
}

WINBOOL WINAPI UnregisterClassA(LPCSTR lpClassName, HINSTANCE hInstance)
{
	return TRUE;
}

WINBOOL WINAPI UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance)
{
	return TRUE;
}

HWND WINAPI CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName,
		LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	HWND hWnd;
	WINPR_WND* pWnd;
	WNDCLASSEXA* lpwcx;

	InitializeWindowClasses();

	if (!lpClassName)
		return NULL;

	lpwcx = FindWindowClass(lpClassName);

	if (!lpwcx)
		return NULL;

	pWnd = (WINPR_WND*) calloc(1, sizeof(WINPR_WND));

	if (!pWnd)
		return NULL;

	hWnd = (HWND) pWnd;

	pWnd->X = X;
	pWnd->Y = Y;
	pWnd->nWidth = nWidth;
	pWnd->nHeight = nHeight;
	pWnd->lpClassName = _strdup(lpClassName);

	if (lpWindowName)
		pWnd->lpWindowName = _strdup(lpWindowName);

	pWnd->hWndParent = hWndParent;
	pWnd->hMenu = hMenu;
	pWnd->hInstance = hInstance;
	pWnd->lpParam = lpParam;
	pWnd->lpwcx = lpwcx;

	return hWnd;
}

HWND WINAPI CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName,
		LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight,
		HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return NULL;
}

HWND WINAPI FindWindowA(LPCSTR lpClassName, LPCSTR lpWindowName)
{
	return NULL;
}

HWND WINAPI FindWindowW(LPCWSTR lpClassName, LPCWSTR lpWindowName)
{
	return NULL;
}

HWND WINAPI FindWindowExA(HWND hWndParent, HWND hWndChildAfter, LPCSTR lpszClass, LPCSTR lpszWindow)
{
	return NULL;
}

HWND WINAPI FindWindowExW(HWND hWndParent, HWND hWndChildAfter, LPCWSTR lpszClass, LPCWSTR lpszWindow)
{
	return NULL;
}

WINBOOL WINAPI GetMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	return TRUE;
}

WINBOOL WINAPI GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	return TRUE;
}

DWORD WINAPI GetMessagePos(VOID)
{
	return 0;
}

LONG WINAPI GetMessageTime(VOID)
{
	return 0;
}

LPARAM WINAPI GetMessageExtraInfo(VOID)
{
	return 0;
}

LPARAM WINAPI SetMessageExtraInfo(LPARAM lParam)
{
	return 0;
}

WINBOOL WINAPI SetMessageQueue(int cMessagesMax)
{
	return TRUE;
}

LRESULT WINAPI SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT status;
	WINPR_WND* pWnd;
	WNDPROC lpfnWndProc;

	pWnd = (WINPR_WND*) hWnd;

	if (!pWnd)
		return 0;

	lpfnWndProc = pWnd->lpwcx->lpfnWndProc;

	if (!lpfnWndProc)
		return 0;

	status = lpfnWndProc(hWnd, Msg, wParam, lParam);

	return status;
}

LRESULT WINAPI SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT WINAPI SendMessageTimeoutA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam,
		UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult)
{
	return 0;
}

LRESULT WINAPI SendMessageTimeoutW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam,
		UINT fuFlags, UINT uTimeout, PDWORD_PTR lpdwResult)
{
	return 0;
}

WINBOOL WINAPI SendNotifyMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

WINBOOL WINAPI SendNotifyMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

WINBOOL WINAPI SendMessageCallbackA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam,
		SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData)
{
	return TRUE;
}

WINBOOL WINAPI SendMessageCallbackW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam,
		SENDASYNCPROC lpResultCallBack, ULONG_PTR dwData)
{
	return TRUE;
}

WINBOOL WINAPI TranslateMessage(CONST MSG* lpMsg)
{
	return TRUE;
}

LRESULT WINAPI DispatchMessageA(CONST MSG* lpMsg)
{
	return 0;
}

LRESULT WINAPI DispatchMessageW(CONST MSG* lpMsg)
{
	return 0;
}

WINBOOL WINAPI PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	return TRUE;
}

WINBOOL WINAPI PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	return TRUE;
}

WINBOOL WINAPI ReplyMessage(LRESULT lResult)
{
	return TRUE;
}

WINBOOL WINAPI WaitMessage(VOID)
{
	return TRUE;
}

LRESULT WINAPI CallWindowProcA(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT WINAPI CallWindowProcW(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT WINAPI DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

#endif
