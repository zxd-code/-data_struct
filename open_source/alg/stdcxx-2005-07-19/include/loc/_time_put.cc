/***************************************************************************
 *
 * _time_put.cc - definition of std::time_put members
 *
 * $Id: //stdlib/dev/include/loc/_time_put.cc#21 $
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

#include <streambuf>
#include <rw/_iosbase.h>
#include <loc/_ctype.h>


_RWSTD_NAMESPACE (__rw) {


template <class _CharT>
_CharT* __rw_put_time (const __rw_facet*, _CharT*, _RWSTD_SIZE_T,
                       _STD::ios_base&, _CharT, const _RWSTD_C::tm*,
                       char, char, int, int)
{
    _RWSTD_ASSERT (!"not implemented");

    return 0;
}


_RWSTD_SPECIALIZED_FUNCTION
char* __rw_put_time (const __rw_facet*, char*, _RWSTD_SIZE_T,
                     _STD::ios_base&, char, const _RWSTD_C::tm*,
                     char, char, int, int);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_FUNCTION
wchar_t* __rw_put_time (const __rw_facet*, wchar_t*, _RWSTD_SIZE_T,
                        _STD::ios_base&, wchar_t, const _RWSTD_C::tm*,
                        char, char, int, int);

#endif   // _RWSTD_NO_WCHAR_T

}   // namespace __rw


_RWSTD_NAMESPACE (_V3_LOCALE) { 


template <class _CharT, class _OutputIter>
_RW::__rw_facet_id time_put<_CharT, _OutputIter>::id;


template <class _CharT, class _OutputIter /* = ostreambuf_iterator<_CharT> */ >
/* virtual */ _TYPENAME time_put<_CharT, _OutputIter>::iter_type
time_put<_CharT, _OutputIter>::
do_put (iter_type __it, ios_base &__flags, char_type __fill,
        const tm *__tmb, char __fmt, char __modifier /* = 0 */) const
{
    char_type __buf [256];

    const char_type *__end =
        _RW::__rw_put_time (this, __buf + 0, sizeof __buf / sizeof *__buf,
                            __flags, __fill, __tmb, __fmt, __modifier, -1, -1);

    // number of fill chars to pad with
    streamsize __pad = __flags.width () - streamsize (__end - __buf);

    // adjustfield bits (left, internal, or the default right)
    const int __adj = __flags.flags () & _RWSTD_IOS_ADJUSTFIELD;

    if (__pad > 0 && _RWSTD_IOS_LEFT != __adj) {

        // right justify unless left justification requested
        for (; __pad--; ++__it)
            *__it = __fill;
    }

    for (const char_type *__p = __buf; __p != __end; ++__p, ++__it) {
        *__it = *__p;
    }

    // left justify if necessary
    for (; __pad-- > 0; ++__it)
        *__it = __fill;

    return __it;
}


template <class _CharT, class _OutputIter /* = ostreambuf_iterator<_CharT> */ >
_TYPENAME time_put<_CharT, _OutputIter>::iter_type
time_put<_CharT, _OutputIter>::
put (iter_type __it, ios_base &__flags, char_type __fill, const tm *__tmb,
     const char_type *__pat, const char_type *__end) const
{
    _RWSTD_ASSERT (0 != __pat);

    if (!__end)   // convenience extension
        __end = __pat + char_traits<char_type>::length (__pat);

    const ctype<char_type> &__ctp = 
      _V3_USE_FACET (ctype<char_type>, __flags.getloc ());
    
    for (; __pat != __end; ++__pat) {

        if ('%' == __ctp.narrow (*__pat, 0) && __pat + 1 < __end) {

            char __fmtmod = __ctp.narrow (*++__pat, 0);   // format modifier
            char __fmt;                                   // format specifier

            if ('E' == __fmtmod) {

                __fmt = __ctp.narrow (__pat [1], 0);

                switch (__fmt) {
                    // check valid format modifiers
                case 'c': case 'C': case 'x': case 'X': case 'y': case 'Y':
                    ++__pat;
                    break;

                case '\0':
                    break;

                default:
                    __fmt = 0;   // `pat' doesn't point to a valid format
                }
            }
            else if ('O' == __fmtmod) {

                __fmt = __ctp.narrow (__pat [1], 0);

                switch (__fmt) {
                    // check valid format modifiers
                case 'd': case 'e': case 'H': case 'I': case 'm': case 'M':
                case 'S': case 'u': case 'U': case 'V': case 'w': case 'W':
                case 'y':
                    ++__pat;
                    break;

                case '\0':
                    break;

                default:
                    __fmt = 0;   // `pat' doesn't point to a valid format
                }
            }
            else {
                __fmt    = __fmtmod;
                __fmtmod = 0;
            }

            if (char (-1) != __fmt) {
                __it = do_put (__it, __flags, __fill, __tmb, __fmt, __fmtmod);
                continue;
            }
        }

        *__it = *__pat;
        ++__it;
    }

    return __it;
}


}   // namespace _V3_LOCALE
