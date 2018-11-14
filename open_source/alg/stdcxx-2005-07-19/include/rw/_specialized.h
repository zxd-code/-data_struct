// -*- C++ -*-
/***************************************************************************
 *
 * _specialized.h - definitions of specialized algorithms
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/include/rw/_specialized.h#13 $
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

#ifndef _RWSTD_SPECIALIZED_H_INCLUDED
#define _RWSTD_SPECIALIZED_H_INCLUDED


#include <rw/_new.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE (__rw) { 


#ifndef _RWSTD_NO_NONDEDUCED_CONTEXT
#  define _RWSTD_CONTAINER_SIZE_TYPE _TYPENAME _Container::size_type
#else
#  define _RWSTD_CONTAINER_SIZE_TYPE _RWSTD_SIZE_T
#endif   // _RWSTD_NO_NONDEDUCED_CONTEXT


// returns a suggested new capacity for a container needing more space
template <class _Container>
inline _RWSTD_CONTAINER_SIZE_TYPE
__rw_new_capacity (_RWSTD_CONTAINER_SIZE_TYPE __size, const _Container*)
{
    typedef _RWSTD_CONTAINER_SIZE_TYPE _SizeT;

    _SizeT __cap = _RWSTD_STATIC_CAST (_SizeT,
                                       __size * _RWSTD_NEW_CAPACITY_RATIO);
    return (__size += _RWSTD_MINIMUM_NEW_CAPACITY) > __cap ? __size : __cap;
}                     

#undef _RWSTD_CONTAINER_SIZE_TYPE


template <class _TypeT>
inline void __rw_destroy (_TypeT &__ref)
{
    __ref.~_TypeT ();
}


template <class _TypeT, class _TypeU>
inline void __rw_construct (_TypeT* __p, const _TypeU& __val)
{
    ::new (__p) _TypeT (__val);
}


template <class _ForwardIterator> 
inline void __rw_destroy (_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        __rw_destroy (*__first);
}


#ifndef _RWSTD_NO_PTR_VALUE_TEMPLATE_OVERLOAD

// for compilers that don't optimize "empty" loops
template <class _TypeT> 
inline void __rw_destroy (_TypeT**, _TypeT**)
{ }

#endif   // _RWSTD_NO_PTR_VALUE_TEMPLATE_OVERLOAD


}   // namespace __rw


_RWSTD_NAMESPACE (std) { 

template <class _TypeT> class
allocator;


// 20.4.4.1
template <class _InputIterator, class _ForwardIterator>
inline _ForwardIterator
uninitialized_copy (_InputIterator __first, _InputIterator __last,
                    _ForwardIterator __res)
{
    const _ForwardIterator __start = __res;

    _TRY {
        for (; __first != __last; ++__first, ++__res)
            _RW::__rw_construct (&*__res, *__first);
    }
    _CATCH (...) {
        _RW::__rw_destroy (__start, __res);
        _RETHROW;
    }

    return __res;
}


#ifdef _RWSTD_ALLOCATOR

// extension
template <class _InputIterator, class _ForwardIterator, class _Allocator>
inline _ForwardIterator
uninitialized_copy (_InputIterator __first, _InputIterator __last,
                    _ForwardIterator __res, _Allocator &__alloc)
{
    _ForwardIterator __start = __res;

    _TRY {
        for (; __first != __last; ++__first, ++__res)
            __alloc.construct (__alloc.address (*__res), *__first);
    }
    _CATCH (...) {
        for (; __start != __res; ++__start)
            __alloc.destroy (__alloc.address (*__start));
        _RETHROW;
    }

    return __res;
}

#endif   // _RWSTD_ALLOCATOR


// 20.4.4.2
template <class _ForwardIterator, class _TypeT>
inline void
uninitialized_fill (_ForwardIterator __first, _ForwardIterator __last,
                    const _TypeT& __x)
{
    const _ForwardIterator __start = __first;

    _TRY {
        for (; __first != __last; ++__first)
            _RW::__rw_construct (&*__first, __x);
    }
    _CATCH (...) {
        _RW::__rw_destroy (__start, __first);
        _RETHROW;
    }
}


// 20.4.4.3
template <class _ForwardIterator, class _Size, class _TypeT>
inline void
uninitialized_fill_n (_ForwardIterator __first, _Size __n, const _TypeT& __x)
{
    const _ForwardIterator __start = __first;

    _TRY {
        for (; __n; --__n, ++__first)
            _RW::__rw_construct (&*__first, __x);
    }
    _CATCH (...) {
        _RW::__rw_destroy (__start, __first);
        _RETHROW;
    }
}


#ifdef _RWSTD_ALLOCATOR

// extension
template <class _ForwardIter, class _Size, class _TypeT, class _Allocator>
inline void
uninitialized_fill_n (_ForwardIter __first, _Size __n,
                      const _TypeT& __x, _Allocator& __alloc)
{
    _ForwardIter __start = __first;

    _TRY {
        for (; __n; --__n, ++__first)
            __alloc.construct (__alloc.address (*__first), __x);
    }
    _CATCH (...) {
        for (; __start != __first; ++__start)
            __alloc.destroy (__alloc.address (*__start));
        _RETHROW;
    }
}

#else   // if !defined (_RWSTD_ALLOCATOR)


template <class _Allocator, class _TypeT>
class allocator_interface;

// Specializations for non-standard allocators.  When vector calls
// uninitialized_{copy,fill_n} with non-standard allocator, a temporary
// instance of allocator_interface is passed to these functions.  Since
// C++ forbids temporaries to be passed as non-const references, we
// use these specializations to pass a const reference (and we can force
// allocator_interface members construct & destroy to be const).

template <class _InputIterator, class _ForwardIterator,
          class _Allocator, class _TypeT>
inline _ForwardIterator
uninitialized_copy (_InputIterator   __first,
                    _InputIterator   __last,
                    _ForwardIterator __res,
                    allocator_interface<_Allocator, _TypeT> &__alloc)
{
    _ForwardIterator __start = __res;

    _TRY {
        for (; __first != __last; ++__first, ++__res)
            __alloc.construct (__alloc.address (*__res), *__first);
    }
    _CATCH (...) {
        for (; __start != __res; ++__start)
            __alloc.destroy (__alloc.address (*__start));
        _RETHROW;
    }

    return __res;
}

template <class _ForwardIter, class _Size,
          class _TypeT, class _Allocator, class _TypeU>
inline void
uninitialized_fill_n (_ForwardIter __first, _Size __n,
                      const _TypeT& __x,
                      allocator_interface<_Allocator, _TypeU> &__alloc)
{
    _ForwardIter __start = __first;

    _TRY {
        for (; __n; --__n, ++__first)
            __alloc.construct (__alloc.address (*__first), __x);
    }
    _CATCH (...) {
        for (; __start != __first; ++__start)
            __alloc.destroy (__alloc.address (*__start));
        _RETHROW;
    }
}

#endif   // _RWSTD_ALLOCATOR

}   // namespace std


#endif   // _RWSTD_SPECIALIZED_H_INCLUDED
