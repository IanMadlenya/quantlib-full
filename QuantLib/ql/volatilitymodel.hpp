/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2002, 2003 Ferdinando Ametrano
 Copyright (C) 2003, 2004, 2005 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file volatilitymodel.hpp
    \brief Volatility term structures
*/

#ifndef quantlib_volatility_model_hpp
#define quantlib_volatility_model_hpp

#include <vector>
#include <ql/types.hpp>

namespace QuantLib {
    const QuoteTimeSeries;

    /*! This abstract class defines the interface of concrete
        volatility model

        Volatilities are assumed to be expressed on an annual basis.
    */
    class VolatilityModel {
      public:
        TimeSeries<Volatility>
        calculate(const QuoteTimeSeries &) = 0;
        void calibrate(const QuoteTimeSeries &) = 0;
    };
}


#endif
