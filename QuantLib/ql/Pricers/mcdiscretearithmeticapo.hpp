
/*
 * Copyright (C) 2000-2001 QuantLib Group
 *
 * This file is part of QuantLib.
 * QuantLib is a C++ open source library for financial quantitative
 * analysts and developers --- http://quantlib.org/
 *
 * QuantLib is free software and you are allowed to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of it under the conditions stated
 * in the QuantLib License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You should have received a copy of the license along with this file;
 * if not, please email quantlib-users@lists.sourceforge.net
 * The license is also available at http://quantlib.org/LICENSE.TXT
 *
 * The members of the QuantLib Group are listed in the Authors.txt file, also
 * available at http://quantlib.org/group.html
*/

/*! \file mcdiscretearithmeticapo.hpp
    \brief Discrete Arithmetic Average Price Option

    \fullpath
    ql/Pricers/%mcdiscretearithmeticapo.hpp
*/

// $Id$

#ifndef quantlib_pricers_mc_discrete_arithmetic_average_price_option_h
#define quantlib_pricers_mc_discrete_arithmetic_average_price_option_h

#include <ql/option.hpp>
#include <ql/types.hpp>
#include <ql/Pricers/mcpricer.hpp>
#include <ql/MonteCarlo/mctypedefs.hpp>

namespace QuantLib {

    namespace Pricers {

        //! example of Monte Carlo pricer using a control variate
        /*! \todo Continous Averaging version     */
         class McDiscreteArithmeticAPO : public McPricer<Math::Statistics,
            MonteCarlo::GaussianPathGenerator,
            MonteCarlo::PathPricer<MonteCarlo::Path> > {
          public:
            McDiscreteArithmeticAPO(Option::Type type,
                                    double underlying,
                                    double strike,
                                    Spread dividendYield,
                                    Rate riskFreeRate,
                                    const std::vector<Time>& times,
                                    double volatility,
                                    bool antitheticVariance,
                                    bool controlVariate,
                                    long seed=0);
        };

    }

}


#endif
