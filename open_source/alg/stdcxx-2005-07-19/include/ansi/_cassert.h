// -*- C++ -*-
/***************************************************************************
 *
 * _cassert.h - C++ Standard library interface to the ANSI C header assert.h
 *
 * $Id: //stdlib/dev/include/ansi/_cassert.h#10 $
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

_RWSTD_NAMESPACE (__rw) { 

void _RWSTD_EXPORT
__rw_assert_fail (const char*, const char*, int, const char*)
    _RWSTD_GNUC_ATTRIBUTE ((noreturn));


}   // namespace __rw

#undef assert

#if !defined (NDEBUG)
#  if defined (__GNUG__) || defined (__HP_aCC)
#    define assert(expr)                                                   \
     ((expr) ? (void)0 : _RW::__rw_assert_fail (#expr, __FILE__, __LINE__, \
                                                __PRETTY_FUNCTION__))
#  elif !defined (_RWSTD_NO_FUNC)
#    define assert(expr)                                                   \
     ((expr) ? (void)0 : _RW::__rw_assert_fail (#expr, __FILE__, __LINE__, \
                                           __func__))
#  else   // if !__GNUG__ && _RWSTD_NO_FUNC
#    define assert(expr)   \
     ((expr) ? (void)0 : _RW::__rw_assert_fail (#expr, __FILE__, __LINE__, 0))
#  endif
#else   //  if !defined (NDEBUG)
#  define assert(ignore)   ((void)0)
#endif   // NDEBUG
