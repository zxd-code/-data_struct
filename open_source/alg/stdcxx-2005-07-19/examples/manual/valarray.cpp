/**************************************************************************
 *
 * valarray.cpp -- Valarray examples 
 *
 * $Id: //stdlib/dev/examples/stdlib/manual/valarray.cpp#9 $
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

#include <valarray.h>   // for valarray stream inserter
#include <iostream>     // for cout, endl

int main ()
{
    const int ibuf[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };  
    const int ibuf2[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };  

    // create 2 valarrays of ints
    std::valarray<int> vi (ibuf, sizeof ibuf / sizeof *ibuf);
    std::valarray<int> vi2 (ibuf2, sizeof ibuf2 / sizeof *ibuf2);

    // print them out
    std::cout << vi << std::endl << vi2 << std::endl;

    vi += vi2;
    vi2 *= 2;

    std::valarray<int> vi3 = vi2 % vi;

    // print them out again
    std::cout << vi << std::endl << vi2 << std::endl << vi3 << std::endl;

    return 0;
}
