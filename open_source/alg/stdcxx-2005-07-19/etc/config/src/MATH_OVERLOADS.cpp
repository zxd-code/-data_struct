// checking for function overloads in <math.h>

#include <math.h>
#include <stdio.h>

#if defined (_RWSTD_USE_CONFIG)
#  include "config.h"
#endif   // _RWSTD_USE_CONFIG


const char* type_name (float) { return "FLT"; }
const char* type_name (double) { return "DBL"; }
const char* type_name (long double) { return "LDBL"; }


// define function templates with the same signatures as
// the required overloads; if the required overloads are
// not defined, the corresponding specializations of the
// function templates will be instantiated and called,
// and will print the appropriate config macros;
// otherwise the macros are not printed/#defined

#define FUN_1(name, NAME)                                 \
    template <class T>                                    \
    T name (T) {                                          \
        return printf ("#define _RWSTD_NO_%s_%s\n",       \
                       NAME, type_name (T ()));           \
    } typedef void unused_typedef

#define FUN_2(name, NAME)                                 \
    template <class T>                                    \
    T name (T, T) {                                       \
        return printf ("#define _RWSTD_NO_%s_%s\n",       \
                       NAME, type_name (T ()));           \
    } typedef void unused_typedef

#define FUN_2_PTR(name, NAME)                             \
    template <class T>                                    \
    T name (T, T*) {                                      \
        return printf ("#define _RWSTD_NO_%s_%s\n",       \
                       NAME, type_name (T ()));           \
    } typedef void unused_typedef

#define FUN_2_INT(name, NAME)                             \
    template <class T>                                    \
    T name (T, int) {                                     \
        return printf ("#define _RWSTD_NO_%s_%s_INT\n",   \
                       NAME, type_name (T ()));           \
    } typedef void unused_typedef

#define FUN_2_PINT(name, NAME)                            \
    template <class T>                                    \
    T name (T, int*) {                                    \
        return printf ("#define _RWSTD_NO_%s_%s\n",       \
                       NAME, type_name (T ()));           \
    } typedef void unused_typedef


// abs() is not declared in the C header <math.h> but is required
// to be declared in the C++ headers <math.h> and <cmath>
FUN_1 (abs, "ABS");
FUN_1 (acos, "ACOS");
FUN_1 (asin, "ASIN");
FUN_1 (atan, "ATAN");
FUN_2 (atan2, "ATAN2");
FUN_1 (ceil, "CEIL");
FUN_1 (cos, "COS");
FUN_1 (cosh, "COSH");
FUN_1 (exp, "EXP");
FUN_1 (fabs, "FABS");
FUN_1 (floor, "FLOOR");
FUN_2 (fmod, "FMOD");
FUN_2_PINT (frexp, "FREXP");
FUN_2_INT (ldexp, "LDEXP");
FUN_1 (log, "LOG");
FUN_1 (log10, "LOG10");
FUN_2_PTR (modf, "MODF");
FUN_2 (pow, "POW");
FUN_2_INT (pow, "POW");
FUN_1 (sin, "SIN");
FUN_1 (sinh, "SINH");
FUN_1 (sqrt, "SQRT");
FUN_1 (tan, "TAN");
FUN_1 (tanh, "TANH");


int main ()
{
    float f = 0;
    double d = 0;

#ifndef _RWSTD_NO_LONG_DOUBLE

    long double ld = 0;

#  define IF_LDBL   if (1)
#else

    double ld = 0;

#  define IF_LDBL   if (0)
#endif   // _RWSTD_NO_LONG_DOUBLE

    int i = 0;

#if !defined (_RWSTD_USE_CONFIG)

    printf ("/**/\n#undef _RWSTD_MATH_OVERLOADS\n");

#endif   // _RWSTD_USE_CONFIG

    abs (d); abs (f); IF_LDBL abs (ld);
    acos (d); acos (f); IF_LDBL acos (ld);
    asin (d); asin (f); IF_LDBL asin (ld);
    atan (d); atan (f); IF_LDBL atan (ld);
    atan2 (d, d); atan2 (f, f); IF_LDBL atan2 (ld, ld);
    ceil (d); ceil (f); IF_LDBL ceil (ld);
    cos (d); cos (f); IF_LDBL cos (ld);
    cosh (d); cosh (f); IF_LDBL cosh (ld);
    exp (d); exp (f); IF_LDBL exp (ld);
    fabs (d); fabs (f); IF_LDBL fabs (ld);
    floor (d); floor (f); IF_LDBL floor (ld);
    fmod (d, d); fmod (f, f); IF_LDBL fmod (ld, ld);
    frexp (d, &i); frexp (f, &i); IF_LDBL frexp (ld, &i);
    ldexp (d, i); ldexp (f, i); IF_LDBL ldexp (ld, i);
    log (d); log (f); IF_LDBL log (ld);
    log10 (d); log10 (f); IF_LDBL log10 (ld);
    modf (d, &d); modf (f, &f); IF_LDBL modf (ld, &ld);
    pow (d, d); pow (f, f); IF_LDBL pow (ld, ld);
    pow (d, i); pow (f, i); IF_LDBL pow (ld, i);
    sin (d); sin (f); IF_LDBL sin (ld);
    sinh (d); sinh (f); IF_LDBL sinh (ld);
    sqrt (d); sqrt (f); IF_LDBL sqrt (ld);
    tan (d); tan (f); IF_LDBL tan (ld);
    tanh (d); tanh (f); IF_LDBL tanh (ld);

    return 0;
}
