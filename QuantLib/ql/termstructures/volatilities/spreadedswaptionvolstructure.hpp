/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Giorgio Facchinetti

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

/*! \file spreadedswaptionvolstructure.hpp
    \brief Spreaded swaption volatility
*/

#ifndef quantlib_spreaded_swaption_volstructure_h
#define quantlib_spreaded_swaption_volstructure_h

#include <ql/termstructures/swaptionvolstructure.hpp>
#include <ql/time/period.hpp>

namespace QuantLib {

    class SpreadedSwaptionVolatilityStructure : public SwaptionVolatilityStructure {
      public:
        SpreadedSwaptionVolatilityStructure(
            const Handle<SwaptionVolatilityStructure>& underlyingVolStructure,
            Spread spread = 0.);

      protected:
        //! \name TermStructure interface
        //@{
        Date maxDate() const;
        //@}
        //! \name SwaptionVolatilityStructure interface
        //@{
        const Period& maxSwapTenor() const;
        Rate minStrike() const;
        Rate maxStrike() const;
        Time maxSwapLength() const;
        BusinessDayConvention businessDayConvention() const;
        
        std::pair<Time,Time> convertDates(const Date& optionDate,
                                          const Period& swapTenor) const;
    

        Volatility volatilityImpl(Time optionTime,
                                  Time swapLength,
                                  Rate strike) const;

        boost::shared_ptr<SmileSection> smileSectionImpl(
                                        Time optionTime, 
                                        Time swapLength) const;
        
        virtual boost::shared_ptr<SmileSection> smileSectionImpl(
                                        const Date& optionDate,
                                        const Period& swapTenor) const;
        
        //@} 

    private:
        const Handle<SwaptionVolatilityStructure> underlyingVolStructure_;
        Spread spread_;

    };

}

#endif
