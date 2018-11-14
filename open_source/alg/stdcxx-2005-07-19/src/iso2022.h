/***************************************************************************
 *
 * rw_iso2022.h
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/source/stdlib/iso2022.h#4 $
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

#ifndef _RWSTD_LOC_ISO2022_H_INCLUDED
#define _RWSTD_LOC_ISO2022_H_INCLUDED

#include <rw/_defs.h>


_RWSTD_NAMESPACE (__rw) { 

enum encoding_type
{
    stateless,
    iso2022_jp, 	 /* stateful ISO-2022-JP encoding  */
    iso2022_jp2,         /* stateful ISO-2022-JP2 encoding */
    iso2022_kr, 	 /* stateful ISO-2022-KR encoding  */
    iso2022_cn  	 /* stateful ISO-2022-CN encoding  */
};
typedef enum encoding_type encoding_type_t;

// Type of encoding as indicated in the name string
int __rw_encoding_from_name (const char*);


// Conversions for ISO-2022-JP
_V3_LOCALE::codecvt_base::result
__rw_iso2022jp_do_in (_RWSTD_C::mbstate_t&,
                      const char*&, const char*,
                      wchar_t*&, wchar_t*);

_V3_LOCALE::codecvt_base::result
__rw_iso2022jp_do_out (_RWSTD_C::mbstate_t&,
                       const wchar_t*&, const wchar_t*,
                       char*&, char*);

_V3_LOCALE::codecvt_base::result 
__rw_iso2022jp_do_unshift (_RWSTD_C::mbstate_t&, char*&, char*);

int  __rw_iso2022jp_do_encoding ();
 
_RWSTD_SIZE_T __rw_iso2022jp_do_length (_RWSTD_C::mbstate_t&, const char*, 
                                        const char*, _RWSTD_SIZE_T);

_RWSTD_SIZE_T __rw_iso2022jp_do_max_length ();

bool __rw_iso2022_do_always_noconv ();


// Conversions for ISO-2022-JP-2
_V3_LOCALE::codecvt_base::result
__rw_iso2022jp2_do_in (_RWSTD_C::mbstate_t&,
                       const char*&, const char*,
                       wchar_t*&, wchar_t*);

_V3_LOCALE::codecvt_base::result
__rw_iso2022jp2_do_out (_RWSTD_C::mbstate_t&,
                       const wchar_t*&, const wchar_t*,
                       char*&, char*);

_V3_LOCALE::codecvt_base::result 
__rw_iso2022jp2_do_unshift (_RWSTD_C::mbstate_t&, char*&, char*);

int  __rw_iso2022jp2_do_encoding  ();

_RWSTD_SIZE_T __rw_iso2022jp2_do_length (_RWSTD_C::mbstate_t&, const char*, 
                                         const char*, _RWSTD_SIZE_T);

_RWSTD_SIZE_T __rw_iso2022jp2_do_max_length ();

bool __rw_iso2022jp2_do2_always_noconv ();


}   // namespace __rw

#endif   // _RWSTD_LOC_ISO2022_H_INCLUDED
