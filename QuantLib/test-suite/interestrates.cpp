
/*
 Copyright (C) 2004 Ferdinando Ametrano

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include "interestrates.hpp"
#include "utilities.hpp"
#include <ql/interestrate.hpp>
#include <ql/Math/rounding.hpp>
#include <ql/Math/comparison.hpp>
#include <ql/DayCounters/actual360.hpp>

using namespace QuantLib;
using namespace boost::unit_test_framework;

namespace {

    struct InterestRateData {
        Rate r;
        Compounding comp;
        Frequency freq;
        Time t;
        Compounding comp2;
        Frequency freq2;
        Rate expected;
        Size precision;
    };
}

void InterestRateTest::testConversions() {
    BOOST_MESSAGE("Testing interest rate conversions...");

    InterestRateData cases[] = {
        // data from "Option Pricing Formulas", Haug, pag.181-182
        // Rate,Compounding,        Frequency,   Time, Compounding2,      Frequency2,  Rate2, precision
        {0.0800, Compounded,        Quarterly,   1.00, Continuous,            Annual, 0.0792, 4},
        {0.1200, Continuous,           Annual,   1.00, Compounded,            Annual, 0.1275, 4},
        {0.0800, Compounded,        Quarterly,   1.00, Compounded,            Annual, 0.0824, 4},
        {0.0700, Compounded,        Quarterly,   1.00, Compounded,        Semiannual, 0.0706, 4},
        // undocumented, but reasonable :)
        {0.0100, Compounded,           Annual,   1.00,     Simple,            Annual, 0.0100, 4},
        {0.0200,     Simple,           Annual,   1.00, Compounded,            Annual, 0.0200, 4},
        {0.0300, Compounded,       Semiannual,   0.50,     Simple,            Annual, 0.0300, 4},
        {0.0400,     Simple,           Annual,   0.50, Compounded,        Semiannual, 0.0400, 4},
        {0.0500, Compounded, EveryFourthMonth,  1.0/3,     Simple,            Annual, 0.0500, 4},
        {0.0600,     Simple,           Annual,  1.0/3, Compounded,  EveryFourthMonth, 0.0600, 4},
        {0.0500, Compounded,        Quarterly,   0.25,     Simple,            Annual, 0.0500, 4},
        {0.0600,     Simple,           Annual,   0.25, Compounded,         Quarterly, 0.0600, 4},
        {0.0700, Compounded,        Bimonthly,  1.0/6,     Simple,            Annual, 0.0700, 4},
        {0.0800,     Simple,           Annual,  1.0/6, Compounded,         Bimonthly, 0.0800, 4},
        {0.0900, Compounded,          Monthly, 1.0/12,     Simple,            Annual, 0.0900, 4},
        {0.1000,     Simple,           Annual, 1.0/12, Compounded,           Monthly, 0.1000, 4},

        {0.0300, SimpleThenCompounded,       Semiannual,   0.25,               Simple,            Annual, 0.0300, 4},
        {0.0300, SimpleThenCompounded,       Semiannual,   0.25,               Simple,        Semiannual, 0.0300, 4},
        {0.0300, SimpleThenCompounded,       Semiannual,   0.25,               Simple,         Quarterly, 0.0300, 4},
        {0.0300, SimpleThenCompounded,       Semiannual,   0.50,               Simple,            Annual, 0.0300, 4},
        {0.0300, SimpleThenCompounded,       Semiannual,   0.50,               Simple,        Semiannual, 0.0300, 4},
        {0.0300, SimpleThenCompounded,       Semiannual,   0.75,           Compounded,        Semiannual, 0.0300, 4},

        {0.0400,               Simple,       Semiannual,   0.25, SimpleThenCompounded,         Quarterly, 0.0400, 4},
        {0.0400,               Simple,       Semiannual,   0.25, SimpleThenCompounded,        Semiannual, 0.0400, 4},
        {0.0400,               Simple,       Semiannual,   0.25, SimpleThenCompounded,            Annual, 0.0400, 4},

        {0.0400,           Compounded,        Quarterly,   0.50, SimpleThenCompounded,         Quarterly, 0.0400, 4},
        {0.0400,               Simple,       Semiannual,   0.50, SimpleThenCompounded,        Semiannual, 0.0400, 4},
        {0.0400,               Simple,       Semiannual,   0.50, SimpleThenCompounded,            Annual, 0.0400, 4},

        {0.0400,           Compounded,        Quarterly,   0.75, SimpleThenCompounded,         Quarterly, 0.0400, 4},
        {0.0400,           Compounded,       Semiannual,   0.75, SimpleThenCompounded,        Semiannual, 0.0400, 4},
        {0.0400,               Simple,       Semiannual,   0.75, SimpleThenCompounded,            Annual, 0.0400, 4}
    };

    Rounding roundingPrecision;
    Rate r3, r2;
    Date d1 = Date::todaysDate(), d2;
    InterestRate ir, ir2, ir3, expectedIR;
    Real compoundf, error;
    DiscountFactor disc;


    for (Size i=0; i<LENGTH(cases); i++) {
        ir = InterestRate(cases[i].r, Settings::instance().dayCounter(),
                          cases[i].comp, cases[i].freq);

        d2 = d1+Integer(Settings::instance().dayCounterBase()*cases[i].t+0.5)*Days;
        // check that the compound factor is the inverse of the discount factor
        compoundf = ir.compoundFactor(d1, d2);
        disc = ir.discountFactor(d1, d2);
        error = QL_FABS(disc-1.0/compoundf);
        if (error>1e-16)
            BOOST_FAIL("\n  " + InterestRateFormatter::toString(ir) +
                       "\n  1.0/compound_factor: "
                       + DecimalFormatter::toString(1.0/compoundf,16) +
                       "\n      discount_factor: "
                       + DecimalFormatter::toString(disc, 16) +
                       "\n        error: "
                       + DecimalFormatter::toExponential(error));

        // check that the equivalent InterestRate with *same* daycounter,
        // compounding, and frequency is the *same* InterestRate
        ir2 = ir.equivalentInterestRate(d1, d2, ir.dayCounter(),
                                                ir.compounding(),
                                                ir.frequency());
        error = QL_FABS(ir.rate()-ir2.rate());
        if (error>1e-15)
            BOOST_FAIL("\n    original interest rate: "
                       + InterestRateFormatter::toString(ir, 12) +
                       "\n  equivalent interest rate: "
                       + InterestRateFormatter::toString(ir2,12) +
                       "\n                rate error: "
                       + DecimalFormatter::toExponential(error, 1));
        if (ir.dayCounter()!=ir2.dayCounter())
            BOOST_FAIL("\n day counter error"
                       "\n original interest rate:   "
                       + InterestRateFormatter::toString(ir, 4) +
                       "\n equivalent interest rate: "
                       + InterestRateFormatter::toString(ir2,4));
        if (ir.compounding()!=ir2.compounding())
            BOOST_FAIL("\n compounding error"
                       "\n original interest rate:   "
                       + InterestRateFormatter::toString(ir, 4) +
                       "\n equivalent interest rate: "
                       + InterestRateFormatter::toString(ir2,4));
        if (ir.frequency()!=ir2.frequency())
            BOOST_FAIL("\n frequency error"
                       "\n    original interest rate: "
                       + InterestRateFormatter::toString(ir, 4) +
                       "\n  equivalent interest rate: "
                       + InterestRateFormatter::toString(ir2,4));

        // check that the equivalent rate with *same* daycounter,
        // compounding, and frequency is the *same* rate
        r2 = ir.equivalentRate(d1, d2, ir.dayCounter(),
                                       ir.compounding(),
                                       ir.frequency());
        error = QL_FABS(ir.rate()-r2);
        if (error>1e-15)
            BOOST_FAIL("\n    original rate: "
                       + InterestRateFormatter::toString(ir, 12) +
                       "\n  equivalent rate: "
                       + RateFormatter::toString(r2,12) +
                       "\n            error: "
                       + DecimalFormatter::toExponential(error, 1));

        // check that the equivalent InterestRate with *different*
        // compounding, and frequency is the *expected* InterestRate
        ir3 = ir.equivalentInterestRate(d1, d2, ir.dayCounter(),
                                        cases[i].comp2, cases[i].freq2);
        expectedIR = InterestRate(cases[i].expected, ir.dayCounter(),
                                  cases[i].comp2, cases[i].freq2);
        r3 = roundingPrecision(ir3.rate());
        error = QL_FABS(r3-expectedIR.rate());
        if (error>1.0e-17)
            BOOST_FAIL("\n  calculated equivalent interest rate: "
                       + InterestRateFormatter::toString(ir3,
                                                         cases[i].precision-2) +
                       "\n    expected equivalent interest rate: "
                       + InterestRateFormatter::toString(expectedIR,
                                                         cases[i].precision-2) +
                       "\n                           rate error: "
                       + DecimalFormatter::toExponential(error, 1));
        if (ir3.dayCounter()!=expectedIR.dayCounter())
            BOOST_FAIL("\n day counter error"
                       "\n    original interest rate: "
                       + InterestRateFormatter::toString(ir3, 4) +
                       "\n  equivalent interest rate: "
                       + InterestRateFormatter::toString(expectedIR,4));
        if (ir3.compounding()!=expectedIR.compounding())
            BOOST_FAIL("\n compounding error"
                       "\n    original interest rate: "
                       + InterestRateFormatter::toString(ir3, 4) +
                       "\n  equivalent interest rate: "
                       + InterestRateFormatter::toString(expectedIR,4));
        if (ir3.frequency()!=expectedIR.frequency())
            BOOST_FAIL("\n frequency error"
                       "\n    original interest rate: "
                       + InterestRateFormatter::toString(ir3, 4) +
                       "\n  equivalent interest rate: "
                       + InterestRateFormatter::toString(expectedIR,4));

        // check that the equivalent rate with *different*
        // compounding, and frequency is the *expected* rate
        r3 = ir.equivalentRate(d1, d2, ir.dayCounter(),
                               cases[i].comp2, cases[i].freq2);
        roundingPrecision = Rounding(cases[i].precision);
        r3 = roundingPrecision(r3);
        error = QL_FABS(r3-cases[i].expected);
        if (error>1.0e-17)
            BOOST_FAIL("\n  calculated equivalent rate: "
                       + RateFormatter::toString(r3,cases[i].precision-2) +
                       "\n    expected equivalent rate: "
                       + RateFormatter::toString(cases[i].expected,
                                                 cases[i].precision-2) +
                       "\n                       error: "
                       + DecimalFormatter::toExponential(error, 1));

    }
}

test_suite* InterestRateTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("Interest Rate tests");
    suite->add(BOOST_TEST_CASE(&InterestRateTest::testConversions));
    return suite;
}

