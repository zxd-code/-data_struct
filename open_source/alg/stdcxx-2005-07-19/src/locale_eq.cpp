/***************************************************************************
 *
 * locale_eq.cpp - definition of std::locale::operator==()
 *
 * $Id: //stdlib/dev/source/stdlib/locale_eq.cpp#5 $
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

#include <rw/_defs.h>

#ifndef _RWSTD_NO_V3_LOCALE

#include <string.h>

#include <loc/_locale.h>

#include "locale_body.h"


_RWSTD_NAMESPACE (_V3_LOCALE) {


bool locale::operator== (const locale &rhs) const
{
    _RWSTD_ASSERT (_C_body);
    _RWSTD_ASSERT (_C_body->_C_name);

    _RWSTD_ASSERT (rhs._C_body);
    _RWSTD_ASSERT (rhs._C_body->_C_name);

    // same bodies (i.e., *this is a copy of `rhs') which implies that
    // the locale objects must consist entirely of standard facets,
    // each of which must come from the same locale
    if (_C_body == rhs._C_body)
        return true;

    // different bodies may compare equal if they both contain only
    // ordinary or named standard facet objects, the same sets of
    // which having been replaced with user-defined or constructed
    // facet objects
    if (!_C_body->_C_n_usr_facets && !rhs._C_body->_C_n_usr_facets) {

        // in order to compare equal, both bodies must have the same
        // sets of facets (some slots may still be uninitialized)
        const bool eql =
           _C_body->_C_std_facet_bits == rhs._C_body->_C_std_facet_bits
        && _C_body->_C_byname_facet_bits == rhs._C_body->_C_byname_facet_bits
        && !memcmp (_C_body->_C_std_facets, rhs._C_body->_C_std_facets,
                    _C_body->_C_n_std_facets * sizeof *_C_body->_C_std_facets);

        // at least some standard facets must have been replaced in order
        // for two locale objects not to share the same body
        _RWSTD_ASSERT (   _C_body->_C_std_facet_bits        != _C_body->_C_all
                       || rhs._C_body->_C_std_facet_bits    != _C_body->_C_all
                       || _C_body->_C_byname_facet_bits     != _C_body->_C_all
                       || rhs._C_body->_C_byname_facet_bits != _C_body->_C_all);

        // facet bits of both objects must be the same if the two objects
        // share the exact same facets (otherwise the bits may or may not
        // be the same)
        _RWSTD_ASSERT (   !eql
                       ||    _C_body->_C_std_facet_bits
                          == rhs._C_body->_C_std_facet_bits
                       &&    _C_body->_C_byname_facet_bits
                          == rhs._C_body->_C_byname_facet_bits);

        // the names of both objects must be the same if the two objects
        // share the exact same facets (otherwise the names may or may not
        // be the same
        // e.g., locale (locale ("C"), new F)
        // and   locale (locale ("C"), new G)
        // can both be named "C;C;C;C;C;C" and distinct if F and G are
        // distinct type neither of which defines the static member id,
        // if both are derived from the same standard facet
        _RWSTD_ASSERT (   !eql
                       || !strcmp (_C_body->_C_name, rhs._C_body->_C_name));

        return eql;
    }

    // locales are unequal if *this has a different number of facets than `rhs'
    if (_C_body->_C_n_usr_facets != rhs._C_body->_C_n_usr_facets)
        return false;

    // locales are unequal if their standard facets don't all come
    // from the same named locales
    if (strcmp (_C_body->_C_name, rhs._C_body->_C_name))
        return false;

    _RWSTD_ASSERT (_C_body->_C_n_usr_facets == rhs._C_body->_C_n_usr_facets);

    // locales with the same (non-0) number of user-defined
    // facets compare equal iff all the facets are identical
    for (_RWSTD_SIZE_T i = 0; i != _C_body->_C_n_usr_facets; ++i) {

        _RWSTD_ASSERT (_C_body->_C_usr_facets [i]);
        _RWSTD_ASSERT (rhs._C_body->_C_usr_facets [i]);

        if (_C_body->_C_usr_facets [i] != rhs._C_body->_C_usr_facets [i])
            return false;
    }

    return true;
}


}   // namespace _V3_LOCALE


#endif   // _RWSTD_NO_V3_LOCALE
