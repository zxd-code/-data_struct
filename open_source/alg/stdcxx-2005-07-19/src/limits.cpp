/***************************************************************************
 *
 * limits.cpp - Source for the Standard Library limits class
 *
 * $Id: //stdlib/dev/source/stdlib/limits.cpp#25 $
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
                       
// define generic template and specializations
#include <limits>

#if _MSC_VER != 1300   // working around an MSVC 7.0 bug (PR #26562)
#  undef _RWSTD_LIMITS_INCLUDED
#  define _RWSTD_DEFINE_EXPORTS

   // define static data members of specializations
#  include <limits>
#endif   // MSVC != 7.0
