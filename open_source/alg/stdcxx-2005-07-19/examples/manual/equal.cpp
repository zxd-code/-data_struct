/**************************************************************************
 *
 * equal.cpp - Example program for comparing two ranges for equivalence.
 *
 * $Id: //stdlib/dev/examples/stdlib/manual/equal.cpp#13 $
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

#include <algorithm>    // for equal
#include <functional>   // for equal_to
#include <ios>          // for boolalpha
#include <iostream>     // for cout, endl
#include <iterator>     // for ostream_iterator
#include <vector>       // for vector

#include <examples.h>


int main ()
{
    // Typedef for convenience.
    typedef std::vector<int, std::allocator<int> > Vector;

    typedef std::ostream_iterator <Vector::value_type,
                                   char,
                                   std::char_traits<char> > os_iter;

    const Vector::value_type a1[] = { 1, 2, 3, 4 };
    const Vector::value_type a2[] = { 1, 2, 4, 3 };

    // Set up two vectors.
    const Vector v1 (a1, a1 + sizeof a1 / sizeof *a1);
    const Vector v2 (a2, a2 + sizeof a2 / sizeof *a2);

    // Check for equality.
    bool same = std::equal (v1.begin (), v1.end (), v2.begin ());

    same = same && std::equal (v1.begin (), v1.end (), v2.begin (), 
                               std::equal_to<Vector::value_type>());

    // Output the sequences.
    std::cout << "{ ";
    std::copy (v1.begin (), v1.end (), os_iter (std::cout, " "));

    std::cout << "} == { ";
    std::copy (v2.begin (), v2.end (), os_iter (std::cout, " "));

    // Output the result.
    std::cout << "} --> " << std::boolalpha << same << std::endl;

    return 0;
}
