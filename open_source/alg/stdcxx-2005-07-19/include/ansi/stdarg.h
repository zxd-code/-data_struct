/***************************************************************************
 *
 * stdarg.h
 *
 * $Id: //stdlib/dev/include/ansi/stdarg.h#14 $
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

#include <rw/_defs.h>

#ifndef _RWSTD_NO_DEPRECATED_C_HEADERS

#  ifndef _RWSTD_STDARG_H_INCLUDED
#    define _RWSTD_STDARG_H_INCLUDED

#    include <cstdarg>

#    ifndef _RWSTD_NO_NAMESPACE

// not implemented - va_list is declared in the global namespace
// see <ansi/_cstdarg.h>
// using std::va_list;

#    endif   // _RWSTD_NO_NAMESPACE

#  endif   // _RWSTD_STDARG_H_INCLUDED

#else

#if !defined (__EDG__) || defined (__DECCXX) || defined (__INTEL_COMPILER)
#  include _RWSTD_ANSI_C_STDARG_H
#else   // vanilla EDG eccp
#  include <../include/stdarg.h>
#endif

#endif   // _RWSTD_NO_DEPRECATED_C_HEADERS
