// checking for long double printf format prefix

#include <stdio.h>

#if defined (_RWSTD_USE_CONFIG)
#  include "config.h"
#endif

#ifndef _RWSTD_NO_FLOAT_H
#  include <float.h>

#  if defined (__EDG__) && defined (__linux__)

     // gcc on Linux #defines these using its own propriterary __extension__
#    undef LDBL_EPSILON
#    undef LDBL_MIN
#    undef LDBL_MAX

     // redefine to prevent compilation errors
#    define LDBL_EPSILON 1.0842021724855044e-19L
#    define LDBL_MIN     3.3621031431120935e-4932L
#    define LDBL_MAX     1.1897314953572317e+4932L
#  endif   // defined (__EDG__) && defined (__linux__)

#endif   // _RWSTD_NO_FLOAT_H


// avoid having to #include <string.h>
int compare (const char *s1, const char *s2)
{
    for ( ; *s1 && *s1 == *s2; ++s1, ++s2);
    return *s1 - *s2;
}


// checks to see if sprintf/sscanf works with the given format
int chkfmt (const char *fmt)
{
    char outbuf [4096];
    outbuf [0] = '\0';

    long double vals [] = {
        // avoid using edge values to prevent rounding errors
        LDBL_MIN * 10, DBL_MIN * 10, FLT_MIN * 10, -2, -1, -0.1, -0.001,
        0.001, 0.1, 1, FLT_MAX / 10, DBL_MAX / 10, LDBL_MAX / 10
    };

    for (unsigned i = 0; i != sizeof vals / sizeof *vals; ++i) {
        long double ld = 0.0;
        char inbuf [sizeof outbuf] = "";

        // format into a char buffer
        if (0 > sprintf (outbuf, fmt, vals [i]))
            return -1;

        // parse buffer
        if (0 > sscanf (outbuf, fmt, &ld))
            return -1;

        // format parsed number into another buffer
        if (0 > sprintf (inbuf, fmt, ld))
            return -1;

        // compare buffers, expect identical contents
        if (compare (inbuf, outbuf))
            return -1;
    }
    return 0;
}


// avoid having to #include <string.h>
char* append (char *dst, const char *src)
{
    for (; *dst; ++dst);
    for (; (*dst = *src); ++dst, ++src);
    return dst;
}

int main ()
{
#if !defined (_RWSTD_USE_CONFIG)

    printf ("/**/\n#undef _RWSTD_LDBL_PRINTF_PREFIX\n");

#endif   // _RWSTD_USE_CONFIG

    const char* pfx[] = {
        "L",   // C89, C99
        "l",   // on systems where sizeof (long double) == sizeof (double)
        ""     // on systems where sizeof (long double) == sizeof (float)
    };

    for (unsigned i = 0; i != sizeof pfx / sizeof *pfx; ++i) {
        char fmt[8] = "%";
        append (append (fmt, pfx [i]), "e");

        if (0 == chkfmt (fmt)) {
            printf ("#define _RWSTD_LDBL_PRINTF_PREFIX \"%s\"\n", pfx [i]);
            return 0;
        }
    }

    return 1;   // fail
}
