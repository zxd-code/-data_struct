// checking for operator new[] (size_t)

#if defined (_RWSTD_USE_CONFIG)
#  include "config.h"
#endif   // _RWSTD_USE_CONFIG

#include <stddef.h>

#ifndef _RWSTD_NO_NAMESPACE
namespace std {
#endif

    struct bad_alloc {};

#ifndef _RWSTD_NO_NAMESPACE
}
#else
# define std
#endif

#ifdef _RWSTD_NO_EXCEPTION_SPECIFICATION
#  define throw(e)
#endif   // _RWSTD_NO_EXCEPTION_SPECIFICATION


void* operator new[] (size_t) throw (std::bad_alloc);

int main ()
{
    operator new[] (1);

    return 0;
}
