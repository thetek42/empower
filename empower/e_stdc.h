#ifndef EMPOWER_STDC_H_
#define EMPOWER_STDC_H_

/******************************************************************************
 * 
 * This module provides a simpler way of checking standard C versions as well
 * as different compilers and platforms.
 *
 * The contents of this file can effectively be summarised as:
 *   #define E_STDC     E_STDC_<version>
 *   #define E_COMPILER E_COMPILER_<name>
 *   #define E_PLATFORM E_PLATFORM_<name>
 *
 * This file will probably be revised and improved in the future.
 *
 ******************************************************************************/

/* Standard C version */

#define E_STDC_C89 198900L
#define E_STDC_C95 199409L
#define E_STDC_C99 199901L
#define E_STDC_C11 201112L
#define E_STDC_C17 201710L
#define E_STDC_C23 202311L

#if !defined (__STDC_VERSION__)
# define E_STDC E_STDC_C89
#elif __STDC_VERSION__ >= E_STDC_C23
# define E_STDC E_STDC_C23
#elif __STDC_VERSION__ >= E_STDC_C17
# define E_STDC E_STDC_C17
#elif __STDC_VERSION__ >= E_STDC_C11
# define E_STDC E_STDC_C11
#elif __STDC_VERSION__ >= E_STDC_C99
# define E_STDC E_STDC_C99
#elif __STDC_VERSION__ >= E_STDC_C95
# define E_STDC E_STDC_C95
#else
# define E_STDC E_STDC_C89
#endif

/* Compilers */

#define E_COMPILER_OTHER 0
#define E_COMPILER_CLANG 1
#define E_COMPILER_GCC 2
#define E_COMPILER_TCC 3
#define E_COMPILER_MSVC 4
#define E_COMPILER_ICC 5
#define E_COMPILER_BORLAND 6

#if defined (__clang__)
# define E_COMPILER E_COMPILER_CLANG
#elif defined (__GNUC__)
# define E_COMPILER E_COMPILER_GCC
#elif defined (__TINYC__)
# define E_COMPILER E_COMPILER_TCC
#elif defined (_MSC_VER)
# define E_COMPILER E_COMPILER_MSVC
#elif defined (__INTEL_COMPILER)
# define E_COMPILER E_COMPILER_ICC
#elif defined (__BORLANDC__)
# define E_COMPILER E_COMPILER_BORLAND
#else
# define E_COMPILER E_COMPILER_OTHER
#endif

/* Platforms */

#define E_PLATFORM_OTHER 0
#define E_PLATFORM_LINUX 1
#define E_PLATFORM_MACOS 2
#define E_PLATFORM_MINGW 3
#define E_PLATFORM_WINDOWS 4
#define E_PLATFORM_FREEBSD 5
#define E_PLATFORM_NETBSD 6
#define E_PLATFORM_OPENBSD 7
#define E_PLATFORM_DRAGONFLY 8
#define E_PLATFORM_SOLARIS 9
#define E_PLATFORM_ANDROID 10

#if defined (__linux__)
# define E_PLATFORM E_PLATFORM_LINUX
#elif defined (__APPLE__) && defined (__MACH__)
# define E_PLATFORM E_PLATFORM_MACOS
#elif defined (__MINGW32__)
# define E_PLATFORM E_PLATFORM_MINGW
#elif defined (_WIN32)
# define E_PLATFORM E_PLATFORM_WINDOWS
#elif defined (__FreeBSD__)
# define E_PLATFORM E_PLATFORM_FREEBSD
#elif defined (__NetBSD__)
# define E_PLATFORM E_PLATFORM_NETBSD
#elif defined (__OpenBSD__)
# define E_PLATFORM E_PLATFORM_OPENBSD
#elif defined (__DragonFly__)
# define E_PLATFORM E_PLATFORM_DRAGONFLY
#elif defined (__sun)
# define E_PLATFORM E_PLATFORM_SOLARIS
#elif defined (__ANDROID__)
# define E_PLATFORM E_PLATFORM_ANDROID
#else
# define E_PLATFORM E_PLATFORM_OTHER
#endif

#endif /* EMPOWER_STDC_H_ */
