/***************************************************************************
 *
 * valaray.cc - Declarations for the Standard Library valarray
 *
 * $Id: //stdlib/dev/include/valarray.cc#12 $
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


_RWSTD_NAMESPACE (std) { 


template <class _TypeT>
valarray<_TypeT> valarray<_TypeT>::shift (int __n) const
{
    if (0 == __n)
        return *this;

    if (size () <= (_RWSTD_SIZE_T)(__n < 0 ? -__n : __n))
        return valarray (_TypeT (), size ());
        
    _RW::__rw_array <_TypeT> __tmp =
        _RW::__rw_array <_TypeT> (_TypeT (), size ());

    // 26.3.2.7, p5 - negative n shifts right, positive left
    if (__n < 0)
        copy (_C_array.begin (), _C_array.end () + __n, __tmp.begin () - __n);
    else
        copy (_C_array.begin () + __n, _C_array.end (), __tmp.begin ());

    return valarray (__tmp);
} 


template <class _TypeT>
valarray<_TypeT> valarray<_TypeT>::cshift (int __n) const
{
    // compute non-negative modulus - the sign of (a % b) is
    // implementation-defined if either argument is negative (5.6, p4)
    _RWSTD_PTRDIFF_T __mod = __n % _RWSTD_PTRDIFF_T (size ());
    _RWSTD_SIZE_T    __rem = __mod < 0 ? -__mod : __mod;

    if (0 == __rem)
        return *this;

    _RW::__rw_array <_TypeT> __tmp (size ());

    // 26.3.2.7, p7 - negative n rotates right, positive left
    rotate_copy (_C_array.begin (),
                 __n < 0 ? _C_array.end () - __rem : _C_array.begin () + __rem,
                 _C_array.end (), __tmp.begin ());

    return valarray (__tmp);
} 


}   // namespace std
