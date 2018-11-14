/***************************************************************************
 *
 * string.cc - Definitions for the Standard Library string classes
 *
 * $Id: //stdlib/dev/include/string.cc#63 $
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

#ifdef _MSC_VER
   // shut up the dumb MSVC warning C4345:
   // behavior change: an object with POD type constructed with
   // an initializer of the form () will be default-initialized
#  pragma warning (push)
#  pragma warning (disable: 4345)
#endif   // _MSC_VER


_RWSTD_NAMESPACE (std) {


#ifndef _RWSTD_NO_STATIC_CONST_MEMBER_INIT
#  if !defined (_MSC_VER) || _MSC_VER > 1300 || defined (__INTEL_COMPILER)
   // MSVC 7.0 allows initializers for static const integral members
   // but out-of-line definitions cause multiply defined symbol errors
   // (see PR #26562)

#    ifndef _RWSTD_NO_STRING_NPOS_TYPE

template <class _CharT, class _Traits, class _Allocator>
const _TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
npos /* = SIZE_MAX */;

#    else   // if defined (_RWSTD_NO_STRING_NPOS_TYPE)

template <class _CharT, class _Traits, class _Allocator>
const _RWSTD_SIZE_T
basic_string<_CharT, _Traits, _Allocator>::
npos /* = SIZE_MAX */;

#    endif   // _RWSTD_NO_STRING_NPOS_TYPE
#  endif   // MSVC > 7.0
#endif   // _RWSTD_NO_STATIC_CONST_MEMBER_INIT


#ifndef _RWSTD_NO_COLLAPSE_TEMPLATE_STATICS

template <class _CharT, class _Traits, class _Allocator>
_RW::__null_ref<_CharT, _Traits, _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
_C_null_ref;

#endif   // _RWSTD_NO_COLLAPSE_TEMPLATE_STATICS


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::_C_string_ref_type*
basic_string<_CharT, _Traits, _Allocator>::
_C_get_rep (size_type __cap, size_type __len)
{
    if (__cap > max_size ()) {
        __cap = __len;

        _RWSTD_REQUIRES (__cap <= max_size (),
                         (_RWSTD_ERROR_LENGTH_ERROR,
                          _RWSTD_FUNC ("basic_string::_C_get_rep(size_type, "
                                       "size_type)"), __cap, max_size ()));
    }

    _RWSTD_REQUIRES (__len <= __cap,
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::_C_get_rep(size_type, "
                                   "size_type)"), __len, __cap));

    if (!__cap) {
        _RWSTD_ASSERT (!__len);
        return _C_nullref ();
    }

    // allocate, initialize the __string_ref, and initialize each character
    _C_string_ref_type* const __ret =
    _RWSTD_REINTERPRET_CAST (_C_string_ref_type*,
        _RWSTD_VALUE_ALLOC (_C_value_alloc_type, *this,
            allocate (__cap + sizeof (_C_string_ref_type) /
                          sizeof (value_type) + 2)));

    // avoid copy construction (mutex isn't copy-constructible)
    // _C_ref_alloc_type (*this).construct (__ret, _C_string_ref_type ());
    new (__ret) _C_string_ref_type ();

    // set initial reference count, capacity, and length
    __ret->_C_init (__cap, __len);

    _RWSTD_VALUE_ALLOC (_C_value_alloc_type, *this,
                        construct (__ret->data () + __len, value_type ()));

    return __ret;
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
basic_string (const basic_string &__s, size_type __pos, size_type __n,
              const allocator_type& __alloc /* = allocator_type () */)
    : allocator_type (__alloc)
{
    _RWSTD_REQUIRES (__pos <= __s.size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE,
                      _RWSTD_FUNC ("basic_string::basic_string(const "
                                   "basic_string&, size_type, size_type)"),
                      __pos, __s.size ()));

    size_type __slen = __s.size () - __pos;
    size_type __rlen = __n < __slen ? __n : __slen; 
    size_type __nlen =  __n == npos ? 0 : __n;
    size_type __maxlen = __nlen > __rlen ? __nlen : __rlen; 
    if (__maxlen) 
      _C_data = _C_get_rep (_C_grow (size_type (), __maxlen), __rlen)->data ();
    else
      _C_data = _C_get_rep (__maxlen, __rlen)->data ();

    traits_type::copy (_C_data, &__s._C_data [__pos], __rlen);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
