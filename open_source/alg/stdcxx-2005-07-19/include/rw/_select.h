/***************************************************************************
 *
 * _select.h - Definitions of helper templates
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/include/rw/_select.h#8 $
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

#ifndef _RWSTD_SELECT_H_INCLUDED
#define _RWSTD_SELECT_H_INCLUDED


#include <rw/_defs.h>


_RWSTD_NAMESPACE (__rw) {

struct __rw_false_t { enum { _C_val }; };
struct __rw_true_t { enum { _C_val = 1 }; };


template <class _TypeT>
struct __rw_select_int
{
    typedef void* _SelectT;

#if defined (__SUNPRO_CC) && __SUNPRO_CC <= 0x530

    void* _C_selector;

#endif   // SunPro
};


#define _RWSTD_SPECIALIZE_IS_INT(T)    \
    _RWSTD_SPECIALIZED_CLASS           \
    struct __rw_select_int<T> {        \
        typedef int _SelectT;          \
        enum { _C_selector = 1 };      \
    }

#ifndef _RWSTD_NO_NATIVE_BOOL
_RWSTD_SPECIALIZE_IS_INT (bool);
#endif   // _RWSTD_NO_NATIVE_BOOL

_RWSTD_SPECIALIZE_IS_INT (char);
_RWSTD_SPECIALIZE_IS_INT (signed char);
_RWSTD_SPECIALIZE_IS_INT (unsigned char);
_RWSTD_SPECIALIZE_IS_INT (short);
_RWSTD_SPECIALIZE_IS_INT (unsigned short);
_RWSTD_SPECIALIZE_IS_INT (int);
_RWSTD_SPECIALIZE_IS_INT (unsigned int);
_RWSTD_SPECIALIZE_IS_INT (long);
_RWSTD_SPECIALIZE_IS_INT (unsigned long);

#ifdef _RWSTD_LONG_LONG
_RWSTD_SPECIALIZE_IS_INT (_RWSTD_LONG_LONG);
_RWSTD_SPECIALIZE_IS_INT (unsigned _RWSTD_LONG_LONG);
#endif   // _RWSTD_LONG_LONG

#ifndef _RWSTD_NO_NATIVE_WCHAR_T
_RWSTD_SPECIALIZE_IS_INT (wchar_t);
#endif   // _RWSTD_NO_NATIVE_WCHAR_T


#if !defined (__SUNPRO_CC) || __SUNPRO_CC > 0x530
#  define _RWSTD_DISPATCH(iter)   \
       (_TYPENAME _RW::__rw_select_int< iter >::_SelectT (1))

#else
   // working around a SunPro 5.3 a prior bug (PR #28739)
#  define _RWSTD_DISPATCH(iter) _RW::__rw_select_int< iter >()._C_selector
#endif


template <class _TypeT, class _TypeU>
struct __rw_is_same
{
    typedef __rw_false_t _C_type;
    enum { _C_val };
};

#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

template <class _TypeT>
struct __rw_is_same<_TypeT, _TypeT>
{
    typedef __rw_true_t _C_type;
    enum { _C_val = 1 };
};

#else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC)

_RWSTD_SPECIALIZED_CLASS
struct __rw_is_same<char, char>
{
    typedef __rw_true_t _C_type;
    enum { _C_val = 1 };
};

#  ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_CLASS
struct __rw_is_same<wchar_t, wchar_t>
{
    typedef __rw_true_t _C_type;
    enum { _C_val = 1 };
};

#  endif   // _RWSTD_NO_WCHAR_T
#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


}   // namespace __rw


#  if defined (__IBMCPP__) && __IBMCPP__ <= 500

// working around an xlC 5.0 bug
_USING (__rw::__rw_select_int);

#  endif   // VisualAge 5.0


#endif   // _RWSTD_SELECT_H_INCLUDED
