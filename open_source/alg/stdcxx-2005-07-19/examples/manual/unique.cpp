/**************************************************************************
 *
 * unique.cpp - Example program of unique algorithm.
 *
 * $Id: //stdlib/dev/examples/stdlib/manual/unique.cpp#11 $
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

#include <algorithm>   // for copy, unique_copy
#include <iostream>    // for cout, endl
#include <iterator>    // for insert_iterator, ostream_iterator
#include <vector>      // for vector

int main ()
{
    // for convenience
    typedef std::vector<int, std::allocator<int> >          Vector;
    typedef std::insert_iterator<Vector>                    Iter;
    typedef std::ostream_iterator<Vector::value_type, char,
                                  std::char_traits<char> >  OSIter;

    // Initialize two vectors.
    const int a1 [] = {
        4, 5, 5, 9, -1, -1, -1, 3, 7, 5, 
        5, 5, 6, 7,  7,  7,  4, 2, 1, 1
    };

    Vector v (a1, a1 + sizeof a1 / sizeof *a1);
    Vector result;

    // Create an insert_iterator for results.
    Iter ins (result, result.begin ());

    // Demonstrate includes.
    std::cout << "The vector: " << std::endl << "    ";

    std::copy (v.begin (), v.end (), OSIter (std::cout, " "));

    // Find the unique elements.
    std::unique_copy(v.begin(), v.end(), ins);

    // Display the results.
    std::cout << "\n\nHas the following unique elements:\n     ";

    std::copy (result.begin (), result.end (), OSIter (std::cout, " "));
    std::cout << std::endl;
    
    return 0;
}