basic_string (size_type __n, value_type __c, 
              const allocator_type &__alloc /* = allocator_type () */)
    : allocator_type (__alloc)
{
    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::basic_string(size_type, "
                                   "value_type)"), __n, max_size ()));

    if (__n)
        _C_data = _C_get_rep (_C_grow (size_type (), __n), __n)->data ();
    else
        _C_data = _C_nullref ()->data ();

    traits_type::assign (_C_data, __n, __c);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
basic_string (const_pointer __s, size_type __n,
              const allocator_type &__alloc /* = allocator_type () */)
    : allocator_type (__alloc)
{
    // extension: if `s' is 0 then `n' uninitialized elements are allocated

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::basic_string(const_pointer,"
                                   "size_type, const allocator_type&)"),
                      __n, max_size ()));

    _C_data = __n ?
        _C_get_rep (_C_grow (size_type (), __n), __n)->data ()
      : _C_nullref ()->data ();

    if (__s)
        traits_type::copy (_C_data, __s, __n);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
basic_string (const_pointer __s,
              const allocator_type &__alloc /* = allocator_type () */)
    : allocator_type(__alloc)
{     
    _RWSTD_ASSERT (__s != 0);

    const size_type __n = traits_type::length (__s);

    _C_data = __n ?
        _C_get_rep (_C_grow (size_type (), __n), __n)->data ()
      : _C_nullref ()->data ();

    traits_type::copy (_C_data, __s, __n);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
basic_string (const_pointer __first, const_pointer __last, 
              const allocator_type &__alloc /* = allocator_type () */)
    : allocator_type (__alloc)
{
    const size_type __n = size_type (__last - __first);

    _C_data = __n ?
        _C_get_rep (_C_grow (size_type (), __n), __n)->data ()
      : _C_nullref ()->data ();

    traits_type::copy (_C_data, __first, __n);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
operator= (const basic_string &__rhs)
{
    if (__rhs._C_pref ()->_C_get_ref () > 0) {
        __rhs._C_pref ()->_C_inc_ref ();
        _C_unlink (__rhs._C_data);
    }
    else if (this != &__rhs)
        // `rhs' has reference counting disabled
        replace (size_type (), size (), __rhs.data (), __rhs.size ());

    return *this;
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
append (const basic_string &__str, size_type __pos, size_type __n)
{
    _RWSTD_REQUIRES (__pos <= __str.size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE,
                      _RWSTD_FUNC ("basic_string::append(const basic_string&,"
                                   " size_type, size_type)"),
                      __pos, __str.size ()));

    if (__n > __str.size () - __pos)
        __n = __str.size () - __pos;

    const size_type __rlen = _C_min (__str.size() - __pos, __n);

    _RWSTD_REQUIRES (size () <= max_size () - __rlen,
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::append(const basic_string&,"
                                   " size_type, size_type)"),
                      size (), max_size () - __rlen));

    replace (size (), size_type (), __str.c_str () + __pos, __n);

    return *this;
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
assign (const basic_string &__str, size_type __pos, size_type __n)
{
    _RWSTD_REQUIRES (__pos <= __str.size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE,
                      _RWSTD_FUNC ("basic_string::assign(basic_string&, "
                                   "size_type, size_type)"),
                      __pos, __str.size ()));

    const size_type __rlen = _C_min (__str.size () - __pos, __n);

    return replace (size_type (), size (), __str, __pos, __rlen);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
insert (size_type __pos1,
        const basic_string& __str, size_type __pos2, size_type __n)
{
    _RWSTD_REQUIRES (__pos1 <= size() && __pos2 <= __str.size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE,
                      _RWSTD_FUNC ("basic_string::insert(size_type, const "
                                   "basic_string&, size_type, size_type)"), 
                      __pos1 > size() ? __pos1:__pos2, __str.size ()));
    
    const size_type __rlen = _C_min (__str.size () - __pos2, __n);

    _RWSTD_REQUIRES (size () <= max_size () - __rlen,
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::insert(size_type, const "
                                   "basic_string&, size_type, size_type)"), 
                      size (), max_size () - __rlen));

    return replace (__pos1, size_type (), __str, __pos2, __n);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
insert (size_type __pos1, const basic_string &__str)
{
    _RWSTD_REQUIRES (__pos1 <= size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE,
                      _RWSTD_FUNC ("basic_string::insert(size_type, const "
                                   "basic_string&)"), __pos1, size ()));
    
    _RWSTD_REQUIRES (size() <= max_size() - __str.size(),
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::insert(size_type, "
                                   "const basic_string&)"),
                      size (), max_size () - __str.size ()));

    return replace (__pos1, size_type (), __str);
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
replace (size_type __pos1, size_type __n1, const_pointer __s, size_type __n2)
{
    const size_type __size0 = size ();

    if (npos == __n2)
        __n2 = traits_type::length (__s);

    _RWSTD_REQUIRES (__pos1 <= __size0,
                     (_RWSTD_ERROR_OUT_OF_RANGE, 
                     _RWSTD_FUNC ("basic_string::replace(size_type, size_type"
                                  ", const_pointer, size_type, size_type, "
                                  "size_type)"), 
                      __pos1, __size0 > __n2 ? __size0 : __n2));

    // number of characters to delete
    const size_type __xlen = _C_min (__n1, __size0 - __pos1);

    _RWSTD_REQUIRES (__size0 - __xlen <= max_size () - __n2,
                     (_RWSTD_ERROR_LENGTH_ERROR,
                     _RWSTD_FUNC ("basic_string::replace(size_type, size_type"
                                  ", const_pointer, size_type, size_type, "
                                  "size_type)"), 
                      __size0 - __xlen, max_size() - __n2));

    // compute the resulting string size
    const size_type __size1 = __size0 - __xlen + __n2;

    if (__size1) {

        // compute the length of the bit at the end
        const size_type __rem = __size0 - __xlen - __pos1;

        // check for shared representation, insufficient capacity,
        // and overlapping regions
        if (   _C_pref ()->_C_get_ref () > 1
            || capacity () < __size1
            || __s >= data () && __s < data () + __size0) {

            // Need to allocate a new reference.
            const size_type __cap = _C_grow (__size0, __size1);

            const pointer __data = _C_get_rep (__cap, __size1)->data ();

            traits_type::copy (__data, _C_data, __pos1);

            traits_type::copy (__data + __pos1, __s, __n2);

            traits_type::copy (__data + __pos1 + __n2,
                               _C_data + __pos1 + __n1, __rem);

            _C_unlink (__data);
        }
        else {
            // current reference has enough space

            const pointer __beg = _C_data + __pos1;

            traits_type::move (__beg + __n2, __beg + __n1, __rem);

            traits_type::move (__beg, __s, __n2);

            traits_type::assign (_C_data [__size1], value_type ());

            _C_pref ()->_C_size._C_size = __size1;
        }
    }
    else {
        // special case a substitution that leaves the string empty.
        _C_unlink (_C_nullref ()->data ());
    }

    return *this;
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>&
basic_string<_CharT, _Traits, _Allocator>::
replace (size_type __pos, size_type __n, size_type __n2, value_type __c)
{
    const size_type __size0 = size ();

    _RWSTD_REQUIRES (__pos <= __size0,
                     (_RWSTD_ERROR_OUT_OF_RANGE, 
                      _RWSTD_FUNC ("basic_string::replace(size_type, "
                                   "size_type, size_type, value_type)"),
                      __pos, __size0));

    const size_type __xlen = _C_min (__size0 - __pos, __n);

    _RWSTD_REQUIRES (__size0 - __xlen <= max_size () - __n2,
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::replace(size_type, "
                                   "size_type, size_type, value_type)"), 
                      __size0 - __xlen, max_size () - __n2));

    // compute the resulting string size
    const size_type __size1 = __size0 - __xlen + __n2;

    if (__size1) {

        // compute the length of the bit at the end
        const size_type __rem = __size0 - __xlen - __pos;

        // check for shared representation, insufficient capacity
        if (_C_pref ()->_C_get_ref () > 1 || capacity () < __size1) {

            // need to allocate a new reference
            const size_type __cap = _C_grow (__size0, __size1);

            const pointer __data = _C_get_rep (__cap, __size1)->data ();

            traits_type::copy (__data, _C_data, __pos);

            traits_type::assign (__data + __pos, __n2, __c);

            traits_type::copy (__data + __pos + __n2,
                               _C_data + __pos + __n, __rem);

            _C_unlink (__data);
        }
        else {
            // current reference has enough space

            const pointer __beg = _C_data + __pos;

            traits_type::move (__beg + __n2, __beg + __n, __rem);

            traits_type::assign (__beg, __n2, __c);

            traits_type::assign (_C_data [__size1], value_type ());

            _C_pref ()->_C_size._C_size = __size1;
        }
    }
    else {
        // construct the empty string
        _C_unlink (_C_nullref ()->data ());
    }

    return *this;
}


