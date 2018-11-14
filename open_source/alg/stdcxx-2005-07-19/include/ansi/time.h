/***************************************************************************
 *
 * time.h
 *
 * $Id: //stdlib/dev/include/ansi/time.h#14 $
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

#  ifndef _RWSTD_TIME_H_INCLUDED
#    define _RWSTD_TIME_H_INCLUDED

#    include <ctime>

#    ifndef _RWSTD_NO_NAMESPACE

using std::clock_t;
using std::size_t;
using std::time_t;
using std::tm;

using std::asctime;
using std::clock;
using std::ctime;
using std::difftime;
using std::gmtime;
using std::localtime;
using std::mktime;
using std::strftime;
using std::time;

#    endif   // _RWSTD_NO_NAMESPACE

#  endif   // _RWSTD_TIME_H_INCLUDED

#else   // if defined (_RWSTD_NO_DEPRECATED_C_HEADERS)

#  include _RWSTD_ANSI_C_TIME_H

#endif   // _RWSTD_NO_DEPRECATED_C_HEADERS
