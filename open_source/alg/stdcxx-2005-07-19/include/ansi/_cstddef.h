// -*- C++ -*-
/***************************************************************************
 *
 * _cstddef.h - C++ Standard library interface to the ANSI C header stddef.h
 *
 * $Id: //stdlib/dev/include/ansi/_cstddef.h#7 $
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

#ifndef _RWSTD_CSTDDEF_H_INCLUDED
#define _RWSTD_CSTDDEF_H_INCLUDED


#include <rw/_defs.h>


_RWSTD_NAMESPACE (std) { 


#ifndef NULL
#  define NULL 0
#endif   // NULL


extern "C" {

#ifdef _RWSTD_PTRDIFF_T
typedef _RWSTD_PTRDIFF_T ptrdiff_t;
#else
typedef long ptrdiff_t;
#endif   // _RWSTD_PTRDIFF_T

#ifdef _RWSTD_SIZE_T
typedef _RWSTD_SIZE_T size_t;
#else
typedef unsigned long size_t;
#endif   // _RWSTD_SIZE_T


#define offsetof(class, member)  ((size_t)&(((class*)0)->member))


}   // extern "C"


}   // namespace std


#endif   // _RWSTD_CSTDDEF_H_INCLUDED
