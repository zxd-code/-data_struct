/***************************************************************************
 *
 * strtol.h - declarations of __rw_strtol, __rw_strtoul, and other helpers
 *
 * $Id: //stdlib/dev/source/stdlib/strtol.h#2 $
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


_RWSTD_EXPORT extern const unsigned char
__rw_digit_map[];

unsigned long
 __rw_strtoul (const char*, int*, int);

long
 __rw_strtol (const char*, int*, int);


#ifdef _RWSTD_LONG_LONG

   // using LLONG_SIZE instead of ULLONG_MAX in the preprocessor
   // conditional below to work around a gcc 3.2 bug (PR #28595)
#  if (_RWSTD_LLONG_SIZE <= _RWSTD_LONG_SIZE)

inline unsigned _RWSTD_LONG_LONG
__rw_strtoull (const char *nptr, int *errptr, int base)
{
    _RWSTD_ASSERT (sizeof (_RWSTD_LONG_LONG) == sizeof (long));

    return __rw_strtoul (nptr, errptr, base);
}


inline _RWSTD_LONG_LONG
__rw_strtoll (const char *nptr, int *errptr, int base)
{
    _RWSTD_ASSERT (sizeof (_RWSTD_LONG_LONG) == sizeof (long));

    return __rw_strtol (nptr, errptr, base);
}

#  else   // if (_RWSTD_LLONG_SIZE > _RWSTD_LONG_SIZE)

unsigned _RWSTD_LONG_LONG
__rw_strtoull (const char*, int*, int);

_RWSTD_LONG_LONG
__rw_strtoll (const char*, int*, int);

#  endif   // _RWSTD_LLONG_SIZE <= _RWSTD_LONG_SIZE
#endif   // _RWSTD_LONG_LONG


}   // namespace __rw
