/**************************************************************************
 *
 * codecvt.cpp - Example program of codecvt facet. 
 *
 * $Id: //stdlib/dev/examples/stdlib/manual/codecvt.cpp#14 $
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
 
#include <iostream>

#include <codecvte.h>
#include <examples.h>

int main ()
{
    // not used, must be supplied to facet
    std::mbstate_t state;

    // three strings to use as buffers
    std::string ins ("\xfc \xcc \xcd \x61 \xe1 \xd9 \xc6 \xe6 \xf5");
    std::string ins2 (ins.size (), '.');
    std::string outs (ins.size () / ex_codecvt ().encoding (), '.');

    // Print initial contents of buffers
    std::cout << "Before:\n"
              << ins << std::endl
              << ins2 << std::endl
              << outs << std::endl << std::endl;

    // Create a user defined codecvt fact
    // This facet converts from ISO Latin Alphabet No. 1 (ISO 8859-1)
    // to  U.S. ASCII code page 437.

    // Replace the default codecvt<char, char, mbstate_t>.
    std::locale loc (std::cout.getloc (), new ex_codecvt);

    // Retrieve the facet from the locale.
    typedef std::codecvt<char, char, std::mbstate_t> CodeCvt;
    const CodeCvt& cdcvt = std::use_facet<CodeCvt>(loc);

    // unused, must be provided to codecvt<>::in/out
    const char *const_out_next = 0;
    const char *const_in_next  = 0;
          char *in_next        = 0;
          char *out_next       = 0;

    // convert the buffer
    cdcvt.in (state, ins.c_str (), ins.c_str () + ins.length (), const_in_next,
              &outs [0], &outs [0] + outs.length (), out_next);
	
    std::cout << "After in:\n"
              << ins << std::endl
              << ins2 << std::endl
              << outs << std::endl << std::endl;

    // Finally, convert back to the original codeset

    cdcvt.out (state, outs.c_str (), outs.c_str () + outs.length (),
               const_out_next, &ins [0], &ins [0] + ins.length (), in_next);
	
    std::cout << "After out:\n"
              << ins << std::endl
              << ins2 << std::endl
              << outs << std::endl;

    return 0;
}
