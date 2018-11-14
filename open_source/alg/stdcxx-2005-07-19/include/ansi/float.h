/***************************************************************************
 *
 * float.h - C++ Standard library wrapper for the ANSI C library header
 *
 * $Id: //stdlib/dev/include/ansi/float.h#12 $
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
#  include <cfloat>
#else
#  include _RWSTD_ANSI_C_FLOAT_H

#  if defined (__EDG__) && defined (__GLIBC__)

     // avoid relying on dubious gcc extensions
#    undef LDBL_MIN
#    undef LDBL_MAX
#    define LDBL_MIN _RWSTD_LDBL_MIN
#    define LDBL_MAX _RWSTD_LDBL_MAX

#  endif   // __EDG__ && __GLIBC__

#endif   // _RWSTD_NO_DEPRECATED_C_HEADERS
