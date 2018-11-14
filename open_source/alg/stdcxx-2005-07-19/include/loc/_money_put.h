/***************************************************************************
 *
 * _money_put.h - definition of the std::num_put class templates
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/include/loc/_money_put.h#25 $
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

#ifndef _RWSTD_LOC_MONEY_PUT_H_INCLUDED
#define _RWSTD_LOC_MONEY_PUT_H_INCLUDED


#if __GNUG__ >= 3
#  pragma GCC system_header
#endif   // gcc >= 3


#include <string>
#include <rw/_ioiter.h>
#include <loc/_facet.h>


_RWSTD_NAMESPACE (std) { 

struct _RWSTD_EXPORT ios_base;

}   // namespace std


_RWSTD_NAMESPACE (_V3_LOCALE) { 

// 22.2.6.2
_EXPORT
template <class _CharT,
    class _OutputIter _RWSTD_COMPLEX_DEFAULT (ostreambuf_iterator<_CharT>) >
struct money_put: _RW::__rw_facet
{
    typedef _CharT      char_type;
    typedef _OutputIter iter_type;

    typedef
    basic_string<char_type, char_traits<char_type>, allocator<char_type> >
    string_type;

    _EXPLICIT money_put (_RWSTD_SIZE_T __ref = 0)
        : _RW::__rw_facet (__ref) { }

    // 22.2.6.2.1, p1
    iter_type put (iter_type __it, bool __intl, ios_base &__flags,
                   char_type __fill, long double __val) const {
        return do_put (__it, __intl, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, bool __intl, ios_base &__flags,
                   char_type __fill, const string_type &__val) const {
        return do_put (__it, __intl, __flags, __fill, __val);
    }

    static _RW::__rw_facet_id id;

protected:

    // 22.2.6.2.2, p1
    virtual iter_type
    do_put (iter_type, bool, ios_base&, char_type, long double) const;

    virtual iter_type
    do_put (iter_type, bool, ios_base&, char_type, const string_type&) const;

private:

    enum { _C_intl = 0x1, _C_ldbl = 0x2 };

    iter_type _C_put (iter_type, int, ios_base&, char_type,
                      const char_type*, _RWSTD_SIZE_T, int,
                      const char*, _RWSTD_SIZE_T) const;
};


#ifndef _RWSTD_NO_SPECIALIZED_FACET_ID

_RWSTD_SPECIALIZED_CLASS
_RW::__rw_facet_id money_put<char, _RWSTD_OSTREAMBUF_ITER (char) >::id;

#  ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_CLASS
_RW::__rw_facet_id money_put<wchar_t, _RWSTD_OSTREAMBUF_ITER (wchar_t) >::id;

#  endif   // _RWSTD_NO_WCHAR_T
#endif   // _RWSTD_NO_SPECIALIZED_FACET_ID

}   // namespace _V3_LOCALE


#if _RWSTD_DEFINE_TEMPLATE_FIRST (_MONEY_PUT)
#  include <loc/_money_put.cc>
#endif   // _RWSTD_DEFINE_TEMPLATE_FIRST (_MONEY_PUT)


_RWSTD_NAMESPACE (_V3_LOCALE) { 

#if _RWSTD_INSTANTIATE (_MONEY_PUT, _CHAR)

_RWSTD_INSTANTIATE_2 (struct _RWSTD_EXPORT
                      money_put<char, _RWSTD_OSTREAMBUF_ITER (char) >);

#endif   // _RWSTD_INSTANTIATE (_MONEY_PUT, _CHAR)

#if _RWSTD_INSTANTIATE (_MONEY_PUT, _WCHAR_T)

_RWSTD_INSTANTIATE_2 (struct _RWSTD_EXPORT
                      money_put<wchar_t, _RWSTD_OSTREAMBUF_ITER (wchar_t) >);

#endif   // _RWSTD_INSTANTIATE (_MONEY_PUT, _WCHAR_T)

}   // namespace _V3_LOCALE


#if _RWSTD_DEFINE_TEMPLATE_LAST (_MONEY_PUT)
#  include <loc/_money_put.cc>
#endif   // _RWSTD_DEFINE_TEMPLATE_LAST (_MONEY_PUT)


#endif   // _RWSTD_LOC_MONEY_PUT_H_INCLUDED
