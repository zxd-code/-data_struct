/***************************************************************************
 *
 * ostream1.cpp - ostream example
 *
 * $Id: //stdlib/dev/examples/stdlib/manual/ostream1.cpp#15 $
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

#include <climits>    // for limits macrors
#include <iomanip>    // for setfill, setprecision, setw
#include <ios>        // for dec, hex, oct, showbase, showpos, etc.
#include <iostream>   // for cout
#include <sstream>    // for istringstream

#include <examples.h>


int main ()
{
    // create a read/write stringbuf object on tiny char
    // and attach it to an istringstream object
    std::istringstream in (std::ios::in | std::ios::out);
      
    // tie the ostream object to the istringstream object
    std::ostream out (in.rdbuf ());

    // output integer values in decimal, octal and hexadecimal notation

    // assume 16-bit short, 32-bit int, and 64-bit long or long long
    const short          i16_min = SHRT_MIN;
    const short          i16_max = SHRT_MAX;

    const unsigned short u16_max = USHRT_MAX;

    const int            i32_min = INT_MIN;
    const int            i32_max = INT_MAX;

    const unsigned int   u32_max = UINT_MAX;

#if INT_MAX < LONG_MAX

    // sizeof (long) > sizeof (int) or there is no type wider than long
    typedef long          Int64;
    typedef unsigned long UInt64;

#else

    // sizeof (long long) > sizeof (long) and sizeof (long) == sizeof (int)
    typedef long long          Int64;
    typedef unsigned long long UInt64;

#endif

    // assuming a 2's complement representation, compute LLONG_MIN,
    // LLONG_MAX, and ULLONG_MAX (the C99 macros may not be #defined)
    const Int64  i64_min = (UInt64 (-1) >> 1) + 1;
    const Int64  i64_max = UInt64 (-1) >> 1;

    const UInt64 u64_max = UInt64 (-1);

    // output the '0' and "0x" prefix to indicate values in octal
    // and hexadecimal notation
    out << std::showbase;

    // output the leading plus sign for positive signed integers
    // (but not for unsigned integers)
    out << std::showpos;

    out << "int16_t min  = "
        << std::dec << std::setw (6) << i16_min << ' '
        << std::oct << std::setw (7) << i16_min << ' '
        << std::hex << i16_min << '\n';

    out << "int16_t max  = "
        << std::dec << std::setw (6) << i16_max << ' '
        << std::oct << std::setw (7) << i16_max << ' '
        << std::hex << i16_max << '\n';

    out << "uint16_t max = "
        << std::dec << std::setw (6) << u16_max << ' '
        << std::oct << std::setw (7) << u16_max << ' '
        << std::hex << u16_max << '\n';


    out << "int32_t min  = "
        << std::dec << std::setw (11) << i32_min << ' '
        << std::oct << std::setw (12) << i32_min << ' '
        << std::hex << i32_min << '\n';

    out << "int32_t max  = "
        << std::dec << std::setw (11) << i32_max << ' '
        << std::oct << std::setw (12) << i32_max << ' '
        << std::hex << i32_max << '\n';

    out << "uint32_t max = "
        << std::dec << std::setw (11) << u32_max << ' '
        << std::oct << std::setw (12) << u32_max << ' '
        << std::hex << u32_max << '\n';


    out << "int64_t min  = "
        << std::dec << std::setw (20) << i64_min << ' '
        << std::oct << std::setw (23) << i64_min << ' '
        << std::hex << i64_min << '\n';

    out << "int64_t max  = "
        << std::dec << std::setw (20) << i64_max << ' '
        << std::oct << std::setw (23) << i64_max << ' '
        << std::hex << i64_max << '\n';

    out << "uint64_t max = "
        << std::dec << std::setw (20) << u64_max << ' '
        << std::oct << std::setw (23) << u64_max << ' '
        << std::hex << u64_max << '\n';


    const char sep[] = "----------------------------------------";

    // output a leading portion of sep
    out.write (sep, 10).put ('\n');


    // set the field width to 10, the the padding character to '*'
    // and and output the floating point pi constant with 2 digits
    // after the decimal point

    const double pi = 3.14159123;
    out << std::noshowpos          // do not output the leading plus sign
        << std::setprecision (3)   // two digits after the decimal point
        << std::setfill ('*')      // padded to width with the asterisk
        << std::setw (10)          // field width
        << pi
        << '\n';

    // output the entire contents of the buffer to standard output
    std::cout << in.rdbuf ();

    return 0;
}
