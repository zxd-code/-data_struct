/**************************************************************************
 *
 * set_s_di.cpp - Example program of set symmetric difference algorithm.
 *
 * $Id: //stdlib/dev/examples/stdlib/manual/set_s_di.cpp#11 $
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

#include <algorithm>   // for copy, set_symmetric_difference
#include <set>         // for set
#include <iostream>    // for cout, endl
#include <iterator>    // for ostream_iterator

int main ()
{
    // Initialize some sets.
    const int a1[] = { 1, 3, 5, 7, 9, 11 };
    const int a3[] = { 3, 5, 7, 8 };

    std::set<int, std::less<int>, std::allocator<int> >
        odd (a1 + 0, a1 + 6),
        result,
        smalll (a3 + 0, a3 + 4);

    // Create an insert_iterator for result.
    std::insert_iterator<std::set<int, std::less<int>, std::allocator<int> > >
        res_ins (result, result.begin ());

    // Demonstrate set_symmetric_difference.

    typedef std::ostream_iterator<int, char, std::char_traits<char> > Iter;

    std::cout << "The symmetric difference of:\n{";

    std::copy (smalll.begin (), smalll.end (), Iter (std::cout, " "));
    std::cout << "} with {";

    std::copy (odd.begin (),odd.end (), Iter (std::cout, " "));
    std::cout << "} =\n{";

    std::set_symmetric_difference (smalll.begin (), smalll.end (),
                                   odd.begin (), odd.end (), res_ins);

    std::copy (result.begin (), result.end (), Iter (std::cout, " "));
    std::cout << "}\n\n";

    return 0;
}