#if    !defined (_RWSTD_NO_INLINE_MEMBER_TEMPLATES) \
    && (!defined (_MSC_VER) || _MSC_VER >= 1300 || defined (__INTEL_COMPILER)) 

#  ifdef _RWSTD_NO_MEMBER_TEMPLATES

template<class _CharT, class _Traits, class _Alloc, class _StringIter, class _InputIter>
_STD::basic_string<_CharT, _Traits, _Alloc>& 
__rw_replace (_STD::basic_string<_CharT, _Traits, _Alloc> &__s, 
              _StringIter __first1, _StringIter __last1,
              _InputIter __first2, _InputIter __last2) {

    typedef _Traits                               traits_type;
    typedef _TYPENAME traits_type::char_type      value_type;
    typedef _Alloc                                allocator_type;
    typedef _TYPENAME allocator_type::size_type   size_type;

    typedef _RW::__string_ref<value_type, traits_type, allocator_type>
     _C_string_ref_type;


#  else   // !defined (_RWSTD_NO_MEMBER_TEMPLATES)

template<class _CharT, class _Traits, class _Allocator>
template<class _InputIter>
basic_string<_CharT, _Traits, _Allocator>& 
basic_string<_CharT, _Traits, _Allocator>::
replace (iterator __first1, iterator __last1,
         _InputIter __first2, _InputIter __last2, void*)
{
  basic_string &__s = *this;

#  endif   // _RWSTD_NO_MEMBER_TEMPLATES

    _RWSTD_ASSERT_RANGE (__first1, __s._C_make_iter (__s._C_data + __s.size ()));
    _RWSTD_ASSERT_RANGE (__first1, __last1);
    _RWSTD_ASSERT_RANGE (__first2, __last2);

     // use a (probably) faster algorithm if possible
     if (_STD::__is_bidirectional_iterator(_RWSTD_ITERATOR_CATEGORY(_InputIter,
                                                                   __last2)))
         return __s.__replace_aux (__first1, __last1, __first2, __last2);

     for ( ; !(__first2 == __last2); ++__first1, ++__first2) {

         size_type __off = __first1 - __s._C_make_iter (__s._C_data);

         _RWSTD_REQUIRES (__off <= __s.max_size(),
                          (_RWSTD_ERROR_LENGTH_ERROR,
                           _RWSTD_FUNC ("basic_string::replace(iterator, "
                                        "iterator, InputIterator, "
                                        "InputIterator)"),
                           __first1 - __s._C_make_iter (__s._C_data),
                           __s.max_size ()));
         
         // extend the string if necessary
         if (__first1 == __last1) {
             // compute the size of new buffer
             const size_type __cap = __s._C_grow (__s.size (), size_type ());

             const size_type __delta = __cap - __s.size ();

             // allocate a new buffer
             _C_string_ref_type *__tmp = __s._C_get_rep (__cap, __cap);

             // copy data from old to new, leaving a hole for additions
             traits_type::copy (__tmp->data (), __s._C_data, __off);
             traits_type::copy (__tmp->data () + __off + __delta,
                                __s._C_data + __off,
                                __s._C_make_iter (__s._C_data + __s.size ()) 
                                - __last1);
             __s._C_unlink (__tmp->data ());
             __first1 = __s._C_make_iter (__s._C_data + __off);
             __last1  = __first1 + __delta;
         }

         // copy data over
         traits_type::assign (*__first1, *__first2);
     }

     if (!(__first1 == __last1))
         __s.replace (__first1 - __s._C_make_iter (__s._C_data), 
                      __last1 - __first1,
                      size_type (), value_type ());

     return __s;
}

// Special function for random access and bi-directional iterators
// Avoids the possibility of multiple allocations
// We still have to copy characters over one at a time.

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

template<class _CharT, class _Traits, class _Alloc, class _StringIter, class _InputIter>
_STD::basic_string<_CharT, _Traits, _Alloc>& 
__rw_replace_aux (_STD::basic_string<_CharT, _Traits, _Alloc> &__s, 
                  _StringIter __first1, _StringIter __last1,
                  _InputIter __first2, _InputIter __last2)
{
    typedef _Traits                                   traits_type;
    typedef _TYPENAME traits_type::char_type          value_type;
    typedef _Alloc                                    allocator_type;
    typedef _TYPENAME allocator_type::size_type       size_type;
    typedef _TYPENAME allocator_type::difference_type difference_type;

    typedef _RW::__string_ref<value_type, traits_type, allocator_type>
     _C_string_ref_type;

#  else

template<class _CharT, class _Traits, class _Allocator>
template<class _InputIter>
basic_string<_CharT, _Traits, _Allocator>& 
basic_string<_CharT, _Traits, _Allocator>::
__replace_aux (iterator __first1, iterator __last1, 
               _InputIter __first2, _InputIter __last2)
{
  basic_string &__s = *this;

#  endif  // _RWSTD_NO_MEMBER_TEMPLATES

    _RWSTD_ASSERT_RANGE (__first1, __s._C_make_iter (__s._C_data 
                                                     + __s.size ()));
    _RWSTD_ASSERT_RANGE (__first1, __last1);
    _RWSTD_ASSERT_RANGE (__first2, __last2);

    difference_type __n2  = _DISTANCE (__first2, __last2, difference_type);
    size_type       __n   = __last1 - __first1;
    size_type       __pos = __first1 - __s._C_make_iter (__s._C_data);

    _RWSTD_REQUIRES (__pos <= __s.size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE,
                      _RWSTD_FUNC ("basic_string::__replace_aux(iterator, "
                                   "iterator, InputIterator, InputIterator)"),
                      __pos, __s.size ()));

    size_type __slen = __s.size() - __pos;
    size_type __xlen = __n < __slen ? __n : __slen; 

    _RWSTD_REQUIRES (__s.size () - __xlen <= __s.max_size() - __n2,
                     (_RWSTD_ERROR_LENGTH_ERROR, 
                      _RWSTD_FUNC ("basic_string::__replace_aux(iterator, "
                                   "iterator, InputIterator, InputIterator)"),
                      __s.size () - __xlen, __s.max_size () - __n2));

    size_type __len = __s.size() - __xlen + __n2;  // Final string length.

    if (!__len) {
        // Special case a substitution that leaves the string empty.
        __s._C_unlink (__s._C_nullref ()->data ());
    }
    else {
      size_type __d = 0;
      size_type __rem = __s.size() - __xlen - __pos; // length of bit at the end
      // Check for shared representation, insufficient capacity, 
      if ( (__s._C_pref()->_C_get_ref () > 1) || (__s.capacity() < __len))
      {
        // Need to allocate a new reference.
          const size_type __cap = __s._C_grow (__s.size (), __len);

        _C_string_ref_type * __temp = __s._C_get_rep (__cap, __len);
        if (__pos) traits_type::copy(__temp->data(), __s._C_data, __pos);
        for (__d = 0; __d < (size_type)__n2; __d++)
            traits_type::assign (*(__temp->data()+__pos+__d), *__first2++);
        if (__rem)
            traits_type::copy (__temp->data () + __pos + __n2,
                               __s._C_data + __pos + __n, __rem);
        __s._C_unlink (__temp->data ());
      }
      else
      {
        // Current reference has enough room.
        if (__rem)  
          traits_type::move(__s._C_data+__pos+__n2, __s._C_data+__pos+__n, 
                            __rem);
        for (__d = 0; __d < (size_type)__n2; __d++)
            traits_type::assign (*(__s._C_data+__pos+__d), *__first2++);
        traits_type::assign (__s._C_data[__s._C_pref()->_C_size._C_size 
                                         = __len], value_type());
      }
    }
    return __s;
}

#endif   //_RWSTD_NO_INLINE_MEMBER_TEMPLATES


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
copy (pointer __s, size_type __n, size_type __pos) const
{
    _RWSTD_REQUIRES (__pos <= size(),
                     (_RWSTD_ERROR_OUT_OF_RANGE, 
                      _RWSTD_FUNC ("basic_string::copy(pointer, size_type, "
                                   "size_type)"), __pos, size ()));

    const size_type __rlen = _C_min (size () - __pos, __n);

    traits_type::copy (__s, _C_data + __pos, __rlen);

    return __rlen;
}


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
find (const_pointer __s, size_type __pos, size_type __n) const
{
    _RWSTD_ASSERT (__s != 0);

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR, 
                      _RWSTD_FUNC ("basic_string::find(const_pointer, "
                                   "size_type, size_type) const"),
                      __n, max_size ()));

    for (size_type __xpos = __pos; __xpos + __n <= size (); ++__xpos) {
        if (!traits_type::compare (_C_data + __xpos, __s, __n))
            return __xpos;
    }

    return npos;
}


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
rfind (const_pointer __s,  size_type __pos, size_type __n) const
{
    _RWSTD_ASSERT(__s != 0);

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR, 
                      _RWSTD_FUNC ("basic_string::rfind(const_pointer, "
                                   "size_type, size_type) const"),
                      __n, max_size ()));

    if (size() < __n)
      return npos;
    
    size_type __slen = size() -__n;
    size_type __xpos_start = __slen < __pos ? __slen : __pos; 

    for (size_type __xpos = __xpos_start+1; __xpos != 0 ; __xpos--)
    {
      if (!traits_type::compare(_C_data+__xpos-1, __s, __n))
        return __xpos-1;
    }

    return npos;
}


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
find_first_of (const_pointer __s, size_type __pos, size_type __n) const
{
    _RWSTD_ASSERT(__s != 0);

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR,
                      _RWSTD_FUNC ("basic_string::find_first_of(const_pointer, "
                                   "size_type, size_type) const"),
                      __n, max_size ()));

    for (size_type __xpos = __pos; __xpos < size() ; __xpos++)
    {
      for (size_type __i = 0; __i < __n ; __i++)
        if (traits_type::eq(_C_data[__xpos], __s[__i]))
          return __xpos;
    }

    return npos;
}


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
find_last_of (const_pointer __s, size_type __pos, size_type __n) const
{
    _RWSTD_ASSERT(__s != 0);

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR, 
                      _RWSTD_FUNC ("basic_string::find_last_of(const_pointer, "
                                   "size_type, size_type) const"),
                      __n, max_size ()));

    if (size())
    {
      size_type __slen = size() -1;
      size_type __xpos_start = __pos < __slen ? __pos : __slen; 
      for (size_type __xpos = __xpos_start+1; __xpos != 0 ; __xpos--)
      {
        for(size_type __i = 0; __i < __n ; __i++)
          if (traits_type::eq(_C_data[__xpos-1], __s[__i]))
            return __xpos-1;
      } 
    }
    return npos;
}


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
find_first_not_of (const_pointer __s, size_type __pos, size_type __n) const
{
    _RWSTD_ASSERT(__s != 0);

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR, 
                      _RWSTD_FUNC ("basic_string::find_first_not_of("
                                  "const_pointer, size_type, size_type) const"),
                      __n, max_size ()));

    for (size_type __xpos = __pos; __xpos < size() ; __xpos++)
    {
      bool __found = false;
      for (size_type __i = 0; __i < __n ; __i++)
      {
        if (traits_type::eq(_C_data[__xpos], __s[__i]))
        {
          __found = true;
          break;
        }
      }
      if (!__found)
        return __xpos;
    }

    return npos;
}


