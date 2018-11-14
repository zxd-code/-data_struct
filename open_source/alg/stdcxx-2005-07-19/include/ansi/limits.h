/***************************************************************************
 *
 * limits.h
 *
 * $Id: //stdlib/dev/include/ansi/limits.h#17 $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994-2005 Quovadx,  Inc., acting through its  Rogue Wave
 * Software division. Licensed under the Apache License, Version 2.0 (the
 * "License");  you may  not use this file except  in compliance with the
 * License.    You    may   obtain   a   copy   of    the   License    at
 * http://www.apache.org/licenses/LICENSE-2.0.    Unless   required    by
 * applicable law  or agreed to  in writing,  software  distributed under
 * the License is distributed on an "AS IS" BASIS,  WITHOUT WARRANTIES OR
 * CONDITIONS OF  ANY KIND, either  express or implied.  See  the License
 * for the specific language governing permissions  and limitations under
 * the License.
 * 
 **************************************************************************/

#ifndef _RWSTD_LIMITS_H_INCLUDED
#define _RWSTD_LIMITS_H_INCLUDED


#include <rw/_defs.h>


#if !defined (_RWSTD_NO_PURE_C_HEADERS) || defined (_RWSTD_NO_LIMITS_H)

// 18.2.2, p2
#ifndef CHAR_BIT
#  define CHAR_BIT   _RWSTD_CHAR_BIT
#endif   // CHAR_BIT

#ifndef CHAR_MAX
#  define CHAR_MAX   _RWSTD_CHAR_MAX
#endif   // CHAR_MAX

#ifndef CHAR_MIN
#  define CHAR_MIN   _RWSTD_CHAR_MIN
#endif   // CHAR_MIN

#ifndef SCHAR_MAX
#  define SCHAR_MAX  _RWSTD_SCHAR_MAX
#endif   // SCHAR_MAX

#ifndef SCHAR_MIN
#  define SCHAR_MIN  _RWSTD_SCHAR_MIN
#endif   // SCHAR_MIN

#ifndef UCHAR_MAX
#  define UCHAR_MAX  _RWSTD_UCHAR_MAX
#endif   // UCHAR_MAX

#ifndef SHRT_MAX
#  define SHRT_MAX   _RWSTD_SHRT_MAX
#endif   // SHRT_MAX

#ifndef SHRT_MIN
#  define SHRT_MIN   _RWSTD_SHRT_MIN
#endif   // SHRT_MIN

#ifndef USHRT_MAX
#  define USHRT_MAX  _RWSTD_USHRT_MAX
#endif   // USHRT_MAX

#ifndef INT_MAX
#  define INT_MAX    _RWSTD_INT_MAX
#endif   // INT_MAX

#ifndef INT_MIN
#  define INT_MIN    _RWSTD_INT_MIN
#endif   // INT_MIN

#ifndef UINT_MAX
#  define UINT_MAX   _RWSTD_UINT_MAX
#endif   // UINT_MAX

#ifndef LONG_MAX
#  define LONG_MAX   _RWSTD_LONG_MAX
#endif   // LONG_MAX

#ifndef LONG_MIN
#  define LONG_MIN   _RWSTD_LONG_MIN
#endif   // LONG_MIN

#ifndef ULONG_MAX
#  define ULONG_MAX  _RWSTD_ULONG_MAX
#endif   // ULONG_MAX

#ifndef MB_LEN_MAX
#  define MB_LEN_MAX _RWSTD_MB_LEN_MAX
#endif   // MB_LEN_MAX

#else   // if defined (_RWSTD_NO_DEPRECATED_C_HEADERS)

#  if __GNUC__ > 2
#    pragma GCC system_header
#  endif

#  include _RWSTD_ANSI_C_LIMITS_H
#endif   // _RWSTD_NO_DEPRECATED_C_HEADERS

#if defined (__hpux__) || defined (__hpux) || defined (hpux)
#  ifndef _POSIX_PATH_MAX
#    define _POSIX_PATH_MAX 255
#  endif   // _POSIX_PATH_MAX
#endif   // HP-UX

#endif   // _RWSTD_LIMITS_H_INCLUDED
