
/*
 Copyright (C) 2003 Neil Firth
 Copyright (C) 2003 Ferdinando Ametrano
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

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

/*! \file barrieroption.cpp
    \brief Barrier option on a single asset
*/

#include <ql/Volatilities/blackconstantvol.hpp>
#include <ql/Instruments/barrieroption.hpp>
#include <ql/PricingEngines/Barrier/barrierengines.hpp>

namespace QuantLib {

    BarrierOption::BarrierOption(
                         Barrier::Type barrierType,
                         double barrier,
                         double rebate,
                         Handle<StrikedTypePayoff> payoff,
                         const RelinkableHandle<Quote>& underlying,
                         const RelinkableHandle<TermStructure>& dividendTS,
                         const RelinkableHandle<TermStructure>& riskFreeTS,
                         const Exercise& exercise,
                         const RelinkableHandle<BlackVolTermStructure>& volTS,
                         const Handle<PricingEngine>& engine,
                         const std::string& isinCode, 
                         const std::string& description)
    : OneAssetStrikedOption(payoff, underlying, dividendTS, riskFreeTS,
      exercise, volTS, engine, isinCode, description),
      barrierType_(barrierType), barrier_(barrier), rebate_(rebate) {

        if (IsNull(engine))
            setPricingEngine(Handle<PricingEngine>(new AnalyticBarrierEngine));

        registerWith(underlying_);
        registerWith(dividendTS_);
        registerWith(riskFreeTS_);
        registerWith(volTS_);
    }

    void BarrierOption::setupArguments(Arguments* args) const {

        BarrierOption::arguments* moreArgs =
            dynamic_cast<BarrierOption::arguments*>(args);
        QL_REQUIRE(moreArgs != 0,
                   "BarrierOption::setupArguments : "
                   "wrong argument type");
        moreArgs->barrierType = barrierType_;
        moreArgs->barrier = barrier_;
        moreArgs->rebate = rebate_;

        OneAssetStrikedOption::arguments* arguments =
            dynamic_cast<OneAssetStrikedOption::arguments*>(args);
        QL_REQUIRE(arguments != 0,
                   "BarrierOption::setupArguments : "
                   "wrong argument type");
        OneAssetStrikedOption::setupArguments(arguments);

    }

    void BarrierOption::performCalculations() const {
        // enforce in this class any check on engine/payoff
        OneAssetStrikedOption::performCalculations();
    }

    void BarrierOption::arguments::validate() const {
        #if defined(QL_PATCH_MICROSOFT)
        OneAssetStrikedOption::arguments copy = *this;
        copy.validate();
        #else
        OneAssetStrikedOption::arguments::validate();
        #endif

        switch (barrierType) {
          case Barrier::DownIn:
            QL_REQUIRE(underlying >= barrier, "underlying (" +
                       DoubleFormatter::toString(underlying) +
                       ") < barrier (" +
                       DoubleFormatter::toString(barrier) +
                       "): down-and-in barrier undefined");
            break;
          case Barrier::UpIn:
            QL_REQUIRE(underlying <= barrier, "underlying ("+
                       DoubleFormatter::toString(underlying) +
                       ") > barrier ("+
                       DoubleFormatter::toString(barrier) +
                       "): up-and-in barrier undefined");
            break;
          case Barrier::DownOut:
            QL_REQUIRE(underlying >= barrier, "underlying ("+
                       DoubleFormatter::toString(underlying) +
                       ") < barrier ("+ 
                       DoubleFormatter::toString(barrier) +
                       "): down-and-out barrier undefined");
            break;
          case Barrier::UpOut:
            QL_REQUIRE(underlying <= barrier, "underlying ("+
                       DoubleFormatter::toString(underlying) +
                       ") > barrier ("+
                       DoubleFormatter::toString(barrier) +
                       "): up-and-out barrier undefined");
            break;
          default:
            throw Error("Barrier Option: unknown type");
        }
    }

}

