// printf.cpp

// based on:
// LIBCTINY - Matt Pietrek 2001
// MSDN Magazine, January 2001

// 08/12/06 (mv)

#pragma once

CLASS_DECL_____ int printf_dup(const char *format, ...);
CLASS_DECL_____ int wprintf_dup(const wchar_t *format, ...);
CLASS_DECL_____ int vprintf_dup(const char *format, va_list args);
CLASS_DECL_____ int vwprintf_dup(const wchar_t *format, va_list args);

