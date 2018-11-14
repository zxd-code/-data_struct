// -*- C++ -*-
/***************************************************************************
 *
 * _rawiter - declarations for the C++ Standard Library raw_storage_iterator
 *
 * $Id: //stdlib/dev/include/rw/_rawiter.h#11 $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
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

#ifndef _RWSTD_RAWITER_H_INCLUDED
#define _RWSTD_RAWITER_H_INCLUDED

#include <rw/_iterbase.h>
#include <rw/_new.h>
#include <rw/_pair.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE (std) {


// 20.4.2
template <class _OutputIterator, class _TypeT>
class raw_storage_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
{
    _OutputIterator _C_iter;
public:

    // for completeness and genericity
    typedef _OutputIterator iterator_type;

    // 20.4.2, p2
    _EXPLICIT raw_storage_iterator (iterator_type __x): _C_iter (__x) { }

    // 20.4.2, p3
    raw_storage_iterator& operator* () {
        return *this;
    }

    // 20.4.2, p4
    raw_storage_iterator& operator= (const _TypeT& __rhs) {
        ::new (&(*_C_iter)) _TypeT (__rhs);
        return *this;
    }

    // 20.4.2, p6
    raw_storage_iterator& operator++ () {
        return ++_C_iter, *this;
    }

    // 20.4.2, p7
    raw_storage_iterator operator++ (int) {
        const raw_storage_iterator __tmp = *this;
        return ++*this, __tmp;
    }
};


}   // namespace std


_RWSTD_NAMESPACE (__rw) {

extern "C" {

// [de]allocates a previously allocated temporary buffer
// the constant _RWSTD_TMPBUF_SIZE controls the size of a static buffer
// if request for area larger than _RWSTD_TMPBUF_SIZE comes in, space
// is allocated dynamically, otherwise the static buffer is used
// return value meaningful only when allocating
_RWSTD_EXPORT _RWSTD_SIZE_T
__rw_tmpbuf (void**, _RWSTD_PTRDIFF_T, _RWSTD_SIZE_T);

}

}   // namespace __rw


_RWSTD_NAMESPACE (std) {


// 20.4.3 only specifies a get_temporary_buffer<>() that takes a ptrdiff_t.
// We overload on all types so that signed integral types other than ptrdiff_t
// can be used. This is important in getting algorithms to compile with
// user-defined iterators (not derived from iterator<...>) whose difference
// type is something other than ptrdiff_t.

// having this overload is important in some cases for compilers that
// do not support partial class specialization (and where as a consequence
// iterator_traits<> isn't available)
template <class _TypeT, class _Distance>
inline pair<_TypeT*, _Distance>
get_temporary_buffer (_Distance __nelems, _TypeT*)
{
    pair<_TypeT*, _Distance> __res (0, 0);

    __res.second =
        _RW::__rw_tmpbuf (_RWSTD_REINTERPRET_CAST (void**, &__res.first),
                          __nelems < 0 ? 0 : __nelems, sizeof (_TypeT));

    return __res;
}


#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

// 20.4.3, p1
template <class _TypeT>
inline pair<_TypeT*, _RWSTD_PTRDIFF_T>
get_temporary_buffer (_RWSTD_PTRDIFF_T __n)
{
    return get_temporary_buffer (__n, (_TypeT*)0);
}

#endif   // _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE


// 20.4.3, p3
template <class _TypeT>
inline void
return_temporary_buffer (_TypeT *__p)
{
    _RW::__rw_tmpbuf (_RWSTD_REINTERPRET_CAST (void**, &__p),
                      0, sizeof (_TypeT));
}


}   // namespace std


#endif   // _RWSTD_MEMORY_INCLUDED
