// -*- C++ -*-
/***************************************************************************
 *
 * _relops.h - definition of the std::rel_ops namespace and members
 *
 * $Id: //stdlib/dev/include/rw/_relops.h#6 $
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

#ifndef _RWSTD_RELOPS_H_INCLUDED
#define _RWSTD_RELOPS_H_INCLUDED

#include <rw/_defs.h>


_RWSTD_NAMESPACE (std) { 


#ifndef _RWSTD_NO_NAMESPACE

// rel_ops contents not available if namespaces are disabled
// to avoid ambiguities with other overloaded operators

_RWSTD_NAMESPACE (rel_ops) { 


template <class _TypeT>
inline bool operator!= (const _TypeT& __x, const _TypeT& __y)
{
    return !(__x == __y);
}

template <class _TypeT>
inline bool operator> (const _TypeT& __x, const _TypeT& __y)
{
    return __y < __x;
}

template <class _TypeT>
inline bool operator<= (const _TypeT& __x, const _TypeT& __y)
{
    return !(__y < __x);
}

template <class _TypeT>
inline bool operator>= (const _TypeT& __x, const _TypeT& __y)
{
    return !(__x < __y);
}


}   // namespace rel_ops


#endif   // _RWSTD_NO_NAMESPACE

}   // namespace std


#endif   // _RWSTD_RELOPS_H_INCLUDED