template <class _CharT, class _Traits, class _Allocator>
_TYPENAME basic_string<_CharT, _Traits, _Allocator>::size_type
basic_string<_CharT, _Traits, _Allocator>::
find_last_not_of (const_pointer __s, size_type __pos, size_type __n) const
{
    _RWSTD_ASSERT(__s != 0);

    _RWSTD_REQUIRES (__n <= max_size (),
                     (_RWSTD_ERROR_LENGTH_ERROR, 
                      _RWSTD_FUNC ("basic_string::find_last_not_of("
                                  "const_pointer, size_type, size_type) const"),
                      __n, max_size ()));
    
    if (size())
    {
      size_type __slen = size() -1;
      size_type __xpos_start = __pos < __slen ? __pos : __slen; 
      for (size_type __xpos = __xpos_start+1; __xpos != 0 ; __xpos--)
      {
        bool __found = false;
        for (size_type __i = 0; __i < __n ; __i++)
        {
          if (traits_type::eq(_C_data[__xpos-1], __s[__i]))
          {
            __found = true;
            break;
          }
        }
        if (!__found)
          return __xpos-1;
      }
    }

    return npos;
}


template <class _CharT, class _Traits, class _Allocator>
basic_string<_CharT, _Traits, _Allocator>
basic_string<_CharT, _Traits, _Allocator>::
substr (size_type __pos, size_type __n) const
{
    _RWSTD_REQUIRES (__pos <= size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE, 
                      _RWSTD_FUNC ("basic_string::substr(size_type, size_type) "
                                   "const"), __pos, size ()));

    size_type __slen = size() -__pos;
    size_type __rlen = __n < __slen ? __n : __slen;
    return basic_string (_C_data + __pos, __rlen);
}


