/***************************************************************************
 *
 * time.cpp
 *
 * $Id: //stdlib/dev/source/stdlib/util/time.cpp#4 $
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

#include "def.h"          // for Def
#include "diagnostic.h"   // for issue_diag()
#include "path.h"         // for get_pathname()
#include "scanner.h"      // for scanner

#include <cassert>        // for assert()
#include <cstdio>         // for fscanf()
#include <fstream>        // for ofstream


void Def::parse_era (const token_t& t) 
{
    // get the name of the era token
    std::string era_str (t.name);

    // to make processing the era a little easier, first convert the era_str
    // with possible symbolic names to a narrow string without symbolic names
    era_st tmp_era;
    std::string era = convert_string (era_str);
    
    // we need to also parse the wide version of this string so that we
    // may get the wide version of the era name and format
    std::wstring wera = convert_wstring (t);

    char*    erap  = _RWSTD_CONST_CAST (char*, era.c_str());
    wchar_t* werap = _RWSTD_CONST_CAST (wchar_t*, wera.c_str());

    char* tokp;

    // first get the direction
    tokp = std::strtok (erap, ":");
    char direction = *tokp;

    // now get the offset
    tokp = std::strtok (0, ":");
    std::sscanf (tokp, "%d", &tmp_era.era_out.offset);
    if (direction == '-')
        tmp_era.era_out.offset *= -1;
    
    // now get the start date
    tokp = std::strtok (0, ":");
    unsigned int tmp_mon, tmp_day;
    std::sscanf (tokp, "%d/%u/%u", &tmp_era.era_out.year[0], 
                 &tmp_mon, &tmp_day);
    // the month is offset by one ot be in the range [0-11]
    tmp_era.era_out.month[0] = char(tmp_mon - 1);
    tmp_era.era_out.day[0] = char(tmp_day);


    // now get the end date (this may be the beginning or end of time
    tokp = std::strtok (0, ":");
    if (std::strcmp (tokp, "-*") == 0) {
        tmp_era.era_out.year[1] = _RWSTD_INT_MIN;
        tmp_era.era_out.month[1] = _RWSTD_CHAR_MIN;
        tmp_era.era_out.day[1] = _RWSTD_CHAR_MIN;
    }
    else if (std::strcmp (tokp, "+*") == 0) {
        tmp_era.era_out.year[1] = _RWSTD_INT_MAX;
        tmp_era.era_out.month[1] = _RWSTD_CHAR_MAX;
        tmp_era.era_out.day[1] = _RWSTD_CHAR_MAX;
    }
    else {
        std::sscanf (tokp, "%d/%u/%u", &tmp_era.era_out.year[1], 
                     &tmp_mon, &tmp_day);
        // the month is offset by one to be in the range [0-11]
        tmp_era.era_out.month[1] = char(tmp_mon - 1);
        tmp_era.era_out.day[1] = char(tmp_day);

    }
    // now get the name of the era
    tokp = std::strtok (0, ":");
    tmp_era.name = tokp;
    
    // finally get the format string if one is available
    tokp = std::strtok (0, ":");
    if (0 != tokp)
        tmp_era.fmt = tokp;

    // FIXME: check the values
    //advance to name of the era inside the wide char string
    wchar_t *wtokp = werap;
    for (int i = 0; i < 4 && *wtokp; i++) 
        while (*wtokp && *(wtokp++) != L':');

    if (*wtokp) {
        while (*wtokp != L':')
            tmp_era.wname += *wtokp++;
    
        // advance past the current ':'
        wtokp++;
    }

    if (*wtokp)
        tmp_era.wfmt = wtokp;        

    era_list_.push_back (tmp_era);
    time_out_.num_eras++;
}


void Def::process_time ()
{
    // nesting level
    int nesting_level = 0;

    time_def_found_ = true;

    while((next = scanner_.next_token()).token != Scanner::tok_time) {

        int i;

        switch(next.token) {
        case Scanner::tok_end:
            next = scanner_.next_token();
            if (next.token == Scanner::tok_time) {
                // end of numeric block
                if (nesting_level == 0) 
                    return;

                nesting_level--;
                scanner_.close ();
            } else
                issue_diag (E_SYNTAX, true, &next,
                            "wrong section name in END directive\n");

            break;
        case Scanner::tok_copy: {
            next = scanner_.next_token();
            if (next.token != Scanner::tok_string)
                issue_diag (E_SYNTAX, true, &next,
                            "expected string following \"copy\" directive\n");

            // bump up the nesting level
            nesting_level++;

            // open the file
            scanner_.open (get_pathname (strip_quotes (next.name), next.file));

            // get comment char and escape char; 
            // these informations are stored by the scanner
            while ((next = scanner_.next_token ()).token 
                   != Scanner::tok_time ){
                // the LC_IDENTIFICATION section may also have a 
                // LC_TIME token that will mess up the parsing
                if (next.token == Scanner::tok_identification) {
                    while ((next = scanner_.next_token()).token
                           != Scanner::tok_end );
                    next = scanner_.next_token();
                }
            }
            break;
        }
        case Scanner::tok_abday:
            for (i = 0; i < 7; i++) {
                next = scanner_.next_token();
                time_st_.abday[i] = convert_string (next.name);
                time_st_.wabday[i] = convert_wstring (next);
            }
            break;
        case Scanner::tok_day:
            for (i = 0; i < 7; i++) {
                next = scanner_.next_token(); 
                time_st_.day[i] = convert_string (next.name);
                time_st_.wday[i] = convert_wstring (next);
            }
            break;
        case Scanner::tok_abmon:
            for (i = 0; i < 12; i++) {
                next = scanner_.next_token(); 
                time_st_.abmon[i] = convert_string (next.name);
                time_st_.wabmon[i] = convert_wstring (next);
            }
            break;
        case Scanner::tok_mon:
            for (i = 0; i < 12; i++) {
                next = scanner_.next_token(); 
                time_st_.mon[i] = convert_string (next.name);
                time_st_.wmon[i] = convert_wstring (next);
            }
            break;
        case Scanner::tok_d_t_fmt:
            next = scanner_.next_token(); 
            time_st_.d_t_fmt = convert_string (next.name);
            time_st_.wd_t_fmt = convert_wstring (next);
            break;
        case Scanner::tok_d_fmt:
            next = scanner_.next_token(); 
            time_st_.d_fmt = convert_string (next.name);
            time_st_.wd_fmt = convert_wstring (next);
            break;
        case Scanner::tok_t_fmt:
            next = scanner_.next_token(); 
            time_st_.t_fmt = convert_string (next.name);
            time_st_.wt_fmt = convert_wstring (next);
            break;
        case Scanner::tok_am_pm:
            for (i = 0; i < 2; i++) {
                next = scanner_.next_token(); 
                time_st_.am_pm[i] = convert_string (next.name);
                time_st_.wam_pm[i] = convert_wstring (next);
            }
            break;
        case Scanner::tok_t_fmt_ampm:
            next = scanner_.next_token(); 
            time_st_.t_fmt_ampm = convert_string (next.name);
            time_st_.wt_fmt_ampm = convert_wstring (next);
            break;
            // The time_get and time_put facets do not make use of eras or 
            // alternate digits, so we will ignore this part of the locale
            // definition
        case Scanner::tok_era:
            while ((next = scanner_.next_token()).token == Scanner::tok_string)
                parse_era (next);
            break;
        case Scanner::tok_era_d_fmt:
            next = scanner_.next_token();
            time_st_.era_d_fmt = convert_string (next.name);
            time_st_.wera_d_fmt = convert_wstring (next);
            break;
        case Scanner::tok_era_t_fmt:
            next = scanner_.next_token();
            time_st_.era_t_fmt = convert_string (next.name);
            time_st_.wera_t_fmt = convert_wstring (next);
            break;
        case Scanner::tok_era_d_t_fmt:
            next = scanner_.next_token();
            time_st_.era_d_t_fmt = convert_string (next.name);
            time_st_.wera_d_t_fmt = convert_wstring (next);
            break;
        case Scanner::tok_alt_digits:
            while ((next = scanner_.next_token()).token == Scanner::tok_string)
            {
                alt_digit_t digit;
                digit.n_alt_digit = convert_string (next.name);
                digit.w_alt_digit = convert_wstring (next);
                digit.n_offset = 0;
                digit.w_offset = 0;
                alt_digits_.push_back (digit);
            }
            break;
        default:
            break;
        }

    }
}


void Def::write_time(std::string dir_name)
{
    assert (!dir_name.empty());

    if (time_def_found_ && !time_written_) {
        // write out all the information in the LC_TIME category

        (dir_name += _RWSTD_PATH_SEP) += "LC_TIME";

        issue_diag (I_OPENWR, false, 0, "writing %s\n", dir_name.c_str ());

        std::ofstream out (dir_name.c_str(), std::ios::binary);
        out.exceptions (std::ios::failbit | std::ios::badbit);

        int i;

        time_out_.num_alt_digits = alt_digits_.size();
        time_out_.era_off = 0;

        time_out_.alt_digits_off = time_out_.era_off +
            sizeof (_RW::__rw_time_t::era_t) * era_list_.size();

        // now calculate all the offsets for the wide string representations
        time_out_.abday_off[1][0] = time_out_.alt_digits_off +
            2 * sizeof (unsigned int) * time_out_.num_alt_digits;
        
        for (i = 1; i < 7; i++) {
            time_out_.abday_off[1][i] = time_out_.abday_off[1][i-1]
                + (time_st_.wabday[i-1].size() * sizeof (wchar_t)) 
                + sizeof(wchar_t);
        }

        time_out_.day_off[1][0] = time_out_.abday_off[1][6] 
            + time_st_.wabday[6].size() * sizeof (wchar_t) 
            + sizeof (wchar_t);
        for (i = 1; i < 7; i++) {
            time_out_.day_off[1][i] = time_out_.day_off[1][i-1]
                + time_st_.wday[i-1].size() * sizeof (wchar_t) 
                + sizeof (wchar_t);
        }

        time_out_.abmon_off[1][0] = time_out_.day_off[1][6] 
            + time_st_.wday[6].size() * sizeof (wchar_t) + sizeof (wchar_t);
        for (i = 1; i < 12; i++) {
            time_out_.abmon_off[1][i] = time_out_.abmon_off[1][i-1]
                + time_st_.wabmon[i-1].size() * sizeof (wchar_t) 
                + sizeof (wchar_t);
        }

        time_out_.mon_off[1][0] = time_out_.abmon_off[1][11] 
            + time_st_.wabmon[11].size() * sizeof (wchar_t) 
            + sizeof (wchar_t);
        for (i = 1; i < 12; i++) {
            time_out_.mon_off[1][i] = time_out_.mon_off[1][i-1]
                + time_st_.wmon[i-1].size() * sizeof (wchar_t) 
                + sizeof (wchar_t);
        }
        
        time_out_.am_pm_off[1][0] = time_out_.mon_off[1][11] 
            + time_st_.wmon[11].size() * sizeof (wchar_t) + sizeof (wchar_t);
        time_out_.am_pm_off[1][1] = time_out_.am_pm_off[1][0] 
            + time_st_.wam_pm[0].size() * sizeof (wchar_t) + sizeof (wchar_t);
        
        time_out_.d_t_fmt_off[1] = time_out_.am_pm_off[1][1] 
            + time_st_.wam_pm[1].size() * sizeof (wchar_t) 
            + sizeof (wchar_t);
        time_out_.d_fmt_off[1] = time_out_.d_t_fmt_off[1]
            + time_st_.wd_t_fmt.size() * sizeof (wchar_t) + sizeof (wchar_t);
        time_out_.t_fmt_off[1] = time_out_.d_fmt_off[1] 
            + time_st_.wd_fmt.size() * sizeof (wchar_t) + sizeof (wchar_t);
        time_out_.t_fmt_ampm_off[1] = time_out_.t_fmt_off[1]
            + time_st_.wt_fmt.size() * sizeof (wchar_t) + sizeof (wchar_t);
        
        time_out_.era_d_t_fmt_off[1] = time_out_.t_fmt_ampm_off[1]
            + time_st_.wt_fmt_ampm.size() * sizeof (wchar_t) 
            + sizeof (wchar_t);
        time_out_.era_d_fmt_off[1] = time_out_.era_d_t_fmt_off[1]
            + time_st_.wera_d_t_fmt.size() * sizeof (wchar_t) 
            + sizeof (wchar_t);
        time_out_.era_t_fmt_off[1] = time_out_.era_d_fmt_off[1] 
            + time_st_.wera_d_fmt.size() * sizeof (wchar_t) + sizeof (wchar_t);

        unsigned int next_off = time_out_.era_t_fmt_off[1]
            + time_st_.wera_t_fmt.size() * sizeof (wchar_t) + sizeof (wchar_t);  
        era_list_iter era_list_it;
        for (era_list_it = era_list_.begin(); era_list_it != era_list_.end();
             era_list_it ++) {
            era_list_it->era_out.name_off[1] = next_off;
            next_off += era_list_it->wname.size() 
                * sizeof (wchar_t) + sizeof (wchar_t);
            era_list_it->era_out.fmt_off[1] = next_off;
            next_off += era_list_it->wfmt.size() 
                * sizeof (wchar_t) + sizeof (wchar_t);
        }

        alt_digits_iter alt_digits_it;
        for (alt_digits_it = alt_digits_.begin(); 
             alt_digits_it != alt_digits_.end(); alt_digits_it ++ ){
            alt_digits_it->w_offset = next_off;
            next_off += (alt_digits_it->w_alt_digit.size() + 1) 
                * sizeof(wchar_t);
        }

        time_out_.abday_off[0][0] = next_off;
        
        for (i = 1; i < 7; i++) {
            // calculate the offsets for the abreviated days
            time_out_.abday_off[0][i] = time_out_.abday_off[0][i-1]
                + time_st_.abday[i - 1].size() + 1;
        }

        time_out_.day_off[0][0] = time_out_.abday_off[0][6] 
            + time_st_.abday[6].size() + 1;
        for (i = 1; i < 7; i++) {
            // calculate the offsets for the days
            time_out_.day_off[0][i] = time_out_.day_off[0][i-1]
                + time_st_.day[i-1].size() + 1;
        }

        time_out_.abmon_off[0][0] = time_out_.day_off[0][6] 
            + time_st_.day[6].size() + 1;
        for (i = 1; i < 12; i++) {
            // calculate the offsets for the abreviated months
            time_out_.abmon_off[0][i] = time_out_.abmon_off[0][i-1]
                + time_st_.abmon[i-1].size() + 1;
        }

        time_out_.mon_off[0][0] = time_out_.abmon_off[0][11] 
            + time_st_.abmon[11].size() + 1;
        for (i = 1; i < 12; i++) {
            // calculate the offsets for the months
            time_out_.mon_off[0][i] = time_out_.mon_off[0][i-1]
                + time_st_.mon[i-1].size() + 1;
        }
        
        // calculate the offsets for am and pm
        time_out_.am_pm_off[0][0] = time_out_.mon_off[0][11] 
            + time_st_.mon[11].size() + 1;
        time_out_.am_pm_off[0][1] = time_out_.am_pm_off[0][0] 
            + time_st_.am_pm[0].size() + 1;
        
        time_out_.d_t_fmt_off[0] = time_out_.am_pm_off[0][1] 
            + time_st_.am_pm[1].size() + 1;
        time_out_.d_fmt_off[0] = time_out_.d_t_fmt_off[0] 
            + time_st_.d_t_fmt.size() + 1;
        time_out_.t_fmt_off[0] = time_out_.d_fmt_off[0] 
            + time_st_.d_fmt.size() + 1;
        time_out_.t_fmt_ampm_off[0] = time_out_.t_fmt_off[0]
            + time_st_.t_fmt.size() + 1;

        time_out_.era_d_t_fmt_off[0] = time_out_.t_fmt_ampm_off[0] 
            + time_st_.t_fmt_ampm.size() + 1;
        time_out_.era_d_fmt_off[0] = time_out_.era_d_t_fmt_off[0] 
            + time_st_.era_d_t_fmt.size() + 1;
        time_out_.era_t_fmt_off[0] = time_out_.era_d_fmt_off[0] 
            + time_st_.era_d_fmt.size() + 1;



        next_off = time_out_.era_t_fmt_off[0]
            + time_st_.era_t_fmt.size() + 1;

        for (era_list_it = era_list_.begin(); era_list_it != era_list_.end();
             era_list_it ++) {
            era_list_it->era_out.name_off[0] = next_off;
            next_off += era_list_it->name.size() + 1;
            era_list_it->era_out.fmt_off[0] = next_off;
            next_off += era_list_it->fmt.size() + 1;
        }

        for (alt_digits_it = alt_digits_.begin(); 
             alt_digits_it != alt_digits_.end(); alt_digits_it ++ ){
            alt_digits_it->n_offset = next_off;
            next_off += alt_digits_it->n_alt_digit.size() + 1;
        }


        time_out_.codeset_off = next_off;
      
        time_out_.charmap_off = time_out_.codeset_off
            + charmap_.get_code_set_name().size() + 1;
        
        // write the time struct    
        out.write ((char*)&time_out_, sizeof(time_out_));
        
        // first write out the era structs
        for (era_list_it = era_list_.begin(); era_list_it != era_list_.end();
             era_list_it ++) {
            out.write ((const char*) &era_list_it->era_out,
                       sizeof (era_list_it->era_out));
        }

        // next write out the offsets to where the alternate digits are stored
        for (alt_digits_it = alt_digits_.begin(); 
             alt_digits_it != alt_digits_.end(); alt_digits_it ++ ){
            out.write ((const char*) &alt_digits_it->n_offset,
                       sizeof (alt_digits_it->n_offset));
            out.write ((const char*) &alt_digits_it->w_offset,
                       sizeof (alt_digits_it->w_offset));
        }

        // now write out the wchar_t version of LC_TIME
        for (i = 0; i < 7; i++){
            out.write ((const char*)time_st_.wabday[i].c_str(), 
                       (time_st_.wabday[i].size() + 1) * sizeof (wchar_t));
        }
        for (i = 0; i < 7; i++)
            out.write ((const char*)time_st_.wday[i].c_str(), 
                       (time_st_.wday[i].size() + 1) * sizeof (wchar_t));
        for (i = 0; i < 12; i++)
            out.write ((const char*)time_st_.wabmon[i].c_str(),
                       (time_st_.wabmon[i].size() + 1) * sizeof (wchar_t));
        for (i = 0; i < 12; i++)
            out.write ((const char*)time_st_.wmon[i].c_str(), 
                       (time_st_.wmon[i].size() + 1) * sizeof (wchar_t));
        for (i = 0; i < 2; i++)
            out.write ((const char*)time_st_.wam_pm[i].c_str(),
                       (time_st_.wam_pm[i].size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wd_t_fmt.c_str(), 
                   (time_st_.wd_t_fmt.size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wd_fmt.c_str(),
                   (time_st_.wd_fmt.size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wt_fmt.c_str(), 
                   (time_st_.wt_fmt.size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wt_fmt_ampm.c_str(), 
                   (time_st_.wt_fmt_ampm.size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wera_d_t_fmt.c_str(), 
                   (time_st_.wera_d_t_fmt.size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wera_d_fmt.c_str(),
                   (time_st_.wera_d_fmt.size() + 1) * sizeof (wchar_t));
        out.write ((const char*)time_st_.wera_t_fmt.c_str(), 
                   (time_st_.wera_t_fmt.size() + 1) * sizeof (wchar_t));
        for (era_list_it = era_list_.begin(); era_list_it != era_list_.end();
             era_list_it ++) {
            out.write ((const char*) era_list_it->wname.c_str(),
                       (era_list_it->wname.size() + 1) * sizeof (wchar_t));
            out.write ((const char*) era_list_it->wfmt.c_str(),
                       (era_list_it->wfmt.size() + 1) * sizeof (wchar_t));
        }
        for (alt_digits_it = alt_digits_.begin(); 
             alt_digits_it != alt_digits_.end(); alt_digits_it ++ ){
            out.write ((const char*) alt_digits_it->w_alt_digit.c_str(),
                       (alt_digits_it->w_alt_digit.size() + 1) 
                       * sizeof (wchar_t));
        }

        // write out the char version of LC_TIME
        for (i = 0; i < 7; i++)
            out << time_st_.abday[i] << std::ends;
        for (i = 0; i < 7; i++)
            out << time_st_.day[i] << std::ends;
        for (i = 0; i < 12; i++)
            out << time_st_.abmon[i] << std::ends;
        for (i = 0; i < 12; i++)
            out << time_st_.mon[i] << std::ends;
        for (i = 0; i < 2; i++)
            out << time_st_.am_pm[i] << std::ends;
        out << time_st_.d_t_fmt << std::ends;
        out << time_st_.d_fmt << std::ends;
        out << time_st_.t_fmt << std::ends;
        out << time_st_.t_fmt_ampm << std::ends;
        out << time_st_.era_d_t_fmt << std::ends;
        out << time_st_.era_d_fmt << std::ends;
        out << time_st_.era_t_fmt << std::ends;
        for (era_list_it = era_list_.begin(); era_list_it != era_list_.end();
             era_list_it ++) {
            out << era_list_it->name << std::ends;
            out << era_list_it->fmt << std::ends;
        }
        for (alt_digits_it = alt_digits_.begin(); 
             alt_digits_it != alt_digits_.end(); alt_digits_it ++ ){
            out << alt_digits_it->n_alt_digit << std::ends;
        }


        out << charmap_.get_code_set_name() << std::ends;
        out << charmap_.get_charmap_name() << std::ends;
    }
}
