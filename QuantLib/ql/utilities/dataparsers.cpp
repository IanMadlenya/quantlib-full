/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2002, 2003 Decillion Pty(Ltd)
 Copyright (C) 2006 Joseph Wang

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/utilities/dataparsers.hpp>
#include <ql/utilities/null.hpp>
#include <ql/time/period.hpp>
#include <ql/errors.hpp>
//#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <cctype>
#if defined(BOOST_NO_STDC_NAMESPACE)
    namespace std { using ::toupper; }
#endif

namespace QuantLib {

    Period PeriodParser::parse(const std::string& str) {
        QL_REQUIRE(str.length()>1, "period string length must be at least 2");
        
        std::vector<std::string > subStrings;
        std::string reducedString = str;

        Size iPos, reducedStringDim = 100000, max_iter = 0;
        while (reducedStringDim>0) {
            iPos = reducedString.find_first_of("DdWwMmYy");
            Size subStringDim = iPos+1;
            reducedStringDim = reducedString.length()-subStringDim;
            subStrings.push_back(reducedString.substr(0, subStringDim));
            reducedString = reducedString.substr(iPos+1, reducedStringDim);
            ++max_iter;
            QL_REQUIRE(max_iter<str.length(), "unknown '" << str << "' unit");
        }
         
        Period result = parseOnePeriod(subStrings[0]);
        for (Size i=1; i<subStrings.size(); ++i)
            result += parseOnePeriod(subStrings[i]);
        return result;
    }

    Period PeriodParser::parseOnePeriod(const std::string& str) {
        QL_REQUIRE(str.length()>1, "single period require a string of at "
                   "least 2 characters");

        Size iPos = str.find_first_of("DdWwMmYy");
        QL_REQUIRE(iPos==str.length()-1, "unknown '" <<
                   str.substr(str.length()-1, str.length()) << "' unit");
        TimeUnit units = Days;
        char abbr = std::toupper(str[iPos]);
        if      (abbr == 'D') units = Days;
        else if (abbr == 'W') units = Weeks;
        else if (abbr == 'M') units = Months;
        else if (abbr == 'Y') units = Years;

        Size nPos = str.find_first_of("-+0123456789");
        QL_REQUIRE(nPos<iPos, "no numbers of " << units << " provided");
        Integer n;
        try {
            n = //boost::lexical_cast<Integer>(str.substr(nPos,iPos));
                atoi(str.substr(nPos,iPos).c_str());
        } catch (std::exception& e) {
            QL_FAIL("unable to parse the number of units of " << units <<
                    " in '" << str << "'. Error:" << e.what());
        }

        return Period(n, units);
    }

    std::vector<std::string> DateParser::split(const std::string& str,
                                               char delim) {
        std::vector<std::string> list;
        Size sx= str.find(delim), so=0;

        while (sx != std::string::npos) {
            list.push_back(str.substr(so,sx));
            so += sx+1;
            sx = str.substr(so).find(delim);
        }
        list.push_back(str.substr(so));
        return list;
    }

    Date DateParser::parse(const std::string& str, const std::string& fmt) {
        std::vector<std::string> slist;
        std::vector<std::string> flist;
        Integer d=0, m=0, y=0;

        slist = split(str,'/');
        flist = split(fmt,'/');
        if (slist.size() != flist.size())
            return Null<Date>();
        Size i;
        for (i=0;i<flist.size();i++) {
            std::string sub = flist[i];
            if (boost::algorithm::to_lower_copy(sub) == "dd")
              //  d = boost::lexical_cast<Integer>(slist[i]);
              d = atoi(slist[i].c_str());
            else if (boost::algorithm::to_lower_copy(sub) == "mm")
           //     m = boost::lexical_cast<Integer>(slist[i]);
              m = atoi( slist[i].c_str());          
            else if (boost::algorithm::to_lower_copy(sub) == "yyyy") {
           //     y = boost::lexical_cast<Integer>(slist[i]);
                y=atoi(slist[i].c_str());
                if (y < 100)
                    y += 2000;
            }
        }
        return Date(d,Month(m),y);
    }

    Date DateParser::parseISO(const std::string& str) {
        QL_REQUIRE(str.size() == 10 && str[4] == '-' && str[7] == '-',
                   "invalid format");
        Integer year = //boost::lexical_cast<Integer>(str.substr(0, 4));
                        atoi(str.substr(0, 4).c_str());
        Month month =
          //  static_cast<Month>(boost::lexical_cast<Integer>(str.substr(5, 2)));
                static_cast<Month>(atoi(str.substr(5, 2).c_str()));
        Integer day = //boost::lexical_cast<Integer>(str.substr(8, 2));
                   static_cast<Month>(atoi(str.substr(8, 2).c_str()));
                    
        return Date(day, month, year);
    }

}
