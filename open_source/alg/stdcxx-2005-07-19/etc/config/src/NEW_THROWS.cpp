// checking if operator new() throws

#if defined (_RWSTD_USE_CONFIG)
#  include "config.h"
#endif   // _RWSTD_USE_CONFIG

#include <stdio.h>

#ifndef _RWSTD_NO_SETRLIMIT
#  include <sys/resource.h>   // for setrlimit()
#endif   // _RWSTD_NO_SETRLIMIT


#ifndef _RWSTD_NO_HONOR_STD
#  ifdef _RWSTD_NO_STD_TERMINATE

namespace std {

void terminate ()
{
    static int *ip;

loop:

    while ((ip [0] = ip [1])) {  // force a SIGSEGV
        ++ip;
        terminate ();            // recurse infinitely
    }

    goto loop;
}

}   // namespace std

#  endif   // _RWSTD_NO_STD_TERMINATE
#endif   // _RWSTD_NO_HONOR_STD


#if (defined (_WIN32) || defined (_WIN64)) && !defined (__CYGWIN__)

#  include <crtdbg.h>  // for _CrtSetReportMode()

void nodbg ()
{
    // prevent the Microsoft C library from popping up GUI windows
    // on various failures and direct error messages to the console
    // instead
    _CrtSetReportMode (_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode (_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode (_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
}
#else

void nodbg () { }

#endif   // _WIN32 || _WIN64 && !__CYGWIN__


// 18.4.1.1
// -3- Required behavior: Return a non-null pointer ... or else throw 
// a bad_alloc exception.

int main ()
{
#if !defined (_RWSTD_USE_CONFIG)
    printf ("#undef _RWSTD_NO_NEW_THROWS\n");
#endif

    nodbg ();

    void *p = 0;

    try {
        p = ::operator new (size_t (-1));
    }
    catch (...) {
        // exit successfully if new threw

        // this may give a false positive if operaor new()
        // fails to throw for arguments other than -1 as
        // happens to be the case with gcc 2.x
        printf ("// #define _RWSTD_NO_NEW_OFLOW_SAFE\n");
        return 0;
    }

    // otherwise note that operator new() may have not checked
    // its argument for overflow and try to coerce it to throw
    // by passing it a smaller argument
    printf ("#define _RWSTD_NO_NEW_OFLOW_SAFE\n");

#ifndef _RWSTD_NO_SETRLIMIT

    // decrease resource limit to a minimum to induce a failure
    // without unreasonably stressing the system

    struct rlimit rl = { 0, 0 };
    setrlimit (RLIMIT_DATA, &rl);

#endif   // _RWSTD_NO_SETRLIMIT

    p = (void*)1;

    try {
        for (unsigned long n = 1U << sizeof (long) * 8 - 1;
             p && 0 != n; n = n | (n >> 1)) {
            // try to allocate a huge amount of memory
            p = ::operator new (n);
            // do not delete
        }
    }
    catch (...) {
        return 0;
    }

    return 1;
}
