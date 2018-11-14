// checking for explicit member specialization

struct A
{
    int i_;

    // no default ctor declared to exercise the compiler's ability
    // distinguish between a declaration of an explicir member
    // specialization and a definition of the same (see below)
    A (int i): i_ (i) { }
};

template <class T>
struct B {

    int foo () { return a.i_; }
    int bar ();

    static A a;
};

// declare explicit specialization of template members
template <> inline int B<int>::foo ();
template <> int B<int>::bar ();

// make sure this is taken as a declaration and not a definition
// as, for example, with SunPro 5.5 (see bug #411)
template <> A B<int>::a;

// define explicit specialization of template members
template <> inline int B<int>::foo () { return 1; }
template <> int B<int>::bar () { return 1; }
template <> A B<int>::a (1);

// define members of the primary template
template <class T> int B<T>::bar () { return foo (); }
template <class T> A B<T>::a (0);

int main ()
{
    B<char> bc;
    bc.foo ();
    bc.bar ();

    B<int> bi;

    int status = 0;

    if (1 != bi.foo ())
        status |= 1;

    if (1 != bi.bar ())
        status |= 2;

    if (1 != bi.a.i_)
        status |= 4;

    return status;
}