template <class _CharT, class _Traits, class _Allocator>
int basic_string<_CharT, _Traits, _Allocator>::
compare (size_type __pos1, size_type __n1, 
         const basic_string& __str, size_type __pos2, size_type __n2) const
{
    _RWSTD_REQUIRES (__pos2 <= __str.size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE, 
                      _RWSTD_FUNC ("basic_string::compare(size_type, size_type,"
                                   "const basic_string&, size_type, size_type) "
                                   "const"), __pos2, __str.size ()));

    if (__str.size () - __pos2 < __n2)
        __n2 = __str.size () - __pos2;

    return compare (__pos1, __n1, __str.c_str()+__pos2, __n2);
}


template <class _CharT, class _Traits, class _Allocator>
int basic_string<_CharT, _Traits, _Allocator>::
compare (size_type __pos, size_type __n1,
         const_pointer __s, size_type __n2) const
{
    _RWSTD_REQUIRES (__pos <= size (),
                     (_RWSTD_ERROR_OUT_OF_RANGE, 
                      _RWSTD_FUNC ("basic_string::compare(size_type, size_type,"
                                   " const const_pointer, size_type) const"),
                      __pos, size ()));

    if(size() - __pos < __n1)
      __n1 = size() - __pos;
    size_type __rlen = __n1 < __n2 ? __n1 : __n2; 
    int __res = traits_type::compare(_C_data+__pos,__s, __rlen);

    if (__res == 0)
      __res = (__n1 < __n2) ? -1 : (__n1 != __n2);

    return __res;
}


}   // namespace std


#ifdef _MSC_VER
#  pragma warning (pop)
#endif   // _MSC_VER
