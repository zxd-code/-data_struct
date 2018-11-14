/***************************************************************************
 *
 * _math.h - Standard Library vs math.h exception conflict hack.
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/include/rw/_math.h#9 $
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

#ifndef _RWSTD_MATH_H_INCLUDED
#define _RWSTD_MATH_H_INCLUDED

#include <rw/_defs.h>

// avoid a conflicting exception structure on platforms where
// struct exception is defined unguarded in <math.h>
#ifndef _RWSTD_NO_MATH_EXCEPTION
#  undef exception
#  define exception math_exception
#endif   // _RWSTD_NO_MATH_EXCEPTION

#include _RWSTD_CMATH

#undef exception

// MSVC provides its own complex macro
#ifdef _MSC_VER
#  ifdef complex
#    undef complex
#  endif
#endif


#endif   // _RWSTD_MATH_H_INCLUDED
