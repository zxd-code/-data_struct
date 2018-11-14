/***************************************************************************
 *
 * ctype.h
 *
 * $Id: //stdlib/dev/include/ansi/ctype.h#12 $
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

#include <rw/_defs.h>

#ifndef _RWSTD_NO_DEPRECATED_C_HEADERS

#  ifndef _RWSTD_CTYPE_H_INCLUDED
#    define _RWSTD_CTYPE_H_INCLUDED

#    include <cctype>

#    ifndef _RWSTD_NO_NAMESPACE

using std::isalnum;
using std::isalpha;
using std::iscntrl;
using std::isdigit;
using std::isgraph;
using std::islower;
using std::isprint;
using std::ispunct;
using std::isspace;
using std::isupper;
using std::isxdigit;
using std::tolower;
using std::toupper;

#    endif   // _RWSTD_NO_NAMESPACE
#  endif   // _RWSTD_CTYPE_H_INCLUDED
#else
#  include _RWSTD_ANSI_C_CTYPE_H
#endif   // _RWSTD_NO_DEPRECATED_C_HEADERS
