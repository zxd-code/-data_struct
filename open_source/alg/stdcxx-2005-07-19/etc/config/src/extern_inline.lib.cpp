
extern inline int foo (int i)
{
    return i + 1;
}

extern int bar (int i)
{
    return i + 1;
}

// take the address of the inline above to prevent
// it from being optimized away
extern int (*volatile pf)(int) = foo;
