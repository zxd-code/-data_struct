// checking for POD zero initialization

#if defined (_RWSTD_USE_CONFIG)
#  include "config.h"
#endif

#ifndef _RWSTD_NO_NEW
#  include <new>
#elif !defined (_RWSTD_NO_NEW_H)
#  include <new.h>
#else

#  ifndef _RWSTD_SIZE_T
#    define _RWSTD_SIZE_T unsigned long
#  endif

inline void* operator new (_RWSTD_SIZE_T, void *p)
{
    return p;
}

#endif


struct A {
    char   m1;
    short  m2;
    int    m3;
    float  m4;
    double m5;
    void*  m6;
    char   m7 [7];
    void (A::*m8)();
};


template <class T>
struct S
{
    T t_;

    S (T t = T ()): t_ (t) { }   // must zero initialize `t' if POD
};


template <class T>
struct X
{
    S<A> s;

    X (): s () { }   // must zero initialize `s' if POD
};


int is_zero (const void *pv, unsigned n)
{
    const char *pc = (const char*)pv;

    for (unsigned i = 0; i != n; ++i)
        if (pc [i])
            return 0;

    return -1;
}

int is_A_zero (const A &a)
{
    return    is_zero (&a.m1, sizeof a.m1)
           && is_zero (&a.m2, sizeof a.m2)
           && is_zero (&a.m3, sizeof a.m3)
           && is_zero (&a.m4, sizeof a.m4)
           && is_zero (&a.m5, sizeof a.m5)
           && is_zero (&a.m6, sizeof a.m6)
           && is_zero (&a.m7, sizeof a.m7)
           && is_zero (&a.m8, sizeof a.m8);
}

int is_SA_zero (const S<A> &s)
{
    return is_A_zero (s.t_);
}

template <class T>
int is_X_zero (const X<T> &x)
{
    return is_SA_zero (x.s);
}

void setmem (void *pv, unsigned n, char b)
{
    char buf [256];

    char *pc = (char*)pv;

    // set memory to the value of `b'
    for (unsigned i = 0; i != n; ++i)
        pc [i] = b;

    // dirty up the stack in the process
    for (unsigned j = 0; j != sizeof buf; ++j)
        buf [j] = b;
}


int main ()
{
    union {
        char buf [256];
        void *pad;
    } u;

    int result = 0;

    setmem (&u, sizeof u, 'x');

    // check that a fundamental type is zeroed out on initialization
    int i = int ();

    if (i != 0)
        result |= 1;

    // check that a POD object is zeroed out on initialization
    A a = A ();

    if (!is_A_zero (a))
        result |= 2;

    // check that a POD object is zeroed out on initialization with operator new
    A *pa = new (&u) A ();

    if (!is_A_zero (*pa))
        result |= 4;

    setmem (&u, sizeof u, 'x');

    S<A> *ps = new (&u) S<A>();

    if (!is_SA_zero (*ps))
        result |= 8;

    setmem (&u, sizeof u, 'x');

    X<int> *px = new (&u) X<int>();

    if (!is_X_zero (*px))
        result |= 16;

    setmem (&u, sizeof u, 'x');

    *px = X<int>();

    if (!is_X_zero (*px))
        result |= 32;

    return result;
}
