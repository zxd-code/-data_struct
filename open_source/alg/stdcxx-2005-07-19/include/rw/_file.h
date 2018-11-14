/***************************************************************************
 *
 * _file.h - Wrapper definitions for platform independent file I/O
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: //stdlib/dev/include/rw/_file.h#23 $
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

#ifndef _RWSTD_FILE_H_INCLUDED
#define _RWSTD_FILE_H_INCLUDED


#include <rw/_defs.h>


_RWSTD_NAMESPACE (__rw) {

_RWSTD_EXPORT void* __rw_fopen (const char*, int, long);
_RWSTD_EXPORT void* __rw_fdopen (int);
_RWSTD_EXPORT int __rw_fclose (void*, int);
_RWSTD_EXPORT int __rw_fileno (void*, int);
_RWSTD_EXPORT int __rw_fdmode (int);
_RWSTD_EXPORT int __rw_fmode (void*, int);

_RWSTD_EXPORT _RWSTD_SIZE_T
__rw_fread (void*, int, void*, _RWSTD_SIZE_T);

_RWSTD_EXPORT _RWSTD_PTRDIFF_T
__rw_fwrite (void*, int, const void*, _RWSTD_SIZE_T);

_RWSTD_EXPORT long __rw_fseek (void*, int, _RWSTD_PTRDIFF_T, int);
_RWSTD_EXPORT int __rw_fflush (void*, int);

}   // namespace __rw


#endif // _RWSTD_FILE_H_INCLUDED
