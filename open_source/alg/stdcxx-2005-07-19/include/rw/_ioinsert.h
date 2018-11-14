// -*- C++ -*-
/***************************************************************************
 *
 * _ioinsert.h - declarations for the C++ Standard Library inserter
 *               helper templates
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/include/rw/_ioinsert.h#12 $
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

#ifndef _RWSTD_IOINSERT_INCLUDED
#define _RWSTD_IOINSERT_INCLUDED

#if __GNUG__ >= 3
#  pragma GCC system_header
#endif   // gcc >= 3

#ifndef _RWSTD_NO_REDUNDANT_DEFINITIONS
#  include <rw/_ioiter.h>
#  include <streambuf>
#endif   // _RWSTD_NO_REDUNDANT_DEFINITIONS

#include <loc/_num_get.h>
#include <loc/_num_put.h>

#include <rw/_basic_ios.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE (std) {


#ifndef _RWSTD_IOSFWD_INCLUDED

_EXPORT
template <class _CharT,
          class _Traits _RWSTD_COMPLEX_DEFAULT (char_traits<_CharT>) >
class basic_ostream;

typedef basic_ostream<char, char_traits<char> > ostream;

#  ifndef _RWSTD_NO_WCHAR_T

typedef basic_ostream<wchar_t, char_traits<wchar_t> > wostream;

#  endif   // _RWSTD_NO_WCHAR_T
#endif   // _RWSTD_IOSFWD_INCLUDED

}


_RWSTD_NAMESPACE (__rw) { 

// helper - implements insertion of arithmetic and pointer types
_EXPORT
template <class _CharT, class _Traits, class _NativeType>
_STD::basic_ostream<_CharT, _Traits>&
__rw_insert (_STD::basic_ostream<_CharT, _Traits>&, _NativeType);


// helper - implements insertion of character strigs
_EXPORT
template<class _CharT, class _Traits, class _StringT>
_STD::basic_ostream<_CharT, _Traits>&
__rw_insert (_STD::basic_ostream<_CharT, _Traits>&, const _StringT*,
             _RWSTD_STREAMSIZE, _RWSTD_STREAMSIZE);

}   // namespace __rw


#if _RWSTD_DEFINE_TEMPLATE_FIRST (_INSERTER)
#  include <rw/_ioinsert.cc>
#endif   // _RWSTD_DEFINE_TEMPLATE_FIRST (_INSERTER)


_RWSTD_NAMESPACE (__rw) { 


#if _RWSTD_INSTANTIATE (_INSERT_INT, _CHAR)

#  ifndef _RWSTD_NO_NATIVE_BOOL

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, bool));

#  endif   // _RWSTD_NO_NATIVE_BOOL

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, long));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, unsigned long));

#  ifdef _RWSTD_LONG_LONG

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, _RWSTD_LONG_LONG));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&,
                                       unsigned _RWSTD_LONG_LONG));

#  endif   // _RWSTD_LONG_LONG

#endif   // _RWSTD_INSTANTIATE (_INSERT_INT, _CHAR)


#if _RWSTD_INSTANTIATE (_INSERT_DBL, _CHAR)

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, double));

#  ifndef _RWSTD_NO_LONG_DOUBLE

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, long double));

#  endif   // _RWSTD_NO_LONG_DOUBLE

#endif    // _RWSTD_INSTANTIATE (_INSERT_DBL, _CHAR)


#if _RWSTD_INSTANTIATE (_INSERT_PTR, _CHAR)

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, const void*));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::ostream&
                          __rw_insert (_STD::ostream&, const char*,
                                       _RWSTD_STREAMSIZE, _RWSTD_STREAMSIZE));

#endif   // _RWSTD_INSTANTIATE (_INSERT_PTR, _CHAR)


#if _RWSTD_INSTANTIATE (_INSERT_INT, _WCHAR_T)

#  ifndef _RWSTD_NO_NATIVE_BOOL

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, bool));

#  endif   // _RWSTD_NO_NATIVE_BOOL

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, long));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, unsigned long));

#  ifdef _RWSTD_LONG_LONG

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, _RWSTD_LONG_LONG));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&,
                                       unsigned _RWSTD_LONG_LONG));

#  endif   // _RWSTD_LONG_LONG

#endif   // _RWSTD_INSTANTIATE (_INSERT_INT, _WCHAR_T)

#if _RWSTD_INSTANTIATE (_INSERT_DBL, _WCHAR_T)

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, double));

#    ifndef _RWSTD_NO_LONG_DOUBLE

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, long double));

#  endif   // _RWSTD_NO_LONG_DOUBLE

#endif   // _RWSTD_INSTANTIATE (_INSERT_DBL, _WCHAR_T)

#if _RWSTD_INSTANTIATE (_INSERT_PTR, _WCHAR_T)

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                      __rw_insert (_STD::wostream&, const void*));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, const char*,
                                       _RWSTD_STREAMSIZE, _RWSTD_STREAMSIZE));

_RWSTD_INSTANTIATE_FUN_1 (_RWSTD_EXPORT _STD::wostream&
                          __rw_insert (_STD::wostream&, const wchar_t*,
                                       _RWSTD_STREAMSIZE, _RWSTD_STREAMSIZE));

#endif   // _RWSTD_INSTANTIATE (_INSERT_PTR, _WCHAR_T)

}   // namespace __rw


#if _RWSTD_DEFINE_TEMPLATE_LAST (_INSERTER)
#  include <rw/_ioinsert.cc>
#endif   // _RWSTD_DEFINE_TEMPLATE_LAST (_INSERTER)


#endif   // _RWSTD_IOINSERT_INCLUDED
