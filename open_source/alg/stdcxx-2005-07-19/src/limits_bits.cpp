/***************************************************************************
 *
 * limits_bits.cpp - source for the C++ Standard Library limits constants
 *
 * $Id: //stdlib/dev/source/stdlib/limits_bits.cpp#6 $
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

#define _RWSTD_LIB_SRC

#include <float.h>    // for INFINITY, NAN
#include <stdlib.h>   // for atof()
                       
#include <rw/_defs.h>  
                       

_RWSTD_NAMESPACE (__rw) { 

#ifndef _RWSTD_NO_INFINITY

#  if !defined (_RWSTD_DBL_INF_BITS)
#    error one of _RWSTD_NO_INFINITY and _RWSTD_DBL_INF_BITS must be #defined
#  endif

// infinity computed at config time
static const union {
    char _C_bits [sizeof (double)];
    double _C_val;
} __rw_dbl_inf_bits = { _RWSTD_DBL_INF_BITS };

_RWSTD_EXPORT extern const double __rw_dbl_infinity = __rw_dbl_inf_bits._C_val;

static const union {
    char _C_bits [sizeof (float)];
    float _C_val;
} __rw_flt_inf_bits = { _RWSTD_FLT_INF_BITS };

_RWSTD_EXPORT extern const float __rw_flt_infinity = __rw_flt_inf_bits._C_val;


#  ifndef _RWSTD_NO_LONG_DOUBLE

static const union {
    char _C_bits [sizeof (long double)];
    long double _C_val;
} __rw_ldbl_inf_bits = { _RWSTD_LDBL_INF_BITS };


_RWSTD_EXPORT extern const long double
__rw_ldbl_infinity = __rw_ldbl_inf_bits._C_val;

#  endif   // _RWSTD_NO_LONG_DOUBLE


// quiet NaN computed at config time
static const union {
    char _C_bits [sizeof (double)];
    double _C_val;
} __rw_dbl_qNaN_bits = { _RWSTD_DBL_QNAN_BITS };

_RWSTD_EXPORT extern const double __rw_dbl_qNaN  = __rw_dbl_qNaN_bits._C_val;

static const union {
    char _C_bits [sizeof (float)];
    float _C_val;
} __rw_flt_qNaN_bits = { _RWSTD_FLT_QNAN_BITS };

_RWSTD_EXPORT extern const float __rw_flt_qNaN  = __rw_flt_qNaN_bits._C_val;


#  ifndef _RWSTD_NO_LONG_DOUBLE

static const union {
    char _C_bits [sizeof (long double)];
    long double _C_val;
} __rw_ldbl_qNaN_bits = { _RWSTD_LDBL_QNAN_BITS };


_RWSTD_EXPORT extern const long double
__rw_ldbl_qNaN = __rw_ldbl_qNaN_bits._C_val;

#  endif   // _RWSTD_NO_LONG_DOUBLE


// signaling NaN computed at config time
static const union {
    char _C_bits [sizeof (double)];
    double _C_val;
} __rw_dbl_sNaN_bits = { _RWSTD_DBL_SNAN_BITS };

_RWSTD_EXPORT extern const double __rw_dbl_sNaN  = __rw_dbl_sNaN_bits._C_val;

static const union {
    char _C_bits [sizeof (float)];
    float _C_val;
} __rw_flt_sNaN_bits = { _RWSTD_FLT_SNAN_BITS };

_RWSTD_EXPORT extern const float __rw_flt_sNaN  = __rw_flt_sNaN_bits._C_val;


#  ifndef _RWSTD_NO_LONG_DOUBLE

static const union {
    char _C_bits [sizeof (long double)];
    long double _C_val;
} __rw_ldbl_sNaN_bits = { _RWSTD_LDBL_SNAN_BITS };


_RWSTD_EXPORT extern const long double
__rw_ldbl_sNaN = __rw_ldbl_sNaN_bits._C_val;

#  endif   // _RWSTD_NO_LONG_DOUBLE


// denormalized minima computed at config time
static const union {
    char _C_bits [sizeof (double)];
    double _C_val;
} __rw_dbl_denorm_min_bits = { _RWSTD_DBL_DENORM_MIN_BITS };

_RWSTD_EXPORT extern const double
__rw_dbl_denorm_min = __rw_dbl_denorm_min_bits._C_val;

static const union {
    char _C_bits [sizeof (float)];
    float _C_val;
} __rw_flt_denorm_min_bits = { _RWSTD_FLT_DENORM_MIN_BITS };

_RWSTD_EXPORT extern const float
__rw_flt_denorm_min = __rw_flt_denorm_min_bits._C_val;


#  ifndef _RWSTD_NO_LONG_DOUBLE

static const union {
    char _C_bits [sizeof (long double)];
    long double _C_val;
} __rw_ldbl_denorm_min_bits = { _RWSTD_LDBL_DENORM_MIN_BITS };


_RWSTD_EXPORT extern const long double
__rw_ldbl_denorm_min = __rw_ldbl_denorm_min_bits._C_val;

#  endif   // _RWSTD_NO_LONG_DOUBLE

#else   // if defined (_RWSTD_NO_INFINITY)

// as the last resort compute values at dynamic initialization time
_RWSTD_EXPORT extern const float __rw_flt_infinity =

#  if defined (INFINITY)

    INFINITY;

#  elif defined (FLT_INFINITY)

    FLT_INFINITY;

#  elif defined (__osf__)

    0.0f;   // prevent SIGFPE at startup (PR #23163)

#  else   // if !defined (__osf__)

_RWSTD_STATIC_CAST (float, atof ("INF"));

#  endif   // __osf__

_RWSTD_EXPORT extern const double __rw_dbl_infinity = __rw_flt_infinity;

#  ifndef _RWSTD_NO_LONG_DOUBLE

_RWSTD_EXPORT extern const long double __rw_ldbl_infinity = __rw_flt_infinity;

#  endif   // _RWSTD_NO_LONG_DOUBLE

_RWSTD_EXPORT extern const float __rw_flt_qNaN =

#  if defined (NAN)

    NAN;

#  elif defined (FLT_QNAN)

    FLT_QNAN;

#  elif defined (__osf__)

    0.0;   // prevent SIGFPE at startup (PR #23163)

#  else   // if !defined (__osf__)

_RWSTD_STATIC_CAST (float, atof ("NAN"));

#  endif   // __osf__

_RWSTD_EXPORT extern const float __rw_flt_sNaN =

#  if defined (FLT_SNAN)
    FLT_SNAN;
#  else   // if !defined (FLT_SNAN)
    __rw_flt_qNaN;
#  endif   // FLT_SNAN

_RWSTD_EXPORT extern const double __rw_dbl_qNaN = __rw_flt_qNaN;
_RWSTD_EXPORT extern const double __rw_dbl_sNaN = __rw_flt_sNaN;

_RWSTD_EXPORT extern const float __rw_flt_denorm_min = 0;
_RWSTD_EXPORT extern const float __rw_dbl_denorm_min = 0;

#  ifndef _RWSTD_NO_LONG_DOUBLE

_RWSTD_EXPORT extern const long double __rw_ldbl_qNaN = __rw_dbl_qNaN;
_RWSTD_EXPORT extern const long double __rw_ldbl_sNaN = __rw_dbl_sNaN;

_RWSTD_EXPORT extern const long double __rw_ldbl_denorm_min = 0;

#  endif   // _RWSTD_NO_LONG_DOUBLE

#endif   // _RWSTD_NO_INFINITY


}   // namespace __rw
